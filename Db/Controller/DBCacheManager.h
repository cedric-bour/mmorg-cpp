//
//  DBManager.h
//  Db
//
//  Created by Cédric B. on 17/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef DBManager_h
#define DBManager_h

class DBCacheManager {
public:
    static void dumpAll(sql::Connection *con, std::vector<Quest> &mmo_quest, std::vector<Player> &mmo_player) {
        std::cout << "Dump databases" << std::endl;
        
        sql::Statement *pstmt = con->createStatement();
        
        std::cout << "/PLAYER/" << std::endl;
        sql::ResultSet *res = pstmt->executeQuery("SELECT id, id_account, name, level FROM player;");
        Player player;
        while(res->next()) {
            player.setId(res->getInt(1));
            player.setIdAccount(res->getInt(2));
            player.setName(res->getString(3));
            player.setLevel(res->getBoolean(4));
            mmo_player.push_back(player);
            std::cout << player.getId() << "|" << player.getIdAccount() << "|" << player.getName() << "|" << player.getLevel() << std::endl;
        }
        
        std::cout << "/QUEST/" << std::endl;
        res = pstmt->executeQuery("SELECT id, id_player, name, status FROM quest;");
        Quest quest;
        while(res->next()) {
            quest.setId(res->getInt(1));
            quest.setIdPlayer(res->getInt(2));
            quest.setName(res->getString(3));
            quest.setStatus(res->getBoolean(4));
            mmo_quest.push_back(quest);
            std::cout << quest.getId() << "|" << quest.getIdPlayer() << "|" << quest.getName() << "|" << quest.isStatus() << std::endl;
        }
    }
    
    static void updateAll(sql::Connection *con, std::vector<Quest> &mmo_quest, std::vector<Player> &mmo_player) {
        std::cout << "Restore databases" << std::endl;
        
        sql::Statement *pstmt = con->createStatement();
        
        std::cout << "/PLAYER/" << std::endl;
        pstmt->execute("DELETE FROM player;");
        pstmt->execute("ALTER TABLE player AUTO_INCREMENT=0;");
        if (mmo_player.size() > 0) {
            std::string query = "INSERT INTO player(id_account, name, level) VALUES";
            for (int i = 0; i < mmo_player.size(); i++) {
                std::stringstream ss;
                if (i > 0) ss << ",";
                ss << "(" << mmo_player[i].getIdAccount() << ", '" << mmo_player[i].getName() << "', " << mmo_player[i].getLevel() << ")";
                if (i == mmo_player.size() - 1) ss << ";";
                query += ss.str();
                std::cout << mmo_player[i].getIdAccount() << "|" << mmo_player[i].getName() << "|" << mmo_player[i].getLevel() << std::endl;
            }
            
            pstmt->execute(query);
        }
        
        std::cout << "/QUEST/" << std::endl;
        pstmt->execute("ALTER TABLE quest AUTO_INCREMENT=0;");
        if (mmo_quest.size() > 0) {
            std::string query = "INSERT INTO quest(id_player, name, status) VALUES";
            for (int i = 0; i < mmo_quest.size(); i++) {
                std::stringstream ss;
                if (i > 0) ss << ",";
                ss << "(" << mmo_quest[i].getIdPlayer() << ", '" << mmo_quest[i].getName() << "', " << mmo_quest[i].isStatus() << ")";
                if (i == mmo_quest.size() - 1) ss << ";";
                query += ss.str();
                std::cout << mmo_quest[i].getIdPlayer() << "|" << mmo_quest[i].getName() << "|" << mmo_quest[i].isStatus() << std::endl;
            }
            
            pstmt->execute(query);
        }
    }
};

#endif /* DBManager_h */
