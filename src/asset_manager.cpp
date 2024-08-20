#include "asset_manager.hpp"
#include "config.hpp"
#include <raylib.h>

AssetManager::AssetManager() {
  MyLoadSound(death, "assets/death.wav");
  MyLoadSound(ghost, "assets/ghost.wav");
  MyLoadSound(power, "assets/power.wav");
  MyLoadSound(ball, "assets/ball.wav");
  MyLoadSound(intro, "assets/intro.wav");

  pacman[0] = LoadTexture("assets/pacman/0.png");
  pacman[1] = LoadTexture("assets/pacman/1.png");
  pacman[2] = LoadTexture("assets/pacman/2.png");

  eyes[0] = LoadTexture("assets/eyes/u.png");
  eyes[1] = LoadTexture("assets/eyes/d.png");
  eyes[2] = LoadTexture("assets/eyes/l.png");
  eyes[3] = LoadTexture("assets/eyes/r.png");

  ghostTex[0][0] = LoadTexture("assets/ghosts/b-0.png");
  ghostTex[0][1] = LoadTexture("assets/ghosts/b-1.png");

  ghostTex[1][0] = LoadTexture("assets/ghosts/p-0.png");
  ghostTex[1][1] = LoadTexture("assets/ghosts/p-1.png");

  ghostTex[2][0] = LoadTexture("assets/ghosts/r-0.png");
  ghostTex[2][1] = LoadTexture("assets/ghosts/r-1.png");

  ghostTex[3][0] = LoadTexture("assets/ghosts/y-0.png");
  ghostTex[3][1] = LoadTexture("assets/ghosts/y-1.png");
}

void AssetManager::MyLoadSound(Sound &sound, const char *path) {
  sound = LoadSound(path);
  SetSoundVolume(sound, SOUND_VOLUME);
}

AssetManager::~AssetManager() {
  UnloadSound(death);
  UnloadSound(ghost);
  UnloadSound(power);
  UnloadSound(ball);
  UnloadSound(intro);

  for (int i = 0; i < PACMAN_FRAME_COUNT; ++i) {
    UnloadTexture(pacman[i]);
  }
  for (int i = 0; i < EYES_FRAME_COUNT; ++i) {
    UnloadTexture(eyes[i]);
  }
  for (int i = 0; i < GHOST_COLOR_COUNT; ++i) {
    for (int j = 0; j < GHOST_FRAME_COUNT; ++j) {
      UnloadTexture(ghostTex[i][j]);
    }
  }
}
