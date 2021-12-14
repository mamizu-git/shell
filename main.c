#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "parser/parse.h"

extern char **environ;

int myexec(job *j, char **environ);

int main(int argc, char *argv[]) {
	char s[LINELEN];
	job *curr_job;

	while(get_line(s, LINELEN)) {

		if (s[0] == '\n') continue;

		if(!strcmp(s, "exit\n")) {
			break;
		}

		curr_job = parse_line(s);

		myexec(curr_job, environ);

		free_job(curr_job);
	}

	return 0;
}
