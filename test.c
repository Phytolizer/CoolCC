#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERRBUF_SIZE 64

static bool run_assertion(char* input, int expected) {
    char template[] = "/tmp/CoolChibiCC_XXXXXX.s";
    int tempfd = mkstemps(template, 2);

    if (tempfd == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to open temp.s: %s\n", errbuf);
        return false;
    }

    pid_t cpid = fork();

    if (cpid == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to fork process: %s\n", errbuf);
        return false;
    }

    if (cpid == 0) {
        dup2(tempfd, STDOUT_FILENO);
        char* args[] = {
            "./CoolChibiCC",
            NULL,
            NULL,
        };
        args[1] = input;
        execv(args[0], args);
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to exec CoolChibiCC: %s\n", errbuf);
        return false;
    }

    int status;

    if (waitpid(cpid, &status, 0) == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to wait for CoolChibiCC: %s\n", errbuf);
        return false;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "CoolChibiCC did not exit normally!\n");
        return false;
    }

    cpid = fork();

    if (cpid == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to fork process: %s\n", errbuf);
        return false;
    }

    if (cpid == 0) {
        char* args[] = {
            "gcc",
            template,
            "-o",
            "./temp.x",
            NULL,
        };
        execvp(args[0], args);
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to exec gcc: %s\n", errbuf);
        return false;
    }

    if (waitpid(cpid, &status, 0) == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to wait for gcc: %s\n", errbuf);
        return false;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "gcc did not exit normally!\n");
        return false;
    }

    cpid = fork();

    if (cpid == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to fork process: %s\n", errbuf);
        return false;
    }

    if (cpid == 0) {
        char* args[] = {
            "./temp.x",
            NULL,
        };
        execvp(args[0], args);
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to exec temp.x: %s\n", errbuf);
        return false;
    }

    if (waitpid(cpid, &status, 0) == -1) {
        char errbuf[ERRBUF_SIZE];
        strerror_r(errno, errbuf, sizeof errbuf);
        fprintf(stderr, "Failed to wait for temp.x: %s\n", errbuf);
        return false;
    }

    if (!WIFEXITED(status)) {
        fprintf(stderr, "gcc did not exit normally!\n");
        return false;
    }

    if (WEXITSTATUS(status) != expected) {
        fprintf(stderr, "expected: %d != %d\n", expected, WEXITSTATUS(status));
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    bool success = run_assertion("0", 0);
    success = success && run_assertion("42", 42);
    success = success && run_assertion("5+20-4", 21);
    return success ? 0 : 1;
}
