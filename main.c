#include <stdio.h>
#include <locale.h>

//PRIVATE INC
#include "unit.h"
//END PRIVATE

int main(){
    
	setlocale(LC_ALL, "RU");
	FILE* file = fopen("example.csv", "r");
	int col = 0;
	int row = 0;
	if (file == NULL)
	{
		printf("Ошибка: не удалось открыть файл\n");
		return 1;

	}
	ReadFile(file,&col,&row);
	printf("Первый тест:\r");
	Write_in_mass(file, col, row);
	fclose(file);

	printf("\r\n");

	printf("Второй тест:\r");
	FILE* file2 = fopen("example2.csv", "r");
	col = 0;
	row = 0;
	if (file2 == NULL)
	{
		printf("Ошибка: не удалось открыть файл\n");
		return 1;

	}
	ReadFile(file2, &col, &row);
	Write_in_mass(file2, col, row);
	fclose(file2);
	
	
	return 0;
}