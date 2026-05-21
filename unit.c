#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "unit.h"

void ReadFile(FILE* file,int* col_number,int* row_number) {
	int col = 1; //число столбцов = кол-во запятых + 1
	int row = 0;
	char buffer_lines[4096];
	char form_lines[4096];
    rewind(file);
	while (fgets(buffer_lines, sizeof(buffer_lines), file)) {
		col = 1;
		int j = 0;
		for (size_t i = 0; i < 4095; i++)
		{
			if (buffer_lines[i] == '\0') break; //конец строки-> останавливаемся
			
			if (buffer_lines[i] == ',')  col++;

		}
		row++;
		
	}
	*col_number = col;
	*row_number = row; 
}
int get_arg_value(char mass[50][50][50], int total_cols, int rows, char* arg) {
    // Если это число
    if (arg[0] >= '0' && arg[0] <= '9') {
        return atoi(arg);
    }

    // Если это ссылка (например "A1" или "Cell30")
    char letters[50] = { 0 };
    char numbers[50] = { 0 };
    int li = 0, ni = 0;

    for (int idx = 0; arg[idx] != '\0'; idx++) {
        if ((arg[idx] >= 'A' && arg[idx] <= 'Z') ||
            (arg[idx] >= 'a' && arg[idx] <= 'z')) {
            letters[li++] = arg[idx];
        }
        else {
            numbers[ni++] = arg[idx];
        }
    }
    letters[li] = '\0';
    numbers[ni] = '\0';

    // Ищем столбец
    int col_idx = -1;
    for (int c = 1; c < total_cols; c++) {
        if (strcmp(mass[0][c], letters) == 0) {
            col_idx = c;
            break;
        }
    }

    // Ищем строку
    int row_idx = -1;
    int target_row = atoi(numbers);
    for (int r = 1; r < rows; r++) {
        if (atoi(mass[r][0]) == target_row) {
            row_idx = r;
            break;
        }
    }

    if (col_idx != -1 && row_idx != -1) {
        if (mass[row_idx][col_idx][0] == '=') {
            return -999; // формула, еще не решена
        }
        return atoi(mass[row_idx][col_idx]);
    }
    return 0;
}
void Write_in_mass(FILE* file, int col_number, int row_number) {
    char buffer_lines[4096];
    char mass[50][50][50] = { 0 };
    rewind(file);

    int row = 0;
    int col = 0;
    int form_count = 0;
    while (fgets(buffer_lines, sizeof(buffer_lines), file) && row < 50) {
        buffer_lines[strcspn(buffer_lines, "\n")] = 0;

        col = 0;
        int cell_start = 0;
        int cell_index = 0;

        for (int i = 0; i <= (int)strlen(buffer_lines); i++) {
            if (buffer_lines[i] == ',' || buffer_lines[i] == '\0') {
                int len = i - cell_start;
                if (len > 49) len = 49;

                if (row == 0 && cell_index == 0) {
                    cell_start = i + 1;
                    cell_index++;
                    continue;
                }

                strncpy(mass[row][col], buffer_lines + cell_start, len);
                mass[row][col][len] = '\0';

                if (mass[row][col][0] == '=') {
                    form_count++;
                }

                col++;
                cell_start = i + 1;
                cell_index++;
            }
        }
        row++;
    }

    
    int total_cols = col;
    for (int c = total_cols; c > 0; c--) {
        strcpy(mass[0][c], mass[0][c - 1]);
    }
    mass[0][0][0] = ' ';
    mass[0][0][1] = '\0';
    total_cols++;

    // РЕШАЕМ ФОРМУЛЫ
    int iter_x = 1;
    int iter_y = 1;
    int solved = 0;

    while (form_count != 0) {
        if (mass[iter_y][iter_x][0] == '=') {
            char formula[50];
            strcpy(formula, mass[iter_y][iter_x]);

            char left[50] = { 0 };
            char right[50] = { 0 };
            char op = 0;

            int i = 1;
            int j = 0;

            while (formula[i] != '+' && formula[i] != '-' &&
                formula[i] != '*' && formula[i] != '/' &&
                formula[i] != '\0') {
                left[j++] = formula[i++];
            }
            op = formula[i++];
            j = 0;

            while (formula[i] != '\0') {
                right[j++] = formula[i++];
            }

            int val1 = get_arg_value(mass, total_cols, row, left);
            int val2 = get_arg_value(mass, total_cols, row, right);

            if (val1 != -999 && val2 != -999) {
                int result = 0;
                switch (op) {
                case '+': result = val1 + val2; break;
                case '-': result = val1 - val2; break;
                case '*': result = val1 * val2; break;
                case '/': result = (val2 != 0) ? val1 / val2 : 0; break;
                }
                sprintf(mass[iter_y][iter_x], "%d", result);
                form_count--;
                solved++;
            }
        }

        iter_x++;
        if (iter_x >= total_cols) {
            iter_x = 1;
            iter_y++;
        }
        if (iter_y >= row) {
            if (form_count > 0 && solved > 0) {
                iter_x = 1;
                iter_y = 1;
                solved = 0;
            }
            else if (form_count > 0 && solved == 0) {
                break;
            }
        }
    }

    
    printf("\n");
    printf(",");
    for (int c = 1; c < total_cols; c++) {
        printf("%s", mass[0][c]);
        if (c < total_cols - 1) printf(",");
    }
    printf("\n");

   
    for (int r = 1; r < row; r++) {
        
        printf("%s", mass[r][0]);

       
        for (int c = 1; c < total_cols; c++) {
            printf(",%s", mass[r][c]);
        }
        printf("\n");
    }
}


