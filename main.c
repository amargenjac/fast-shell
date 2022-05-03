#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "shell.h"
int main(int argc, char **argv)
{
    char *line;
    char **args;
    int status;
    char machine[HOST_NAME_MAX + 1];
    gethostname(machine, HOST_NAME_MAX + 1);
    char *user = getlogin();
    do {
        
        printf("\n%s",machine);
        
        printf("@");
        
        printf("%s~",user);
        
        printDir();
        
        line = readInput();
        args = getArguments(line);
        status = launchProgram(args);

        free(line);
        free(args);
  } while (status);
}

