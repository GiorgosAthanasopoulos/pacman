#pragma once

#include "config.hpp"
#include <raylib.h>

class AssetManager {
public:
  Sound death, ghost, power, ball, intro;
  Texture2D pacman[PACMAN_FRAME_COUNT], eyes[EYES_FRAME_COUNT],
      ghostTex[GHOST_COLOR_COUNT][GHOST_FRAME_COUNT];

  AssetManager();
  void MyLoadSound(Sound &sound, const char *path);
  ~AssetManager();
};
