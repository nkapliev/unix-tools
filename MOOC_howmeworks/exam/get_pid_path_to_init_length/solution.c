#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <dirent.h>
#include <sys/types.h>

const char *INIT_PID = "1";

char *build_process_status_path(char *buff, const char *pid) {
    strcpy(buff, "/proc/");
    strcat(buff, pid);
    strcat(buff, "/status");

    return buff;
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Incorrect pid\n");

        return 1;
    }

    char pid[16];
    strcpy(pid, argv[1]);

    regex_t ppid_regex;

    if (regcomp(&ppid_regex, "^PPid:\t([0-9]+)$", REG_EXTENDED|REG_NEWLINE)) {
        fprintf(stderr, "Could not compile ppid regex\n");

        return 1;
    }

    size_t number_of_pmatches = 2;
    regmatch_t pmatch[number_of_pmatches];
    int reti;

    char buff[255];
    FILE *status_file;

    int counter = 1;

    while (
        (0 != strcmp(pid, INIT_PID)) &&
        (NULL != (status_file = fopen(build_process_status_path(buff, pid), "r")))
    ) {
        while(NULL != fgets(buff, sizeof(buff), status_file)) {
            reti = regexec(&ppid_regex, buff, number_of_pmatches, pmatch, 0);

            if (0 == reti) {
                buff[pmatch[1].rm_eo] = 0;
		        strcpy(pid, &buff[pmatch[1].rm_so]);                

                counter++;

                break;
            } else if (reti == REG_NOMATCH) {
                continue;
            } else {
                regerror(reti, &ppid_regex, buff, sizeof(buff));
                fprintf(stderr, "PPid regex match failed: %s\n", buff);

                fclose(status_file);
                regfree(&ppid_regex);

                return 1;
            }
        }

        fclose(status_file);
    }

    regfree(&ppid_regex);

    printf("%d\n", counter);

    return 0;
}

