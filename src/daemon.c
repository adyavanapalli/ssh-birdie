// daemon.c

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "daemon.h"


/**
 * Forks the current process and exits, leaving the child process to return.
 */
void make_child()
{
    pid_t fork_result = fork();
    if (fork_result > 0)
    {
        // We exit from the parent process.
        exit(EXIT_SUCCESS);
    }
    else if (fork_result == -1)
    {
        // And log any errors and exit by failing.
        fprintf(stderr, "error: %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }
    else if (fork_result == 0)
    {
        // The child should not exit, but simply return.
        return;
    }
    else // fork_result < -1
    {
        // For completeness, we exit failing here even though fork() will never
        // return anything less than -1.
        exit(EXIT_FAILURE);
    }
}

/**
 * Calls setsid().
 */
void create_new_session()
{
    if (setsid() == -1)
    {
        fprintf(stderr, "error: %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }
}

/**
 * Changes the process's current directory to the root directory.
 */
void change_to_root_directory()
{
    if (chdir("/") == -1)
    {
        fprintf(stderr, "error: %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }
}

/**
 * Closes the current process's open file descriptors.
 */
void close_open_file_descriptors()
{
    long max_fd = 0;
    if ((max_fd = sysconf(_SC_OPEN_MAX)) == -1)
    {
        fprintf(stderr, "error: %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }

    for (long fd = 0; fd < max_fd &&
                      // We do not close stderr just yet in case we need to
                      // print an error to it.
                      fd != stderr->_fileno; fd++)
    {
        if (close(fd) == -1)
        {
            fprintf(stderr, "error: %s\n", strerror(errno));

            exit(EXIT_FAILURE);
        }
    }

    // But now we close it.
    if (fclose(stderr) != 0)
    {
        fprintf(stderr, "error: %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }
}

/**
 * Redirects stdin, stdout, and stderr to /dev/null.
 */
void redirect_std_to_dev_null()
{
    if (open("/dev/null", O_RDWR) != STDIN_FILENO)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    if (open("/dev/null", O_RDWR) != STDOUT_FILENO)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }

    if (open("/dev/null", O_RDWR) != STDERR_FILENO)
    {
        // TODO: We should log this to the system log.

        exit(EXIT_FAILURE);
    }
}

/**
 * Daemonizes the current process. The grandchild process is the process that
 * becomes the daemon; the parent and child process exit.
 */
void daemonize()
{
    make_child();

    // We call setsid to create a new session and free it of any association
    // with a controlling terminal.
    create_new_session();

    // We create another child process from the current child process to ensure
    // it can no longer acquire a controlling terminal since it is no longer the
    // session leader.
    make_child();

    // We clear the file mode creation mask so that when the daemon creates any
    // files, all users have access permissions to those files.
    umask(0);

    // Since the daemon is a long-running process, it will inevitably live until
    // system shutdown, but if it lives on a file system other than root, that
    // file system cannot be unmounted.
    change_to_root_directory();

    // Since the daemon no longer has the controlling terminal, it has no use
    // for the usual std* file descriptors and any other file descriptors for
    // that matter.
    close_open_file_descriptors();

    // And finally, we redirect all data written to std* to /dev/null so that if
    // the daemon processes any I/O on those descriptors, it will not fail.
    redirect_std_to_dev_null();
}
