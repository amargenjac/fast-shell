#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "shell.h"



void printDir(void)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("[%s]: ", cwd);
}

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *functionNames[] = {
  "cd",
  "help",
  "clear",
  "exit",
  "cowsay",
  "rm",
  "cat",
  "forkbomb",
  "color"
};

int (*definedFunctions[]) (char **) = {
  &changeDir,
  &helpFunct,
  &clearTerminal,
  &exitShell,
  &cowsay,
  &removeFile,
  &concat,
  &forkbomb,
  &changeColor
};

int numOfFunctions() {
  return sizeof(functionNames) / sizeof(char *);
}

int clearTerminal(char **args){
system("clear");
return 1;
}
int changeDir(char **args)
{
    if(args[1] != NULL){
        if(strcmp(args[1], "h") == 0){
        cyan();
        printf("\nUSAGE: cd directory");
        printf("\nChange cwd to given directory");
        randomColor();
        return 1;
    }

    }
    
  if (args[1] == NULL) {
    fprintf(stderr, "fastshell: expected argument to \"cd\", type cd h for help\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("fastshell");
    }
  }
  return 1;
}

int helpFunct(char **args)
{
  int i;
  printf("The following are implemented:\n");

  for (i = 0; i < numOfFunctions(); i++) {
    printf("  %s\n", functionNames[i]);
  }

    printf("All functions have h available as first arguments for help.");
  return 1;
}

int exitShell(char **args)
{
  return 0;
}

int createProcess(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("fastshell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("fastshell");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int launchProgram(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < numOfFunctions(); i++) {
    if (strcmp(args[0], functionNames[i]) == 0) {
      return (*definedFunctions[i])(args);
    }
  }

  return createProcess(args);
}

char *readInput(void)
{
#ifdef STD_GETLINE
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We received an EOF
    } else  {
      perror("fastshell: getline() error.\n");
      exit(EXIT_FAILURE);
    }
  }
  return line;
#else
#define RL_BUFSIZE 1024
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "fastshell: not allocated sucessfully.\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "fastshell: additional memory not allocated.\n");
        exit(EXIT_FAILURE);
      }
    }
  }
#endif
}

#define TOK_BUFSIZE 64
#define TOK_DELIMITERS " \t\r\n\a"
char **getArguments(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "fastshell: not allocated sucessfully.\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIMITERS);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "fastshell: not allocated sucessfully.\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIMITERS);
  }
  tokens[position] = NULL;
  return tokens;
}

void red(void) {
  printf("\033[1;31m");
}

void blue(void) {
  printf("\033[0;34m");
}

void green(void) {
    printf("\033[0;32m");
}

void cyan(void) {
    printf("\033[0;36m");
}

void white(void) {
    printf("\033[0;37m");
}

void color_reset (void) {
  printf("\033[0m");
}

int cowsay(char **args){
    if(args[1] != NULL && strcmp(args[1], "h") != 0 && strcmp(args[1], "o") != 0){
        printf("\nfastshell: Invalid input. Type cowsay h for help");
        return 1;
    }
#define SIZE_OF_WORD 1024
char text[SIZE_OF_WORD];
if(args[1] != NULL){
    if(strcmp(args[1], "h") == 0){
        cyan();
        printf("\nUSAGE: cowsay [argument]");
        printf("\no - output what the cow will say to output.txt");
        printf("\nno args - enter what the cow will say");
        randomColor();
        return 1;
    }
}
if(args[1] != NULL && strcmp(args[1],"o") == 0){
FILE *f;
f = fopen("output.txt", "w");
if (f == NULL)
{
    printf("fastshell: Error opening file!\n");
    return 1;
}
printf("What do you want cow to say \n");
    scanf("%[^\n]%*c", text);
    fprintf(f,"\t   ");
    for(int i = 0; i<strlen(text) + 2; i++)
        fprintf(f,"_");  
    fprintf(f,"\n\t   |%s|\n",text);
    fprintf(f,"\t   ");
    fprintf(f,"\\/");
    for(int i = 0; i<strlen(text); i++)
       fprintf(f,"‾");
    fprintf(f,"\n");
    fprintf(f,"         __n__n__\n");
    fprintf(f,"  .------`-\\00/-'\n");
    fprintf(f," /  ##  ## (oo)\n");
    fprintf(f,"/ \\## __   ./\n");
    fprintf(f,"   |//YY \\|/ \n");
    fprintf(f,"   |||   |||\n");
    printf("\nWritten to output.txt");
    fclose(f);
 
 
}
else{
    printf("What do you want cow to say \n");
    scanf("%[^\n]%*c", text);
    printf("\t   ");
    for(int i = 0; i<strlen(text) + 2; i++)
        printf("_");  
    printf("\n\t   |%s|\n",text);
    printf("\t   ");
    printf("\\/");
    for(int i = 0; i<strlen(text); i++)
        printf("‾");
    printf("\n");
    printf("         __n__n__\n");
    printf("  .------`-\\00/-'\n");
    printf(" /  ##  ## (oo)\n");
    printf("/ \\## __   ./\n");
    printf("   |//YY \\|/ \n");
    printf("   |||   |||\n");
 
}
return 1;
}


