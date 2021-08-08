//
//  TCPClient.h
//  Client
//
//  Created by Cédric B. on 21/03/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef TCPClient_h
#define TCPClient_h

class TCPClient {
private:
    unsigned short sock;
    
public:
    TCPClient(const char* host, unsigned short port) {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Socket creation error");
            exit(1);
        }
        
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
            perror("Invalid address");
            exit(1);
        }
        
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("Connection Failed");
            exit(1);
        }
    }
    
    void notify(const char *message) {
        if (send(sock, message, strlen(message), 0) != strlen(message)) {
            perror("Notify Failed");
            exit(1);
        }
    }
    
    Packet reader() {
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        
        std::stringstream ss(buffer);
        boost::archive::text_iarchive ia(ss);
        Packet packet;
        ia >> packet;
        
        return packet;
    }
    
    template <typename T>
    void sender(unsigned short type, T &obj) {
        std::stringstream ss;
        boost::archive::text_oarchive *oa = new boost::archive::text_oarchive(ss);
        *oa << obj;
        
        Packet packet(type, ss.str().c_str());
        
        ss.str("");
        oa = new boost::archive::text_oarchive(ss);
        *oa << packet;
        
        notify((ss.str()).c_str());
    }
    
    void sender(unsigned short type) {
        Packet packet(type);
        
        std::stringstream ss;
        boost::archive::text_oarchive oa(ss);
        oa << packet;
        
        notify((ss.str()).c_str());
    }
};

#endif /* TCPClient_h */
