//
//  AccountManager.h
//  Game
//
//  Created by Cédric B. on 13/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef AccountManager_h
#define AccountManager_h

class AccountManager {
public:
    static void token(Packet &packet, Player &player, TCPClient &clientAuth) {
        std::string string = packet.getDatas<std::string>();
        clientAuth.sender(Packet::REQ_TOKEN, string);
        
        packet = clientAuth.reader();
        if (packet.getType() == Packet::REP_OK) {
            player.setId(packet.getDatas<unsigned short>());
        }
    }
};

#endif /* AccountManager_h */
