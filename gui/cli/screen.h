#ifndef SCREEN_H
#define SCREEN_H
#define WIDTH 12
#define HEIGHT 21
#include "../../brick_game/tetris/tetris.h"
void drawField(Extra_info extr);
WINDOW *get_main_window();
int menu(WINDOW *w);
void fill(int y, int x, int color);
void pauseMenu();
void renderNext(Extra_info extr);
void startLoop(WINDOW *w, Extra_info *extr);
GameInfo_t updateCurrentState(GameInfo_t *gameInfo);
#endif