//
//  main.cpp
//  Auth
//
//  Created by Cédric B. on 24/02/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#include "Utils/includes.h"

#ifdef __APPLE__
#define HOST_DB "127.0.0.1"
#else
#define HOST_DB "10.98.92.244"
#endif

int main()
{
    Log syslog(Log::SYSLOG);
    Log syserr(Log::SYSERR);

    TCPClient clientDb(HOST_DB, 5000);
    
    TCPServer server(11002);
    server.start(clientDb);
    
    return 0;
}
