#include "250313.h"

#define NCURSES_STATIC
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

#include <string.h>

typedef enum Understanding {
	YES,
	MAYBE,
	NO,
	UNKNOWN
} Understanding;

typedef struct ankiWord {
	char* word;
	char* meaning;
	Understanding level;
} AnkiWord;

typedef struct ankiWordBank {
	AnkiWord* words;
	size_t count;
} AnkiWordBank;

int newWord(AnkiWordBank *wb, const char* word, const char* meaning) {
	if (!wb || !word || !meaning) return -1;
	size_t opIndex = 0;
	if (!wb->words) { wb->words = (AnkiWord*) calloc(1, sizeof(AnkiWord)); count = 1; }
	else { opIndex = wb->count; wb->count ++; wb->words = (AnkiWord*) realloc(wb->count * sizeof(AnkiWord)); }
	wb->words[opIndex].word = (char*) calloc(strlen(word) + 1, sizeof(char));
}
