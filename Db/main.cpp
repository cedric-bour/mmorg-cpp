//
//  main.cpp
//  Auth
//
//  Created by Cédric B. on 24/02/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#include "Utils/includes.h"

int main()
{
    Log syslog(Log::SYSLOG);
    Log syserr(Log::SYSERR);
    
    sql::Driver *driver = get_driver_instance();
#ifdef __APPLE__
    sql::Connection *con = driver->connect("tcp://127.0.0.1:3306", "root", "toor");
#else
    sql::Connection *con = driver->connect("tcp://10.104.126.48:3306", "root", "toor");
#endif
    con->setSchema("mmo");
    
    TCPServer server(5000, con);
    server.start();
    
    return 0;
}
