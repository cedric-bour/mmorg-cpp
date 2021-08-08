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
    unsigned short _id;
    bool logged;
    std::string token;
    unsigned short socket;
    std::vector<Player> players;
    Player player;
    
public:
    Account(unsigned short _id = 0, bool logged = false, std::string token = "", unsigned short socket = 0, std::vector<Player> players = {}, Player player = {}) {
        this->_id = _id;
        this->logged = logged;
        this->token = token;
        this->socket = socket;
        this->players = players;
        this->player = player;
    }
    
    unsigned short getId() {
        return _id;
    }
    
    bool isLogged() {
        return logged;
    }
    
    std::string getToken() {
        return token;
    }
    
    unsigned short getSocket() {
        return socket;
    }
    
    std::vector<Player> getPlayers() {
        return players;
    }
    
    Player getPlayer() {
        return player;
    }
    
    void setId(unsigned short _id) {
        this->_id = _id;
    }
    
    void setLogged(bool logged) {
        this->logged = logged;
    }
    
    void setToken(std::string token) {
        this->token = token;
    }
    
    void setSocket(unsigned short socket) {
        this->socket = socket;
    }
    
    void setPlayers(std::vector<Player> players) {
        this->players = players;
    }
    
    void setPlayer(Player player) {
        this->player = player;
    }
    
    void reset() {
        setId(0);
        setLogged(false);
        setToken("");
        setPlayers({});
        setPlayer({});
    }
};

#endif /* Account_h */
