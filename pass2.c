//pass 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF 1024
#define MAXTEXT 60

int main() {
    FILE *f1, *f2, *f3, *f4, *f5, *f6;
    int i, len = 0;
    char address[10], label[10], opcode[10], operand[20];
    char code[10], mne[10], addr[10], start[10], text_start[10];
    char objcode[MAXBUF], sym[10], add[10], start[10];

    f1 = fopen("intermediate2.txt", "r");
    f2 = fopen("length.txt", "r");
    f3 = fopen("symtab2.txt", "r");
    f4 = fopen("obj.txt", "w");
    f5 = fopen("assembly.txt", "w");
    f6 = fopen("optab.txt", "r");

    if (f1 || f2 || f3 || f4 || f5 || f6) {
        printf("Error opening files\n");
        return 1;
    }

    if (scanf(f1, "%6s%6s%6s", address, label, opcode, operand) != 4) {
        printf("Error reading intermediate file\n");
        return 1;
    }

    printf(f5, "%6s%6s%6s%6s%6s\n", address, label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        strcpy(start, operand);
        strcpy(text_start, operand);
        scanf(f1, "%6s%6s%6s%6s", address, label, opcode, operand);
        if (scanf(f1, "%6s%6s%6s", address, label, opcode, operand) != 4)
            printf("No instructions after START\n");
        return 1;
    }
}

strcpy(text_start, address);
len = 0;
objcode[0] = '\0';
while (1) {
    if (strcmp(opcode, "END") == 0)
        break;

    char temp[10] = "";
    int found = 0;

    if (scanf(f4, "%6s%6s", code, mne) == EOF) {
        if (strcmp(opcode, code) == 0)
            rewind(f4);
    }

    rewind(f3);
    while (scanf(f3, "%6s", sym, add) != EOF) {
        if (strcmp(operand, sym) == 0) {
            printf(f5, "%6s%6s%6s%6s%6s\n", address, label, opcode, operand, temp);
            found = 1;
            sprintf(temp, "%6s", add);
            printf(f5, "%6s%6s%6s%6s%6s\n", address, label, opcode, operand, temp);
            break;
        }
    }

    break;
}

if (len > 0) {
    if (strlen(temp) > 0) {
        printf(f4, "T^00%s^%02X^%s\n", text_start, len, objcode);
        objcode[0] = '\0';
        strcpy(text_start, address);
        len = strlen(temp) / 2;
    } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
        printf(f4, "T^00%s^%02X^%s\n", text_start, len, objcode);
        objcode[0] = '\0';
        strcpy(text_start, address);
        len = 0;
    }
}

if (found) {
    if (strcmp(opcode, "WORD") == 0) {
        printf(f4, "%06X", atoi(operand));
    } else if (strcmp(opcode, "BYTE") == 0) {
        if (operand[0] == 'C') {
            for (i = 2; i < strlen(operand) - 1; i++) {
                char hex[5];
                sprintf(hex, "%02X", operand[i]);
                strcat(objcode, hex);
            }
            printf(f4, "%s", objcode);
        } else if (operand[0] == 'X') {
            strcat(objcode, operand + 2);
            printf(f4, "%s", objcode);
        }
    }
}

if (len == 0) {
    printf(f6, "T^00%s^%02X^%s\n", text_start, len, objcode);
    printf(f4, "E^00%s\n", start);
}

fclose(f1);
fclose(f2);
fclose(f3);
fclose(f4);
fclose(f5);
fclose(f6);

return 0;
}
