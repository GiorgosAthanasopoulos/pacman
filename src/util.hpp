#pragma once

#include "config.hpp"
#include "direction.hpp"
#include "entity.hpp"
#include <raylib.h>

Vector2 GetWindowSize();

bool ValidMove(Entity map[MAP_SIZE_Y][MAP_SIZE_X], Vector2 pacman,
               Direction dir);

Vector2 AssertTextFitsInViewport(const char *text, float fontSize,
                                 Vector2 maxSize);
void DrawCenteredText(const char *text, float fontSize, Vector2 maxSize,
                      Color color);

Vector2 operator/(Vector2 a, float b);
