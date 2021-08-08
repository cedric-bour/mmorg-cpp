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
    std::string name;
    unsigned short level;
    bool online;
    
public:
    Player(std::string name = "", unsigned short level = 0, bool online = false) {
        this->name = name;
        this->level = level;
        this->online = online;
    }
    
    std::string getName() {
        return name;
    }
    
    bool isOnline() {
        return online;
    }
    
    void setName(std::string name) {
        this->name = name;
    }
    
    void setLevel(unsigned short level) {
        this->level = level;
    }
    
    void setOnline(bool online) {
        this->online = online;
    }
};

#endif /* Player_h */
