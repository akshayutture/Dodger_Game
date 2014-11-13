/*
DODGER GAME
Author: Akshay Utture
Roll No: cs13b031

 Tasks completed: 

-color added. 
-boundaries added. 
-Death condition added. 
-pause button added. 
-Basic start menu added. 
-High scores page done. 
-Difference from version 8 is that the actual high scores page is hidden, but a fake high scores page is printed and continuously updated.
*/

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include<sys/time.h>
#include <string.h>

/* This file holds the functions for creating a player or enemy window at the given x-y co-ordinate.
	It also holds a function to destroy a window (it may be player or enemy)*/
#include "create_destroy_windows.h"
/* This has 1 function to display the start screen and 1 to display the end screen (high scores) */
#include "start_end_screen.h"

//explanation for these functions is given when they are defined
int drop_enemy(int *address_score,int player_y,int player_x);
double get_time_gap();
void swap_strings( char *a, char *b);
void swap_numbers(int *a,int *b);

//used to space out the dropping of enemies
struct timeval time1,time2;


int main(int argc, char *argv[])
{	
	//initializing
	initscr(); 				//starts ncurses			
	cbreak();				//allows input to be accessed as soon as it is typed without having to press the 'enter' key		
	keypad(stdscr, TRUE); 	//allows arrow keys and F1,F2,etc to be read
	noecho();   			//does not echo the typed characters on the screen
	curs_set(0); 			//does not display the curser
	start_color();  		//starts colour functionality
	
	
	start_screen();			//calls the start screen function (in start_end_screen.c)
	
	//variable declaration
	WINDOW *my_win,*enemy_win;
	int startx, starty;
	int ch,end_game=0,first_time=1;
	int score=0;
	time_t t;
	//seed the random function
	srand((unsigned) time(&t));
		
	//initial placement of player is in the centre of the screen
	starty = (LINES) / 2;	
	startx = (COLS) / 2;	
	refresh();
	//create player
	my_win = create_playerwin(starty, startx);
	
	
	//this is to space out the enemies to be dropped and make sure that even if a value for getchar() isnt enetered the game loop continues. i.e. enemies continue to be dropped. - but remember with just this, the game will do a loop without this delay if the player gives an input in less than 0.5 seconds. So this case is handled later. Lets call it 'case1'. . 
	/* So if this is changed for difficulty also correspondingly change the place where case1 is handled where get_time_gap() is called in main*/
	halfdelay(5);	//this waits for 0.5 seconds for an input, if no input is given, it skips the scan statement and continues the program
	
	//game loop - ch holds the currently entered button
	while((ch = getch()) != 'q')
	{	
		//for movement - the player window is destroyed and recreated at the new position
		switch(ch)
		{	case KEY_LEFT:
				if (startx==0) //left side boundary for player
				{
					break;
				}
				destroy_win(my_win);
				my_win = create_playerwin(starty,--startx);
				break;
			case KEY_RIGHT:
				if (startx>=COLS-6)  //right side boundary for the player (the -6 is to account for the size of the player object)
				{
					break;
				}
				destroy_win(my_win);
				my_win = create_playerwin(starty,++startx);
				break;
			case KEY_UP:
				if (starty==1)  //top boundary for the player
				{
					break;
				}
				destroy_win(my_win);
				my_win = create_playerwin(--starty,startx);
				break;
			case KEY_DOWN:   //bottom boundary for the player (the -4 is to account for the size of the player object)
				if (starty==LINES-4)
				{
					break;
				}
				destroy_win(my_win);
				my_win = create_playerwin(++starty,startx);
				break;
			//pausing the game
			case 'p':
				cbreak();
				mvprintw(0,0,"Press any key to resume. SCORE: %d           ",score);
				getch();
				halfdelay(5);
				break;
		}
		
		//for the first time the time must be set (the time at the previous instant when the enemy was dropped is recorded and the time elapsed since then is checked. If it reaches 0.5 seconds, it drops the next enemy)
		if (first_time==1)
		{
			first_time=0;
			gettimeofday(&time1,NULL);
		}
		gettimeofday(&time2,NULL);
		
		//print important game statistics and instructions
		mvprintw(0,0,"Press 'q' to quit. 'p' to pause. SCORE: %d",score);
		
		//this condition is to ensure that the game loop waits for 0.5 sec before executing again in the situation that the player has entered an input quickly. It takes care of 'case1'
	/* So if this is changed for difficulty, also correspondingly change the place where case1 was mentioned in the half_delay function*/
		if (get_time_gap()<500000.0) 
		{
			continue;
		}
		//end game becomes 1 if you have been hit
		end_game=drop_enemy(&score,starty,startx);
		if (end_game==1)
		{
			break; //breaks the game loop
		}
		gettimeofday(&time1,NULL);
	}
	//call the high scores screen (in start_end_screen.c)
	high_scores(score);	
	endwin();
	/* End curses mode */			
	return 0;
}





//THIS CREATES ALL THE ENEMIES AND DROPS ALL ENEMIES ONE UNIT DOWN. ALSO CHECKS FOR PLAYER DEATH CONDITION
int drop_enemy(int *address_score,int player_y,int player_x)
{
	//increases the score
	(*address_score)++;
	time_t t;
	//this is an array of windows where each enemy is represented by a window
	static WINDOW *enemies[100];
	static int enemy_num=0;
	static int i=0;
	int j,x,y;

	srand((unsigned) time(&t));
	
				   
	//just creating the enemies the first time (create 2 enemies at a time)   
	if (enemy_num<81)
	{
		enemies[enemy_num]=create_enemywin(0,rand()%COLS); 
		enemies[++enemy_num]=create_enemywin(0,rand()%COLS); 
		enemy_num++;
	}
	//after the enemies have been created for the first time
	else
	{
		//destroy the enemy windows when they reach the bottom of the screen and recreate them at random positions at the top (2 at a time)
		destroy_win(enemies[i]);
		destroy_win(enemies[i+1]);
		enemies[i]=create_enemywin(0,rand()%COLS);  
		enemies[i+1]=create_enemywin(0,rand()%COLS);  
		i=i+2;
		if (i==80)
		{
			i=0;
		}
	}
	
	//move all enemy windows 2 spaces down by destroying them and recreating them 2 spaces down
	for (j=0;j<=enemy_num;j++)
	{
		getbegyx(enemies[j],y,x);
		//checks for player collision with enemies
		if ((player_x<x+7) && (player_x>x-5) && (player_y<=y+5) && (player_y>=y-3))
		{
			return 1;
		}
		destroy_win(enemies[j]);
		enemies[j]=create_enemywin(y+2,x); 
	}

	refresh();
	return 0;
}
   
   
//SUPPLEMENTARY FUNCTIONS

//gives the time gap between the last time an enemy was dropped and now. time1 and time2 are global variables and hence can be accessed by it.
double get_time_gap()
{
	int k;
    double time;
    time=(time2.tv_sec-time1.tv_sec)*1000000+time2.tv_usec-time1.tv_usec;
	return time;
}   

//swaps 2 numbers
void swap_numbers(int *a,int *b)
{
	int temp;
	temp=*a;
	*a=*b;
	*b=temp;
	return;
}

//swaps 2 strings
void swap_strings( char a[], char b[])
{
	char temp[15];
	strcpy(temp,a);
	strcpy(a,b);
	strcpy(b,temp);
	return;
}
	
