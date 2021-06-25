// ssh-birdie.c

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <regex.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <systemd/sd-journal.h>

#include "constants.h"
#include "daemon.h"
#include "logger.h"
#include "telegram_bot_api_client.h"

/**
 * Opens the system journal filtered to the sshd identifier and messages
 * originating from now on.
 * 
 * @returns A pointer to a sd_journal struct.
 */
sd_journal* open_journal_to_sshd_at_current_time()
{
    sd_journal* sd_journal_ptr = NULL;
    sd_journal_open(&sd_journal_ptr, SD_JOURNAL_SYSTEM);

    struct timespec ts = { .tv_sec = 0L, .tv_nsec = 0L };
    clock_gettime(CLOCK_REALTIME, &ts);

    uint64_t usec = SECONDS_TO_MICROSECONDS(ts.tv_sec) +
                    NANOSECONDS_TO_MICROSECONDS(ts.tv_nsec);
    sd_journal_seek_realtime_usec(sd_journal_ptr, usec);

    sd_journal_add_match(sd_journal_ptr,
                         SSHD_PROCESS_NAME,
                         0);

    return sd_journal_ptr;
}

/**
 * A flag indicating whether SIGINT or SIGTERM was caught. This is used to gate
 * cleanup logic for this program.
 */
static bool was_sigint_or_sigterm_caught = false;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
/**
 * A signal handler for catching SIGINT or SIGTERM.
 * 
 * @param signum        The value of the signal that was caught.
 * @param siginfo_ptr   Information associated with the signal.
 * @param unused_ptr    Unused.
 */
void signal_handler(int signum, siginfo_t* siginfo_ptr, void* unused_ptr)
{
    log_info("SIGINT/SIGTERM was caught.");

    was_sigint_or_sigterm_caught = true;
}
#pragma GCC diagnostic pop

/**
 * Sets up the signal handler for catching SIGINT and SIGTERM.
 */
void setup_signal_handler()
{
    struct sigaction action =
    {
        .sa_flags = SA_SIGINFO,
        .sa_sigaction = signal_handler
    };

    sigemptyset(&action.sa_mask);

    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

int main()
{
    log_info("Started.");

    daemonize();
    log_info("The program was successfully daemonized.");

    setup_signal_handler();

    sd_journal* sd_journal_ptr = open_journal_to_sshd_at_current_time();
    log_info("Opened system journal to the sshd identifier.");

    regex_t regex;
    memset(&regex, 0, sizeof(regex_t));

    regcomp(&regex,
            SSHD_SESSION_JOURNAL_ENTRY_PATTERN,
            REG_EXTENDED);

    while (!was_sigint_or_sigterm_caught)
    {
        while (sd_journal_next(sd_journal_ptr) == 1)
        {
            const char *message = NULL;
            size_t message_length = 0;

            sd_journal_get_data(sd_journal_ptr,
                                USER_JOURNAL_FIELD_MESSAGE,
                                (const void **)&message,
                                &message_length);

            regmatch_t matches[3] =
            {
                // The first match should match the entire string.
                { .rm_so = 0, .rm_eo = 0 },

                // The second match should match either `opened` or `closed`.
                { .rm_so = 0, .rm_eo = 0 },

                // The third match should match the username of the user.
                { .rm_so = 0, .rm_eo = 0 }

                // There is also another match, but since we do not care about
                // its value, we do not allocate a regmatch_t for it.
            };

            regexec(&regex,
                    message,
                    sizeof(matches) / sizeof(regmatch_t),
                    matches,
                    0);

            // If we find a match, the first match will be the entire string, so
            // its end offset should be positive.
            if (matches[0].rm_eo > 0)
            {
                log_info("Found new SSH session state change.");

                char username[BUFSIZ];
                memset(username, 0, BUFSIZ * sizeof(char));

                char session_state[BUFSIZ];
                memset(session_state, 0, BUFSIZ * sizeof(char));

                sprintf(username,
                        "%.*s",
                        matches[2].rm_eo - matches[2].rm_so,
                        message + matches[2].rm_so);

                sprintf(session_state,
                        "%.*s",
                        matches[1].rm_eo - matches[1].rm_so,
                        message + matches[1].rm_so);

                send_ssh_session_notification(username, session_state);
                log_info("Sent SSH session notification to Telegram.");
            }
        }

        sleep(1);
    }

    log_info("Cleaning up.");

    sd_journal_close(sd_journal_ptr);

    regfree(&regex);

    log_info("Stopped.");

    return 0;
}
