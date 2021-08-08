//
//  TCPServer.cpp
//  Auth
//
//  Created by Cédric B. on 13/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef TCPServer_h
#define TCPServer_h

class TCPServer {
private:
    int opt = 1;
    unsigned short master_socket, addrlen, i;
    struct sockaddr_in address;
    std::vector<Account> client_socket;
    
public:
    TCPServer(unsigned short PORT) {
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
        
        addrlen = sizeof(address);
        Log::write(Log::SYSLOG, "Waiting for connections...");
    }
    
    void start(TCPClient clientDb) {
        while (1) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(master_socket, &readfds);
            unsigned short max_sd = master_socket;
            
            for (i = 0 ; i < client_socket.size(); i++) {
                if (client_socket[i].getSocket() > 0) {
                    FD_SET(client_socket[i].getSocket(), &readfds);
                }
                
                if (client_socket[i].getSocket() > max_sd) {
                    max_sd = client_socket[i].getSocket();
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
                
                Account account;
                account.setSocket(socket);
                client_socket.push_back(account);
                Log::write(Log::SYSLOG, "Socket " + std::to_string(socket) + " is connected");
            }
            
            for (i = 0; i < client_socket.size(); i++) {
                if (client_socket[i].getSocket() != 0) {
                    if (FD_ISSET(client_socket[i].getSocket() , &readfds)) {
                        Packet packet;
                        if (reader(packet)) {
                            if (packet.getType() == Packet::REQ_LOGIN) {
                                AccountManager::login(packet, clientDb);
                                if (packet.getType() == Packet::REP_NOK) {
                                    sender(Packet::REP_NOK, client_socket[i].getSocket());
                                } else if (packet.getType() == Packet::REP_OK) {
                                    if (any_of(client_socket.begin(), client_socket.end(), [&](Account& account) {
                                        return account.getId() == packet.getDatas<Packet::rep_login>()._id && account.isLogged();
                                    })) {
                                        sender(Packet::REP_ALREADY, client_socket[i].getSocket());
                                    } else {
                                        client_socket[i].setId(packet.getDatas<Packet::rep_login>()._id);
                                        client_socket[i].setLogged(true);
                                        std::string name(packet.getDatas<Packet::rep_login>().name);
                                        sender(Packet::REP_OK, name, client_socket[i].getSocket());
                                    }
                                } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
                            } else if (packet.getType() == Packet::REQ_GAUTH) {
                                AccountManager::loginGAuth(packet, clientDb);
                                if (packet.getType() == Packet::REP_NOK) {
                                    sender(Packet::REP_NOK, client_socket[i].getSocket());
                                } else if (packet.getType() == Packet::REP_OK) {
                                    if (any_of(client_socket.begin(), client_socket.end(), [&](Account& account) {
                                        return account.getId() == packet.getDatas<Packet::rep_login>()._id && account.isLogged();
                                    })) {
                                        sender(Packet::REP_ALREADY, client_socket[i].getSocket());
                                    } else {
                                        client_socket[i].setId(packet.getDatas<Packet::rep_login>()._id);
                                        client_socket[i].setLogged(true);
                                        std::string name(packet.getDatas<Packet::rep_login>().name);
                                        sender(Packet::REP_OK, name, client_socket[i].getSocket());
                                    }
                                } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
                            } else if (packet.getType() == Packet::REQ_LOGOUT) {
                                client_socket[i].reset();
                                sender(Packet::REP_OK, client_socket[i].getSocket());
                            } else if (packet.getType() == Packet::REQ_TOKEN) {
                                std::string token(packet.getDatas<std::string>());
                                bool success = false;
                                for (unsigned short t = 0 ; t < client_socket.size(); t++) {
                                    if (token == client_socket[t].getToken()) {
                                        unsigned short _id = client_socket[t].getPlayer().getId();
                                        sender(Packet::REP_OK, _id, client_socket[i].getSocket());
                                        success = true;
                                        break;
                                    }
                                }
                                
                                if (success == false) sender(Packet::REP_NOK, client_socket[i].getSocket());
                            } else if (packet.getType() == Packet::REQ_PLAYERS) {
                                 unsigned short _id = client_socket[i].getId();
                                 clientDb.sender(Packet::REQ_PLAYERS, _id);
                                 
                                 packet = clientDb.reader();
                                 if (packet.getType() == Packet::REP_EMPTY)
                                     sender(Packet::REP_EMPTY, client_socket[i].getSocket());
                                 else if (packet.getType() == Packet::REP_OK) {
                                     std::vector<Packet::rep_list_players> rep_list_players = packet.getDatas<std::vector<Packet::rep_list_players>>();
                                     
                                     std::vector<Player> players;
                                     Player player;
                                     for (unsigned short t = 0; t < rep_list_players.size(); t++) {
                                         player.setId(rep_list_players[t]._id);
                                         player.setIdClient(t + 1);
                                         players.push_back(player);
                                         rep_list_players[t]._id = t + 1;
                                     }
                                     client_socket[i].setPlayers(players);
                                     
                                     sender(Packet::REP_OK, rep_list_players, client_socket[i].getSocket());
                                 } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
                            } else if (packet.getType() == Packet::REQ_PLAYER) {
                                unsigned short _id = packet.getDatas<unsigned short>();
                                
                                bool success = false;
                                for (unsigned short t = 0; t < client_socket[i].getPlayers().size(); t++) {
                                    
                                    if (client_socket[i].getPlayers()[t].getIdClient() == _id) {
                                        std::string token;
                                        bool already = false;
                                        do {
                                            token = boost::uuids::to_string(boost::uuids::random_generator()());
                                            if (any_of(client_socket.begin(), client_socket.end(), [&](Account& account) {
                                                return account.getToken() == token;
                                            })) {
                                                already = true;
                                            }
                                        } while(already);

                                        Player player;
                                        player.setId(client_socket[i].getPlayers()[t].getId());
                                        client_socket[i].setPlayer(player);
                                        client_socket[i].setPlayers({});
                                        client_socket[i].setToken(token);
                                        sender(Packet::REP_OK, token, client_socket[i].getSocket());
                                        success = true;
                                        break;
                                    }
                                }
                                
                                if (success == false) sender(Packet::REP_NOK, client_socket[i].getSocket());
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
        if (read(client_socket[i].getSocket(), buffer, 1024) <= 0) {
            close(client_socket[i].getSocket());
            Log::write(Log::SYSLOG, "Socket " + std::to_string(client_socket[i].getSocket()) + " as closed");
            client_socket.erase(client_socket.begin() + i);
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

