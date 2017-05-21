#include <unistd.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    pid_t pid;

    if (0 == (pid = fork())) {
        chdir("/");
        setsid();

        fclose(stdin);
        fclose(stdout);
        fclose(stderr);

        while(1) {}
    } else {
        printf("%d\n", pid);
    }

    return 0;
}

