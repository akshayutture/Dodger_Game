/* This file holds the functions for creating a player or enemy window at the given x-y co-ordinate.
	It also holds a function to destroy a window (it may be player or enemy)
	
	All design changes to the enemy and player shapes may be made here.
*/

#include <ncurses.h>
#include <stdlib.h>


//creates a new player window at the specified postion
WINDOW *create_playerwin(int starty, int startx)
{	
	//define the color
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	//initialize values
	WINDOW *local_win;
	int height=3,width=5;
	//create the new window
	local_win = newwin(height, width, starty, startx);
	//set background color
	wbkgd(local_win, COLOR_PAIR(1));
	
	//this creates the shape of the player
	wprintw(local_win,"--^-- (0) [] []");
	wrefresh(local_win);		

	return local_win;
}

//creates a new enemy window at the specified position
WINDOW *create_enemywin(int starty, int startx)
{	
	//define the color
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	//initialize values
	WINDOW *local_win;
	int height=5,width=7;
	//create the new window
	local_win = newwin(height, width, starty, startx);
	//this creates the shape of the player
	wprintw(local_win,"|-----||     ||     ||     ||-----|");					 
	//set background color				 
	wbkgd(local_win, COLOR_PAIR(2));
	wrefresh(local_win);		

	return local_win;
}

//this destroys the given window
void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

