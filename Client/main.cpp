//
//  main.cpp
//  Client
//
//  Created by Cédric B. on 25/02/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

//#define LOCAL
#ifdef LOCAL
#define HOST_AUTH "127.0.0.1"
#define HOST_GAME "127.0.0.1"
#else
#define HOST_AUTH "91.121.82.115"
#define HOST_GAME "91.121.82.115"
#endif

#include "Utils/includes.h"

int main()
{
    Log syslog(Log::SYSLOG);
    Log syserr(Log::SYSERR);
    
    TCPClient clientAuth(HOST_AUTH, 11002);
    
    unsigned short choix;
    printf("Enter number of channel: "); std::cin >> choix;
    TCPClient clientGame(HOST_GAME, 13000 + choix);
    
    Account account;
    while (1) {
        if (account.getPlayer().isOnline()) {
            printf("Hello %s, your player is %s\n[1] List Quests\n[2] Change character\n[3] Logout\nYour choice: ", account.getName().c_str(), account.getPlayer().getName().c_str());
            std::cin >> choix;
            
            if (choix == 1) QuestManager::getQuests(clientGame);
            else if (choix == 2) PlayerManager::getPlayers(clientAuth, clientGame, account);
            else if (choix == 3) AccountManager::logout(clientAuth, account);
        } else if (account.isLogged()) {
            printf("Hello %s\n[1] Choose character\n[2] Logout\nYour choice: ", account.getName().c_str());
            std::cin >> choix;
            
            if (choix == 1) PlayerManager::getPlayers(clientAuth, clientGame, account);
            else if (choix == 2) AccountManager::logout(clientAuth, account);
        } else {
            printf("[1] Login\n[2] Login with Google Account\n[3] Exit\nYour choice: ");
            std::cin >> choix;
            
            if (choix == 1) AccountManager::login(clientAuth, account);
            else if (choix == 2) AccountManager::loginGAuth(clientAuth, account);
            else if (choix == 3) break;
        }
        std::cout << std::endl;
    }
    
    return 0;
}
