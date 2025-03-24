#include "250313.h"

#define NCURSES_STATIC
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif


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

