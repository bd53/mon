#include "menu.h"
#include "system_info.h"

void init_menu_items(MenuItem *items) {
    items[0] = (MenuItem){"Language", get_language};
    items[1] = (MenuItem){"Locales", get_locale};
    items[2] = (MenuItem){"Mirrors and repositories", get_mirrors};
    items[3] = (MenuItem){"Disk configuration", get_disk_config};
    items[4] = (MenuItem){"Swap", get_swap};
    items[5] = (MenuItem){"Bootloader", get_bootloader};
    items[6] = (MenuItem){"Unified kernel images", get_kernel};
    items[7] = (MenuItem){"Hostname", get_hostname};
    items[8] = (MenuItem){"Root password", get_root_password};
    items[9] = (MenuItem){"User account", get_user_account};
    items[10] = (MenuItem){"Profile", get_profile};
    items[11] = (MenuItem){"Audio", get_audio};
    items[12] = (MenuItem){"Kernels", get_kernels};
    items[13] = (MenuItem){"Network configuration", get_network};
    items[14] = (MenuItem){"Additional packages", get_packages};
    items[15] = (MenuItem){"Timezone", get_timezone};
    items[16] = (MenuItem){"Automatic time sync (NTP)", get_ntp};
}