#define _USE_MATH_DEFINES
#include "250313.h"
#include "250320.h"
#include <string.h>

int _tnbs_sayIndex = 0;
int _tnbs_say_putchar(int ch) { for (int i = 3; i < 10; i ++) { mvprintw(i, _tnbs_sayIndex + 4, "%c        ", (signed char)ch); } _tnbs_sayIndex ++; return 0; }

int test_nbsay() {
	#define QUOTE_LEN 5
	const sayfstr quotes[QUOTE_LEN] = {
		"Apples... ^3Pears... ^3 Machine gun...",
		"Argh! ^5Argh! Argh! Argh! Argh!",
		"Dubo - Dubon - Dubonnet",
		"Bla bla bla bla bla ^7 bla bla",
		"Crap huh?"
	};
	signed char ch = ERR;
	double wkT; double idT;
	int sayReturn = SCNB_FINISHED;
	int qi = 0;
	SayConfig cfg = (SayConfig) {0.1, 0.3, 0.1};
	
	init_frame_control(60.0);

	while (ch != 'q') {
		qi = rand_int() % QUOTE_LEN; 
		mvprintw(12, 4, "%d", qi);
		refresh(); frame_control(&wkT, &idT);
		// erase();
		ch = getch();
		if (ch == ' ' && sayReturn == SCNB_IN_PROGRESS) { sayReturn = say_nonblock_skip(_tnbs_say_putchar); }
		else if (ch == ' ' && sayReturn == SCNB_FINISHED) { say_nonblock_init(quotes[qi], &cfg, getsec()); _tnbs_sayIndex = 0; }
		else { sayReturn = say_nonblock_process(_tnbs_say_putchar, getsec()); }
	}
	
	say_nonblock_skip(_tnbs_say_putchar);
	end_frame_control();
	return 0;
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
	initscr(); cbreak(); noecho(); nodelay(stdscr, TRUE);
	start_color(); curs_set(0);
	
	test_nbsay();

	endwin(); printf("\n");
	return 0;
}
