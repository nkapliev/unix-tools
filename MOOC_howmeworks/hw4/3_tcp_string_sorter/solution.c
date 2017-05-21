#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

struct sockaddr_in local;

int comp(const void * a, const void * b) {
    return *(char *)b - *(char *)a;
}

int main(int c, char **v) {
    if (c < 2) {
        return -1;
    }
    int port = atoi(v[1]);

    int ss = socket(AF_INET, SOCK_STREAM, 0);
    int cs;

    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    bind(ss, (struct sockaddr*) &local, sizeof(local));
    listen(ss, 5);

    cs = accept(ss, NULL, NULL);

    char buf[BUFSIZ];
    int res;

    while ((res = recv(cs, buf, BUFSIZ, MSG_NOSIGNAL)) > 0) {
        buf[res] = '\0';

        if (strcmp(buf, "OFF\n") == 0) {
            break;
        }

        qsort(buf, res, sizeof(char), comp);

        send(cs, buf, strlen(buf) + 1, MSG_NOSIGNAL);
    }

    return 0;
}

