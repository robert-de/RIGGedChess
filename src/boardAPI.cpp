#include "boardAPI.h"
#include "Engine.hpp"
#include "Table.h"
#include "libs.hpp"
#include <string>

boardAPI::boardAPI(std::string fileName) : logfile(fileName, std::ofstream::out)
{
    init();
}

boardAPI::~boardAPI()
{
    logfile.close();
}

response *boardAPI::getCommand(response *r)
{
    char process[25];
    // If API buffer is empty pause execution and wait for more input
    if (strlen(commBuffer) == 0)
    {
        fillCommBuffer();
    }

    // Send to log file all contents of buffer
    std::stringstream stringStream(commBuffer);
    // For each element in API buffer check type
    r->type = -1;
    while (stringStream >> process)
    {

        if (strcmp(process, "new") == 0)
        {
            // Reset game board, engine plays as black
            // If edit mode was set to true, set it to false
            // logfile << "GOT \"new\" COMMAND" << std::endl;
            Engine::getInstance()->playingAs(BLACK);
            Table::getInstance()->editMode = false;
            Table::getInstance()->clearTable();
            Table::getInstance()->tableInit();
            Table::getInstance()->history.clear();

            r->type = NEW;
        }
        else if (strcmp(process, "black") == 0)
        {
            Engine::getInstance()->color = BLACK;
        }
        else if (strcmp(process, "white") == 0)
        {
            Engine::getInstance()->color = WHITE;
        }
        else if (strcmp(process, "go") == 0)
        {
            // Engine makes a move with whichever color has the current turn
            // Switch turn here

            // Workaround for first piece not being updated correctly
            if (Table::getInstance()->history.size() % 2 == 0)
                Table::getInstance()->turn = BLACK;
            else
                Table::getInstance()->turn = WHITE;

            Table::getInstance()->editMode = false;
            r->type = GO;

            // Generate our own move
            r->cm = Engine::getInstance()->generateMove(Table::getInstance());

            if (r->cm.sourceFile == '`')
            {
                write(STDOUT_FILENO, "resign\n", 7);
                Table::getInstance()->clearTable();
                Table::getInstance()->tableInit();
                memset(commBuffer, 0, APIBUF_SIZE);
                r->type = RESIGN;
                return r;
            }
            // Add move sent to Xboard to move history
            Table::getInstance()->history.push_back(r->cm);

            // Update the table with the newly generated move
            Table::getInstance()->updateTable(r->cm);

            // Send the generated move
            sendMove(r->cm);

            // Update the debug file
            Table::getInstance()->debugDraw();
        }
        else if (strcmp(process, "force") == 0)
        {
            // Enables edit mode, moves will be received instead of sent
            // logfile << "GOT \"force\" COMMAND" << std::endl;
            Table::getInstance()->editMode = true;

            r->type = FORCE;
        }
        else if (strcmp(process, "quit") == 0)
        {
            // Exits from forever loop and ends the program
            // logfile << "GOT \"quit\" COMMAND" << std::endl;
            Table::getInstance()->quit = true;

            r->type = QUIT;
        }
        else if (validMoveFormat(process))
        {
            // Check if a valid move was received
            // logfile << "GOT VALID MOVE \"" << process << "\" COMMAND" << std::endl;
            Table::getInstance()->turn = !Table::getInstance()->turn;

            // Get enemy move as response
            r->cm = getMove(process);
            r->type = RECV_MOVE;

            // Update the table with the newly received move
            Table::getInstance()->updateTable(r->cm);
            // Table::getInstance()->debugDraw();
            // Workaround for first piece not being updated correctly
            if (Table::getInstance()->history.size() == 0)
                Table::getInstance()->turn = WHITE;

            // Add move received from Xboard to move history
            Table::getInstance()->history.push_back(r->cm);

            // If thinking is turned off, update the debug file
            // Do not generate a move
            if (Table::getInstance()->editMode)
            {
                // Table::getInstance()->debugDraw();
                continue;
            }

            // Generate our own move
            r->cm = Engine::getInstance()->generateMove(Table::getInstance());

            if (r->cm.sourceFile == '`')
            {
                write(STDOUT_FILENO, "resign\n", 7);
                Table::getInstance()->clearTable();
                Table::getInstance()->tableInit();
                r->type = RESIGN;
                return r;
            }
            // Add move sent to Xboard to move history
            Table::getInstance()->history.push_back(r->cm);

            // Update the table with the newly generated move
            Table::getInstance()->updateTable(r->cm);

            // Send the generated move
            sendMove(r->cm);

            // Update the debug file
            Table::getInstance()->debugDraw();
        }
    }

    memset(commBuffer, 0, APIBUF_SIZE);

    return r;
}
// Checks if a given string can be interpreted as a valid move
bool boardAPI::validMoveFormat(std::string move)
{
    // Check if length is at most 5 characters
    if (move.length() > 5 || move.length() < 4)
        return false;

    // Check each character to make sure it respects 'frfr' format
    if (move[0] < 'a' || move[0] > 'h')
        return false;
    if (move[2] < 'a' || move[2] > 'h')
        return false;
    if (move[1] < '1' || move[1] > '8')
        return false;
    if (move[3] < '1' || move[3] > '8')
        return false;

    return true;
}

