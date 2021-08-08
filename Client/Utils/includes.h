//
//  includes.h
//  Client
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

#include "../Classes/Player.h"
#include "../Classes/Account.h"
#include "../Classes/Packet.h"
#include "../Classes/TCPClient.h"
#include "../Classes/Log.h"

#include "../Controller/QuestManager.h"
#include "../Controller/PlayerManager.h"
#include "../Controller/AccountManager.h"

#endif /* includes_h */
