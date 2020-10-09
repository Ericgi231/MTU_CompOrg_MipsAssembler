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

char line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], imm[MAXREG], shamt[MAXREG], funct[MAXREG];
char tempName[] = "temp.txt";

typedef struct Node {
    char* tag;
    int address;
    struct Node* next;
} Node;

typedef struct {
    struct Node* first;
} Labels;

typedef struct {
    int op;
    int rs;
    int rt;
    int imm;
} IType;

typedef struct {
    int op;
    int imm;
} JType;

union {
    unsigned int x;
    struct
    {
        int funct:6;
        int shamt:5;
        int rd:5;
        int rt:5;
        int rs:5;
        int op:6;
    } RType;
} RTypeU;

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
    //count instructions and data
    //find labels
    int instructionCount = 0, dataCount = 0;
    int countType = 0; //0 = none, 1 = instruction, 2 = data
    int pointer = 0;
    while (fgets(line, MAXLINE, stdin)) {
        //print line to temp file for later use
        fprintf(temp, "%s", line);
        
        //if line contains label, add to list of labels
        if (strstr(line, ":") != NULL)
        {
            addLabels(&labels, getlabel(line), pointer);
        }

        //if not directive, increment address pointer
        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            pointer = pointer + 4;
        }

        //if directive, ready line counters
        if (strcmp(strtok(line,"\n"), ".text") == 0)
        {
            countType = 1;
            instructionCount = -1;
        } 
        else if (strcmp(strtok(line,"\n"), ".data") == 0)
        {
            countType = 2;
            dataCount = -1;
        }

        //increment line counters
        if (countType == 1)
        {
            instructionCount++;
        } 
        else if (countType == 2)
        {
            dataCount++;
        }
    }

    //print header
    printf("%d %d\n", instructionCount, dataCount);

    //close temp then re-open for read
    fclose(temp);
    temp = fopen(tempName, "r");

    //second pass, find commands
    pointer = 0;
    while (fgets(line, MAXLINE, temp)) {
        //if not directive
        if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
        {
            char* s = skiplabel(line);

            // 1. I 3 args
            //if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%d", oper, rt, rs, imm) == 4)
            //{
                
            //}
            // 2. R 3 args
            if (sscanf(line, "%10s $%5[^,],$%5[^,],$%5s", oper, rd, rs, rt) == 4)
            {
                RTypeU.RType.op = 0;
                RTypeU.RType.rd = 16;
                RTypeU.RType.rs = 0;
                RTypeU.RType.rt = 0;
                RTypeU.RType.shamt = 0;
                RTypeU.RType.funct = 33;
                printf("%08x", RTypeU.x);
            }
            // 3. I branch
            //else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            //{

            //}
            // 4. R 2 args
            //else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            //{

            //}
            // 5. R 1 arg
            //else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            //{

            //}
            // 6. I word
            //else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            //{

            //}
            // 7. syscall
            //else if (sscanf(s, "%10s $%5s[^,],$%5s[^,],$%5s", oper, rd, rs, rt) == 4)
            //{
                
            //}
        }
    }

    //close and delete temp file
    uninitLabels(&labels);
    fclose(temp);
    remove(tempName);

    return 0;
}
