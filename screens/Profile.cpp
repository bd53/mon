#include "Profile.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<UserInfo> ProfileScreen::get_system_users() {
    std::vector<UserInfo> users;
    std::ifstream passwd("/etc/passwd");
    std::string line;
    while (std::getline(passwd, line)) {
        std::istringstream iss(line);
        UserInfo user;
        std::string uid_str, gid_str, skip;
        std::getline(iss, user.username, ':');
        std::getline(iss, skip, ':');
        std::getline(iss, uid_str, ':');
        std::getline(iss, gid_str, ':');
        std::getline(iss, skip, ':');
        std::getline(iss, user.home, ':');
        std::getline(iss, user.shell);
        std::istringstream(uid_str) >> user.uid;
        std::istringstream(gid_str) >> user.gid;
        if (user.uid >= 1000 && user.shell != "/usr/sbin/nologin" && user.shell != "/bin/false" && user.shell != "/sbin/nologin" && user.username != "nobody") {
            users.push_back(user);
        }
    }
    std::sort(users.begin(), users.end(), [](const UserInfo& a, const UserInfo& b) {
        return a.uid < b.uid;
    });
    return users;
}

bool ProfileScreen::render(SystemMonitor&) {
    auto users = get_system_users();
    std::cout << std::left << std::setw(20) << "USERNAME" << std::setw(8) << "UID" << std::setw(8) << "GID" << std::setw(30) << "HOME" << "SHELL\n" << "--------------------------------------------------------------------------------\n";
    for (const auto& user : users) {
        std::string home = user.home.length() > 29 ? "..." + user.home.substr(user.home.length() - 26) : user.home;
        std::string shell = user.shell.length() > 20 ? "..." + user.shell.substr(user.shell.length() - 17) : user.shell;
        std::cout << std::left << std::setw(20) << user.username << std::setw(8) << user.uid << std::setw(8) << user.gid << std::setw(30) << home << shell << "\n";
    }
    std::cout << "\nTotal users: " << users.size() << "\n";
    return false;
}
