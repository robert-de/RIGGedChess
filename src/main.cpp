#include "Engine.hpp"
#include "Table.h"
#include "boardAPI.h"
#include "libs.hpp"

int main()
{
    // Initialize Engine
    Engine *rigged = Engine::getInstance();
    response *r = (response *)malloc(sizeof(response));

    forever
    {
        rigged->API->getCommand(r);
        usleep(10000);
        if (r->type == QUIT)
            break;
        // if(r->type == RESIGN)
        //     break;
    }
    free(r);
    return 0;
}