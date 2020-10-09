/*
 *  CS3421 Assignment 2
 *  Name: Eric Grant
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 5

char line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], *s;
char tempName[] = "temp.txt";

typedef struct Node {
    char* tag;
    int address;
    struct Node* next;
} Node;

typedef struct {
    struct Node* first;
} Labels;

//init label
void initLabels(Labels* labels){
    Node* node = malloc(sizeof(Node));
    node->next = NULL;
    labels->first = node;
}

//add label to labels
void addLabels(Labels* labels, char* tag, int address){
    //create current node and an empty node
    Node* node;
    Node* n = malloc(sizeof(Node));
    n->next = NULL;

    //loop through nodes starting at the head until a node with no next is found
    node = labels->first;
    while (node->next != NULL)
    {
        node = node->next;
    }
    
    node->tag = malloc(sizeof(char) * 256);
    strcpy(node->tag, tag);
    node->address = address;
    node->next = n;
}

//search for label by tag
int searchLabel(Labels* labels, char* tag){
    Node* node;
    node = labels->first;
    while (node->next != NULL)
    {
        if (strcmp(node->tag, tag) == 0)
        {
            return node->address;
        }
        node = node->next;
    }
}

//clear labels
void uninitLabels(Labels* labels){
    Node* temp;

    //clear head node and set next to new head, repeat until all nodes clear
    while (labels->first != NULL)
    {
        free(labels->first->tag);
        temp = labels->first;
        labels->first = labels->first->next;
        free(temp);
    }
}

//trim all except label from line
char* getlabel(char* line){
    return strtok(line,":");
}

//trim label from line
char* skiplabel(char* line){
    return strchr(line,':');
}

int main() {
    //vars
    char* outputString = "";
    int sum = 0;

    Labels labels;
    initLabels(&labels);

    //create temp file for storing stdin
    FILE *temp;
    temp = fopen(tempName, "w+");

    //first pass, save stdin to file for repeat reading
    while (fgets(line, MAXLINE, stdin)) {
        fprintf(temp, "%s", line);
    }

    //close temp then re-open for read
    fclose(temp);
    temp = fopen(tempName, "r");

    //second pass, save labels
    int pointer = 0;
    while (fgets(line, MAXLINE, temp)) {
        if (strstr(line, ":") != NULL)
        {
            addLabels(&labels, getlabel(line), pointer);
        }

        //if not directive, increment address pointer
        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            pointer = pointer + 4;
        }
    }

    //close temp then re-open for read
    fclose(temp);
    temp = fopen(tempName, "r");

    //third pass, find commands
    pointer = 0;
    while (fgets(line, MAXLINE, temp)) {
        //if not directive
        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            s = skiplabel(line);

            // 1. L no args
            if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {

            }
            // 2. R 3 args
            else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {

            }
            // 3. L branch
            else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {

            }
            // 4. R 2 args
            else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {

            }
            // 5. R 1 arg
            else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {

            }
            // 6. I word
            else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {

            }
            // 7. syscall
            else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            {
                
            }
        }
    }

    //close and delete temp file
    fclose(temp);
    remove(tempName);

    return 0;
}
