#include "libs.hpp"

#ifndef API_H
#define API_H

#define APIBUF_SIZE 512

enum CMD_OUTS {
    featureCMD,
    resignCMD,
    moveCMD
};

enum FILTER {
    moveFilter,
    timeFilter
};

typedef struct response{
    char type;
    chessMove cm;
    
} response ;

class boardAPI {
    private:
    public:
    char commBuffer[APIBUF_SIZE];
    char bufferLen;
    bool initialised;

    chessMove opponentMove;
    chessMove moveengineMove;

    boardAPI(std::string fileName);
    ~boardAPI();

    std::ofstream logfile;

    int init();
    void fillCommBuffer();
    int sendCommand(int type, char* command);
    chessMove getMove(std::string);
    void sendMove(chessMove engineMove);
    bool validMoveFormat(std::string move);
    response* getCommand(response * r);
    
};

#endif
