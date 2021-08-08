//
//  TCPServer.h
//  Auth
//
//  Created by Cédric B. on 21/03/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef TCPServer_h
#define TCPServer_h

class TCPServer {
private:
    int opt = 1;
    unsigned short master_socket, addrlen, i;
    struct sockaddr_in address;
    std::vector<Player> client_socket;
    
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
    
    void start(TCPClient clientDb, TCPClient clientAuth) {
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
                exit(EXIT_FAILURE);
            }
            
            if (FD_ISSET(master_socket, &readfds)) {
                unsigned short socket;
                if ((socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                    perror("Accept");
                    exit(EXIT_FAILURE);
                }
                
                Player player;
                player.setSocket(socket);
                client_socket.push_back(player);
                Log::write(Log::SYSLOG, "Socket " + std::to_string(socket) + " is connected");
            }
            
            for (i = 0; i < client_socket.size(); i++) {
                if (client_socket[i].getSocket() != 0) {
                    if (FD_ISSET(client_socket[i].getSocket(), &readfds)) {
                        Packet packet;
                        if (reader(packet)) {
                            if (packet.getType() == Packet::REQ_TOKEN) {
                                AccountManager::token(packet, client_socket[i], clientAuth);
                                if (packet.getType() == Packet::REP_OK)
                                    sender(Packet::REP_OK, client_socket[i].getSocket());
                                else if (packet.getType() == Packet::REP_NOK)
                                    sender(Packet::REP_NOK, client_socket[i].getSocket());
                                else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
                            } else if (packet.getType() == Packet::REQ_QUESTS) {
                                unsigned short _id = client_socket[i].getId();
                                clientDb.sender(Packet::REQ_QUESTS, _id);
                                
                                packet = clientDb.reader();
                                if (packet.getType() == Packet::REP_EMPTY)
                                    sender(Packet::REP_EMPTY, client_socket[i].getSocket());
                                else if (packet.getType() == Packet::REP_OK) {
                                    std::vector<Packet::rep_list_quests> quests = packet.getDatas<std::vector<Packet::rep_list_quests>>();
                                    sender(Packet::REP_OK, quests, client_socket[i].getSocket());
                                } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
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
