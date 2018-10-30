/*
 * maze.h
 *
 * Created: 2018-10-30 09:38:15
 *  Author: Albert
 */ 


#ifndef MAZE_H_
#define MAZE_H_
#include "game_framwork.h"
#define TICKS_PER_UPDATE 8 //Update the screen
#define TICKS_PER_INPUT_CHECK 2 //Get input
#define HEIGHT 4
#define WIDTH 20

//Variables for the game
//typedef struct pos_ pos;
//
//struct pos_{
//int x,y;
//};
pos pos_Add(pos a,pos b);
int pos_Cmp(pos a, pos b);

enum DIRECTIONS pl1_dir;
pos pl1_pos;
int running;
//These must be present for the framework
void initGame();
void onTick();
void onUpdate();
//Other functions
void resetBoard();




#endif /* MAZE_H_ */