// ssh-birdie.c

#include <stdbool.h>
#include <unistd.h>

#include "daemon.h"

int main()
{
    daemonize();

    while (true)
    {
        // The brunt of the logic should live here i.e. the bits that check the
        // system log and report the results to Telegram. After each check, the
        // daemon should sleep for one second.

        sleep(1);
    }

    return 0;
}
