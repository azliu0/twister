#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mtwister.h"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_CLEAR "\x1b[0m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_RED "\x1b[31m"

MTRand rng;
uint64_t global_canary;

void seed(MTRand *rng)
{
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

	rng = seedRand(seed);
}

void canary()
{
	printf("selecting canary\n");
}

void game()
{
	printf("playing game\n");
}

void start()
{
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

	while (1)
	{
		if (userAction == 'c')
		{
			canary();
			break;
		}
		else if (userAction == 'p')
		{
			game();
			break;
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

void check_canary(uint64_t canary)
{
	if (canary != global_canary)
	{
		printf("***** stack smashing detected *****");
		exit(1);
	}
}

void generate_canary()
{
}

void test_rand()
{
	int i;
	for (i = 0; i < 1000; i++)
	{
		printf("%ld\n", genRandLong(&rng));
	}
	return;
}

int main()
{
	gid_t gid = getegid();
	setresgid(gid, gid, gid);

	seed(&rng);
	test_rand();
	// start();

	return 0;
}
