// logger.h

#ifndef LOGGER_H
#define LOGGER_H

/**
 * Logs the specified message to the system log at the LOG_INFO level.
 * 
 * @param message The message to log.
 */
void log_info(const char* message);

/**
 * Logs the specified message to the system log at the LOG_ERR level.
 * 
 * @param message The message to log.
 */
void log_error(const char* message);

#endif /* LOGGER_H */
