
#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPMainAux.h"
#include "SPMinimax.h"
#include "SPFIARParser.h"
#include <string.h>

#define HISTORY_SIZE_PER_PLAYER 10

int main()
{
    SPFiarGame *game;
    while (true)
    {
restart:
        game = spFiarGameCreate(2 * HISTORY_SIZE_PER_PLAYER);
prompt_difficulty:
        printf("Please enter the difficulty level between [1-7]:\n");
        char input[1024 + 1];
        fgets(input, sizeof input, stdin);
        
        //remove trailing newline, possibly
        char *pos;
        if ((pos = strchr(input, '\n')) != NULL)
        {
            *pos = NULLCHAR;
        }
        
        if (strcmp(input, "quit") == 0)
        {
            goto program_end;
        }
        if (!spParserIsInt(input))
        {
            printf("Error: invalid level (should be between 1 to 7)\n");
            goto prompt_difficulty;
        }
        unsigned int difficulty = (unsigned int) atoi(input);
        if (difficulty < 1 || difficulty > 7)
        {
            printf("Error: invalid level (should be between 1 to 7)\n");
            goto prompt_difficulty;
        }
        
        bool gameEnd = false;
        
        while (true)
        {
            if (gameEnd || game->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL)
            {
                if (!gameEnd)
                {
                    spFiarGamePrintBoard(game);
                    printf("Please make the next move:\n");
                }
prompt_command:; //a label can only be part of a statement and a declaration is not a statement
                CommandResult result = makePlayerMove(game, gameEnd);
                if (result == QUIT_GAME)
                {
                    printf("Exiting...\n");
                    goto program_end;
                }
                else if (result == INVALID_COMMAND)
                {
                    goto prompt_command;
                }
                else if (result == RESTART_GAME)
                {
                    printf("Game restarted!\n");
                    spFiarGameDestroy(game);
                    goto restart;
                }
                else if (result == UNDO)
                {
                    gameEnd = false;
                }
                //else, continue loop as usual
            }
            else
            {
                int column = spMinimaxSuggestMove(game, difficulty);
                printf("Computer move: add disc to column %d\n", column + 1);
                spFiarGameSetMove(game, column);
            }
            char win = spFiarCheckWinner(game);
            if (win != NULLCHAR)
            {
                spFiarGamePrintBoard(game);
                
                char *winnerString = "WINNER STRING BUG";
                if (win == SP_FIAR_GAME_PLAYER_1_SYMBOL)
                {
                    winnerString = "you win";
                }
                else if (win == SP_FIAR_GAME_PLAYER_2_SYMBOL)
                {
                    winnerString = "computer wins";
                }
                else if (win == SP_FIAR_GAME_TIE_SYMBOL)
                {
                    winnerString = "it's a tie";
                }
                printf("Game over: %s\n", winnerString);
                printf("Please enter 'quit' to exit or 'restart' to start a new game!\n");
                
                gameEnd = true;
            }
        }
    }

program_end:
    spFiarGameDestroy(game);
}