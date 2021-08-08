//
//  Packet.h
//  Client
//
//  Created by Cédric B. on 21/03/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef Packet_h
#define Packet_h

class Packet {
private:
    unsigned short type;
    std::string datas;
    
public:
    Packet(unsigned short type = 0, std::string datas = "") {
        this->type = type;
        this->datas = datas;
    }
    
    unsigned short getType() {
        return type;
    }
    
    template <typename T>
    T getDatas() {
        std::stringstream ss(datas);
        boost::archive::text_iarchive ia(ss);
        T obj;
        ia >> obj;
        return obj;
    }

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & type & datas;
    }
    
    enum {
        REQ_LOGIN,
        REQ_LOGOUT,
        REQ_GAUTH,
        REQ_TOKEN,
        REQ_QUESTS,
        REQ_PLAYERS,
        REQ_PLAYER,
        REP_OK,
        REP_NOK,
        REP_EMPTY,
        REP_ALREADY
    };
    
    struct req_login {
        std::string username = "";
        std::string password = "";
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & username & password;
        }
    };
    
    struct rep_login {
        unsigned short _id = 0;
        std::string name = "";
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & _id & name;
        }
    };
    
    struct rep_list_players {
        unsigned short _id = 0;
        std::string name = "";
        unsigned short level = 0;
        
        template <typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & _id & name & level;
        }
    };
};

#endif /* Packet_h */
