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

int is_parent(const char * child_pid, const char * parent_pid) {
        if (0 == strcmp(child_pid, parent_pid)) {
                    return 1;
                        }

            regex_t ppid_regex;

                if (regcomp(&ppid_regex, "^PPid:\t([0-9]+)$", REG_EXTENDED|REG_NEWLINE)) {
                            fprintf(stderr, "Could not compile ppid regex\n");

                                    return 0;
                                        }

                    int reti;
                        char buff[255];
                            char pid[16];
                                size_t number_of_pmatches = 2;
                                    regmatch_t pmatch[number_of_pmatches];
                                        FILE *status_file;

                                            strcpy(pid, child_pid);

                                                while (
                                                                (0 != strcmp(pid, INIT_PID)) &&
                                                                        (NULL != (status_file = fopen(build_process_status_path(buff, pid), "r")))
                                                                            ) {
                                                            while(NULL != fgets(buff, sizeof(buff), status_file)) {
                                                                            reti = regexec(&ppid_regex, buff, number_of_pmatches, pmatch, 0);

                                                                                        if (0 == reti) {
                                                                                                            buff[pmatch[1].rm_eo] = 0;
                                                                                                                            strcpy(pid, &buff[pmatch[1].rm_so]);

                                                                                                                                            if (0 == strcmp(pid, parent_pid)) {
                                                                                                                                                                    return 1;
                                                                                                                                                                                    }

                                                                                                                                                            break;
                                                                                                                                                                        } else if (reti == REG_NOMATCH) {
                                                                                                                                                                                            continue;
                                                                                                                                                                                                        } else {
                                                                                                                                                                                                                            regerror(reti, &ppid_regex, buff, sizeof(buff));
                                                                                                                                                                                                                                            fprintf(stderr, "PPid regex match failed: %s\n", buff);

                                                                                                                                                                                                                                                            fclose(status_file);
                                                                                                                                                                                                                                                                            regfree(&ppid_regex);

                                                                                                                                                                                                                                                                                            return 0;
                                                                                                                                                                                                                                                                                                        }
                                                                                                }

                                                                    fclose(status_file);
                                                                        }

                                                    regfree(&ppid_regex);

                                                        return 0;
}

int main(int argc, const char **argv) {
        if (argc < 2) {
                    fprintf(stderr, "Incorrect ppid\n");

                            return 1;
                                }

            const char *ppid = argv[1];

                regex_t process_dir_regex;

                    if (regcomp(&process_dir_regex, "^[0-9]+$", REG_EXTENDED|REG_NEWLINE)) {
                                fprintf(stderr, "Could not compile process dir regex\n");

                                        return 1;
                                            }

                        size_t number_of_pmatches = 2;
                            regmatch_t pmatch[number_of_pmatches];
                                int reti;

                                    char buff[255];
                                        const char *proc_dir_name = "/proc/";

                                            DIR *proc_dir = opendir(proc_dir_name);
                                                struct dirent *process_dir;

                                                    int children_counter = 0;

                                                        while (NULL != (process_dir = readdir(proc_dir))) {
                                                                    reti = regexec(&process_dir_regex, process_dir->d_name, 0, NULL, 0);

                                                                            if (0 == reti) {
                                                                                            children_counter += is_parent(process_dir->d_name, ppid);
                                                                                                    } else if (reti == REG_NOMATCH) {
                                                                                                                    continue;
                                                                                                                            } else {
                                                                                                                                            regerror(reti, &process_dir_regex, buff, sizeof(buff));
                                                                                                                                                        fprintf(stderr, "Process dir name regex match failed: %s\n", buff);

                                                                                                                                                                    closedir(proc_dir);

                                                                                                                                                                                regfree(&process_dir_regex);

                                                                                                                                                                                            return 1;
                                                                                                                                                                                                    }
                                                                                }

                                                            printf("%d\n", children_counter);

                                                                closedir(proc_dir);

                                                                    regfree(&process_dir_regex);

                                                                        return 0;
}


