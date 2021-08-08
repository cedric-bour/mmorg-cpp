//
//  TCPServer.h
//  Auth
//
//  Created by Cédric B. on 21/03/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef TCPServer_h
#define TCPServer_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <vector>

class TCPServer {
private:
    int opt = 1;
    unsigned short master_socket, addrlen, i;
    struct sockaddr_in address;
    std::vector<unsigned short> client_socket;
    sql::Connection *con;
    std::vector<Quest> mmo_quest;
    std::vector<Player> mmo_player;
    
public:
    TCPServer(unsigned short PORT, sql::Connection *con) {
        this->con = con;
        if ((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
            perror("socket failed");
            exit(1);
        }
        
        if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            exit(1);
        }
        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        
        if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(1);
        }
        
        Log::write(Log::SYSLOG, "Listener on port " + std::to_string(PORT));
        
        if (listen(master_socket, 3) < 0) {
            perror("listen");
            exit(1);
        }
        
        DBCacheManager::dumpAll(con, mmo_quest, mmo_player);
        
        addrlen = sizeof(address);
        Log::write(Log::SYSLOG, "Waiting for connections...");
    }
    
    void start() {
        while (1) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(master_socket, &readfds);
            unsigned short max_sd = master_socket;

            for (i = 0 ; i < client_socket.size(); i++) {
                if (client_socket[i] > 0) {
                    FD_SET(client_socket[i], &readfds);
                }
                
                if (client_socket[i] > max_sd) {
                    max_sd = client_socket[i];
                }
            }
            
            unsigned short activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
            if ((activity < 0) && (errno != EINTR)) {
                perror("Select error");
                exit(1);
            }
            
            if (FD_ISSET(master_socket, &readfds)) {
                unsigned short socket;
                if ((socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("Accept");
                    exit(1);
                }
                
                client_socket.push_back(socket);
                Log::write(Log::SYSLOG, "Socket " + std::to_string(socket) + " is connected");
            }

            for (i = 0; i < client_socket.size(); i++) {
                if (client_socket[i] != 0) {
                    if (FD_ISSET(client_socket[i], &readfds)) {
                        Packet packet;
                        if (reader(packet)) {
                            if (packet.getType() == Packet::REQ_LOGIN) {
                                Packet::rep_login player = AccountManager::login(packet, con);
                                if (player._id > 0) {
                                    sender(Packet::REP_OK, player, client_socket[i]);
                                } else sender(Packet::REP_NOK, client_socket[i]);
                            } else if (packet.getType() == Packet::REQ_GAUTH) {
                                Packet::rep_login player = AccountManager::loginGAuth(packet, con);
                                if (player._id > 0) {
                                    sender(Packet::REP_OK, player, client_socket[i]);
                                } else sender(Packet::REP_NOK, client_socket[i]);
                            } else if (packet.getType() == Packet::REQ_PLAYERS) {
                                std::vector<Packet::rep_list_players> players = AccountManager::getPlayers(packet, mmo_player);
                                if (players.size() > 0) {
                                    sender(Packet::REP_OK, players, client_socket[i]);
                                } else sender(Packet::REP_EMPTY, client_socket[i]);
                            } else if (packet.getType() == Packet::REQ_QUESTS) {
                               std::vector<Packet::rep_list_quests> quests = QuestManager::getQuests(packet, mmo_quest);
                               if (quests.size() > 0) {
                                   sender(Packet::REP_OK, quests, client_socket[i]);
                               } else sender(Packet::REP_EMPTY, client_socket[i]);
                           } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
                        }
                    }
                }
            }
        }
    }
    
    void notify(unsigned short socket, const char *message) {
        if (send(socket, message, strlen(message), 0) != strlen(message)) {
            perror("Notify Failed");
        }
    }
    
    bool reader(Packet &packet) {
        char buffer[1024] = {0};
        if (read(client_socket[i], buffer, 1024) <= 0) {
            close(client_socket[i]);
            Log::write(Log::SYSLOG, "Socket " + std::to_string(client_socket[i]) + " as closed");
            client_socket.erase(client_socket.begin() + i);
            if (client_socket.size() == 0) {
                DBCacheManager::updateAll(con, mmo_quest, mmo_player);
                exit(0);
            }
            return false;
        }
        
        std::stringstream ss(buffer);
        boost::archive::text_iarchive ia(ss);
        ia >> packet;
        return true;
    }
    
    template <typename T>
    void sender(unsigned short type, T &obj, unsigned short socket) {
        std::stringstream ss;
        boost::archive::text_oarchive *oa = new boost::archive::text_oarchive(ss);
        *oa << obj;
        
        Packet packet(type, ss.str().c_str());
        
        ss.str("");
        oa = new boost::archive::text_oarchive(ss);
        *oa << packet;
        
        notify(socket, (ss.str()).c_str());
    }
    
    void sender(unsigned short type, unsigned short socket) {
        Packet packet(type);
        
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << packet;
        
        notify(socket, (ss.str()).c_str());
    }
};

#endif /* TCPServer_h */
