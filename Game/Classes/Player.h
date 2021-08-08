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
    unsigned short socket;
    
public:
    Player(unsigned short _id = 0, unsigned short socket = 0) {
        this->_id = _id;
        this->socket = socket;
    }
    
    unsigned short getId() {
        return _id;
    }
    
    unsigned short getSocket() {
        return socket;
    }
    
    void setId(unsigned short _id) {
        this->_id = _id;
    }
    
    void setSocket(unsigned short socket) {
        this->socket = socket;
    }
};

#endif /* Player_h */
