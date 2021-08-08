//
//  AccountManager.h
//  Db
//
//  Created by Cédric B. on 12/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef AccountManager_h
#define AccountManager_h

class AccountManager {
public:
    static Packet::rep_login login(Packet &packet, sql::Connection *con) {
        sql::PreparedStatement *pstmt = con->prepareStatement("SELECT id, name FROM account WHERE username=? AND password=SHA1(?);");
        pstmt->setString(1, packet.getDatas<Packet::req_login>().username);
        pstmt->setString(2, packet.getDatas<Packet::req_login>().password);
        sql::ResultSet *res = pstmt->executeQuery();
        
        Packet::rep_login player;
        if (res->rowsCount() && res->next()) {
            player._id = res->getInt(1);
            player.name = res->getString(2);
        }
        
        return player;
    }
    
    static Packet::rep_login loginGAuth(Packet &packet, sql::Connection *con) {
        sql::PreparedStatement *pstmt = con->prepareStatement("SELECT account.id, account.name FROM account LEFT JOIN account_gauth ON account.id = account_gauth.id_account WHERE account_gauth.token=?;");
        pstmt->setString(1, packet.getDatas<std::string>());
        sql::ResultSet *res = pstmt->executeQuery();
        
        Packet::rep_login player;
        if (res->rowsCount() && res->next()) {
            player._id = res->getInt(1);
            player.name = res->getString(2);
        }
        
        return player;
    }
    
    static std::vector<Packet::rep_list_players> getPlayers(Packet &packet, std::vector<Player> &mmo_player) {
        std::vector<Packet::rep_list_players> players;
        for (int i = 0; i < mmo_player.size(); i++) {
            if (mmo_player[i].getIdAccount() == packet.getDatas<unsigned short>()) {
                Packet::rep_list_players player;
                player._id = mmo_player[i].getId();
                player.name = mmo_player[i].getName();
                player.level = mmo_player[i].getLevel();
                players.push_back(player);
            }
        }
        
        return players;
    }
};

#endif /* AccountManager_h */
