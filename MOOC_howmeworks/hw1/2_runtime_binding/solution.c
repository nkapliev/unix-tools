#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>

int (*user_function)(int);

int main(int c, void **v) {
	int length_of_lib_name = strlen(v[1]) + 2;
	char lib_name[length_of_lib_name]; 
	char *current_dir = "./";

	strcat(lib_name, current_dir);
	strcat(lib_name, v[1]);

	void *hdl = dlopen(lib_name, RTLD_LAZY);

	if (NULL == hdl)
		return -1;

	user_function = (int (*)(int))dlsym(hdl, v[2]);

	if (NULL == user_function) {
		dlclose(hdl);
		return -1;
	}

	printf("%d\n", user_function(atoi(v[3])));

	return 0;
}

