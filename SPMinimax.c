
#include "SPMinimax.h"
#include <stdlib.h>
#include <stdio.h>

int calc(char ch) {
    if (ch == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
        return -1;
    } else if (ch == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
        return +1;
    }
    return 0;
}

int calculateScore(SPFiarGame *src) {
    int weights[7] = {-5, -2, -1, 0, +1, +2, +5};
    char (*board)[SP_FIAR_GAME_N_COLUMNS] = src->gameBoard;
    int totalScore = 0;

    //Horizontals
    for (int row = 0; row < SP_FIAR_GAME_N_ROWS; row++) {
        for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS - 4 + 1; col++) {
            /*
             *
             *
             *
             * XXXX
             */
            int lineScoreR = 0
                             + calc(board[row + 0][col + 0])
                             + calc(board[row + 0][col + 1])
                             + calc(board[row + 0][col + 2])
                             + calc(board[row + 0][col + 3]);

            if (lineScoreR == -4) {
                return INT_MIN;
            }
            if (lineScoreR == +4) {
                return INT_MAX;
            }

            totalScore += weights[lineScoreR + 4 - 1];
        }
    }

    //Verticals
    for (int row = 0; row < SP_FIAR_GAME_N_ROWS - 4 + 1; row++) {
        for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++) {
            /*
             * X
             * X
             * X
             * X
             */
            int lineScoreU = 0
                             + calc(board[row + 0][col + 0])
                             + calc(board[row + 1][col + 0])
                             + calc(board[row + 2][col + 0])
                             + calc(board[row + 3][col + 0]);

            if (lineScoreU == -4) {
                return INT_MIN;
            }
            if (lineScoreU == +4) {
                return INT_MAX;
            }

            totalScore += weights[lineScoreU + 4 - 1];
        }
    }

    //Diagonals
    for (int row = 0; row < SP_FIAR_GAME_N_ROWS - 4 + 1; row++) {
        for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS - 4 + 1; col++) {
            /*
             * ---X
             * --X-
             * -X--
             * X---
             */
            int lineScoreUR = 0
                              + calc(board[row + 0][col + 0])
                              + calc(board[row + 1][col + 1])
                              + calc(board[row + 2][col + 2])
                              + calc(board[row + 3][col + 3]);
            /*
             * X---
             * -X--
             * --X-
             * ---X
             */
            int lineScoreUL = 0
                              + calc(board[row + 3][col + 0])
                              + calc(board[row + 2][col + 1])
                              + calc(board[row + 1][col + 2])
                              + calc(board[row + 0][col + 3]);

            if (lineScoreUR == -4
                || lineScoreUL == -4) {
                return INT_MIN;
            }
            if (lineScoreUR == +4
                || lineScoreUL == +4) {
                return INT_MIN;
            }

            totalScore += weights[lineScoreUR + 4 - 1];
            totalScore += weights[lineScoreUL + 4 - 1];
        }
    }

    return totalScore;
}

/**
 * In case of a full board, this will return -[-infinity, -1]
 *
 * @return int[2] containing [bestScore, columnToGetThat]
 */
int *recursiveBestMove(SPFiarGame *game, int depthRemaining) {
    int *result = malloc(2 * sizeof(int));
    if (result == NULL) {
        printf("Error: %s has failed", "recursiveBestMove");
        return NULL;
    }

    int score = calculateScore(game);
    if (score == INT_MAX || score == INT_MIN)
    {
        result[0] = score;
        result[1] = -17;
        return result;
    }

    if (depthRemaining == 0) {
        //It's a leaf
        result[0] = calculateScore(game);
        result[1] = -17;

        return result;
    }

    bool isPlayer1 = (calc(game->currentPlayer) == 1);
    result[0] = isPlayer1 ? INT_MIN : INT_MAX; // score
    result[1] = -1; // column for that score

    depthRemaining--;
    for (int col = 0; col < SP_FIAR_GAME_N_COLUMNS; col++) {
        SP_FIAR_GAME_MESSAGE msg = spFiarGameSetMove(game, col);

        if (msg == SP_FIAR_GAME_SUCCESS) {
            int *possibleResult = recursiveBestMove(game, depthRemaining);
            possibleResult[1] = col;
            //Next line compares the current player to the result comparison
            //This is like doing "min" or "max" depending on player
            if (isPlayer1 == (possibleResult[0] > result[0])) {
                result[0] = possibleResult[0];
                result[1] = possibleResult[1];
            }
            free(possibleResult);
            if (spFiarGameUndoPrevMove(game) == SP_FIAR_GAME_NO_HISTORY)
                printf("Huge bug");

            //Slight optimization - pruning branch if sibling branches have a win in them
            if (((result[0] == INT_MAX) && isPlayer1)
                || ((result[0] == INT_MIN) && !isPlayer1)) {
                return result;
            }
        }
    }


    return result;
}

int spMinimaxSuggestMove(SPFiarGame *currentGame,
                         unsigned int maxDepth) {
    if (currentGame == NULL || maxDepth <= 0) {
        return -1;
    }

    if (currentGame->historyList->maxSize < maxDepth * 2) {
        //Not enough space in the undo-stack to calculate
        return -98421;
    }


    SPFiarGame *game = spFiarGameCopy(currentGame);
    int *best = recursiveBestMove(game, maxDepth);
    spFiarGameDestroy(game);
    int bestColumn = best[1];
    free(best);

    return bestColumn;
}

