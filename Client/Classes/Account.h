//
//  Account.h
//  Client
//
//  Created by Cédric B. on 21/03/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef Account_h
#define Account_h

class Account {
private:
    std::string name;
    bool logged;
    Player player;
    
public:
    Account(std::string name = "", bool logged = false, Player player = {}) {
        this->name = name;
        this->logged = logged;
        this->player = player;
    }
    
    std::string getName() {
        return name;
    }
    
    bool isLogged() {
        return logged;
    }
    
    Player getPlayer() {
        return player;
    }
    
    void setName(std::string name) {
        this->name = name;
    }
    
    void setLogged(bool logged) {
        this->logged = logged;
    }
    
    void setPlayer(Player player) {
        this->player = player;
    }
    
    void reset() {
        setName("");
        setLogged(false);
        setPlayer({});
    }
};

#endif /* Account_h */
