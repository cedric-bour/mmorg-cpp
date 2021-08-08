//
//  Player.h
//  Db
//
//  Created by Cédric B. on 17/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef Player_h
#define Player_h

class Player {
private:
    unsigned short _id;
    unsigned short id_account;
    std::string name;
    unsigned short level;
    
public:
    Player(unsigned short _id = 0, unsigned short id_account = 0, std::string name = "", unsigned short level = 0) {
        this->_id = _id;
        this->id_account = id_account;
        this->name = name;
        this->level = level;
    }
    
    unsigned short getId() {
        return _id;
    }
    
    unsigned short getIdAccount() {
        return id_account;
    }
    
    std::string getName() {
        return name;
    }
    
    unsigned short getLevel() {
        return level;
    }
    
    void setId(unsigned short _id) {
        this->_id = _id;
    }
    
    void setIdAccount(unsigned short id_account) {
        this->id_account = id_account;
    }
    
    void setName(std::string name) {
        this->name = name;
    }
    
    void setLevel(unsigned short level) {
        this->level = level;
    }
};

#endif /* Player_h */
