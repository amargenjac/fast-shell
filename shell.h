#ifndef SHELL_H
#define SHELL_H

void printDir(void);
int changeDir(char **args);
int helpFunct(char **args);
int clearTerminal(char **args);
int exitShell(char **args);
int cowsay(char **args);
int removeFile(char **args);
void red(void);
void blue(void);
void green(void);
void cyan(void);
void white(void);
void color_reset(void);
char *readInput(void);
char **getArguments(char *line);
int launchProgram(char **args);
int concat(char **args);
int forkbomb(char **args);
void randomColor(void);
int changeColor(char **args);
#endif 
