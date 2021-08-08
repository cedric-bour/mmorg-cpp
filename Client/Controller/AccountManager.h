//
//  AccountManager.h
//  Client
//
//  Created by Cédric B. on 12/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef AccountManager_h
#define AccountManager_h

class AccountManager {
public:
    static void login(TCPClient &clientAuth, Account &account) {
        Packet::req_login req_login;
        printf("Username: "); std::cin >> req_login.username;
        printf("Password: "); std::cin >> req_login.password;
        
        clientAuth.sender(Packet::REQ_LOGIN, req_login);
        Packet packet = clientAuth.reader();
        if (packet.getType() == Packet::REP_NOK)
            Log::write(Log::SYSLOG, "Login error: username or password wrong");
        else if (packet.getType() == Packet::REP_ALREADY)
            Log::write(Log::SYSLOG, "Login error: account already connected");
        else if (packet.getType() == Packet::REP_OK) {
            account.setName(packet.getDatas<std::string>());
            account.setLogged(true);
            Log::write(Log::SYSLOG, "Login success");
        } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
    }
    
    static void logout(TCPClient &clientAuth, Account &account) {
        clientAuth.sender(Packet::REQ_LOGOUT);
        
        Packet packet = clientAuth.reader();
        if (packet.getType() == Packet::REP_OK) {
            account.reset();
            Log::write(Log::SYSLOG, "Logout success");
        }
        else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
    }
    
    static void loginGAuth(TCPClient &clientAuth, Account &account) {
#ifdef LOCAL
        system("open https://accounts.google.com/o/oauth2/auth\?scope\\=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fuserinfo.profile%20https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fuserinfo.email\\&redirect_uri\\=http%3A%2F%2F127.0.0.1%2Fgauth.php\\&response_type\\=code\\&client_id\\=922031450654-dg3g0ho00713vqs2lgn9ujps1j0h1hoq.apps.googleusercontent.com\\&access_type\\=online");
#else
        system("open https://accounts.google.com/o/oauth2/auth\?scope\\=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fuserinfo.profile%20https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fuserinfo.email\\&redirect_uri\\=https%3A%2F%2Fwww.cedric-bour.fr%2Fgauth.php\\&response_type\\=code\\&client_id\\=922031450654-dg3g0ho00713vqs2lgn9ujps1j0h1hoq.apps.googleusercontent.com\\&access_type\\=online");
#endif
        printf("Your Browser is open, login you\nEnter Token: "); std::string token; std::cin >> token;
        clientAuth.sender(Packet::REQ_GAUTH, token);
        
        Packet packet = clientAuth.reader();
        if (packet.getType() == Packet::REP_NOK)
            Log::write(Log::SYSLOG, "Login error: token wrong");
        else if (packet.getType() == Packet::REP_ALREADY)
            Log::write(Log::SYSLOG, "Login error: account already connected");
        else if (packet.getType() == Packet::REP_OK) {
            Log::write(Log::SYSLOG, "Login success");
            account.setName(packet.getDatas<std::string>());
            account.setLogged(true);
        } else Log::write(Log::SYSERR, "Packet error: " + std::to_string(packet.getType()));
    }
};

#endif /* AccountManager_h */
