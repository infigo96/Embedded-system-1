/*
 * break.h
 *
 * Created: 2018-10-22 18:56:58
 *  Author: Albert
 */ 


#ifndef BREAK_H_
#define BREAK_H_
#include "game_framwork.h"
#define TICKS_PER_UPDATE 8 //Update the screen
#define TICKS_PER_INPUT_CHECK 2 //Get input


//Variables for the game
//typedef struct pos_ pos;
//
//struct pos_{
	//int x,y;
//};
pos pos_Add(pos a,pos b);
int pos_Cmp(pos a, pos b);

enum DIRECTIONS pl1_dir,pl2_dir,ballmovement;
pos ball_pos,pl1_pos;
int p1_aim;
int blocks_status [9][2];
int running,pl1_score,pl1_life;
int holding;
int ball_tick;
//These must be present for the framework
void initGame();
void onTick();
void onUpdate();
//Other functions
void resetBoard();
void player1Start();
int Hitbehavior(pos next_pos);
int checkEmpty();
#endif /* BREAK_H_ */