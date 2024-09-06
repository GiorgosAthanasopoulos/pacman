#include "pacman.hpp"
#include "util.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <rlgl.h>
#include <sstream>

// FIX: pacman going through original gate position becomes invisible
// TODO: move ghosts with ai towards player
// TODO: add ghost texture (and animation)

Pacman::Pacman() {
  r = new Raylib;
  am = new AssetManager;
  winSize = GetWindowSize();

  Restart();
}

bool Pacman::ShouldClose() { return WindowShouldClose(); }

void Pacman::Update() {
  HandleResize();

  HandlePaused();
  if (paused) {
    return;
  }

  if (won) {
    HandleWon();
    return;
  }

  if (lost) {
    HandleLost();
    return;
  }

  HandleInput();
  HandleTimers();
}

void Pacman::Draw() {
  BeginDrawing();
  ClearBackground(WIN_BG);

  if (paused) {
    DrawPaused();
    goto end_drawing;
    return;
  }

  if (won) {
    DrawWon();
    goto end_drawing;
    return;
  }

  if (lost) {
    DrawLost();
    goto end_drawing;
    return;
  }

  DrawMap();
  DrawScore();
  r->Draw();

end_drawing:
  EndDrawing();
}

Pacman::~Pacman() {
  delete am;
  delete r;
}

void Pacman::Restart() {
  ParseMap();

  dir = Direction::NONE;

  pacmanMoveTimer = PACMAN_MOVE_INTERVAL;
  powerTimer = 0;
  gateOpenTimer = GATE_OPEN_TIME;

  score = 0;
  lost = won = paused = false;

  pacmanAnimFrame = 0;
  pacmanAnimTimer = PACMAN_ANIM_INTERVAL;

  StopAllSounds();
  PlaySound(am->intro);
}

void Pacman::HandleResize() {
  if (IsWindowResized()) {
    Vector2 neew = GetWindowSize();

    winSize = neew;
  }
}

void Pacman::ParseMap() {
  std::ifstream file(MAP_TXT);

  if (!file) {
    std::cerr << "[ERROR]: failed to read map input file: " << MAP_TXT << "\n";
    std::exit(1);
  }

  std::string line;
  int idY = 0;
  int ghostIndex = 0;

  while (std::getline(file, line)) {
    int idX = 0;

    if (line.empty()) {
      break;
    }

    for (char &ch : line) {
      switch (ch) {
      case '0':
        map[idY][idX++] = Entity::BALL;
        break;
      case '1':
        map[idY][idX++] = Entity::WALL;
        break;
      case '2':
        gate = {(float)idX, (float)idY};
        map[idY][idX++] = Entity::GATE;
        break;
      case '3':
        map[idY][idX++] = Entity::EMPTY;
        break;
      case '4':
        map[idY][idX++] = Entity::POWER_BALL;
        break;
      case 'p':
        pacman = {(float)idX, (float)idY};
        map[idY][idX++] = Entity::PACMAN;
        break;
      case 'g':
        ghosts[ghostIndex++] = {(float)idX, (float)idY};
        map[idY][idX++] = Entity::GHOST;
        break;
      }
    }
    assert(idX == MAP_SIZE_X);

    idY++;
  }

  assert(idY == MAP_SIZE_Y);
  assert(CountEntities(Entity::PACMAN) == 1);
  assert(CountEntities(Entity::GATE) == 1);
  assert(CountEntities(Entity::GHOST) == 4);

  file.close();
}

