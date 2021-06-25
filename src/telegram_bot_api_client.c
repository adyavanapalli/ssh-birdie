// telegram_bot_api_client.c

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "constants.h"
#include "telegram_bot_api_client.h"


/**
 * Sends a message to the Telegram channel where this bot lives that the
 * specified username opened/closed an SSH connection with the server.
 *
 * @param username The username of the user that opened or closed the SSH
 *                 connection with the server.
 * @param session_state Either "open" or "closed".
 */
void send_ssh_session_notification(const char* username,
                                   const char* session_state)
{    
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    CURL* curl_ptr = NULL;
    if ((curl_ptr = curl_easy_init()) == NULL)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    FILE* f_ptr = NULL;
    if ((f_ptr = fopen("/dev/null", "w")) == NULL)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    if (curl_easy_setopt(curl_ptr, CURLOPT_WRITEDATA, (void*)f_ptr) != CURLE_OK)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    char get_url[BUFSIZ];
    memset(get_url, 0, BUFSIZ * sizeof(char));

    char text[BUFSIZ];
    memset(text, 0, BUFSIZ * sizeof(char));

    if (sprintf(text,
                SSH_SESSION_NOTIFICATION_TEMPLATE,
                username,
                session_state) < 0)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    char* escaped_text = NULL;
    if ((escaped_text = curl_easy_escape(curl_ptr, text, 0)) == NULL)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    char* token = NULL;
    if ((token = getenv(TELEGRAM_BOT_API_TOKEN_NAME)) == NULL)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    char* chat_id = NULL;
    if ((chat_id = getenv(TELEGRAM_BOT_API_CHAT_ID_NAME)) == NULL)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    if (sprintf(get_url,
                TELEGRAM_BOT_API_SEND_MESSAGE_TEMPLATE,
                token,
                chat_id,
                escaped_text) < 0)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }
    
    curl_free(escaped_text);

    // NOTE: We leave this commented out code for future debugging purposes.
    // if (curl_easy_setopt(curl_ptr, CURLOPT_VERBOSE, 1L) != CURLE_OK)
    // {
    //     // TODO: We should log this to the system log.

    //     exit(EXIT_FAILURE);
    // }

    if (curl_easy_setopt(curl_ptr, CURLOPT_URL, get_url) != CURLE_OK)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    if (curl_easy_perform(curl_ptr) != CURLE_OK)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    if (fclose(f_ptr) != 0)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    curl_easy_cleanup(curl_ptr);
}
