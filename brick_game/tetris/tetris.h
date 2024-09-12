#ifndef TETRIS_H
#define TETRIS_H
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define ESCAPE 27
#define ENTER_KEY 10
// структура ввода пользователя
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;
// структура для состояния игры
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;
// структура для доп информации
typedef struct {
  GameInfo_t gameInfo;
  int currentPiece;
  int nextPiece;
  int rotation;
  int x;
  int y;
  int over;
  int speed_counter;
  int fall;
  char P[7][16];
} Extra_info;

UserAction_t getAction(int user_input);
void userInput(UserAction_t action, bool hold);

void gameLogic(Extra_info *extr);
void timing(Extra_info *extr);
char get_cell(Extra_info extr, int y, int x);
int rc2i(int y, int x);
int fit(Extra_info extr, int rot, int row, int col);
int fitCol(Extra_info extr, int col);
int fitRow(Extra_info extr, int row);
int fitRot(Extra_info extr, int rot);
void create_new_piece(Extra_info *extr);
void check_for_rows(Extra_info *extr);
void fill(int y, int x, int color);
int rotate(int y, int x, int type);
void newgame(WINDOW *w, Extra_info extr);
void initializeGameInfo(Extra_info *extr);
void freeField(Extra_info *extr);
#endif