/*
 * pong.h
 *
 * Created: 2018-10-15 18:04:31
 *  Author: Albert
 */ 


#ifndef PONG_H_
#define PONG_H_
#include "game_framwork.h"
#define TICKS_PER_UPDATE 10 //Update the screen
#define TICKS_PER_INPUT_CHECK 3 //Get input


//Variables for the game
typedef struct pos_ pos;

struct pos_{
	int x,y;
};
pos pos_Add(pos a,pos b);
int pos_Cmp(pos a, pos b);

enum DIRECTIONS pl1_dir,pl2_dir,ballmovement;
pos ball_pos,pl1_pos,pl2_pos;
int p1_aim, p2_aim;
int running,pl1_score,pl2_score;
int holding;
int ball_tick;
//These must be present for the framework
void initGame();
void onTick();
void onUpdate();
//Other functions
void resetBoard();
void player1Start();
void player2Start();
#endif /* PONG_H_ */