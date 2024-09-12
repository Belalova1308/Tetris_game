#include "screen.h"

void drawField(Extra_info extr) {
  // Отрисовка поля
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int cellValue = extr.gameInfo.field[y][x];
      switch (cellValue) {
        case 0:
          mvprintw(y, x, " ");
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          fill(y, x, cellValue);
          break;
        case 9:
          attron(COLOR_PAIR(8));
          mvprintw(y, x, " ");
          attroff(COLOR_PAIR(8));
          break;
        default:
          break;
      }
    }
  }

  // Отрисовка текущей фигуры
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (get_cell(extr, y, x) == 'X') {
        fill(extr.y + y, extr.x + x, extr.currentPiece + 1);
      }
    }
  }
  // Отрисовка следующей фигуры
  renderNext(extr);
  mvprintw(1, 14, "SCORE: %8d", extr.gameInfo.score);
  mvprintw(2, 14, "LEVEL: %8d", extr.gameInfo.level);
  refresh();
}
// Инициализация цветов и экрана
WINDOW *get_main_window() {
  WINDOW *w = initscr();
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_RED);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  init_pair(3, COLOR_GREEN, COLOR_YELLOW);
  init_pair(4, COLOR_GREEN, COLOR_BLUE);
  init_pair(5, COLOR_GREEN, COLOR_MAGENTA);
  init_pair(6, COLOR_GREEN, COLOR_CYAN);
  init_pair(7, COLOR_GREEN, COLOR_WHITE);
  init_pair(8, COLOR_WHITE, COLOR_WHITE);
  cbreak();
  noecho();
  keypad(w, true);
  nodelay(w, true);
  curs_set(0);
  refresh();
  return w;
}
// Отрисовка меню
int menu(WINDOW *w) {
  char *menu[2] = {"New game", "Quit"};
  int current = 0;

  nodelay(w, false);
  wclear(w);

  while (1) {
    for (int i = 0; i < 2; i++)
      if (i == current)
        mvprintw(2 + i, 2, ">> %s <<", menu[i]);
      else
        mvprintw(2 + i, 2, "   %s   ", menu[i]);

    UserAction_t action = getAction(getch());

    switch (action) {
      case Up:
        current = current == 0 ? 1 : current - 1;
        break;
      case Down:
        current = current == 1 ? 0 : current + 1;
        break;
      case Start:
        return current;
        break;
      default:
        break;
    }
  }
}
// Печать фигры опр. цвета
void fill(int y, int x, int color) {
  attron(COLOR_PAIR(color));
  mvprintw(y, x, " ");
  attroff(COLOR_PAIR(color));
}
GameInfo_t updateCurrentState(GameInfo_t *gameInfo) { return *gameInfo; }
// Отрисовка экрана паузы
void pauseMenu() {
  WINDOW *pausew = newwin(20, 10, 0, 1);
  box(pausew, ' ', ' ');
  mvwprintw(pausew, 1, 2, "STOPED");
  mvwprintw(pausew, 3, 1, "press 'p'");
  mvwprintw(pausew, 5, 1, "to PLAY");
  wrefresh(pausew);
  int c = getch();
  while (c != 'p') c = getch();
  delwin(pausew);
}
// Отрисовка поля со сл. фигурой
void renderNext(Extra_info extr) {
  mvprintw(10, 16, "  NEXT BLOCK ");
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 20; x++) {
      extr.gameInfo.next[y][x] =
          (x == 0 || x == 19 || y == 9 || y == 0) ? 9 : 0;
    }
  }
  int startX = (20 - 4) / 2;
  int startY = 3;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      int cellValue = extr.P[extr.nextPiece][rotate(y, x, 0)];
      if (cellValue == 'X') {
        extr.gameInfo.next[startY + y][startX + x] = extr.nextPiece + 1;
      }
    }
  }
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 20; x++) {
      int cellValue = extr.gameInfo.next[y][x];
      switch (cellValue) {
        case 0:
          mvprintw(y + 11, x + 13, " ");
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          fill(y + 11, x + 13, cellValue);
          break;
        case 9:
          attron(COLOR_PAIR(8));
          mvprintw(y + 11, x + 13, " ");
          attroff(COLOR_PAIR(8));
          break;
        default:
          break;
      }
    }
  }
}
// Выделение памяти для полей
void startLoop(WINDOW *w, Extra_info *extr) {
  extr->gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr->gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr->gameInfo.field[i][j] = 0;
    }
  }
  extr->gameInfo.next = (int **)malloc((sizeof(int *)) * 10);
  for (int i = 0; i < 10; i++) {
    extr->gameInfo.next[i] = (int *)malloc((sizeof(int)) * 20);
    for (int j = 0; j < 20; j++) {
      extr->gameInfo.next[i][j] = 0;
    }
  }
  extr->nextPiece = rand() % 7;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      extr->gameInfo.field[y][x] =
          (x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) ? 9 : 0;
    }
  }

  wclear(w);
  nodelay(w, true);
}