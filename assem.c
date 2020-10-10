/*
 *  CS3421 Assignment 2
 *  Name: Eric Grant
 *  Assembler to convert from mips instructions to machine code
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAXLINE 80
#define MAXNAME 10
#define MAXREG 5

char line[MAXLINE], oper[MAXNAME], rs[MAXREG], rt[MAXREG], rd[MAXREG], shamt[MAXREG], *s;
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
        unsigned int imm:16;
        unsigned int rt:5;
        unsigned int rs:5;
        unsigned int op:6;
    } IType;
} ITypeU;

//JType data union
union {
    unsigned int x;
    struct
    {
        unsigned int imm:26;
        unsigned int op:6;
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
    char* temp = line;
    if (strstr(line, ":") != NULL)
    {
        temp = strchr(line, ':');
        temp[0] = ' ';
    }
    return temp;
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

//convert opcode to value
int opcodeValue(char* op){
    if (strcmp(op, "addiu") == 0)
    {
        return 9;
    }
    if (strcmp(op, "beq") == 0)
    {
        return 4;
    }
    if (strcmp(op, "bne") == 0)
    {
        return 5;
    }
    if (strcmp(op, "lw") == 0)
    {
        return 35;
    }
    if (strcmp(op, "sw") == 0)
    {
        return 43;
    }
    if (strcmp(op, "j") == 0)
    {
        return 2;
    }
}

//convert funct to value
int functValue(char* op){
    if (strcmp(op, "addu") == 0)
    {
        return 33;
    }
    if (strcmp(op, "and") == 0)
    {
        return 36;
    }
    if (strcmp(op, "div") == 0)
    {
        return 26;
    }
    if (strcmp(op, "mfhi") == 0)
    {
        return 16;
    }
    if (strcmp(op, "mflo") == 0)
    {
        return 18;
    }
    if (strcmp(op, "mult") == 0)
    {
        return 24;
    }
    if (strcmp(op, "or") == 0)
    {
        return 37;
    }
    if (strcmp(op, "slt") == 0)
    {
        return 42;
    }
    if (strcmp(op, "subu") == 0)
    {
        return 35;
    }
    if (strcmp(op, "syscall") == 0)
    {
        return 12;
    }
}

//remove leading spaces from string
char* removeLeadingSpaces(char* str) 
{ 
    static char str1[99]; 
    int count = 0, j, k; 

    while (str[count] == ' ') { 
        count++; 
    } 
  
    for (j = count, k = 0; 
         str[j] != '\0'; j++, k++) { 
        str1[k] = str[j]; 
    } 
    str1[k] = '\0'; 
  
    return str1; 
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
    //save GP
    //find labels
    int instructionCount = 0, dataCount = 0;
    int countType = 0; //0 = none, 1 = instruction, 2 = data
    int pointer = 0;
    int gpPointer;
    int num;
    while (fgets(line, MAXLINE, stdin)) {
        //print line to temp file for later use
        fprintf(temp, "%s", line);

        //if line contains label, add to list of labels
        if (strstr(line, ":") != NULL)
        {
            addLabels(&labels, getlabel(line), pointer);
        }

        //if not directive, increment address pointer
        if (sscanf(line, "%10s %d", oper, &num) == 2)
        {
            if (strcmp(oper, ".space") == 0)
            {
                pointer = pointer + num;
            }
        } 
        else if (!(strcmp(strtok(line,"\n"), ".text") == 0 || strcmp(strtok(line,"\n"), ".data") == 0))
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
            gpPointer = pointer;
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
    unsigned int imm;
    char* label, *part = "", *numbers = "";
    int mode = 0; //0 = none, 1 = .text, 2 = .data
    int spacePrinted = 0;
    while (fgets(line, MAXLINE, temp)) {
        s = skiplabel(line);

        //check for .text and .data directive
        if (strcmp(strtok(s,"\n"), ".text") == 0)
        {
            mode = 1;
        } else if (strcmp(strtok(s,"\n"), ".data") == 0) {
            mode = 2;
        }

        //check for .space directive
        if (sscanf(s, "%10s %u", oper, &imm) == 2)
        {
            if (strcmp(oper, ".space") == 0)
            {
                for (size_t i = 0; i < imm; i++)
                {
                    printf("%08x\n", 0);
                    pointer = pointer + 1;
                }

                spacePrinted = 1;
            }
        }

        //if mode is 2, read words
        if (strcmp(strtok(s,"\n"), ".data") != 0 && mode == 2 && spacePrinted == 0)
        {
            s = removeLeadingSpaces(s);
            
            numbers = strchr(s, ' ');
            part = strtok(numbers, ",");
            while(part != NULL){
                printf("%08x\n", atoi(part));
                part = strtok(NULL, ",");
                pointer = pointer + 4;
            }
        }

        //if not .text or .data directive and mode is 1
        if (!(strcmp(strtok(s,"\n"), ".text") == 0 || strcmp(strtok(s,"\n"), ".data") == 0) && mode == 1 && spacePrinted == 0)
        {
            // 1. I 3 args
            if (sscanf(s, "%10s $%5[^,],$%5[^,],%u", oper, rt, rs, &imm) == 4)
            {
                ITypeU.IType.op = opcodeValue(oper);
                ITypeU.IType.rt = registerValue(rt);
                ITypeU.IType.rs = registerValue(rs);
                ITypeU.IType.imm = imm;
                printf("%08x\n", ITypeU.x);
            }
            // 2. R 3 args
            else if (sscanf(s, "%10s $%5[^,],$%5[^,],$%5[^# ]", oper, rd, rs, rt) == 4)
            {
                RTypeU.RType.op = 0;
                RTypeU.RType.rd = registerValue(rd);
                RTypeU.RType.rs = registerValue(rs);
                RTypeU.RType.rt = registerValue(rt);
                RTypeU.RType.shamt = 0;
                RTypeU.RType.funct = functValue(oper);
                printf("%08x\n", RTypeU.x);
            }
            // 3. I branch
            else if (sscanf(s, "%10s $%5[^,],$%5[^,],%5[^# ]", oper, rs, rt, label) == 4)
            {
                ITypeU.IType.op = opcodeValue(oper);
                ITypeU.IType.rt = registerValue(rt);
                ITypeU.IType.rs = registerValue(rs);
                ITypeU.IType.imm = (searchLabel(&labels, label) - pointer) / 4;
                printf("%08x\n", ITypeU.x);
            }
            // 4. R 2 args
            else if (sscanf(s, "%10s $%5[^,],$%5[^# ]", oper, rs, rt) == 3)
            {
                RTypeU.RType.op = 0;
                RTypeU.RType.rd = 0;
                RTypeU.RType.rs = registerValue(rs);
                RTypeU.RType.rt = registerValue(rt);
                RTypeU.RType.shamt = 0;
                RTypeU.RType.funct = functValue(oper);
                printf("%08x\n", RTypeU.x);
            }
            // 6. I word
            else if (sscanf(s, "%10s $%5[^,],%5[^\(]($%5[^)])", oper, rt, label, rs) == 4)
            {
                ITypeU.IType.op = opcodeValue(oper);
                ITypeU.IType.rt = registerValue(rt);
                ITypeU.IType.rs = registerValue(rs);

                ITypeU.IType.imm = searchLabel(&labels, label) - gpPointer;

                printf("%08x\n", ITypeU.x);
            }
            // 5. R 1 arg
            else if (sscanf(s, "%10s $%5[^,# ]", oper, rd) == 2)
            {
                RTypeU.RType.op = 0;
                RTypeU.RType.rd = registerValue(rd);
                RTypeU.RType.rs = 0;
                RTypeU.RType.rt = 0;
                RTypeU.RType.shamt = 0;
                RTypeU.RType.funct = functValue(oper);
                printf("%08x\n", RTypeU.x);
            }
            // J types
            else if (sscanf(s, "%10s %5[^# ]", oper, label) == 2)
            {
                //hard coded to avoid error with .space
                if (strcmp(oper, "j") == 0)
                {
                    JTypeU.JType.imm = searchLabel(&labels, label) / 4;
                    JTypeU.JType.op = opcodeValue(oper);
                    printf("%08x\n", JTypeU.x);
                }
            }
            // 7. syscall
            else if (sscanf(s, "%10[^# 0123456789]", oper) == 1)
            {
                RTypeU.RType.op = 0;
                RTypeU.RType.rd = 0;
                RTypeU.RType.rs = 0;
                RTypeU.RType.rt = 0;
                RTypeU.RType.shamt = 0;
                RTypeU.RType.funct = functValue(oper);
                printf("%08x\n", RTypeU.x);
            }

            pointer = pointer + 4;
        }

        spacePrinted = 0;
    }

    //close and delete temp file
    uninitLabels(&labels);
    fclose(temp);
    remove(tempName);

    return 0;
}
