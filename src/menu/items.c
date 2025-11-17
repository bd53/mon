#include "menu.h"
#include "info.h"

MenuItem MENU_ITEMS[] = {
    {"Language", get_language},
    {"Locales", get_locale},
    {"Mirrors and repositories", get_mirrors},
    {"Disk configuration", get_disk_config},
    {"Swap", get_swap},
    {"Bootloader", get_bootloader},
    {"Unified kernel images", get_kernel},
    {"Hostname", get_hostname},
    {"Root password", get_root_password},
    {"User account", get_user_account},
    {"Profile", get_profile},
    {"Audio", get_audio},
    {"Kernels", get_kernels},
    {"Network configuration", get_network},
    {"Additional packages", get_packages},
    {"Timezone", get_timezone},
    {"Automatic time sync (NTP)", get_ntp},
    {"Recent news", get_news}
};

const int MENU_ITEM_COUNT = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);