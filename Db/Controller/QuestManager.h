//
//  QuestManager.h
//  Db
//
//  Created by Cédric B. on 14/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef QuestManager_h
#define QuestManager_h

class QuestManager {
public:
    static std::vector<Packet::rep_list_quests> getQuests(Packet &packet, std::vector<Quest> &mmo_quests) {
        std::vector<Packet::rep_list_quests> quests;
        for (int i = 0; i < mmo_quests.size(); i++) {
            if (mmo_quests[i].getIdPlayer() == packet.getDatas<unsigned short>()) {
                Packet::rep_list_quests quest;
                quest._id = mmo_quests[i].getId();
                quest.name = mmo_quests[i].getName();
                quest.status = mmo_quests[i].isStatus();
                quests.push_back(quest);
            }
        }
        
        return quests;
    }
};

#endif /* QuestManager_h */
