#ifndef PROFILE_HPP
#define PROFILE_HPP

#include "../Monitor.hpp"
#include <string>
#include <vector>

struct UserInfo {
    std::string username;
    int uid;
    int gid;
    std::string home;
    std::string shell;
};

class ProfileScreen {
    public:
        static bool render(SystemMonitor& monitor);
        static std::vector<UserInfo> get_system_users();
};

#endif