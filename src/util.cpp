#include "util.hpp"
#include "entity.hpp"
#include <raylib.h>

Vector2 GetWindowSize() {
  return {(float)GetRenderWidth(), (float)GetRenderHeight()};
}

bool ValidMove(Entity map[MAP_SIZE_Y][MAP_SIZE_X], Vector2 pacman,
               Direction dir) {
  switch (dir) {
  case UP:
    pacman.y--;
    break;
  case DOWN:
    pacman.y++;
    break;
  case LEFT:
    pacman.x--;
    break;
  case RIGHT:
    pacman.x++;
    break;
  case NONE:
    break;
  }

  return map[(int)pacman.y][(int)pacman.x] != Entity::WALL &&
         map[(int)pacman.y][(int)pacman.x] != Entity::GATE;
}

Vector2 AssertTextFitsInViewport(const char *text, float fontSize,
                                 Vector2 maxSize) {
  float textW = MeasureText(text, fontSize);

  while (textW > maxSize.x || fontSize > maxSize.y) {
    fontSize--;
    textW = MeasureText(text, fontSize);
  }

  return {textW, fontSize};
}

void DrawCenteredText(const char *text, float fontSize, Vector2 maxSize,
                      Color color) {
  Vector2 textSize = AssertTextFitsInViewport(text, fontSize, maxSize);
  Vector2 winSize = GetWindowSize();
  DrawText(text, winSize.x / 2 - textSize.x / 2, winSize.y / 2 - textSize.y / 2,
           textSize.y, color);
}

Vector2 operator/(Vector2 a, float b) {
  if (b == 0) {
    return {0, 0};
  }
  return {a.x / b, a.y / b};
}
