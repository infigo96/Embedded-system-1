/*
 * break.c
 *
 * Created: 2018-10-22 18:56:33
 *  Author: Albert
 */ 
#include "break.h"
pos pos_Add(pos a,pos b){
	pos c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

int pos_Cmp(pos a, pos b){
	if(a.x == b.x && a.y == b.y)
	return 1;
	return 0;
}
void player1Start(){
	holding = 1;
	ball_pos = pos_Add(pl1_pos,(pos){0,1});
	drawAtPos('O',ball_pos.x,ball_pos.y);
}
void resetBoard(){
	char text[3];
	dip204_clear_display();
	pl1_pos = (pos){4,0};
	pl1_dir = ballmovement = NONE;
	running = p1_aim = 1;
	//Draw game objects
	drawAtPos('W',pl1_pos.x,pl1_pos.y);
	for (int x=11;x<20;x++)
	{
		for (int y=2;y<4;y++)
		{
			drawAtPos('Q',x,y);
			blocks_status[x-11][y-2] = 1;
		}
	}
	ball_tick = -1;
	//Draw border
	for (int i=2;i<4;i++)
	{
		dip204_set_cursor_position(10,i);
		dip204_write_string("HH");
	}
	dip204_set_cursor_position(10,1);
	sprintf(text,"%02d",pl1_score);
	dip204_write_string(text);
	dip204_set_cursor_position(10,4);
	sprintf(text,"%02d",pl1_life);
	dip204_write_string(text);
}
void initGame(){
	pl1_score = 0;
	pl1_life = 4;
	resetBoard(); //Clear the board, resting positions and redraw
	player1Start(); //Set the ball to follow player 1
}
void onTick(){
	unsigned long int local_time = global_time;
	while (1)
	{
		if (local_time==global_time)
		{
			continue;
		}
		local_time = global_time;
		//Read input
		if(global_time%TICKS_PER_INPUT_CHECK)
		{
			//PLAYER 1
			if (getButtonInput(1))
			{
				if(holding == 1)
				{
					holding = 0;
					if (p1_aim==0)
					ballmovement=DOWN;
					else
					ballmovement=RIGHT;
				}
			}
			else if (getButtonInput(0))
			{
				pl1_dir = LEFT;p1_aim=0;
			}
			else if (getButtonInput(2))
			{
				pl1_dir = RIGHT;;p1_aim=1;
			}
			else
			pl1_dir = NONE;
		}
		//Update game
		if (global_time%TICKS_PER_UPDATE==0)
		onUpdate();
	}
	
}

void onUpdate(){
	
	pos pl1_next_pos,ball_next_pos;
	switch(pl1_dir)
	{
		case LEFT:
		pl1_next_pos = pos_Add(pl1_pos,(pos){-1,0});
		break;
		case RIGHT:
		pl1_next_pos = pos_Add(pl1_pos,(pos){1,0});
		break;
		default:
		pl1_next_pos = pl1_pos;
		break;
	}

	//Check bounders
	if(pl1_next_pos.x==-1||pl1_next_pos.x==9)
	pl1_next_pos=pl1_pos;
	if (holding==1)
	{
		ball_next_pos = pos_Add(pl1_next_pos,(pos){0,1});
		if (pos_Cmp(ball_next_pos,ball_pos)==0)
		{
			drawAtPos(' ',ball_pos.x,ball_pos.y);
			drawAtPos('O',ball_next_pos.x,ball_next_pos.y);
		}
		ball_pos = ball_next_pos;
	}
	//Draw the new positions
	if (pos_Cmp(pl1_next_pos,pl1_pos)==0)
	{
		drawAtPos(' ',pl1_pos.x,pl1_pos.y);
		drawAtPos('W',pl1_next_pos.x,pl1_next_pos.y);
	}
	pl1_pos = pl1_next_pos;
	
	
	//Ball movement
	if(holding==0){ball_tick++;}
	
	if(ball_tick%3==0)
	{

		//Bounce against walls
		if (ball_pos.x==0 || ball_pos.x==11)
		{
			if (ballmovement == LEFT)
			ballmovement = UP;
			if(ballmovement == DOWN)
			ballmovement = RIGHT;
		}
		if (ball_pos.x==8 || ball_pos.x==19)
		{
			if (ballmovement == UP)
			ballmovement = LEFT;
			if(ballmovement == RIGHT)
			ballmovement = DOWN;
		}
		switch(ballmovement)
		{
			case UP:
			ball_next_pos = pos_Add(ball_pos,(pos){1,-1});
			break;
			case DOWN:
			ball_next_pos = pos_Add(ball_pos,(pos){-1,1});
			break;
			case LEFT:
			ball_next_pos = pos_Add(ball_pos,(pos){-1,-1});
			break;
			case RIGHT:
			ball_next_pos = pos_Add(ball_pos,(pos){1,1});
			break;
			case NONE:
			ball_next_pos=ball_pos;
			break;
		}
		if (ball_pos.x<10)
		{
			//Hit player flat
			if (pos_Cmp(ball_next_pos,pos_Add(pl1_next_pos,(pos){0,1})))
			{
				ball_next_pos = pos_Add(pl1_next_pos,(pos){0,1});
				if (ballmovement==UP)
				ballmovement=RIGHT;
				if (ballmovement==LEFT)
				ballmovement=DOWN;
			}
			//Hit on corner
			else if (pos_Cmp(ball_next_pos,pl1_next_pos))
			{	
				
				if (ballmovement==UP)
				{
					ballmovement=DOWN;
					ball_next_pos = pos_Add(ball_pos,(pos){-1,1});
				}
				
				if (ballmovement==LEFT)
				{
					ballmovement=RIGHT;
					ball_next_pos = pos_Add(ball_pos,(pos){1,1});
				}
				
			}
			//Outside
			else if(ball_next_pos.y==0){
				//Draw the new positions
				if (pos_Cmp(pl1_next_pos,pl1_pos)==0)
				{
					drawAtPos(' ',pl1_pos.x,pl1_pos.y);
					drawAtPos('W',pl1_next_pos.x,pl1_next_pos.y);
				}
				drawAtPos(' ',ball_pos.x,ball_pos.y);
				drawAtPos('O',ball_next_pos.x,ball_next_pos.y);
				delay(100);
				pl1_life--;
				if (pl1_life==0)
				{
					delay(100);
					initGame();
				}
				else{
					resetBoard();
					player1Start();
				}
				return;
			}
			//Wrap
			else if (ball_next_pos.y==4)
			{
				ball_next_pos.y=0;
				ball_next_pos.x+=11;
			}
		}
		else
		{
			//Outside
			if(ball_next_pos.y>=4){
				if (ballmovement==RIGHT)
				ballmovement=UP;
				if (ballmovement==DOWN)
				ballmovement=LEFT;
				switch(ballmovement)
				{
					case UP:
					ball_next_pos = pos_Add(ball_pos,(pos){1,-1});
					break;
					case DOWN:
					ball_next_pos = pos_Add(ball_pos,(pos){-1,1});
					break;
					case LEFT:
					ball_next_pos = pos_Add(ball_pos,(pos){-1,-1});
					break;
					case RIGHT:
					ball_next_pos = pos_Add(ball_pos,(pos){1,1});
					break;
					case NONE:
					ball_next_pos=ball_pos;
					break;
				}
			}
			//Hit block
			if (Hitbehavior(ball_next_pos)==1)
			{
				char text[6];
				dip204_set_cursor_position(10,1);
				sprintf(text,"%02d",pl1_score);
				dip204_write_string(text);
				//if(checkEmpty()){
					////Board is clear, reset
					//delay(100);
					//resetBoard();
					//player1Start();
				//}
				//A hit has happened, new next position
				switch(ballmovement)
				{
					case UP:
					ball_next_pos = pos_Add(ball_pos,(pos){1,-1});
					break;
					case DOWN:
					ball_next_pos = pos_Add(ball_pos,(pos){-1,1});
					break;
					case LEFT:
					ball_next_pos = pos_Add(ball_pos,(pos){-1,-1});
					break;
					case RIGHT:
					ball_next_pos = pos_Add(ball_pos,(pos){1,1});
					break;
					case NONE:
					ball_next_pos=ball_pos;
					break;
				}
			}
			if(ball_next_pos.y>=4){
				if (ballmovement==RIGHT)
				ballmovement=UP;
				if (ballmovement==DOWN)
				ballmovement=LEFT;
				switch(ballmovement)
				{
					case UP:
					ball_next_pos = pos_Add(ball_pos,(pos){1,-1});
					break;
					case DOWN:
					ball_next_pos = pos_Add(ball_pos,(pos){-1,1});
					break;
					case LEFT:
					ball_next_pos = pos_Add(ball_pos,(pos){-1,-1});
					break;
					case RIGHT:
					ball_next_pos = pos_Add(ball_pos,(pos){1,1});
					break;
					case NONE:
					ball_next_pos=ball_pos;
					break;
				}
			}
			//Wrap
			else if (ball_next_pos.y==-1)
			{
				ball_next_pos.y=3;
				ball_next_pos.x-=11;
			}
		}
	}

	if (pos_Cmp(ball_next_pos,ball_pos)==0)
	{
		drawAtPos(' ',ball_pos.x,ball_pos.y);
		drawAtPos('O',ball_next_pos.x,ball_next_pos.y);
	}

	ball_pos = ball_next_pos;
}
int checkHit(pos thispose){

	for (int x=11;x<20;x++)
	{
		for (int y=2;y<4;y++)
		{
			if (thispose.x == x && thispose.y == y )
			{
				if(blocks_status[x-11][y-2] == 1){
					return 1;
				}
			}
		}
	}
	return 0;
}
int Hitbehavior(pos next_pos){
	pos check1,check2;
	switch(ballmovement)
	{
		case RIGHT:
		//x+y+
		check1 = pos_Add(ball_pos,(pos){1,0});
		check2 = pos_Add(ball_pos,(pos){0,1});
		break;
		case LEFT:
		//x-y-
		check1 = pos_Add(ball_pos,(pos){-1,0});
		check2 = pos_Add(ball_pos,(pos){0,-1});
		break;
		case UP:
		//x+y-
		check1 = pos_Add(ball_pos,(pos){1,0});
		check2 = pos_Add(ball_pos,(pos){0,-1});
		break;
		case DOWN:
		//x-y+
		check1 = pos_Add(ball_pos,(pos){-1,0});
		check2 = pos_Add(ball_pos,(pos){0,1});
		break;
		default:
		break;
	}
	if(checkHit(check1))
	{
		if(checkHit(check2))
		{
			//Two blocks are hit
			pl1_score+=2;
			blocks_status[check1.x-11][check1.y-2] = 0;
			blocks_status[check2.x-11][check2.y-2] = 0;
			drawAtPos(' ',check1.x,check1.y);
			drawAtPos(' ',check2.x,check2.y);
			if (ballmovement==RIGHT)
			ballmovement = LEFT;
			else if (ballmovement==UP)
			ballmovement = DOWN;
			else if (ballmovement==LEFT)
			ballmovement = RIGHT;
			else if (ballmovement==DOWN)
			ballmovement = UP;
		}
		else{
			//Only check1 was hit
			pl1_score+=1;
			blocks_status[check1.x-11][check1.y-2] = 0;
			drawAtPos(' ',check1.x,check1.y);
			if (ballmovement==RIGHT)
			ballmovement = DOWN;
			else if (ballmovement==DOWN)
			ballmovement = RIGHT;
			else if (ballmovement==UP)
			ballmovement = LEFT;
			else if (ballmovement==LEFT)
			ballmovement = UP;
		}
	}
	else if(checkHit(check2)){
		//Only check2 was hit
		pl1_score+=1;
		blocks_status[check2.x-11][check2.y-2] = 0;
		drawAtPos(' ',check2.x,check2.y);
		if (ballmovement==RIGHT)
		ballmovement = UP;
		else if (ballmovement==UP)
		ballmovement = RIGHT;
		else if (ballmovement==LEFT)
		ballmovement = DOWN;
		else if (ballmovement==DOWN)
		ballmovement = LEFT;
	}
	else if(checkHit(next_pos)){
		//Corner hit
		pl1_score+=1;
		blocks_status[next_pos.x-11][next_pos.y-2] = 0;
		drawAtPos(' ',next_pos.x,next_pos.y);
		if (ballmovement==RIGHT)
		ballmovement = LEFT;
		else if (ballmovement==UP)
		ballmovement = DOWN;
		else if (ballmovement==LEFT)
		ballmovement = RIGHT;
		else if (ballmovement==DOWN)
		ballmovement = UP;
	}
	else{
		//No blocks were hit
		return 0;
	}
	//Bounce against walls
	if (ball_pos.x==0 || ball_pos.x==11)
	{
		if (ballmovement == LEFT)
		ballmovement = UP;
		if(ballmovement == DOWN)
		ballmovement = RIGHT;
	}
	if (ball_pos.x==8 || ball_pos.x==19)
	{
		if (ballmovement == UP)
		ballmovement = LEFT;
		if(ballmovement == RIGHT)
		ballmovement = DOWN;
	}
	if(next_pos.y>=4){
		if (ballmovement==RIGHT)
		ballmovement=UP;
		if (ballmovement==DOWN)
		ballmovement=LEFT;
	}
	return 1;
}
int checkEmpty(){
	for (int x=11;x<20;x++)
	{
		for (int y=2;y<4;y++)
		{
			if(blocks_status[x-11][y-2] == 1){
				return 1;
			}
		}
	}
	return 1;
}