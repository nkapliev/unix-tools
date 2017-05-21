#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int c, char **v) {
    if (c != 2) {
        return -1;
    }

    struct hostent *h;
    h = gethostbyname(v[1]);

    if (NULL == h) {
        return -1;
    }

    int i;

    for (i = 0; NULL != h->h_addr_list[i]; i++) {
        struct in_addr *a = (struct in_addr*) h->h_addr_list[i];
        printf("%s\n", inet_ntoa(*a));
    }

    return 0;
}

