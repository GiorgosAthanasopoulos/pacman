#include "pacman.hpp"

int main() {
  Pacman g;

  while (!g.ShouldClose()) {
    g.Update();
    g.Draw();
  }

  return 0;
}
