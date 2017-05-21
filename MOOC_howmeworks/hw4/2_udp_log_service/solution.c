#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct sockaddr_in local;

int main(int c, char **v) {
    if (c < 2) {
        return -1;
    }
    int port = atoi(v[1]);

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(atoi(v[1]));
    local.sin_family = AF_INET;

    int result = bind(s, (struct sockaddr*) &local, sizeof(local));

    char buf[BUFSIZ];
    int res;

    while ((res = read(s, buf, BUFSIZ)) > 0) {
        buf[res] = '\0';

        if (strcmp(buf, "OFF\n") == 0) {
            break;
        }

        printf("%s\n", buf);
    }

    return 0;
}

