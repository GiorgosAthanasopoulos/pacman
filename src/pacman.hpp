#pragma once

#include "asset_manager.hpp"
#include "config.hpp"
#include "direction.hpp"
#include "entity.hpp"
#include "raylib.hpp"
#include <raylib.h>

class Pacman {
public:
  Pacman();
  bool ShouldClose();
  void Update();
  void Draw();
  ~Pacman();

private:
  Vector2 winSize;
  Raylib *r;
  AssetManager *am;

  Entity map[MAP_SIZE_Y][MAP_SIZE_X];
  Vector2 gate;
  Vector2 pacman;
  Vector2 ghosts[GHOST_COLOR_COUNT];

  Direction dir;

  float pacmanMoveTimer;
  float powerTimer;
  float gateOpenTimer;

  int score;
  bool lost, won, paused;

  int pacmanAnimFrame;
  float pacmanAnimTimer;

  void Restart();
  void HandleResize();
  void ParseMap();
  void HandleInput();
  void HandleTimers();
  int CountEntities(Entity entity);
  void HandleWon();
  void HandleLost();
  void HandlePaused();
  void DrawWon();
  void DrawLost();
  void DrawPaused();
  void DrawScore();
  void DrawMap();
  void DrawPacman(int cellXCenter, int cellYCenter);
  void StopAllSounds();
};
