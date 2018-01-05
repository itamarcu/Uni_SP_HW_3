#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SPFIARParser.h"
#include "SPMainAux.h"

bool spParserIsInt(const char *str)
{
    // for null
    if (str == NULL)
    {
        return false;
    }
    // for empty string
    if (!*str)
    {
        return false;
    }
    
    int j = 0;
    // for string that begins with '-'
    if (str[0] == '-')
    {
        j++;
    }
    // for the string "-"
    if (str[j] == NULLCHAR)
    {
        return false;
    }
    //if one character after '-' is not a digit return false
    for (; str[j] != NULLCHAR; j++)
    {
        if (str[j] > '9' || str[j] < '0')
        {
            return false;
        }
    }
    return true;
    
}

/**
 * Creates a command from two strings according to the command types specified in SP_COMMAND.
 *
 * @return
 * a parsed two strings according to the rules specified in the function spParserPraseLine.
 */
SPCommand spParserGetCmdFromTwoWords(char *command, char *arg)
{
    SPCommand cmd;
    cmd.validArg = false;
    cmd.cmd = SP_INVALID_LINE;
    if (0 == strcmp(command, "suggest_move"))
    {
        cmd.cmd = SP_SUGGEST_MOVE;
        return cmd;
    }
    if (0 == strcmp(command, "undo_move"))
    {
        cmd.cmd = SP_UNDO_MOVE;
        return cmd;
    }
    if (0 == strcmp(command, "add_disc"))
    {
        cmd.cmd = SP_ADD_DISC;
        if (spParserIsInt(arg))
        {
            cmd.validArg = true;
            cmd.arg = atoi(arg);
        }
        else
        {
            cmd.validArg = false;
            cmd.arg = -1;
            cmd.cmd = SP_INVALID_LINE;
        }
        return cmd;
    }
    if (0 == strcmp(command, "quit"))
    {
        cmd.cmd = SP_QUIT;
        return cmd;
    }
    if (0 == strcmp(command, "restart_game") || 0 == strcmp(command, "restart"))
    {
        cmd.cmd = SP_RESTART;
        return cmd;
    }
    return cmd;
}

SPCommand spParserPraseLine(const char *str)
{
    char strCopy[SP_MAX_LINE_LENGTH + 1] = {0};
    strncpy(strCopy, str, SP_MAX_LINE_LENGTH); // copy the string to a non-constant string;
    //getting the first word from the string
    char *command;
    command = strtok(strCopy, " \r\t\n");
    //getting the second word from the string
    char *arg;
    arg = strtok(NULL, " \r\t\n");
    //returns invalid command if there is a third word in the string command
    if (strtok(NULL, " \r\t\n") != NULL || command == NULL)
    {
        SPCommand cmd;
        cmd.cmd = SP_INVALID_LINE;
        cmd.validArg = false;
        cmd.arg = -1;
        return cmd;
    }
    //creating the command from the two words and returning it
    SPCommand cmd = spParserGetCmdFromTwoWords(command, arg);
    return cmd;
}

