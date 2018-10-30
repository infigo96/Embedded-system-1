/*
 * game.h
 *
 * Created: 2018-10-08 18:13:02
 *  Author: Albert
 */ 


#ifndef SNAKE_H_
#define SNAKE_H_
#include "game_framwork.h"
#define TICKS_PER_UPDATE 10 //Update the screen
#define TICKS_PER_INPUT_CHECK 3 //Get input

//Starting pos for snake
#define START_X 14
#define START_Y 1


//Variables for the game
typedef struct pos_ pos;

struct pos_{
	int x,y;
};
pos pos_Add(pos a,pos b);
int pos_Cmp(pos a, pos b);

typedef struct snake_part_ part;

struct snake_part_{
	pos p;
	part *next;
};

enum DIRECTIONS snake_dir,actual_dir;
part * head;
part * tail;
pos food_pos;
int running,score;


//These must be present for the framework
void initGame();
void onTick();
void onUpdate();
//Other functions
void addPart(pos p);
int isColliding(pos pos_to_detech);
void freeSpace(part *i);
#endif /* GAME_H_ */