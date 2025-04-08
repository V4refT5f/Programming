#define _USE_MATH_DEFINES
#include <string.h>

#include "250313.h"
#include "250320.h"

int _tnbs_sayIndex = 0;
int _tnbs_say_putchar(int ch) { 
	mvprintw(5, _tnbs_sayIndex + 4, "%c        ", (signed char)ch);  
	_tnbs_sayIndex ++; return 0; 
}

int test_nbsay() {
	#define QUOTE_LEN 34
	const sayfstr quotes[QUOTE_LEN] = {
		"This is my last upload on Bilibili. ^3Seriously.",
		"I ve been making video since I was born.",
		"Like, in 2001 or something.",
		"Oh no;^4 It's 2006, right?",
		"These bloody 06-ers fuk fuk fuk fuk",
		"F**k these collgeg students make me not want to create anything.",
		"WHY? WHY ^2DONT ^2I W^2N^2A^2T^2 TO CREATN THINGS...",
		"Shit bro.",
		"I want to creat things really. hard. like, really hard.",
		"I just sleep when I make songs because I rreally want to create somtehitn good.",
		"That was A Rainy Day in 2025. I'm like, oh lets create somethibg.",
		"NO. I caqnt. I can play phones, but I no creeate thing.",
		"That's bad. really bad.",
		"OK Thatnk you that's all.",
		"What about we read a story? An exerpt from the Hitchhiker's Guide to the Galaxy.",
		"Far out in the uncharted backwaters of the unfashionable end of the western spiral arm of the Galaxy...",
		"A rather remarkable book called The Hitchhiker's Guide to the Galaxy has this to say about panic.",
		"DON'T PANIC. It's the first helpful or intelligible thing it's said all day.",
		"The story so far: In the beginning, the Universe was created. This has made a lot of people very angry.",
		"And is widely regarded as a bad move.",
		"Many races believe it was created by some sort of god, though the Jatravartid people believe it was sneezed out of the nose of a being called the Great Green Arkleseizure.",
		"Time is an illusion. Lunchtime doubly so.",
		"You know, it's at times like this, when I'm stuck in a Vogon airlock with a man from Betelgeuse, about to die of asphyxiation, that I really wish I'd listened to what my mother told me.",
		"Why, what did she tell you?",
		"I don't know, I didn't listen.",
		"Ford, you're turning into a penguin. Stop it.",
		"The ships hung in the sky in much the same way that bricks don't.",
		"So long, and thanks for all the fish.",
		"The answer to the great question of life, the universe and everything is... 42.",
		"For a moment, nothing happened. Then, after a second or two, nothing continued to happen.",
		"Space is big. You just won't believe how vastly, hugely, mind-bogglingly big it is.",
		"The Guide says there is an art to flying. The knack lies in learning how to throw yourself at the ground and miss.",
		"I'd far rather be happy than right any day.",
		"In the beginning the Universe was created. This has made a lot of people very angry and been widely regarded as a bad move."
	};
	signed char ch = ERR;
	double wkT; double idT;
	int sayReturn = SCNB_FINISHED;
	int qi = 0; double rad = .0; double lastrad = 0.;
	init_pair(10, COLOR_BLACK, COLOR_WHITE);
	SayConfig cfg = (SayConfig) {0.1, 0.3, 0.1};
	
	init_frame_control(60.0);

	while (ch != 'q') {
		// qi = rand_int() % QUOTE_LEN; 
		// mvprintw(12, 4, "%d", qi);
		refresh(); frame_control(&wkT, &idT);
		// erase();
		ch = getch();
		if (ch == ' ' && sayReturn == SCNB_IN_PROGRESS) { sayReturn = say_nonblock_skip(_tnbs_say_putchar); }
		else if (ch == ' ' && sayReturn == SCNB_FINISHED) { say_nonblock_init(quotes[(qi ++)% QUOTE_LEN], &cfg, getsec()); _tnbs_sayIndex = 0; }
		else { sayReturn = say_nonblock_process(_tnbs_say_putchar, getsec()); }
		
		lastrad = rad;
		rad += sin(getsec() / 2.0) / 5.0;
		int circleY = cos(rad) * 10;
		int circleX = sin(rad) * 20;
		int lcY = cos(lastrad) * 10;
		int lcX = sin(lastrad) * 20;
		mvprintw(15 + lcY, 40 + lcX, " ");
		attron (COLOR_PAIR(10));
		mvprintw(15 + circleY, 40 + circleX, "#");
		attroff(COLOR_PAIR(10));
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
