#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>

bool is_term = false;

void on_sigurg() {
    is_term = true;
}

int main(int argc, const char **argv) {
    pid_t pid;

    if (0 == (pid = fork())) {
        chdir("/");
        setsid();

        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        signal(SIGURG, on_sigurg);

        while( ! is_term) {}
    } else {
        printf("%d\n", pid);
    }

    return 0;
}

