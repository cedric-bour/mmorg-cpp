//
//  includes.h
//  Db
//
//  Created by Cédric B. on 13/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef includes_h
#define includes_h

#include <jdbc/mysql_connection.h>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

#include <arpa/inet.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "../Classes/Quest.h"
#include "../Classes/Player.h"
#include "../Classes/Packet.h"
#include "../Classes/Log.h"

#include "../Controller/DBCacheManager.h"
#include "../Controller/QuestManager.h"
#include "../Controller/AccountManager.h"

#include "../Classes/TCPServer.h"

#endif /* includes_h */
