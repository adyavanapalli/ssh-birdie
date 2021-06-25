// logger.h

#ifndef LOGGER_H
#define LOGGER_H

/**
 * A log message for a process exiting with a successful exit status.
 */
#define SUCCESSFUL_EXIT_STATUS "Exiting process with successful exit status."

/**
 * A log message for a process exiting with a failing exit status.
 */
#define FAILING_EXIT_STATUS "Exiting process with failing exit status."

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
