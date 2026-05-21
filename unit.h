#ifndef UNIT_H
#define UNIT_H

#include <stdio.h>

typedef struct {
    char form[256];
} Cell;

void ReadFile(FILE* file, int* col_number, int* row_number);
int get_arg_value(char mass[50][50][50], int total_cols, int rows, char* arg);
void Write_in_mass(FILE* file, int col_number, int row_number);


#endif // !UNIT_H