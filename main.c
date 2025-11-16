#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ui.h"
#include "menu.h"

int main(void) {
    if (geteuid() != 0) {
        printf("Note: Some info may require root privileges.\n");
        sleep(1);
    }
    init_screen();
    run_menu();
    cleanup_screen();
    return 0;
}