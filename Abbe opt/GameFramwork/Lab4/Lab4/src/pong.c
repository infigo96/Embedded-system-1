/*
 * pong.c
 *
 * Created: 2018-10-15 18:04:15
 *  Author: Albert
 */ 
#include "pong.h"
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
void player2Start(){
	holding = 2;
	ball_pos = pos_Add(pl2_pos,(pos){0,-1});
	drawAtPos('O',ball_pos.x,ball_pos.y);
}
void resetBoard(){
		char text[3];
		dip204_clear_display();
		pl1_pos = (pos){4,0};
		pl2_pos = (pos){19-4,3};
		pl1_dir = pl2_dir = ballmovement = NONE;
		running = p1_aim = p2_aim = 1;
		//Draw game objects
		drawAtPos('W',pl1_pos.x,pl1_pos.y);
		drawAtPos('M',pl2_pos.x,pl2_pos.y);
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
		sprintf(text,"%02d",pl2_score);
		dip204_write_string(text);
}
void initGame(){
	pl1_score = pl2_score=0;
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

			//PLAYER 2
			DIRECTIONS input = getJoystickInput();
			switch (input)
			{
				case DOWN:
					if(holding == 2){
						holding = 0;
						if (p2_aim==0)
							ballmovement=LEFT;
						else
							ballmovement=UP;
					}
				break;
				case LEFT:
					pl2_dir = LEFT;p2_aim=0;
				break;
				case RIGHT:
					pl2_dir = RIGHT;p2_aim=1;
					break;
				break;
				case NONE:
					pl2_dir = NONE;
				break;
				default:
				break;
			}
		}
		//Update game
		if (global_time%TICKS_PER_UPDATE==0)
			onUpdate();
	}
	
}

void onUpdate(){
	
	pos pl1_next_pos,pl2_next_pos,ball_next_pos;
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
	switch(pl2_dir)
	{
		case LEFT:
			pl2_next_pos = pos_Add(pl2_pos,(pos){-1,0});
		break;
		case RIGHT:
			pl2_next_pos = pos_Add(pl2_pos,(pos){1,0});
		break;
		default:
			pl2_next_pos = pl2_pos;
		break;
	}

	//Check bounders
	if(pl1_next_pos.x==-1||pl1_next_pos.x==9)
		pl1_next_pos=pl1_pos;
	if(pl2_next_pos.x==10||pl2_next_pos.x==20)
		pl2_next_pos=pl2_pos;
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
	else if (holding==2)
	{
		ball_next_pos = pos_Add(pl2_next_pos,(pos){0,-1});
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
	if (pos_Cmp(pl2_next_pos,pl2_pos)==0)
	{
		drawAtPos(' ',pl2_pos.x,pl2_pos.y);
		drawAtPos('M',pl2_next_pos.x,pl2_next_pos.y);
	}
	pl1_pos = pl1_next_pos;
	pl2_pos = pl2_next_pos;
	
	
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
			//Hit player
			if (pos_Cmp(ball_next_pos,pos_Add(pl1_next_pos,(pos){0,1})) || pos_Cmp(ball_next_pos,pl1_next_pos))
			{
				ball_next_pos = pos_Add(pl1_next_pos,(pos){0,1});
				if (ballmovement==UP)
					ballmovement=RIGHT;
				if (ballmovement==LEFT)
					ballmovement=DOWN;
			}
			//Outside
			else if(ball_next_pos.y==0){
				pl2_score++;
				//Draw the new positions
				if (pos_Cmp(pl1_next_pos,pl1_pos)==0)
				{
					drawAtPos(' ',pl1_pos.x,pl1_pos.y);
					drawAtPos('W',pl1_next_pos.x,pl1_next_pos.y);
				}
				if (pos_Cmp(pl2_next_pos,pl2_pos)==0)
				{
					drawAtPos(' ',pl2_pos.x,pl2_pos.y);
					drawAtPos('M',pl2_next_pos.x,pl2_next_pos.y);
				}
				drawAtPos(' ',ball_pos.x,ball_pos.y);
				drawAtPos('O',ball_next_pos.x,ball_next_pos.y);
				delay(100);
				resetBoard();player1Start();return;
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
			//Hit player
			if (pos_Cmp(ball_next_pos,pos_Add(pl2_next_pos,(pos){0,-1}))||pos_Cmp(ball_next_pos,pl2_next_pos))
			{
				ball_next_pos = pos_Add(pl2_next_pos,(pos){0,-1});
				if (ballmovement==RIGHT)
				ballmovement=UP;
				if (ballmovement==DOWN)
				ballmovement=LEFT;
			}
			//Outside
			else if(ball_next_pos.y==4){
				pl1_score++;
				//Draw the new positions
				if (pos_Cmp(pl1_next_pos,pl1_pos)==0)
				{
					drawAtPos(' ',pl1_pos.x,pl1_pos.y);
					drawAtPos('W',pl1_next_pos.x,pl1_next_pos.y);
				}
				if (pos_Cmp(pl2_next_pos,pl2_pos)==0)
				{
					drawAtPos(' ',pl2_pos.x,pl2_pos.y);
					drawAtPos('M',pl2_next_pos.x,pl2_next_pos.y);
				}
				delay(100);
				resetBoard();
				player2Start();
				return;
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