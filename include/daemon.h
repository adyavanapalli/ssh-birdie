// daemon.h

/**
 * Daemonizes the current process. The grandchild process is the process that
 * becomes the daemon; the parent and child process exit.
 */
void daemonize();
