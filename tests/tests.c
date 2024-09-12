#include "tests.h"

START_TEST(start_game_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  ck_assert_int_eq(extr.gameInfo.score, 0);
  ck_assert_int_eq(extr.gameInfo.high_score, 0);
  ck_assert_int_eq(extr.gameInfo.level, 1);
  ck_assert_int_eq(extr.gameInfo.speed, 22);
  ck_assert_int_eq(extr.gameInfo.pause, 0);
}
END_TEST

START_TEST(getAction_test) {
  ck_assert_int_eq(getAction('e'), Action);
  ck_assert_int_eq(getAction(KEY_DOWN), Down);
  ck_assert_int_eq(getAction(KEY_LEFT), Left);
  ck_assert_int_eq(getAction(KEY_RIGHT), Right);
  ck_assert_int_eq(getAction(27), Terminate);
  ck_assert_int_eq(getAction(10), Start);
  ck_assert_int_eq(getAction('p'), Pause);
}
END_TEST
START_TEST(free_memory_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.gameInfo.next = (int **)malloc((sizeof(int *)) * 10);
  for (int i = 0; i < 10; i++) {
    extr.gameInfo.next[i] = (int *)malloc((sizeof(int)) * 20);
    for (int j = 0; j < 20; j++) {
      extr.gameInfo.next[i][j] = 0;
    }
  }
  freeField(&extr);
  ck_assert_int_eq(extr.gameInfo.score, 0);
}
END_TEST
START_TEST(over_game_test) {
  Extra_info extr;
  UserAction_t action = getAction(KEY_DOWN);
  initializeGameInfo(&extr);
  extr.gameInfo.speed = 5;
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.gameInfo.next = (int **)malloc((sizeof(int *)) * 10);
  for (int i = 0; i < 10; i++) {
    extr.gameInfo.next[i] = (int *)malloc((sizeof(int)) * 20);
    for (int j = 0; j < 20; j++) {
      extr.gameInfo.next[i][j] = 0;
    }
  }
  extr.nextPiece = rand() % 7;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      extr.gameInfo.field[y][x] =
          (x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) ? 9 : 0;
    }
  }
  while (extr.over == 0) {
    timing(&extr);
    userInput(action, 0);
    gameLogic(&extr);
    drawField(extr);
  }
  ck_assert_int_eq(extr.over, 1);
  for (int i = 0; i < 10; i++) free(extr.gameInfo.next[i]);
  free(extr.gameInfo.next);
  for (int i = 0; i < HEIGHT; i++) free(extr.gameInfo.field[i]);
  free(extr.gameInfo.field);
}
END_TEST

START_TEST(move_figure_test) {
  Extra_info extr;
  UserAction_t action;
  initializeGameInfo(&extr);
  extr.gameInfo.speed = 5;
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.gameInfo.next = (int **)malloc((sizeof(int *)) * 10);
  for (int i = 0; i < 10; i++) {
    extr.gameInfo.next[i] = (int *)malloc((sizeof(int)) * 20);
    for (int j = 0; j < 20; j++) {
      extr.gameInfo.next[i][j] = 0;
    }
  }
  extr.nextPiece = rand() % 7;
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      extr.gameInfo.field[y][x] =
          (x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) ? 9 : 0;
    }
  }
  while (extr.over == 0) {
    timing(&extr);
    userInput(action, 0);
    action = Right;
    gameLogic(&extr);
    drawField(extr);
    action = Left;
  }
  ck_assert_int_eq(extr.over, 1);
  for (int i = 0; i < 10; i++) free(extr.gameInfo.next[i]);
  free(extr.gameInfo.next);
  for (int i = 0; i < HEIGHT; i++) free(extr.gameInfo.field[i]);
  free(extr.gameInfo.field);
}
END_TEST
START_TEST(rotate_figure_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  extr.rotation = 1;
  rotate(2, 0, extr.rotation);
  extr.rotation = 2;
  rotate(2, 0, extr.rotation);
  extr.rotation = 3;
  rotate(2, 0, extr.rotation);
  ck_assert_int_eq(extr.over, 0);
}
END_TEST

START_TEST(score_100_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.currentPiece = 0;
  for (int j = 2; j < 11; j++) {
    extr.gameInfo.field[19][j] = 1;
  }
  if (fitRot(extr, extr.rotation + 1) == 1) extr.rotation++;
  for (int i = 0; i < 11; i++) {
    if (extr.x > 0) {
      if (fitCol(extr, extr.x - 1) == 1) extr.x--;
    }
    extr.x--;
  }
  extr.y = 16;
  extr.gameInfo.field[19][1] = 1;
  extr.gameInfo.field[18][1] = 1;
  extr.gameInfo.field[17][1] = 1;
  extr.gameInfo.field[16][1] = 1;
  check_for_rows(&extr);
  ck_assert_int_eq(extr.gameInfo.score, 100);
  for (int i = 0; i < HEIGHT; i++) free(extr.gameInfo.field[i]);
  free(extr.gameInfo.field);
}
END_TEST

