//
//  QuestManager.h
//  Client
//
//  Created by Cédric B. on 12/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef QuestManager_h
#define QuestManager_h

class QuestManager {
public:
    static void getQuests(TCPClient &clientGame) {
        clientGame.sender(Packet::REQ_QUESTS);
        
        Packet packet = clientGame.reader();
        if (packet.getType() == Packet::REP_OK) {
            std::vector<Packet::rep_list_quests> quests = packet.getDatas<std::vector<Packet::rep_list_quests>>();
            for (unsigned short i = 0; i < quests.size(); i++) {
                std::cout << quests[i]._id << ") ";
                std::cout << quests[i].name;
                std::cout << " status " << quests[i].status << std::endl;
            }
        } else if (packet.getType() == Packet::REP_EMPTY)
            Log::write(Log::SYSERR, "No quest found");
        else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
    }
};

#endif /* QuestManager_h */
