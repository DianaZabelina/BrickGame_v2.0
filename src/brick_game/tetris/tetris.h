#ifndef TETRIS_H
#define TETRIS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../library_specification.h"

#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#ifdef DESKTOP_VERSION
#define FRAME_DELAY_NANO 1000000
#else
#define FRAME_DELAY_NANO 10000000
#endif

#define FIGURES_COUNT 7
#define FIGURE_SIZE 5
#define FIGURE_AREA 25

typedef struct Figure {
  int x;
  int y;
  int size;
  char* blocks;
} Figure;

typedef struct Field {
  int width;
  int height;
  char* blocks;
} Field;

enum GAME_STATE { GAMEOVER, PLAYING };

typedef struct Game {
  Field* field;
  Figure* figure;
  int speed;
  char playing;
  int score;
  int high_score;
} Game;

char* getFigureFromTemplate(int i);
Field* createField();
Figure* createFigure();
Figure* createNextFigure();
Figure* freeFigure(Figure* figure);
Figure* nextFigure(bool create);
Figure* dropFigure(Figure* figure);
Game* createGame();
Game* getGame();
GameInfo_t* getGameInfo();
void updateNextFigureInfo();
void resetSingletones();
void freeSingletones();
void moveFigureDown();
void moveFigureUp();
void moveFigureRight();
void moveFigureLeft();
char figureCollision();
void plantFigure();
char lineIsFull(int i, Field* field);
void shiftLine(int i, Field* field);
int eraseLines();
Figure* rotateFigure();
void compareHighScores();
void calculateTurn();
long* getFrameDelayLeft();
long timeDifference(const struct timespec* start, const struct timespec* end);

#endif