#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mtwister.h"

#define ANSI_BOLD       "\x1b[1m"
#define ANSI_CLEAR      "\x1b[0m"
#define ANSI_GREEN      "\x1b[32m"
#define ANSI_RED        "\x1b[31m"

uint64_t global_canary;

void canary() {
	printf("selecting canary\n");
}

void game() {
	printf("playing game\n");
}

void start() {
	char userAction;
	
	puts("");
	puts("       .-\"-.");
	puts("      /  ,~a\\_");
	puts("      \\  \\__))>     welcome to MERSENNE");
	puts("      ,) .\" \\        arithmetic game");
	puts("     /  (    \\");
	puts("    /   )    ;     [c] canary select");
	puts("   /   /     /     [p] play game");
	puts(" ,/_.'`  _.-`");
	puts("  /_/`\"\\\\___");
	puts("       `~~~`");
	puts("");
	printf("[" ANSI_GREEN "*" ANSI_CLEAR "]: ");
	scanf(" %c", &userAction);
	
	while(1) {
		if (userAction == 'c') {
			canary();
			break;
		} else if (userAction == 'p') {
			game();
			break;
		} else {
			printf("[" ANSI_GREEN "*" ANSI_CLEAR "] invalid action, please try again: ");
		  while(getchar() != '\n');
			scanf(" %c", &userAction);
		}
	}
}

void check_canary(uint64_t canary) {
	if (canary != global_canary) {
		printf("***** stack smashing detected *****");
		exit(1);
	}
}

void generate_canary() {
}

void test_rand() {
	MTRand r = seedRand(1337);
	int i;
	for (i=0; i < 1000; i++){
		printf("%ld\n", genRandLong(&r));
	}
	return;
}

int main() {
	gid_t gid = getegid();
	setresgid(gid, gid, gid);
	
	test_rand();
	start();

	return 0;
}
