//
//  Quest.h
//  Db
//
//  Created by Cédric B. on 17/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef Quest_h
#define Quest_h

class Quest {
private:
    unsigned short _id;
    unsigned short id_player;
    std::string name;
    bool status;
    
public:
    Quest(unsigned short _id = 0, unsigned short id_player = 0, std::string name = "", bool status = false) {
        this->_id = _id;
        this->id_player = id_player;
        this->name = name;
        this->status = status;
    }
    
    unsigned short getId() {
        return _id;
    }
    
    unsigned short getIdPlayer() {
        return id_player;
    }
    
    std::string getName() {
        return name;
    }
    
    bool isStatus() {
        return status;
    }
    
    void setId(unsigned short _id) {
        this->_id = _id;
    }
    
    void setIdPlayer(unsigned short id_player) {
        this->id_player = id_player;
    }
    
    void setName(std::string name) {
        this->name = name;
    }
    
    void setStatus(bool status) {
        this->status = status;
    }
};

#endif /* Quest_h */
