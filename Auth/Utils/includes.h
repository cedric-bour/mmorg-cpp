//
//  includes.h
//  Auth
//
//  Created by Cédric B. on 13/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef includes_h
#define includes_h

#include <arpa/inet.h>
#include <sstream>
#include <fstream>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "../Classes/Player.h"
#include "../Classes/Packet.h"
#include "../Classes/Account.h"
#include "../Classes/TCPClient.h"
#include "../Classes/Log.h"

#include "../Controller/AccountManager.h"

#include "../Classes/TCPServer.h"

#endif /* includes_h */
