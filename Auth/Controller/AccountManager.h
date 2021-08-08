//
//  AccountManager.cpp
//  Auth
//
//  Created by Cédric B. on 13/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef AccountManager_h
#define AccountManager_h

class AccountManager {
public:
    static void login(Packet &packet, TCPClient &clientDb) {
        Packet::req_login req_login = packet.getDatas<Packet::req_login>();
        clientDb.sender(Packet::REQ_LOGIN, req_login);
        
        packet = clientDb.reader();
    }
    
    static void loginGAuth(Packet &packet, TCPClient &clientDb) {
        std::string token = packet.getDatas<std::string>();
        clientDb.sender(Packet::REQ_GAUTH, token);
        
        packet = clientDb.reader();
    }
};

#endif /* AccountManager_h */
