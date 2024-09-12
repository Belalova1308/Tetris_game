#include "tetris.h"

#include "../../gui/cli/screen.h"
// ввод пользоветеля
UserAction_t getAction(int user_input) {
  UserAction_t rc;
  if (user_input == KEY_UP)
    rc = Up;
  else if (user_input == KEY_DOWN)
    rc = Down;
  else if (user_input == KEY_LEFT)
    rc = Left;
  else if (user_input == KEY_RIGHT)
    rc = Right;
  else if (user_input == ESCAPE)
    rc = Terminate;
  else if (user_input == ENTER_KEY)
    rc = Start;
  else if (user_input == 'p')
    rc = Pause;
  else if (user_input == 'e')
    rc = Action;
  return rc;
}

// Функция контроля скорости
void timing(Extra_info *extr) {
  usleep(50 * 1000);
  extr->speed_counter++;
  extr->fall =
      (extr->speed_counter == extr->gameInfo.speed - (extr->gameInfo.level * 2))
          ? 1
          : 0;
}
// реализация ввода пользователя, сдвиг фигуры
void userInput(UserAction_t action, bool hold) {
  Extra_info *extr;
  if (hold) {
    printf("1");
  }
  action = getAction(getch());
  switch (action) {
    case Left:
      if (fitCol(*extr, extr->x - 1) == 1) extr->x--;
      break;
    case Right:
      if (fitCol(*extr, extr->x + 1) == 1) extr->x++;
      break;
    case Down:
      while (fitRow(*extr, extr->y + 1) == 1) extr->y++;
      break;
    case Action:
      if (fitRot(*extr, extr->rotation + 1) == 1) extr->rotation++;
      break;
    case Pause:
      pauseMenu();
      break;
    case Terminate:
      extr->over = 1;
      break;
    default:
      break;
  }
}
// логика движения
void gameLogic(Extra_info *extr) {
  if (extr->fall == 1) {
    // движение вниз
    if (fitRow(*extr, extr->y + 1)) {
      extr->y++;
    } else {
      // фиксация фигура на поле
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          if (get_cell(*extr, y, x) == 'X') {
            int temp_y = extr->y + y;
            int temp_x = extr->x + x;
            extr->gameInfo.field[temp_y][temp_x] = extr->currentPiece + 1;
          }
        }
      }
      // уничтожение линий
      check_for_rows(extr);
      // генерация новой фигуры
      create_new_piece(extr);
      // проверка на окончание игры
      extr->over = fit(*extr, extr->rotation, extr->y, extr->x) == 0 ? 1 : 0;
    }
    extr->speed_counter = 0;
  }
}
char get_cell(Extra_info extr, int y, int x) {
  return extr.P[extr.currentPiece][rotate(y, x, extr.rotation)];
}
// Конвертация координат в координату одномернного массива
int rc2i(int y, int x) { return y * WIDTH + x; }
// Проверка на движение
int fit(Extra_info extr, int rot, int row, int col) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      int pi = rotate(y, x, rot);
      int fi = rc2i(row + y, col + x);
      if (extr.P[extr.currentPiece][pi] == 'X' &&
          extr.gameInfo.field[fi / WIDTH][fi % WIDTH] != 0) {
        return 0;
      }
    }
  }

  return 1;
}
// Проверка движения вниз
int fitCol(Extra_info extr, int col) {
  return fit(extr, extr.rotation, extr.y, col);
}
// Проверка движения влево вправо
int fitRow(Extra_info extr, int row) {
  return fit(extr, extr.rotation, row, extr.x);
}
// Проверка вращения
int fitRot(Extra_info extr, int rot) { return fit(extr, rot, extr.y, extr.x); }
// Создание новой фигуры
void create_new_piece(Extra_info *extr) {
  extr->y = 0;
  extr->x = WIDTH / 2;
  extr->rotation = 0;
  extr->currentPiece = extr->nextPiece;
  extr->nextPiece = rand() % 7;
}
// Уничтожение строк
void check_for_rows(Extra_info *extr) {
  int found_line;
  int lines_count = 0;

  for (int y = 0; y < 4; y++) {
    if (extr->y + y < HEIGHT - 1) {
      found_line = 1;
      for (int x = 1; x < WIDTH - 1; x++) {
        if (extr->gameInfo.field[extr->y + y][x] == 0) {
          found_line = 0;
          break;
        }
      }

      if (found_line == 1) {
        lines_count++;
        for (int ly = (extr->y + y) - 1; ly > 0; ly--) {
          for (int lx = 1; lx < WIDTH - 1; lx++) {
            extr->gameInfo.field[ly + 1][lx] = extr->gameInfo.field[ly][lx];
          }
        }
      }
    }
  }
  if (lines_count == 1)
    extr->gameInfo.score += 100;
  else if (lines_count == 2)
    extr->gameInfo.score += 300;
  else if (lines_count == 3)
    extr->gameInfo.score += 700;
  else if (lines_count == 4)
    extr->gameInfo.score += 1500;
  if (extr->gameInfo.score >= extr->gameInfo.level * 600) {
    extr->gameInfo.level++;
  }
}
// Изменение формы
int rotate(int y, int x, int type) {
  switch (type % 4) {
    case 0:
      return y * 4 + x;
    case 1:
      return 12 + y - (x * 4);
    case 2:
      return 15 - (y * 4) - x;
    case 3:
      return 3 - y + (x * 4);
  }
  return 0;
}
// Циклы игры
void newgame(WINDOW *w, Extra_info extr) {
  initializeGameInfo(&extr);
  UserAction_t action;
  startLoop(w, &extr);
  while (extr.over == 0 || extr.gameInfo.level == 10) {
    timing(&extr);
    userInput(action, 0);
    gameLogic(&extr);
    drawField(extr);
  }
  freeField(&extr);
}
// Инициализация игры
void initializeGameInfo(Extra_info *extr) {
  extr->gameInfo.score = 0;
  extr->gameInfo.high_score = 0;
  extr->gameInfo.level = 1;
  extr->gameInfo.speed = 22;
  extr->gameInfo.pause = 0;
  extr->currentPiece = 0;
  extr->nextPiece = 0;
  extr->rotation = 0;
  extr->x = 3;
  extr->y = 0;
  extr->over = 0;
  extr->speed_counter = 0;
  extr->fall = 0;
  strcpy(extr->P[0], "..X...X...X...X.");
  strcpy(extr->P[1], "..X..XX..X......");
  strcpy(extr->P[2], ".X...XX...X.....");
  strcpy(extr->P[3], ".....XX..XX.....");
  strcpy(extr->P[4], "..X..XX...X.....");
  strcpy(extr->P[5], "..X...X..XX.....");
  strcpy(extr->P[6], ".XX...X...X.....");
}
void freeField(Extra_info *extr) {
  if (extr->gameInfo.next != NULL) {
    for (int i = 0; i < 10; i++) free(extr->gameInfo.next[i]);
    free(extr->gameInfo.next);
  }
  if (extr->gameInfo.field != NULL) {
    for (int i = 0; i < HEIGHT; i++) free(extr->gameInfo.field[i]);
    free(extr->gameInfo.field);
  }
}