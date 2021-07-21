// telegram-bot-api-client.h

#ifndef TELEGRAM_BOT_API_CLIENT_H
#define TELEGRAM_BOT_API_CLIENT_H

/**
 * Sends a message to the Telegram channel where this bot lives that the
 * specified username opened/closed an SSH connection with the server.
 *
 * @param username The username of the user that opened or closed the SSH
 *                 connection with the server.
 * @param session_state Either "open" or "closed".
 */
void send_ssh_session_notification(const char* username,
                                   const char* session_state);

#endif /* TELEGRAM_BOT_API_CLIENT_H */
