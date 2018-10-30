/*
 * Game.c
 *
 * Created: 2018-10-08 17:48:12
 *  Author: Albert
 */ 
#include "snake.h"
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

void initGame(){
	srand(time(NULL));
	dip204_clear_display();
	snake_dir = LEFT;
	actual_dir = snake_dir;
	running = 1;
	score=0;
	head = tail = (part*)calloc(sizeof(part),1);
	pos	start_pos = {START_X,START_Y};
	tail->p = start_pos;
	drawAtPos('O',start_pos.x,start_pos.y);
	addPart(pos_Add(start_pos,(pos){-1,0}));
	addPart(pos_Add(start_pos,(pos){-2,0}));
		
	//Create food at an unoccupied place

	do 
	{
		food_pos = (pos){rand() % 20,rand() % 4};
	} while (isColliding(food_pos));
	drawAtPos('X',food_pos.x,food_pos.y);
}

void addPart(pos p)
{
	part * newpart = (part*)calloc(sizeof(part),1);
	newpart->p = p;
	head->next = newpart;
	head = newpart;
	drawAtPos('O',p.x,p.y);
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
			DIRECTIONS input = getJoystickInput();
			switch (input)
			{
				case UP:
					if (actual_dir!=DOWN)
					{
						snake_dir = input;
					}
				break;
				case DOWN:
					if (actual_dir!=UP)
					{
						snake_dir = input;
					}
				break;
				case LEFT:
					if (actual_dir!=RIGHT)
					{
						snake_dir = input;
					}
					break;
				case RIGHT:
					if (actual_dir!=LEFT)
					{
						snake_dir = input;
					}
					break;
				break;
				default:
				break;
			}
			if (getButtonInput(0))
			{
				freeSpace(tail);
				initGame();
			}
		}
		//Update game
		if (global_time%TICKS_PER_UPDATE==0 && running)
			onUpdate();
	}
	
}

void unaddress(part ** i){
	*i = NULL;
}
void onUpdate(){
	
	pos next_pos;
	actual_dir = snake_dir;
	switch (actual_dir)
	{
		case UP:
			next_pos = pos_Add(head->p, (pos){0,-1});
			break;
		case DOWN:
			next_pos = pos_Add(head->p, (pos){0,1});
			break;
		case LEFT:
			next_pos = pos_Add(head->p, (pos){-1,0});
			break;
		case RIGHT:
			next_pos = pos_Add(head->p, (pos){1,0});
			break;
		default:
		break;
	}
	//Wrap position around if out of bounds
	if(next_pos.x>19){
		next_pos.x=0;
	}
	if (next_pos.x<0)
	{
		next_pos.x=19;
	}
	if (next_pos.y>3)
	{
		next_pos.y=0;
	}
	if (next_pos.y<0)
	{
		next_pos.y=3;
	}
	//Check for contact with food
	if(pos_Cmp(next_pos,food_pos))
	{
		addPart(next_pos);
		//Create food at an unoccupied place
		do
		{
			food_pos = (pos){rand() % 20,rand() % 4};
		} while (isColliding(food_pos));
		drawAtPos('X',food_pos.x,food_pos.y);
		score++;
	}
	//Check for contact with itself
	else if(isColliding(next_pos)&&!pos_Cmp(next_pos,tail->p))
	{
		drawAtPos('W',head->p.x,head->p.y);
		//Print the score
		dip204_set_cursor_position(1,4);
		char temp[20];
		sprintf(temp,"Score:%d",score);
		dip204_write_string(temp);
		running = 0;
	}
	//Movement
	else
	{
		
		head->next = tail;
		drawAtPos(' ',tail->p.x,tail->p.y); //Clear the old tail
		tail->p = next_pos;
		part * temp = tail->next;
		unaddress(&tail->next);
		drawAtPos('O',tail->p.x,tail->p.y); //Display the new head
		
		head = tail;
		tail = temp;
	}
	
	
}

int isColliding(pos pos_to_detech)
{
	part * i;
	i = tail;
	while (i != NULL)
	{
		if (pos_Cmp(pos_to_detech,i->p))
		{
			return 1;
		}
		i = i->next;
	}
	return 0;
}
void freeSpace(part *i){
	if (i==NULL)
		return;
	freeSpace(i->next);
	free(i);
}