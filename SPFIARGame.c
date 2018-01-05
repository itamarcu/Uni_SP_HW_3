#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARGame.h"
#include "SPMainAux.h"


SPFiarGame *spFiarGameCreate(int historySize)
{
    SPFiarGame *game;
    
    // Memory allocation for game struct and historyList:
    // Here with calloc tops filled with zeros
    game = calloc(1, sizeof(SPFiarGame));
    if (game == NULL)
    {
        return NULL;
    }
    game->historyList = spArrayListCreate(historySize);
    if (game->historyList == NULL)
    {
        return NULL;
    }
    //First player
    game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
    //Starting with empty board
    for (int row = 0; row < SP_FIAR_GAME_N_ROWS; row++)
    {
        for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++)
        {
            game->gameBoard[row][col] = SP_FIAR_GAME_EMPTY_ENTRY;
        }
    }
    return game;
}

SPFiarGame *spFiarGameCopy(SPFiarGame *src)
{
    SPFiarGame *copy;
    if ((copy = malloc(sizeof(SPFiarGame))) == NULL)
    {
        printf("Error: %s has failed", "spFiarGameCopy");
        return NULL;
    }
    memcpy(copy->gameBoard, src->gameBoard, sizeof(char) * SP_FIAR_GAME_N_ROWS * SP_FIAR_GAME_N_COLUMNS);
    memcpy(copy->tops, src->tops, sizeof(int) * SP_FIAR_GAME_N_COLUMNS);
    copy->currentPlayer = src->currentPlayer;
    if ((copy->historyList = spArrayListCopy(src->historyList)) == NULL)
    {
        return NULL;
    }
    return copy;
}

void spFiarGameDestroy(SPFiarGame *src)
{
    if (src == NULL)
    {
        return;
    }
    spArrayListDestroy(src->historyList);
    free(src);
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame *src, int col)
{
    //Check for invalid argument or invalid move is made
    if (src == NULL || col < 0 || col >= SP_FIAR_GAME_N_COLUMNS)
    {
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }
    if (!spFiarGameIsValidMove(src, col))
    {
        return SP_FIAR_GAME_INVALID_MOVE;
    }
    
    //Add element to historyList and if history is full, remove oldest element.
    if (spArrayListIsFull(src->historyList))
    {
        spArrayListRemoveFirst(src->historyList);
    }
    spArrayListAddLast(src->historyList, col);
    //Add the move to the gameBoard, change currentPlayer and return success
    src->gameBoard[src->tops[col]][col] = src->currentPlayer;
    src->tops[col]++;
    (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) ?
    (src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL) :
    (src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL);
    return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame *src, int col)
{
    if (src->tops[col] == SP_FIAR_GAME_N_ROWS)
    {
        return false;
    }
    return true;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame *src)
{
    int col;
    if (src == NULL)
    {
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }
    if (spArrayListIsEmpty(src->historyList))
    {
        return SP_FIAR_GAME_NO_HISTORY;
    }
    //Getting the last move made in col
    col = spArrayListGetLast(src->historyList);
    //Undoing the last move:
    src->tops[col]--;
    src->gameBoard[src->tops[col]][col] = SP_FIAR_GAME_EMPTY_ENTRY;
    //Changing current player
    (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) ?
    (src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL) :
    (src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL);
    //Removing the last move from historyList
    spArrayListRemoveLast(src->historyList);
    return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame *src)
{
    int row;
    int col;
    if (src == NULL)
    {
        return SP_FIAR_GAME_INVALID_ARGUMENT;
    }
    for (row = SP_FIAR_GAME_N_ROWS - 1; row >= 0; row--)
    {
        printf("| ");
        for (col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++)
        {
            printf("%c ", src->gameBoard[row][col]);
        }
        printf("|\n");
    }
    printf("-----------------\n");
    printf("  1 2 3 4 5 6 7  \n");
    return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame *src)
{
    if (src == NULL)
    {
        return SP_FIAR_GAME_EMPTY_ENTRY;
    }
    return src->currentPlayer;
}

