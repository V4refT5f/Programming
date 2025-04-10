#include "tankman.h"
#include "250320.h"



STATE SOLID_draw(Entity* self) {
	Point topLeft = box_topLeft(self->bBox);
	for (int i = 0; i < self->bBox.size.y; i ++) {
		for (int j = 0; j < self->bBox.size.x; j ++) {
			mvprintw(topLeft.y + i, topLeft.x + j, "#");
		}
	}
	return SUCCESS;
}


int main(const int argc, const char* argv[]) {

	initscr();			// Sets up the memories needed by curses to eradicate its doubt of existance
	cbreak();			// Disables line buffering like setbuf(stdout, NULL);.
	noecho();			// Disables input echoing.
	nodelay(stdscr, TRUE); // Sets getch() to non-blocking mode. It will return ERR if no keystrokes are provided.
	start_color();		// Enables coloring. COLOR_PAIR();s are available from now on.
	clear();			// Clears the screen buffer and move the cursor to (0, 0) and forces full screen redraw.
	refresh();			// Blits the buffer.
	curs_set(0);		// Hides the cursor.


	init_frame_control(60);
	double wkT; double idT;
	signed char ch = ERR;
	while (ch != '!') { 
		ch = getch(); 
		frame_control(&wkT, &idT); 
	}
	end_frame_control();

	endwin();			// Bye bye
	putchar('\n');
 	return SUCCESS;
}
