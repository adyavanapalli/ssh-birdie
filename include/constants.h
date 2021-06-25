// constants.h

#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * Converts the specified seconds to microseconds.
 */
#define SECONDS_TO_MICROSECONDS(s) (1000000UL * s)

/**
 * Converts the specified nanoseconds to microseconds.
 */
#define NANOSECONDS_TO_MICROSECONDS(ns) (ns / 1000UL)

/**
 * The human-readable message string for this entry. This is supposed to be the
 * primary text shown to the user. It is usually not translated (but might be in
 * some cases), and is not supposed to be parsed for metadata.
 */
#define USER_JOURNAL_FIELD_MESSAGE "MESSAGE"

/**
 * The name of the process the journal entry originates from.
 */
#define TRUSTED_JOURNAL_FIELD__COMM "_COMM"

/**
 * The name of the sshd process the journal entry originates from.
 */
#define SSHD_PROCESS_NAME TRUSTED_JOURNAL_FIELD__COMM "=sshd"

/**
 * The sshd journal entry pattern we are on the lookout for to determine users
 * who opened/closed SSH connections with the server.
 */
#define SSHD_SESSION_JOURNAL_ENTRY_PATTERN \
    "^MESSAGE=pam_unix\\(sshd:session\\): session (opened|closed) for user (\\w+)(.*){0,1}$"

/**
 * A template for the notification we will be using to notify the client that a
 * user has opened/closed an SSH connection with the server.
 */
#define SSH_SESSION_NOTIFICATION_TEMPLATE \
    "`🐦 A little birdie told me <%s> [%s] a connection with the server 🐦.`"

/**
 * The name of the environment variable used to store the Telegram bot API
 * token.
 */
#define TELEGRAM_BOT_API_TOKEN_NAME "SSH_BIRDIE_TELEGRAM_BOT_API_TOKEN"

/**
 * The name of the environment variable for the chat ID of the chat associated
 * with the Telegram bot.
 */
#define TELEGRAM_BOT_API_CHAT_ID_NAME "SSH_BIRDIE_CHAT_ID"

/**
 * A template for the base URL for the Telegram bot API.
 */
#define TELEGRAM_BOT_API_BASE_URL_TEMPLATE "https://api.telegram.org/bot%s"

/**
 * The Telegram bot API method for sending messages.
 */
#define TELEGRAM_BOT_API_METHOD_SEND_MESSAGE "/sendMessage"

/**
 * A template for the URL parameters used in the Telegram bot API sendMessage
 * method.
 */
#define TELEGRAM_BOT_API_METHOD_SEND_MESSAGE_URL_PARAMETERS_TEMPLATE \
    "?chat_id=%s&text=%s&parse_mode=MarkdownV2"

/**
 * A template for the full URL used to send a message via the Telegram bot API.
 */
#define TELEGRAM_BOT_API_SEND_MESSAGE_TEMPLATE \
    TELEGRAM_BOT_API_BASE_URL_TEMPLATE \
    TELEGRAM_BOT_API_METHOD_SEND_MESSAGE \
    TELEGRAM_BOT_API_METHOD_SEND_MESSAGE_URL_PARAMETERS_TEMPLATE

#endif /* CONSTANTS_H */
