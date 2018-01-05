#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include "SPFIARGame.h"

#define NULLCHAR '\0'

typedef enum command_result_t
{
    CONTINUE,
    INVALID_COMMAND,
    QUIT_GAME,
    RESTART_GAME,
    UNDO
} CommandResult;

CommandResult makePlayerMove(SPFiarGame *game, bool gameEnd);

#endif