// Gets a move from the API buffer and validate it
chessMove boardAPI::getMove(std::string process)
{
    chessMove newMove;

    // Stores the source and destination coordinates
    newMove.sourceFile = process[0];
    newMove.sourceRank = process[1];
    newMove.destinationFile = process[2];
    newMove.destinationRank = process[3];
    
    // If there is a promote character validate it
    if (process.length() == 5)
        newMove.promote = process[4];
    else
        newMove.promote = 0;

    newMove.castle = false;
    newMove.enPassant = false;

    return newMove;
}

// Sends engineMove to STDOUT
void boardAPI::sendMove(chessMove engineMove)
{
    if (engineMove.promote == 0)
    {
        char moveCommand[10] = {'m', 'o', 'v', 'e', ' ', engineMove.sourceFile,
                                engineMove.sourceRank,
                                engineMove.destinationFile,
                                engineMove.destinationRank, '\n'};
        write(STDOUT_FILENO, moveCommand, 10);
    }
    else
    {
        char moveCommand[11] = {'m', 'o', 'v', 'e', ' ', engineMove.sourceFile,
                                engineMove.sourceRank,
                                engineMove.destinationFile,
                                engineMove.destinationRank,
                                engineMove.promote, '\n'};
        write(STDOUT_FILENO, moveCommand, 11);
    }
}

// Adds chars from STDIN to the command buffer
void boardAPI::fillCommBuffer()
{

    strcpy(commBuffer, "\0");
    int len;
    // Declare a file descriptor
    fd_set stdinDescriptor;
    // Clear the file descriptor
    FD_ZERO(&stdinDescriptor);
    // Set it to 0 (STDIN)
    FD_SET(0, &stdinDescriptor);
    select(1, &stdinDescriptor, NULL, NULL, NULL);
    len = read(STDIN_FILENO, commBuffer, APIBUF_SIZE);
    commBuffer[len] = 0;
    if (len < 0)
    {
        perror("Error while getting a new command");
        exit(-1);
    }
    bufferLen = len;
}

int boardAPI::init()
{
    // Get initial XBoard command
    fillCommBuffer();
    if (commBuffer[0] == 'x')
    {
        write(STDOUT_FILENO, "\n", 1);
        // Send features
        write(STDOUT_FILENO, "feature done=0\n", 15);
        // Disable signals
        write(STDOUT_FILENO, "feature sigint=0\n", 17);
        // Get/Send moves in coordinate notations ex: e2e4
        write(STDOUT_FILENO, "feature san=0\n", 14);
        // Name of engine
        write(STDOUT_FILENO, "feature myname=\"Rigged\"\n", 24);
        // All settings have been sent, start the game
        write(STDOUT_FILENO, "feature done=1\n", 15);
        // Initialised flag set to true and game loop can be entered
        initialised = true;
    }
    return 0;
}
