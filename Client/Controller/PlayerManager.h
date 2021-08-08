//
//  PlayerManager.h
//  Client
//
//  Created by Cédric B. on 14/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef PlayerManager_h
#define PlayerManager_h

class PlayerManager {
public:
    static void getPlayers(TCPClient &clientAuth, TCPClient &clientGame, Account &account) {
        clientAuth.sender(Packet::REQ_PLAYERS);
        
        Packet packet = clientAuth.reader();
        if (packet.getType() == Packet::REP_OK) {
            std::vector<Packet::rep_list_players> players = packet.getDatas<std::vector<Packet::rep_list_players>>();
            for (unsigned short i = 0; i < players.size(); i++) {
                std::cout << players[i]._id << ") ";
                std::cout << players[i].name;
                std::cout << " level " << players[i].level << std::endl;
            }
            
            unsigned short choix;
            std::cout << "Choose player : "; std::cin >> choix;
            clientAuth.sender(Packet::REQ_PLAYER, choix);
            
            packet = clientAuth.reader();
            if (packet.getType() == Packet::REP_OK) {
                std::string string(packet.getDatas<std::string>());
                clientGame.sender(Packet::REQ_TOKEN, string);
                
                packet = clientGame.reader();
                if (packet.getType() == Packet::REP_OK) {
                    for (unsigned short i = 0; i < players.size(); i++) {
                        if (players[i]._id == choix) {
                            Player player;
                            player.setName(players[i].name);
                            player.setLevel(players[i].level);
                            player.setOnline(true);
                            account.setPlayer(player);
                            break;
                        }
                    }
                    Log::write(Log::SYSLOG, "Player connected in Game");
                } else if (packet.getType() == Packet::REP_NOK)
                    Log::write(Log::SYSERR, "Token error");
                else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
            } else if (packet.getType() == Packet::REP_NOK)
                Log::write(Log::SYSERR, "No player found");
            else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
        } else if (packet.getType() == Packet::REP_EMPTY)
            Log::write(Log::SYSERR, "No player found");
        else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
    }
};

#endif /* PlayerManager_h */
