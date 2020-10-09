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
char tempName[] = "temp.txt";

char* skiplabal(char* line){
    return "";
}

int main() {
    //vars
    char* outputString = "";
    int sum = 0;

    //create temp file for storing stdin
    FILE *temp;
    temp = fopen(tempName, "w+");

    //first pass, save stdin to file for repeat reading
    while (fgets(line, MAXLINE, stdin)) {
        fprintf(temp, "%s", line);
    }

    //second pass, save labels
    int pointer = 0;
    while (fgets(line, MAXLINE, stdin)) {
        if (strstr(line, ":") != NULL)
        {
            //
            //add code to save labels and their pointers here
            //
            printf("%s %d\n", strtok(line,":"), pointer);
        }
        printf("a");
        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            pointer = pointer + 4;
        }
    }

    //second pass, find commands
    while (fgets(line, MAXLINE, stdin)) {
        if (strstr(line, ":") != NULL)
        {
            //
            //add code to save labels and their pointers here
            //
            printf("%s %d\n", strtok(line,":"), pointer);
        }

        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            pointer = pointer + 4;
        }
    }

    //close and delete temp file
    fclose(temp);
    remove(tempName);

    return 0;
}
