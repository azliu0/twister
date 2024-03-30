#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "mtwister.h"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_CLEAR "\x1b[0m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_RED "\x1b[31m"

#define NUM_QUESTIONS 1

MTRand rng;

void seed(MTRand *rng)
{
	srand((unsigned int)time(NULL));

	int fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		puts("error opening /dev/urandom");
		exit(1);
	}

	unsigned long seed;
	if (read(fd, &seed, sizeof(seed)) != sizeof(seed))
	{
		puts("error reading /dev/urandom");
		exit(1);
	}
	close(fd);

	*rng = seedRand(seed);
}

void canary()
{
	puts("");
	puts("      canary select");
	puts("");

	unsigned long canary = genRandLong(&rng);
	char userAction[0xff];

	printf("[" ANSI_GREEN "*" ANSI_CLEAR "] canary_id: %ld\n", canary);
	printf("[" ANSI_BLUE "*" ANSI_CLEAR "] regenerate? (y/n) ");
	fgets(userAction, 0xff, stdin);

	while (1)
	{
		if (userAction[0] == 'y')
		{
			canary = genRandLong(&rng);
			printf("[" ANSI_GREEN "*" ANSI_CLEAR "] canary_id: %ld\n", canary);
			printf("[" ANSI_BLUE "*" ANSI_CLEAR "] regenerate? (y/n) ");
			fgets(userAction, 0xff, stdin);
		}
		else if (userAction[0] == 'n')
		{
			return;
		}
		else
		{
			printf("[" ANSI_RED "*" ANSI_CLEAR "] invalid action, please try again: ");
			fgets(userAction, 0xff, stdin);
		}
	}
}

void game()
{
	unsigned long canary_id = 0;
	char input[0xff];
	
	while(1)
	{
		puts("");
		puts("      arithmetic game");
		puts("");
		
		int first[NUM_QUESTIONS];
		int second[NUM_QUESTIONS];

		for (int i = 0; i < NUM_QUESTIONS; i++)
		{
			first[i] = rand() % 100 + 5;
			second[i] = rand() % 100 + 5;
		}

		for (int i = 0; i < NUM_QUESTIONS; i++)
		{
			char ans[0xff];

			snprintf(ans, sizeof(ans), "%d", first[i] + second[i]);

			printf("[" ANSI_BLUE "*" ANSI_CLEAR "] %d. %d + %d: ", i + 1, first[i], second[i]);
			
			gets(input);

			if (strcmp(input, ans) == 0)
			{
				printf("[" ANSI_GREEN "*" ANSI_CLEAR "]" ANSI_GREEN " Correct!" ANSI_CLEAR "\n");
			}
			else
			{
				printf("[" ANSI_RED "*" ANSI_CLEAR "]" ANSI_RED " Wrong!" ANSI_CLEAR "\n");
				printf("[" ANSI_RED "*" ANSI_CLEAR "]" ANSI_RED " Your answer was: ");
				printf(input);
				printf(ANSI_CLEAR "\n");
				i--;
			}
		}

		puts("");
		puts("      you finished!");
		puts("");

		printf("[" ANSI_BLUE "*" ANSI_CLEAR "] play again? (y/n) ");

		char playAgain[0xff];
		fgets(playAgain, 0xff, stdin);

		while (playAgain[0] != 'y' && playAgain[0] != 'n')
		{
			printf("[" ANSI_RED "*" ANSI_CLEAR "] invalid action, please try again: ");
			fgets(playAgain, 0xff, stdin);
		}

		if (playAgain[0] == 'n')
		{
			return;
		}
	}

	if (canary_id != 0x0 && canary_id != genRandLong(&rng))
	{
		printf("***** stack smashing detected *****");
		exit(1);
	}
}

void welcome()
{
	puts("");
	puts("       .-\"-.");
	puts("      /  ,~a\\_");
	puts("      \\  \\__))>     welcome to MERSENNE");
	puts("      ,) .\" \\        arithmetic game");
	puts("     /  (    \\");
	puts("    /   )    ;     [c] canary select");
	puts("   /   /     /     [p] play game");
	puts(" ,/_.'`  _.-`      [e] exit");
	puts("  /_/`\"\\\\___");
	puts("       `~~~`");
	puts("");
	printf("[" ANSI_BLUE "*" ANSI_CLEAR "]: ");
}

void start()
{
	char userAction[0xff];

	welcome();
	fgets(userAction, 0xff, stdin);

	while (1)
	{
		if (userAction[0] == 'c')
		{
			canary();
			welcome();
		}
		else if (userAction[0] == 'p')
		{
			game();
			welcome();
		}
		else if (userAction[0] == 'e')
		{
			break;
		}
		else
		{
			printf("[" ANSI_RED "*" ANSI_CLEAR "] invalid action, please try again: ");
		}
		
		fgets(userAction, 0xff, stdin);
	}
}

int main()
{
  gid_t gid = getegid();
	setresgid(gid, gid, gid);

	seed(&rng);
  start();

	return 0;
}
