#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, const char **argv) {
	if (argc < 2) {
        fprintf(stderr, "Incorrect name of process\n");

        return 1;
    }

	const char *process_name = argv[1];

	regex_t process_name_regex;
 
	if (regcomp(&process_name_regex, "^Name:\t(.+)$", REG_EXTENDED|REG_NEWLINE)) {
    		fprintf(stderr, "Could not compile process name regex\n");

    		return 1;
	}

	regex_t process_dir_regex;

	if (regcomp(&process_dir_regex, "^[0-9]+$", REG_EXTENDED|REG_NEWLINE)) {
    		fprintf(stderr, "Could not compile process dir regex\n");

		regfree(&process_name_regex);		

		return 1;
	}

	size_t number_of_pmatches = 2;
	regmatch_t pmatch[number_of_pmatches];
	int reti;

	char buff[255];
	char stat_file_path[255];
	const char *stat_file_name = "/status";
	const char *proc_dir_name = "/proc/";

	DIR *proc_dir = opendir(proc_dir_name);
	struct dirent *process_dir;
	FILE *stat_file;

	int same_name_process_counter = 0;

	while (NULL != (process_dir = readdir(proc_dir))) {
		reti = regexec(&process_dir_regex, process_dir->d_name, 0, NULL, 0);

		if (0 == reti) {
			strcpy(stat_file_path, proc_dir_name);
			strcat(stat_file_path, process_dir->d_name);
			strcat(stat_file_path, stat_file_name);
			
			stat_file = fopen(stat_file_path, "r");

			while(NULL != fgets(buff, sizeof(buff), stat_file)) {
				reti = regexec(&process_name_regex, buff, number_of_pmatches, pmatch, 0);

				if (0 == reti) {
					buff[pmatch[1].rm_eo] = 0;

					if (0 == strcmp(&buff[pmatch[1].rm_so], process_name)) {
						same_name_process_counter++;
					}

					break;
				} else if (reti == REG_NOMATCH) {
					continue;
				} else {
					regerror(reti, &process_name_regex, buff, sizeof(buff));
					fprintf(stderr, "Regex match failed: %s\n", buff);

					closedir(proc_dir);
					fclose(stat_file);

					regfree(&process_dir_regex);		
					regfree(&process_name_regex);		

					return 1;
				}
			}
		} else if (reti == REG_NOMATCH) {
			continue;
		} else {
			regerror(reti, &process_dir_regex, buff, sizeof(buff));
    			fprintf(stderr, "Process dir name regex match failed: %s\n", buff);

			closedir(proc_dir);
			fclose(stat_file);

			regfree(&process_dir_regex);		
			regfree(&process_name_regex);		

			return 1;
		}
	}

	printf("%d\n", same_name_process_counter);

	closedir(proc_dir);
	fclose(stat_file);	

	regfree(&process_dir_regex);		
	regfree(&process_name_regex);		

	return 0;
}

