#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include "SPFIARGame.h"

#define NULLCHAR '\0'

typedef enum command_result_t
{
    ADD_DISC,
    INVALID_COMMAND,
    QUIT_GAME,
    RESTART_GAME,
    UNDO,
    SUGGEST_MOVE
} CommandResult;

CommandResult makePlayerMove(SPFiarGame *game, bool gameEnd, unsigned int player_suggest_depth);

#endif
