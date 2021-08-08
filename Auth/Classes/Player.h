//
//  Player.h
//  Auth
//
//  Created by Cédric B. on 13/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef Player_h
#define Player_h

class Player {
private:
    unsigned short _id;
    unsigned short id_client;
    
public:
    Player(unsigned short _id = 0, unsigned short id_client = 0) {
        this->_id = _id;
        this->id_client = id_client;
    }
    
    unsigned short getId() {
        return _id;
    }
    
    unsigned short getIdClient() {
        return id_client;
    }
    
    void setId(unsigned short _id) {
        this->_id = _id;
    }
    
    void setIdClient(unsigned short id_client) {
        this->id_client = id_client;
    }
};

#endif /* Player_h */
