#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ANSI_BOLD       "\x1b[1m"
#define ANSI_CLEAR      "\x1b[0m"
#define ANSI_GREEN      "\x1b[32m"
#define ANSI_RED        "\x1b[31m"

uint64_t global_canary;

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
	printf("[" ANSI_GREEN "*" ANSI_CLEAR "] ");
	scanf(" %c", &userAction);

	printf("You entered: %c\n", userAction);
}

void check_canary(uint64_t canary) {
	if (canary != global_canary) {
		printf("***** stack smashing detected *****");
		exit(1);
	}
}

void generate_canary() {
}

int main() {
	gid_t gid = getegid();
	setresgid(gid, gid, gid);

	start();

	return 0;
}
