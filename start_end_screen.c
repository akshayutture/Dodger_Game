/* This has 1 function to display the start screen and 1 to display the end screen (high scores) */


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

//prints the start screen
void start_screen()
{
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	//print heading
	char game_name[]="DODGER GAME";
	int game_name_len=11,i,no_of_instructions=5;
	//instructions that will be printed
	char *instructions[200]={
							"1. Aliens have attacked planet earth.", 
							"2. You have to escape them for as long as you can.",
							"3. Use the arrow keys to navigate your player",
							"4. Avoid the enemy aliens for as long as you can.",
							"5. Press q to quit and p to pause",
							" ",
							"PRESS ANY KEY TO START THE GAME"
						  };
	//printing the heading
	attron(COLOR_PAIR(1) | A_UNDERLINE | A_BOLD );
	mvprintw(LINES/3,(COLS-13)/7,game_name);
	attroff(COLOR_PAIR(1) | A_BOLD );
	//printing the label instructions
	mvprintw(LINES/2,(COLS-13)/7,"Instructions");
	attroff( A_UNDERLINE);
	//printing all the instructions
	for (i=0;i<=no_of_instructions+1;i++)
	{
		mvprintw(LINES/2 + i + 1,(COLS-13)/7,instructions[i]);
	}	
	//waiting for a keypress before clearing the screen and starting the game
	getch();
	clear();
	return;
} 

//displays the high_scores page and takes in the user's name if his is a high score
void high_scores(int score)
{
	//clear the main game screen
	clear();
	keypad(stdscr, FALSE);
	//read - reads from high score page, write - writes to it
	FILE *read,*write,*write2;
	//3 high scores are maintained. Should this be changed, the limits for the for() loop should be changed
	//the first high score corresponds to the first name and so on.
	char h_score_strings[3][10],names[3][15],a;
	int i,high_scores[3];
	
	mvprintw(LINES/3 - 2,(COLS-15)/2,"Your score: %d",score);
	
	//print the high scores heading
	attron(COLOR_PAIR(1) | A_UNDERLINE | A_BOLD );
	mvprintw(LINES/3,(COLS-15)/2,"High Scores");
	attroff(COLOR_PAIR(1) | A_UNDERLINE | A_BOLD );
	
	read=fopen(".high_scores.txt","r");
	//file is read into the high_score and names arrays and simultaneously printed
	for (i=0;i<3;i++)
	{
		fscanf(read,"%d %s",&high_scores[i],names[i]);
		mvprintw(LINES/3+i+2,(COLS-20)/2,"%d %s",high_scores[i],names[i]);
	}
	
	//if your score is greater than the 3rd highest, replace your name/score with his
	if (score>high_scores[2])
	{
		mvprintw(LINES/3+i+3,(COLS-20)/2,"Your score is in the top 3.");
		mvprintw(LINES/3+i+4,(COLS-20)/2,"Please enter your name:");
		echo();    		//waits for the user to type 'enter' before taking in the input
		nocbreak();		//disables the halfdelay function
		curs_set(1);	//shows the curser position
		scanw("%s",&names[2]);
		high_scores[2]=score;
		mvprintw(LINES/3+i+6,(COLS-20)/2,"Your score is added to the hall of fame");
		

		//change this if number of high scores is changed.
		//Putting the current high score in its right place.
		if (high_scores[2]>high_scores[1])
		{
			swap_numbers(&high_scores[2],&high_scores[1]);
			swap_strings(names[2],names[1]);
		}
		
		if (high_scores[1]>high_scores[0])
		{
			swap_numbers(&high_scores[0],&high_scores[1]);
			swap_strings(names[0],names[1]);
		}
			
		//writing all the high scores back to the file - it writes to 2 files, 1 hidden and the other an open one. Actually the hidden one only is used for reading. The other is just a dummy
		write=fopen(".high_scores.txt","w");	
		write2=fopen("high_scores.txt","w");
		for (i=0;i<3;i++)
		{
			fprintf(write,"%d %s\n",high_scores[i],names[i]);
			fprintf(write2,"%d %s\n",high_scores[i],names[i]);
		}
	}
	
	//printing the q=quit text and wait for the input.
	mvprintw(LINES/3+i+7,(COLS-20)/2,"PRESS q TO QUIT");
	cbreak();
	while(a=getch())
	{
		if (a=='q')
		{
			return;
		}
	}
}