void Pacman::HandleInput() {
  if (IsKeyPressed(KEY_PLAYER_MOVE_UP)) {
    dir = Direction::UP;
  } else if (IsKeyPressed(KEY_PLAYER_MOVE_DOWN)) {
    dir = Direction::DOWN;
  } else if (IsKeyPressed(KEY_PLAYER_MOVE_LEFT)) {
    dir = Direction::LEFT;
  } else if (IsKeyPressed(KEY_PLAYER_MOVE_RIGHT)) {
    dir = Direction::RIGHT;
  }

  if (dir == Direction::NONE) {
    return;
  }
  bool canMove = pacmanMoveTimer > 0.0f;
  if (canMove) {
    pacmanMoveTimer -= GetFrameTime();
    return;
  }
  if (!ValidMove(map, pacman, dir)) {
    return;
  }

  Vector2 old = pacman;
  Vector2 neew = pacman;
  switch (dir) {
  case UP:
    neew.y--;
    break;
  case DOWN:
    neew.y++;
    break;
  case LEFT:
    neew.x--;
    break;
  case RIGHT:
    neew.x++;
    break;
  case NONE:
    break;
  }

  Entity eaten = map[(int)neew.y][(int)neew.x];
  switch (eaten) {
  case BALL:
    score += SCORE_INTERVAL;
    PlaySound(am->ball);
    if (CountEntities(Entity::BALL) == 1) {
      won = true;
      return;
    }
    break;
  case WALL:
    break;
  case GATE:
    break;
  case EMPTY:
    break;
  case POWER_BALL:
    powerTimer = POWER_TIME;
    StopAllSounds();
    PlaySound(am->power);
    break;
  case PACMAN:
    break;
  case GHOST:
    if (powerTimer <= 0.0f) {
      StopAllSounds();
      PlaySound(am->death);
      lost = true;
      return;
    } else {
      if (CountEntities(Entity::GHOST) == 1) {
        won = true;
      }

      score += SCORE_INTERVAL_KILL;
      StopAllSounds();
      PlaySound(am->ghost);
    }
    break;
  }

  map[(int)neew.y][(int)neew.x] = Entity::PACMAN;
  map[(int)old.y][(int)old.x] = Entity::EMPTY;
  pacman = neew;
  pacmanMoveTimer = PACMAN_MOVE_INTERVAL;
}

void Pacman::HandleTimers() {
  float delta = GetFrameTime();

  gateOpenTimer -= delta;
  if (gateOpenTimer <= 0.0f) {
    map[(int)gate.y][(int)gate.x] = Entity::EMPTY;
  }

  powerTimer -= delta;

  pacmanAnimTimer -= delta;
  if (pacmanAnimTimer <= 0.0f) {
    pacmanAnimTimer = PACMAN_ANIM_INTERVAL;
    pacmanAnimFrame++;
    if (pacmanAnimFrame == PACMAN_FRAME_COUNT) {
      pacmanAnimFrame = 0;
    }
  }
}

int Pacman::CountEntities(Entity entity) {
  int res = 0;

  for (int i = 0; i < MAP_SIZE_Y; ++i) {
    for (int j = 0; j < MAP_SIZE_X; ++j) {
      if (map[i][j] == entity) {
        res++;
      }
    }
  }

  return res;
}

void Pacman::HandleWon() {
  if (IsKeyPressed(KEY_RESTART)) {
    Restart();
  }
}

void Pacman::HandleLost() {
  if (IsKeyPressed(KEY_RESTART)) {
    Restart();
  }
}

void Pacman::HandlePaused() {
  if (IsKeyPressed(KEY_PAUSE_GAME)) {
    paused = !paused;
  }
}

void Pacman::DrawWon() {
  DrawCenteredText("Won", LARGE_FONT_SIZE, GetWindowSize() / 2, COLOR_UI_TEXT);
}

void Pacman::DrawLost() {
  DrawCenteredText("Lost", LARGE_FONT_SIZE, GetWindowSize() / 2, COLOR_UI_TEXT);
}

void Pacman::DrawPaused() {
  DrawCenteredText("Paused", LARGE_FONT_SIZE, GetWindowSize() / 2,
                   COLOR_UI_TEXT);
}

