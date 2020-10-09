/*
 *  CS3421 Assignment 2
 *  Name: Eric Grant
 *
 */
#include <stdio.h>
#include <string.h>

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 5

char line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], *s;

char* skiplabal(char* line){
    return "";
}

int main() {

    char* outputString = "";
    int sum = 0;

    //first pass, find labels
    int pointer = 0;
    while (fgets(line, MAXLINE, stdin)) {
        if (strstr(line, ":") != NULL)
        {
            printf("%s %d\n", strtok(line,":"), pointer);
        }

        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            pointer = pointer + 4;
        }
    }

    return 0;
}
