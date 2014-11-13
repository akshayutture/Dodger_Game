//These corresponding functions are written in the corresponding .c file


//creates a new player window at the specified postion
WINDOW *create_playerwin(int starty, int startx);

//creates a new enemy window at the specified position
WINDOW *create_enemywin(int starty, int startx);

//this destroys the given window
void destroy_win(WINDOW *local_win);
