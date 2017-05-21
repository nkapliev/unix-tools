#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char * argv[]) {
    int result;
    fd_set readset;

    bool is_in_1_open, is_in_2_open;
    int sum = 0;
    int in_1, in_2;
    bool in_1_closed, in_2_closed;
    char buff[255];

    if (-1 == (in_1 = open("in1", O_RDONLY))) {
        return 1;
    }
    in_1_closed = false;

    if (-1 == (in_2 = open("in2", O_RDONLY))) {
        return 1;
    }
    in_2_closed = false;

    while(
        ! in_1_closed && FD_ISSET(in_1, &readset) ||
        ! in_2_closed && FD_ISSET(in_2, &readset)
    ) {
        FD_ZERO(&readset);
        FD_SET(in_1, &readset);
        FD_SET(in_2, &readset);

        result = select(in_2 + 1, &readset, NULL, NULL, NULL);

        if (result > 0) {
            if ( ! in_1_closed && FD_ISSET(in_1, &readset)) {
                result = read(in_1, buff, sizeof(buff));

                if (0 == result)
                    in_1_closed = true;
                else
                    sum += atoi(buff);
            }

            if ( ! in_2_closed && FD_ISSET(in_2, &readset)) {
                result = read(in_2, buff, sizeof(buff));

                if (0 == result)
                    in_2_closed = true;
                else
                    sum += atoi(buff);
            }
        }
        else if (result < 0) {
            printf("Error on select()\n");
        }
    }

    printf("%d\n", sum);

    return 0;
}