void Pacman::DrawMap() {
  int cellW = GetRenderWidth() / MAP_SIZE_X;
  int cellH = GetRenderHeight() / MAP_SIZE_Y;
  int radiusMoveable = cellW / 2.5;
  int radiusStatic = radiusMoveable / 2;
  int gateThick = cellH / 4;
  int wallThick = 2;
  int ghostIndex = 0;

  for (int i = 0; i < MAP_SIZE_Y; ++i) {
    for (int j = 0; j < MAP_SIZE_X; ++j) {
      int cellX = cellW * j;
      int cellY = cellH * i;
      int cellXCenter = cellX + cellW / 2;
      int cellYCenter = cellY + cellH / 2;

      switch (map[i][j]) {
      case BALL:
        DrawCircle(cellXCenter, cellYCenter, radiusStatic, COLOR_BALL);
        break;
      case WALL:
        DrawRectangle(cellX, cellY, cellW, cellH, COLOR_WALL);
        DrawRectangleLinesEx(
            {(float)cellX, (float)cellY, (float)cellW, (float)cellH}, wallThick,
            COLOR_WALL_PERIMETER);
        break;
      case GATE:
        if (gateOpenTimer <= 0.0f) {
          break;
        }
        DrawLineEx({(float)cellX, (float)cellYCenter},
                   {(float)cellX + cellW, (float)cellYCenter}, gateThick,
                   COLOR_GATE);
        break;
      case EMPTY:
        break;
      case POWER_BALL:
        DrawCircle(cellXCenter, cellYCenter, radiusStatic, COLOR_POWER_BALL);
        break;
      case PACMAN:
        DrawPacman(cellXCenter, cellYCenter);
        break;
      case GHOST:
        DrawTexturePro(am->ghostTex[ghostIndex][0],
                       {0, 0, (float)am->ghostTex[ghostIndex]->width,
                        (float)am->ghostTex[ghostIndex]->height},
                       {(float)cellX, (float)cellY, (float)cellW, (float)cellH},
                       {0, 0}, 0, WHITE);
        ghostIndex++;
        DrawTexturePro(
            am->eyes[1],
            {0, 0, (float)am->eyes[0].width, (float)am->eyes[1].height},
            {(float)cellX, (float)cellY, (float)cellW, (float)cellH}, {0, 0}, 0,
            WHITE);
        break;
      }
    }
  }
}

void Pacman::DrawScore() {
  std::ostringstream oss;
  oss << "Score: " << score;
  std::string s = oss.str();
  const char *t = s.data();
  Vector2 tSz = AssertTextFitsInViewport(
      t, FONT_SIZE,
      {(float)GetRenderWidth() / 6, (float)GetRenderHeight() / MAP_SIZE_Y});
  DrawText(t, 0, winSize.y - tSz.y, tSz.y, COLOR_UI_TEXT);
}

void Pacman::DrawPacman(int cellXCenter, int cellYCenter) {
  Texture2D tex = am->pacman[pacmanAnimFrame];
  int rotation = GetRotationFromDirection(dir);
  Texture2D eye = am->eyes[static_cast<Direction>(
      dir != static_cast<Direction>(NONE) ? dir : Direction::LEFT)];

  rlPushMatrix();
  rlTranslatef(cellXCenter, cellYCenter, 0);
  rlRotatef(rotation, 0, 0, 1);
  rlTranslatef(-tex.width / 2.0f, -tex.height / 2.0f, 0);
  DrawTexture(tex, 0, 0, powerTimer > 0.0f ? COLOR_PACMAN_POWER : COLOR_PACMAN);
  rlPopMatrix();

  rlPushMatrix();
  if (dir != Direction::UP) {
    rlTranslatef(cellXCenter, cellYCenter, 0);
  } else {
    rlTranslatef(cellXCenter, cellYCenter, 0);
  }
  rlTranslatef(-tex.width / 2.0f, -tex.height / 2.0f, 0);
  DrawTexture(eye, 0, 0, WHITE);
  rlPopMatrix();
}

void Pacman::StopAllSounds() {
  StopSound(am->death);
  StopSound(am->ghost);
  StopSound(am->power);
  StopSound(am->ball);
  StopSound(am->intro);
}
