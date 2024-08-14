#pragma once

#include <raylib.h>

// window
#define WIN_FLAGS FLAG_WINDOW_RESIZABLE
#define WIN_W 1280
#define WIN_H 720
#define WIN_TITLE "Pacman"
#define WIN_FPS 30
#define WIN_MIN_W 640
#define WIN_MIN_H 480
#define WIN_BG BLACK

// keybind
#define KEY_EXIT KEY_NULL
#define KEY_PLAYER_MOVE_UP KEY_W
#define KEY_PLAYER_MOVE_DOWN KEY_S
#define KEY_PLAYER_MOVE_LEFT KEY_A
#define KEY_PLAYER_MOVE_RIGHT KEY_D
#define KEY_RESTART KEY_SPACE
#define KEY_PAUSE_GAME KEY_P

// debug
#define DEBUG true

// map
#define MAP_SIZE_X 29
#define MAP_SIZE_Y 20
#define MAP_TXT "assets/map.txt"

// colors
#define COLOR_BALL PINK
#define COLOR_WALL DARKBLUE
#define COLOR_GATE RAYWHITE
#define COLOR_POWER_BALL ORANGE
#define COLOR_PACMAN YELLOW
#define COLOR_GHOST GRAY
#define COLOR_WALL_PERIMETER BLACK
#define COLOR_UI_TEXT WHITE
#define COLOR_PACMAN_POWER ORANGE

// game
#define GATE_OPEN_TIME 2.0f
#define SCORE_INTERVAL 10
#define PACMAN_MOVE_INTERVAL 0.1f
#define POWER_TIME 10.0f
#define SCORE_INTERVAL_KILL 50

// ui
#define FONT_SIZE 100
#define LARGE_FONT_SIZE FONT_SIZE * 10
