/**
 *    file: twister.c
 *    author: azliu0
 *    date: 3/30/2024
 *    source code for twister, a HackMIT 2024 puzzle challenge
 **/

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "mt19937.h"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_CLEAR "\x1b[0m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_RED "\x1b[31m"

#define NUM_QUESTIONS 10

mt19937 rng;

void seed(mt19937 *rng)
{
    srand((unsigned int)time(NULL));

    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
    {
        puts("error opening /dev/urandom");
        exit(1);
    }

    uint32_t seed;
    if (read(fd, &seed, sizeof(seed)) != sizeof(seed))
    {
        puts("error reading /dev/urandom");
        exit(1);
    }
    close(fd);

    seed_rand(rng, seed);
}

void canary()
{
    puts("");
    puts("      canary select");
    puts("");

    uint32_t canary = gen_rand(&rng);

    char userAction[0xff];

    printf("[" ANSI_GREEN "*" ANSI_CLEAR "] canary_id: %u\n", canary);
    printf("[" ANSI_BLUE "*" ANSI_CLEAR "] regenerate? (y/n) ");
    if (fgets(userAction, 0xff, stdin) == NULL)
    {
        return;
    }

    printf("[" ANSI_BLUE "*" ANSI_CLEAR "]" ANSI_BLUE " user input: ");
    printf(userAction);
    printf(ANSI_CLEAR "\n");

    while (1)
    {
        if (userAction[0] == 'y')
        {
            canary = gen_rand(&rng);
            printf("[" ANSI_GREEN "*" ANSI_CLEAR "] canary_id: %u\n", canary);
            printf("[" ANSI_BLUE "*" ANSI_CLEAR "] regenerate? (y/n) ");
            if (fgets(userAction, 0xff, stdin) == NULL)
            {
                return;
            }
            printf("[" ANSI_BLUE "*" ANSI_CLEAR "]" ANSI_BLUE " user input: ");
            printf(userAction);
            printf(ANSI_CLEAR "\n");
        }
        else if (userAction[0] == 'n')
        {
            return;
        }
        else
        {
            printf("[" ANSI_RED "*" ANSI_CLEAR "] invalid action, please try again: ");
            if (fgets(userAction, 0xff, stdin) == NULL)
            {
                return;
            }
            printf("[" ANSI_BLUE "*" ANSI_CLEAR "]" ANSI_BLUE " user input: ");
            printf(userAction);
            printf(ANSI_CLEAR "\n");
        }
    }
}

void game()
{
    volatile uint32_t canary_id;
    volatile char input[0xff];

    canary_id = gen_rand_no_state_update(&rng);

    while (1)
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
                i--;
            }
        }

        puts("");
        puts("      you finished!");
        puts("");

        printf("[" ANSI_BLUE "*" ANSI_CLEAR "] play again? (y/n) ");

        char playAgain[0xff];
        if (fgets(playAgain, 0xff, stdin) == NULL)
        {
            return;
        }

        while (playAgain[0] != 'y' && playAgain[0] != 'n')
        {
            printf("[" ANSI_RED "*" ANSI_CLEAR "] invalid action, please try again: ");
            if (fgets(playAgain, 0xff, stdin) == NULL)
            {
                return;
            }
        }

        if (playAgain[0] == 'n')
        {
            break;
        }
    }

    if (canary_id != gen_rand(&rng))
    {
        printf("***** stack smashing detected *****\n");
        exit(1);
    }
}

void welcome()
{
    puts("");
    puts("       .-\"-.");
    puts("      /  ,~a\\_");
    puts("      \\  \\__))>     welcome to TWISTER");
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
    if (fgets(userAction, 0xff, stdin) == NULL)
    {
        return;
    }

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

        if (fgets(userAction, 0xff, stdin) == NULL)
        {
            return;
        }
    }
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    gid_t gid = getegid();
    setresgid(gid, gid, gid);

    seed(&rng);
    start();

    return 0;
}
