//
//  Log.h
//  Client
//
//  Created by Cédric B. on 12/05/2020.
//  Copyright © 2020 Cédric B. All rights reserved.
//

#ifndef Log_h
#define Log_h

class Log {
private:
    std::ofstream file;
    
public:
    Log(unsigned short filetype) {
        file.open(filetype == SYSLOG ? "syslog" : "syserr");
        file << getDate() << "File " << (filetype == SYSLOG ? "syslog" : "syserr") << " created" << "\n";
        file.close();
    }
    
    static void write(unsigned short filetype, std::string string) {
        printf("%s\n", string.c_str());
        std::ofstream file;
        file.open(filetype == SYSLOG ? "syslog" : "syserr", std::ios::app);
        file << getDate() << string << "\n";
        file.close();
    }
    
    static std::string getDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        
        std::stringstream ss;
        ss << "[" << 1900 + ltm->tm_year << "-"
        << 1 + ltm->tm_mon << "-" << ltm->tm_mday
        << " " << ltm->tm_hour << ":" << ltm->tm_min << ":"
        << ltm->tm_sec << "] ";
        
        return ss.str();
    }
    
    enum {
        SYSLOG,
        SYSERR
    };
};

#endif /* Log_h */
