#include "../../brick_game/tetris/tetris.h"
#include "screen.h"

int main() {
  WINDOW *w = get_main_window();
  Extra_info extr;
  while (menu(w) == 0) newgame(w, extr);

  endwin();
  return 0;
}