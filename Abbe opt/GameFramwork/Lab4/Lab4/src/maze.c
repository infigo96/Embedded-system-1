/*
 * maze.c
 *
 * Created: 2018-10-30 09:38:05
 *  Author: Albert
 */
#include "maze.h"
typedef struct
{
	int x, y; //Node position - little waste of memory, but it allows faster generation
	void *parent; //Pointer to parent node
	char c; //Character to be displayed
	char dirs; //Directions that still haven't been explored
} Node;

Node *nodes; //Nodes array
int width, height; //Maze dimensions
Node *start, *last;

int init()
{
	int i, j;
	Node *n;
	
	//Allocate memory for maze
	nodes = calloc( width * height, sizeof( Node ) );
	if ( nodes == NULL ) return 1;
	
	//Setup crucial nodes
	for ( i = 0; i < width; i++ )
	{
		for ( j = 0; j < height; j++ )
		{
			n = nodes + i + j * width;
			if ( i * j % 2 )
			{
				n->x = i;
				n->y = j;
				n->dirs = 15; //Assume that all directions can be explored (4 youngest bits set)
				n->c = ' ';
			}
			else n->c = '#'; //Add walls between nodes
		}
	}
	return 0;
}

Node *link( Node *n )
{
	//Connects node to random neighbor (if possible) and returns
	//address of next node that should be visited

	int x, y;
	char dir;
	Node *dest;
	
	//Nothing can be done if null pointer is given - return
	if ( n == NULL ) return NULL;
	
	//While there are directions still unexplored
	while ( n->dirs )
	{
		//Randomly pick one direction
		dir = ( 1 << ( rand( ) % 4 ) );
		
		//If it has already been explored - try again
		if ( ~n->dirs & dir ) continue;
		
		//Mark direction as explored
		n->dirs &= ~dir;
		
		//Depending on chosen direction
		switch ( dir )
		{
			//Check if it's possible to go right
			case 1:
			if ( n->x + 2 < width )
			{
				x = n->x + 2;
				y = n->y;
			}
			else continue;
			break;
			
			//Check if it's possible to go down
			case 2:
			if ( n->y + 2 < height )
			{
				x = n->x;
				y = n->y + 2;
			}
			else continue;
			break;
			
			//Check if it's possible to go left
			case 4:
			if ( n->x - 2 >= 0 )
			{
				x = n->x - 2;
				y = n->y;
			}
			else continue;
			break;
			
			//Check if it's possible to go up
			case 8:
			if ( n->y - 2 >= 0 )
			{
				x = n->x;
				y = n->y - 2;
			}
			else continue;
			break;
		}
		
		//Get destination node into pointer (makes things a tiny bit faster)
		dest = nodes + x + y * width;
		
		//Make sure that destination node is not a wall
		if ( dest->c == ' ' )
		{
			//If destination is a linked node already - abort
			if ( dest->parent != NULL ) continue;
			
			//Otherwise, adopt node
			dest->parent = n;
			
			//Remove wall between nodes
			nodes[n->x + ( x - n->x ) / 2 + ( n->y + ( y - n->y ) / 2 ) * width].c = ' ';
			
			//Return address of the child node
			return dest;
		}
	}
	
	//If nothing more can be done here - return parent's address
	return n->parent;
}
void draw()
{
	int i, j;
	
	for ( i = 0; i < height; i++ )
	{
		for ( j = 0; j < width; j++ )
		{
			dip204_set_cursor_position(j+1,i+1);
			dip204_write_string(nodes[j + i * width].c );
		}
	}
}

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
void resetBoard(){
	init();
	start = nodes + 1 + width;
	start->parent = start;
	last = start;
	while ( ( last = link( last ) ) != start );
	dip204_clear_display();
	pl1_pos = (pos){4,0};
	pl1_dir = ballmovement = NONE;
	running = 1;
	//Draw game objects
	drawAtPos('W',pl1_pos.x,pl1_pos.y);
}
void initGame(){
	height = HEIGHT;
	width = WIDTH;
	resetBoard(); //Clear the board, resting positions and redraw
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

void onUpdate(){}