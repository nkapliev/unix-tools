#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

int sigusr1_counter = 0;
int sigusr2_counter = 0;
bool is_term = false;

void sigusr_counter_inc(int sig_num) {
    if (sig_num == SIGUSR1) {
        sigusr1_counter++;
    } else if (sig_num == SIGUSR2) {
        sigusr2_counter++;
    }
}

void on_sigterm() {
    printf("%d %d\n", sigusr1_counter, sigusr2_counter);
    is_term = true;
}

int main() {
    signal(SIGUSR1, sigusr_counter_inc);
    signal(SIGUSR2, sigusr_counter_inc);
    signal(SIGTERM, on_sigterm);

    while( ! is_term) {};

    return 0;
}