int removeFile(char **args){
    if(args[1] != NULL){
        if(( strcmp(args[1], "h")) == 0){
        cyan();
        printf("\nUSAGE: rm [argument] file1 file2 etc.");
        printf("\nrm arguments:");
        printf("\ni - asks for confirmation before every file.");
        printf("\nI - asks for confirmation if 3 or more files are selected, else it will delete without confirmation\n");
        printf("no args - deletes without confirmation");
        randomColor();
        return 1;
    }

    }
    if(args[1] == NULL) {
        fprintf(stderr, "fastshell: expected argument to \"rm\", type rm h for help\n");
        return 1;
    }
char input;
int size = 0;

for(int i = 2; args[i] != NULL; i++) {
    size++;
}
    
    
    if(( strcmp(args[1], "i")) == 0){
        
    
        for (int i = 2; args[i] != NULL; i++) {
        printf("Press y if you want to delete: ");
        printf("%s \n", args[i]);
        scanf("%c",&input);
        if(input == 'y'){
            if(remove(args[i]) == 0){
                printf("\n%s deleted.", args[i]);
                scanf("%c",&input);

            }
            
                        
        } else {
            printf("File not deleted.\n");
            scanf("%c",&input);
        }
    }
}
else if ((strcmp(args[1], "I") == 0) && (size>=3)){
    printf("Press y if you want to delete: ");
    scanf("%c",&input);
    if(input=='y') {
        for(int i = 2; args[i] != NULL; i++) {
            if(remove(args[i]) == 0){
                printf("\n%s deleted.", args[i]);

            }
            
            
        }
    }
}
else if (strcmp(args[1], "I") == 0){
    for(int i = 2; args[i] != NULL; i++) {
            if(remove(args[i]) == 0){
                printf("\n%s deleted.", args[i]);

            }
            
        }

}
else {
    for(int i = 1; args[i] != NULL; i++) {
            if(remove(args[i]) == 0){
                printf("\n%s deleted.", args[i]);

            }
            
        }
}
return 1;
}

