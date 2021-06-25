// logger.c

#include <sys/syslog.h>

#include "logger.h"

/**
 * Logs the specified message to the system log at the LOG_INFO level.
 * 
 * @param message The message to log.
 */
void log_info(const char* message)
{
    syslog(LOG_INFO, "%s", message);
}

/**
 * Logs the specified message to the system log at the LOG_ERR level.
 * 
 * @param message The message to log.
 */
void log_error(const char* message)
{
    syslog(LOG_ERR, "%s", message);
}
