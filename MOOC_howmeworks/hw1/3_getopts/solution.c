#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>


int main (int argc, char *argv[]){
	opterr = 0;

	const char* short_options = "q:iv";

	const struct option long_options[] = {
		{"query",required_argument,NULL,'q'},
		{"longinformationrequest",no_argument,NULL,'i'},
		{"version",no_argument,NULL,'v'},
		{NULL,0,NULL,0}
	};

	bool succ = true;
	int rez;
	int option_index;

	while ((rez=getopt_long(argc,argv,short_options,
		long_options,&option_index))!=-1){

		switch(rez){
			case 'q': {
				if (NULL == optarg)
					succ = false;
				break;
			}; 
			case 'i': 
			case 'v': {
				break;
			};
			case '?': default: {
				succ = false;
				break;
			};
		};
	};

	printf("%s\n", succ ? "+" : "-");

	return 0;
};
