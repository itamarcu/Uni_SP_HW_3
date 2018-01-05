#include <stdio.h>
#include <string.h>
#include "SPFIARParser.h"
#include "SPMainAux.h"
#include "SPMinimax.h"

CommandResult addDisc(SPFiarGame *game, int columnPlusOne)
{
    SP_FIAR_GAME_MESSAGE message = spFiarGameSetMove(game, columnPlusOne - 1);
    if (message == SP_FIAR_GAME_INVALID_ARGUMENT)
    {
        printf("Error: column number must be in range 1-%d\n", SP_FIAR_GAME_N_COLUMNS);
        return INVALID_COMMAND;
    }
    else if (message == SP_FIAR_GAME_INVALID_MOVE)
    {
        printf("Error: column %d is full\n", columnPlusOne);
        return INVALID_COMMAND;
    }
    else //message == SP_FIAR_GAME_SUCCESS
    {
        //print will only happen in computer's turn
        return ADD_DISC;
    }
}

CommandResult suggestMove(SPFiarGame *game, unsigned int player_suggest_depth)
{
    int suggestionColumn = spMinimaxSuggestMove(game, player_suggest_depth);
    printf("Suggested move: drop a disc to column %d\n", (suggestionColumn + 1));
    return SUGGEST_MOVE;
}

CommandResult undoMove(SPFiarGame *game)
{
    int firstRemoved = 12345;
    if (!spArrayListIsEmpty(game->historyList))
    {
        firstRemoved = spArrayListGetLast(game->historyList);
    }
    SP_FIAR_GAME_MESSAGE message = spFiarGameUndoPrevMove(game);
    if (message == SP_FIAR_GAME_NO_HISTORY)
    {
        printf("Error: cannot undo previous move!\n");
        return INVALID_COMMAND;
    }
    else if (message == SP_FIAR_GAME_INVALID_ARGUMENT)
    {
        printf("Real error: game is null in UNDO_MOVE?");
        return INVALID_COMMAND;
    }
    else
    {
        printf("Remove disc: remove computer's disc at column %d\n", firstRemoved + 1);
        //Used undo after computer won, so no need for extra undo?
        if (game->currentPlayer == SP_FIAR_GAME_PLAYER_2_SYMBOL) //"else"
        {
            int secondRemoved = spArrayListGetLast(game->historyList);
            spFiarGameUndoPrevMove(game);
            printf("Remove disc: remove user's disc at column %d\n", secondRemoved + 1);
        }
        
        return UNDO;
    }
}

CommandResult makePlayerMove(SPFiarGame *game, bool gameEnd, unsigned int player_suggest_depth)
{
    
    char input[1024 + 1];
    
    fgets(input, 1024 + 1, stdin);
    //remove trailing newline, possibly
    char *pos;
    if ((pos = strchr(input, '\n')) != NULL)
    {
        *pos = NULLCHAR;
    }
    
    SPCommand cmd = spParserPraseLine(input);
    
    if (gameEnd)
    {
        switch (cmd.cmd)
        {
            case SP_ADD_DISC:
                if (!cmd.validArg)
                {
                    printf("Error: column number must be in range 1-%d\n", SP_FIAR_GAME_N_COLUMNS);
                    return INVALID_COMMAND;
                }
            case SP_SUGGEST_MOVE:
                printf("Error: the game is over\n");
                return INVALID_COMMAND;
            case SP_UNDO_MOVE:
                return undoMove(game);
            case SP_QUIT:
                return QUIT_GAME;
            case SP_RESTART:
                return RESTART_GAME;
            case SP_INVALID_LINE:
                printf("Error: invalid command\n");
                return INVALID_COMMAND;
        }
    }
    
    switch (cmd.cmd)
    {
        case SP_UNDO_MOVE:
            return undoMove(game);
        case SP_ADD_DISC:
            if (!cmd.validArg)
            {
                printf("Error: column number must be in range 1-%d\n", SP_FIAR_GAME_N_COLUMNS);
                return INVALID_COMMAND;
            }
            return addDisc(game, cmd.arg);
        case SP_SUGGEST_MOVE:
            return suggestMove(game, player_suggest_depth);
        case SP_QUIT:
            return QUIT_GAME;
        case SP_RESTART:
            return RESTART_GAME;
        case SP_INVALID_LINE:
            printf("Error: invalid command\n");
            return INVALID_COMMAND;
    }
    
    printf("Bug in SPMainAux.makePlayerMove !");
    return INVALID_COMMAND;
}