/**
 * Checks if in the specified game status, row and column there is
 * a winner by scoring 4 diagonal discs with right or left diagonal from the specified position up.
 * @param src - the source game
 * @param row - row position of the disc to check.
 * @param col - column position of the disc to check.
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 has 4 right or left diagonal
 * discs from the specified position up.
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 has 4 right or left diagonal
 * discs from the specified position up.
 * null character - otherwise
 */
char spFiarCheckDiagonalWinnerFromPos(SPFiarGame *src, int row, int col)
{
    char player_symbol = src->gameBoard[row][col];
    if (row + 3 < SP_FIAR_GAME_N_ROWS && col + 3 < SP_FIAR_GAME_N_COLUMNS &&
        player_symbol == src->gameBoard[row + 1][col + 1] &&
        player_symbol == src->gameBoard[row + 2][col + 2] &&
        player_symbol == src->gameBoard[row + 3][col + 3])
    {
        return player_symbol;
    }
    if (row + 3 < SP_FIAR_GAME_N_ROWS && col - 3 >= 0 &&
        player_symbol == src->gameBoard[row + 1][col - 1] &&
        player_symbol == src->gameBoard[row + 2][col - 2] &&
        player_symbol == src->gameBoard[row + 3][col - 3])
    {
        return player_symbol;
    }
    return NULLCHAR;
}

/**
 * Checks if in the specified game status, row and column there is
 * a winner by scoring 4 vertical discs from the specified position up.
 * @param src - the source game
 * @param row - row position of the disc to check.
 * @param col - column position of the disc to check.
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 has 4 vertical discs
 * from the specified position up.
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 had 4 vertical discs
 * from the specified position up.
 * null character - otherwise
 */
char spFiarCheckVerticalWinnerFromPos(SPFiarGame *src, int row, int col)
{
    char player_symbol = src->gameBoard[row][col];
    if (row + 3 < SP_FIAR_GAME_N_ROWS &&
        player_symbol == src->gameBoard[row + 1][col] &&
        player_symbol == src->gameBoard[row + 2][col] &&
        player_symbol == src->gameBoard[row + 3][col])
    {
        return player_symbol;
    }
    return NULLCHAR;
}

/**
 * Checks if in the specified game status, row and column there is
 * a winner by scoring 4 horizontal discs from the specified position right.
 * @param src - the source game
 * @param row - row position of the disc to check.
 * @param col - column position of the disc to check.
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 has 4 horizontal discs
 * from the specified position right.
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 had 4 horizontal discs
 * from the specified position right.
 * null character - otherwise
 */
char spFiarCheckHorizontalWinnerFromPos(SPFiarGame *src, int row, int col)
{
    char player_symbol = src->gameBoard[row][col];
    if (col + 3 < SP_FIAR_GAME_N_COLUMNS &&
        player_symbol == src->gameBoard[row][col + 1] &&
        player_symbol == src->gameBoard[row][col + 2] &&
        player_symbol == src->gameBoard[row][col + 3])
    {
        return player_symbol;
    }
    return NULLCHAR;
}

char spFiarCheckWinner(SPFiarGame *src)
{
    if (src == NULL)
    {
        return NULLCHAR;
    }
    
    bool isFullBoard = true;
    char winner;
    
    //Checking winner from left to right and down to up.
    //This way its enough to check a 4 in a row only to the right or up diagonaly, verticaly or horizontaly.
    for (int row = 0; row < SP_FIAR_GAME_N_ROWS; row++)
    {
        for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++)
        {
            if (src->gameBoard[row][col] != SP_FIAR_GAME_EMPTY_ENTRY)
            {
                if ((winner = spFiarCheckDiagonalWinnerFromPos(src, row, col)) != NULLCHAR)
                {
                    return winner;
                }
                if ((winner = spFiarCheckVerticalWinnerFromPos(src, row, col)) != NULLCHAR)
                {
                    return winner;
                }
                if ((winner = spFiarCheckHorizontalWinnerFromPos(src, row, col)) != NULLCHAR)
                {
                    return winner;
                }
            }
            else
            {
                isFullBoard = false;
            }
        }
    }
    //If we got here and no winner was found and the board is full its a tie
    if (isFullBoard)
    {
        return SP_FIAR_GAME_TIE_SYMBOL;
    }
    return NULLCHAR;
}