START_TEST(score_300_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.currentPiece = 0;
  for (int j = 2; j < 11; j++) {
    extr.gameInfo.field[19][j] = 1;
    extr.gameInfo.field[18][j] = 1;
  }
  if (fitRot(extr, extr.rotation + 1) == 1) extr.rotation++;
  for (int i = 0; i < 11; i++) {
    if (extr.x > 0) {
      if (fitCol(extr, extr.x - 1) == 1) extr.x--;
    }
    extr.x--;
  }
  extr.y = 16;
  extr.gameInfo.field[19][1] = 1;
  extr.gameInfo.field[18][1] = 1;
  extr.gameInfo.field[17][1] = 1;
  extr.gameInfo.field[16][1] = 1;
  check_for_rows(&extr);
  ck_assert_int_eq(extr.gameInfo.score, 300);
  for (int i = 0; i < HEIGHT; i++) free(extr.gameInfo.field[i]);
  free(extr.gameInfo.field);
}
END_TEST
START_TEST(score_700_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.currentPiece = 0;
  for (int j = 2; j < 11; j++) {
    extr.gameInfo.field[19][j] = 1;
    extr.gameInfo.field[18][j] = 1;
    extr.gameInfo.field[17][j] = 1;
  }
  if (fitRot(extr, extr.rotation + 1) == 1) extr.rotation++;
  for (int i = 0; i < 11; i++) {
    if (extr.x > 0) {
      if (fitCol(extr, extr.x - 1) == 1) extr.x--;
    }
    extr.x--;
  }
  extr.y = 16;
  extr.gameInfo.field[19][1] = 1;
  extr.gameInfo.field[18][1] = 1;
  extr.gameInfo.field[17][1] = 1;
  extr.gameInfo.field[16][1] = 1;
  check_for_rows(&extr);
  ck_assert_int_eq(extr.gameInfo.score, 700);
  for (int i = 0; i < HEIGHT; i++) free(extr.gameInfo.field[i]);
  free(extr.gameInfo.field);
}
END_TEST
START_TEST(score_1500_test) {
  Extra_info extr;
  initializeGameInfo(&extr);
  extr.gameInfo.field = (int **)malloc((sizeof(int *)) * HEIGHT);
  for (int i = 0; i < HEIGHT; i++) {
    extr.gameInfo.field[i] = (int *)malloc((sizeof(int)) * WIDTH);
    for (int j = 0; j < WIDTH; j++) {
      extr.gameInfo.field[i][j] = 0;
    }
  }
  extr.currentPiece = 0;
  for (int j = 2; j < 11; j++) {
    extr.gameInfo.field[19][j] = 1;
    extr.gameInfo.field[18][j] = 1;
    extr.gameInfo.field[17][j] = 1;
    extr.gameInfo.field[16][j] = 1;
  }
  if (fitRot(extr, extr.rotation + 1) == 1) extr.rotation++;
  for (int i = 0; i < 11; i++) {
    if (extr.x > 0) {
      if (fitCol(extr, extr.x - 1) == 1) extr.x--;
    }
    extr.x--;
  }
  extr.y = 16;
  extr.gameInfo.field[19][1] = 1;
  extr.gameInfo.field[18][1] = 1;
  extr.gameInfo.field[17][1] = 1;
  extr.gameInfo.field[16][1] = 1;
  check_for_rows(&extr);
  ck_assert_int_eq(extr.gameInfo.score, 1500);
  ck_assert_int_eq(extr.gameInfo.level, 2);
  for (int i = 0; i < HEIGHT; i++) free(extr.gameInfo.field[i]);
  free(extr.gameInfo.field);
}
END_TEST
Suite *s21_game_suite(void) {
  Suite *s = suite_create("\033[45m-=S21_GAME_TEST=-\033[0m");
  TCase *tc;

  tc = tcase_create("s21_game");
  tcase_add_test(tc, start_game_test);
  tcase_add_test(tc, free_memory_test);
  tcase_add_test(tc, over_game_test);
  tcase_add_test(tc, move_figure_test);
  tcase_add_test(tc, getAction_test);
  tcase_add_test(tc, rotate_figure_test);
  tcase_add_test(tc, score_100_test);
  tcase_add_test(tc, score_300_test);
  tcase_add_test(tc, score_700_test);
  tcase_add_test(tc, score_1500_test);
  suite_add_tcase(s, tc);

  return s;
}