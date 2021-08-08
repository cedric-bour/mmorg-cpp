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
#define HOST_AUTH "127.0.0.1"
#else
#define HOST_DB "10.98.92.244"
#define HOST_AUTH "10.106.226.253"
#endif

int main()
{
    Log syslog(Log::SYSLOG);
    Log syserr(Log::SYSERR);

    TCPClient clientDb(HOST_DB, 5000);
    TCPClient clientAuth(HOST_AUTH, 11002);

#ifdef __APPLE__
    unsigned short choix;
    std::cout << "Enter number of channel: ";
    std::cin >> choix;
    
    TCPServer server(13000 + choix);
    server.start(clientDb, clientAuth);
#else
    TCPServer server(13001);
    server.start(clientDb, clientAuth);
#endif
    
    return 0;
}
