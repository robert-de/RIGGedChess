#ifndef __LIBS__
#define __LIBS__

// SYS INCLUDES
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// CPP INCLUDES
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

#include "chessMove.hpp"

// DEFINES
#define forever for (int timer = 500; timer > 0; timer--)
#define BLACK false
#define WHITE true

// RESPONSE TYPES

enum TYPES
{
    RECV_MOVE,
    SEND_MOVE,
    NEW,
    FORCE,
    GO,
    QUIT,
    INVALID,
    RESIGN
};

#endif