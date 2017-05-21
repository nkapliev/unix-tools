#include <stdio.h>
#include <string.h>

const char SYMBOL = '0';

int main(int argc, char * argv[]) {
    if (argc < 3) {
        return 1;
    }

    int symbol_counter = 0;
    FILE* command_pipe;
    char buff[255];

    strcpy(buff, argv[1]);
    strcat(buff, " ");
    strcat(buff, argv[2]);

    if (NULL == (command_pipe = popen(buff, "r"))) {
        return 1;
    }

    while(NULL != fgets(buff, sizeof(buff), command_pipe)) {
        char *pch = strchr(buff, SYMBOL);
        while (pch != NULL) {
            symbol_counter++;
            pch = strchr(pch + 1, SYMBOL);
        }
    }

    printf("%d\n", symbol_counter);

    pclose(command_pipe);

    return 0;
}

