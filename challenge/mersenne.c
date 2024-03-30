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

#define NUM_QUESTIONS 10

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
	char userAction;

	printf("[" ANSI_BLUE "*" ANSI_CLEAR "] canary_id: %ld\n", canary);
	printf("[" ANSI_GREEN "*" ANSI_CLEAR "] regenerate? (y/n) ");
	scanf(" %c", &userAction);

	while (1)
	{
		if (userAction == 'y')
		{
			canary = genRandLong(&rng);
			printf("[" ANSI_BLUE "*" ANSI_CLEAR "] canary_id: %ld\n", canary);
			printf("[" ANSI_GREEN "*" ANSI_CLEAR "] regenerate? (y/n) ");
			while (getchar() != '\n')
				;
			scanf(" %c", &userAction);
		}
		else if (userAction == 'n')
		{
			return;
		}
		else
		{
			printf("[" ANSI_GREEN "*" ANSI_CLEAR "] invalid action, please try again: ");
			while (getchar() != '\n')
				;
			scanf(" %c", &userAction);
		}
	}
}

void game()
{
	char playAgain;
	unsigned long canary_id;
	char input[0xff];
	
	do
	{
		while(getchar() != '\n');

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

			printf("%d. %d + %d: ", i + 1, first[i], second[i]);
			
			gets(input);

			if (strcmp(input, ans) == 0)
			{
				printf(ANSI_GREEN "Correct!" ANSI_CLEAR "\n");
			}
			else
			{
				printf(ANSI_RED "Wrong!\n");
				printf("Your answer was: ");
				printf(input);
				printf(ANSI_CLEAR "\n");
				i--;
			}
		}

		printf("play again? (y/n) ");
		scanf(" %c", &playAgain);
		while (playAgain != 'y' && playAgain != 'n')
		{
			printf("[" ANSI_GREEN "*" ANSI_CLEAR "] invalid action, please try again: ");
			while (getchar() != '\n')
				;
			scanf(" %c", &playAgain);
		}
	} while (playAgain == 'y');
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
	printf("[" ANSI_GREEN "*" ANSI_CLEAR "]: ");
}

void start()
{
	char userAction;

	welcome();
	scanf(" %c", &userAction);

	while (1)
	{
		if (userAction == 'c')
		{
			canary();
			welcome();
		}
		else if (userAction == 'p')
		{
			game();
			welcome();
		}
		else if (userAction == 'e')
		{
			break;
		}
		else
		{
			printf("[" ANSI_GREEN "*" ANSI_CLEAR "] invalid action, please try again: ");
		}
		while (getchar() != '\n')
			;
		scanf(" %c", &userAction);
	}
}

void check_canary(uint64_t canary)
{
	if (canary != genRandLong(&rng))
	{
		printf("***** stack smashing detected *****");
		exit(1);
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
