#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

int main() {
	regex_t regex;
	int reti = regcomp(&regex, "^PPid:\t([0-9]+)$", REG_EXTENDED|REG_NEWLINE);
	size_t number_of_pmatches = 2;
	regmatch_t pmatch[number_of_pmatches];
	
	if (reti) {
    		fprintf(stderr, "Could not compile regex\n");
    		return 1;
	}

	char buff[255];
	FILE *stat_file = fopen("/proc/self/status", "r");
    char pid_buff[10];
	int pid = -1;

	while(NULL != fgets(buff, sizeof(buff), stat_file)) {
		reti = regexec(&regex, buff, number_of_pmatches, pmatch, 0);

		if (0 == reti) {
			//printf("%.*s\n", pmatch[1].rm_eo - pmatch[1].rm_so, &buff[pmatch[1].rm_so]);
			buff[pmatch[1].rm_eo] = 0;
			
            strcpy(pid_buff, &buff[pmatch[1].rm_so]);
            pid = atoi(pid_buff);    				

			break;
		} else if (reti == REG_NOMATCH) {
			continue;
		} else {
			regerror(reti, &regex, buff, sizeof(buff));
    			fprintf(stderr, "Regex match failed: %s\n", buff);

			fclose(stat_file);
			regfree(&regex);		

			return 1;
		}
	}

    if (1 != pid) {
        fclose(stat_file);

        strcpy(buff, "/proc/");
        strcat(buff, pid_buff);
        strcat(buff, "/status");

        stat_file = fopen(buff, "r");

        while(NULL != fgets(buff, sizeof(buff), stat_file)) {
            reti = regexec(&regex, buff, number_of_pmatches, pmatch, 0);

            if (0 == reti) {
                //printf("%.*s\n", pmatch[1].rm_eo - pmatch[1].rm_so, &buff[pmatch[1].rm_so]);
                buff[pmatch[1].rm_eo] = 0;
                pid = atoi(&buff[pmatch[1].rm_so]);    				

                break;
            } else if (reti == REG_NOMATCH) {
                continue;
            } else {
                regerror(reti, &regex, buff, sizeof(buff));
                    fprintf(stderr, "Regex match failed: %s\n", buff);

                fclose(stat_file);
                regfree(&regex);		

                return 1;
            }
        }
    }

	printf("%d\n", pid);

	fclose(stat_file);
	regfree(&regex);		
	
	return 0;
}

