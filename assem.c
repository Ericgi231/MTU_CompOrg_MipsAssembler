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

char line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], imm[MAXREG], shamt[MAXREG], funct[MAXREG], *s;
char tempName[] = "temp.txt";

//linked list node
typedef struct Node {
    char* tag;
    int address;
    struct Node* next;
} Node;

//head node
typedef struct {
    struct Node* first;
} Labels;

//RType data union
union {
    unsigned int x;
    struct
    {
        unsigned int funct:6;
        unsigned int shamt:5;
        unsigned int rd:5;
        unsigned int rt:5;
        unsigned int rs:5;
        unsigned int op:6;
    } RType;
} RTypeU;

//IType data union
union {
    unsigned int x;
    struct
    {
        unsigned int op:6;
        unsigned int rs:5;
        unsigned int rt:5;
        unsigned int imm:16;
    } IType;
} ITypeU;

//JType data union
union {
    unsigned int x;
    struct
    {
        unsigned int op:6;
        unsigned int imm:26;
    } JType;
} JTypeU;

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

//convert register to value
int registerValue(char* reg){
    if (strcmp(reg, "zero") == 0)
    {
        return 0;
    }
    if (strcmp(reg, "at") == 0) {
        return 1;
    }
    if (strcmp(reg, "v0") == 0) {
        return 2;
    }
    if (strcmp(reg, "v1") == 0) {
        return 3;
    }
    if (strcmp(reg, "a0") == 0) {
        return 4;
    }
    if (strcmp(reg, "a1") == 0) {
        return 5;
    }
    if (strcmp(reg, "a2") == 0) {
        return 6;
    }
    if (strcmp(reg, "a3") == 0) {
        return 7;
    }
    if (strcmp(reg, "t0") == 0) {
        return 8;
    }
    if (strcmp(reg, "t1") == 0) {
        return 9;
    }
    if (strcmp(reg, "t2") == 0) {
        return 10;
    }
    if (strcmp(reg, "t3") == 0) {
        return 11;
    }
    if (strcmp(reg, "t4") == 0) {
        return 12;
    }
    if (strcmp(reg, "t5") == 0) {
        return 13;
    }
    if (strcmp(reg, "t6") == 0) {
        return 14;
    }
    if (strcmp(reg, "t7") == 0) {
        return 15;
    }
    if (strcmp(reg, "s0") == 0) {
        return 16;
    }
    if (strcmp(reg, "s1") == 0) {
        return 17;
    }
    if (strcmp(reg, "s2") == 0) {
        return 18;
    }
    if (strcmp(reg, "s3") == 0) {
        return 19;
    }
    if (strcmp(reg, "s4") == 0) {
        return 2;
    }
    if (strcmp(reg, "s5") == 0) {
        return 21;
    }
    if (strcmp(reg, "s6") == 0) {
        return 22;
    }
    if (strcmp(reg, "s7") == 0) {
        return 23;
    }
    if (strcmp(reg, "t8") == 0) {
        return 24;
    }
    if (strcmp(reg, "t9") == 0) {
        return 25;
    }
    if (strcmp(reg, "k0") == 0) {
        return 26;
    }
    if (strcmp(reg, "k1") == 0) {
        return 27;
    }
    if (strcmp(reg, "gp") == 0) {
        return 28;
    }
    if (strcmp(reg, "sp") == 0) {
        return 29;
    }
    if (strcmp(reg, "fp") == 0) {
        return 30;
    }
    if (strcmp(reg, "ra") == 0) {
        return 31;
    }
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
            s = skiplabel(line);

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