int concat(char **args){
#define SIZE_OF_WORD 1024
char text[SIZE_OF_WORD];
//HELP
if(args[1] != NULL) {
        if(strcmp(args[1], "h") == 0){
        cyan();
        printf("\nUSAGE: cat [argument] file1 file2 etc.");
        printf("\ncat arguments:");
        printf("\no - writes from console to output.txt");
        printf("\no file1 file2 etc. - writes from files to output.txt");
        printf("\none or more files - prints contents of file(s) on screen.");
        printf("\nno args - acts as echo until \"quit\" is entered.");
        
        randomColor();
        return 1;
    }
}
//OBICNO ISPISIVANJE IZ FAJLOVA
if((args[1] != NULL) && strcmp(args[1],"o") != 0){
    int c;
    FILE *file;
    file = fopen(args[1],"r");

    if(args[2] != NULL){
        for(int i = 1; args[i] != NULL; i++){
            file = fopen(args[i],"r");
            if (file) {
            while((c = getc(file)) != EOF)
                putchar(c);
            fclose(file);
}
        }
        return 1;
}
if (file) {
    while((c = getc(file)) != EOF)
        putchar(c);
    fclose(file);
}
return 1;
}

//ECHO
if(args[1] == NULL){
  while(1) {
scanf("%[^\n]%*c", text);
if((strcmp(text,"quit") == 0)){
return 1;
}
printf("%s\n",text);
}
}

    /*Ouput vise fajlova*/
if(args[1] != NULL && strcmp(args[1],"o") == 0 && args[2] != NULL){
FILE *f;  
f = fopen("output.txt", "w");
if (f == NULL)
{
    printf("fastshell: Error opening file!\n");
    return 1;
}
    int c;
    FILE *file;

      for(int i = 2; args[i] != NULL; i++){
        file = fopen(args[i],"r");
          if (file) {
            while((c = getc(file)) != EOF)
              fprintf(f,"%c",c);
            fclose(file);
            }
        }
        fclose(f);
        return 1;
}
if(args[1] != NULL && strcmp(args[1],"o") == 0 && args[2] == NULL){
FILE *f;
f = fopen("output.txt", "w");
if (f == NULL)
{
    printf("fastshell: Error opening file!\n");
    return 1;
}
while(1) {
scanf("%[^\n]%*c", text);
if((strcmp(text,"quit") == 0)){
fclose(f);
return 1;
}
fprintf(f,"%s\n",text);
}
}
return 1;
}

int forkbomb(char **args) {
    if(args[1] != NULL){
        if(strcmp(args[1], "h") == 0){
        cyan();
        printf("\nUSAGE: forkbomb");
        printf("\nShouldn't really be used.");
        printf("\nRun if you want to destroy your current session.");
        randomColor();
        return 1;
    }
printf("Wrong input format");
return 1;
    }
        char response;
    printf("\nPress y if you want to restart your machine or press n to quit \n");
    scanf("%c",&response);
    while(response != 'y' && response != 'n'){
    scanf("%c",&response);
    }
    if(response == 'y'){
	    while(1){
	    fork();	
	    }
    }
    else{
    printf("You choose wisely");
    return 1;

    }
    
}


int changeColor(char **args) {
    if (args[1] == NULL) {
    
    randomColor();
    return 1;
  } 
if(args[1] != NULL) {
    if(strcmp(args[1], "h") == 0){
        cyan();
        printf("\nUSAGE: color [argument]");
        printf("\ncolor arguments:");
        printf("\nr - changes text color in shell to red");
        printf("\nb - changes text color in shell to blue");
        printf("\ng - changes text color in shell to green");
        printf("\nw - changes text color in shell to white");
        printf("\nc - changes text color in shell to cyan");
        printf("\nd - changes text color in shell to default");
        printf("\nno args - random color is selected");
        randomColor();
        return 1;
    }

}
  

    char c = args[1][0];        
    if(args[1][1] != '\0') {
        randomColor();
        return 1;
    }
    if(args[1] != NULL){
        switch(c) {
            case 'r':
                red();
                break;

            case 'b':
                blue();
                break;

            case 'g':
                green();
                break;

            case 'w':
                white();
                break;

            case 'c':
                cyan();
                break;

            case 'd':
                color_reset();
                break;

            default:
                randomColor();
                break;
        }
        return 1;
    }
    


}

void randomColor(void) {
    #define LOWER_LIMIT 0
    #define UPPER_LIMIT 4

    int num = (rand() % (UPPER_LIMIT - LOWER_LIMIT + 1)) + LOWER_LIMIT;
    switch(num) {
        case 0:
            red();
            break;
        
        case 1:
            blue();
            break;

        case 2:
            cyan();
            break;

        case 3:
            green();
            break;

        case 4:
            white();
            break;

        default:
            color_reset();
            break;
    }
}