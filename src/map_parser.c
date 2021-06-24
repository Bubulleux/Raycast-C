#include "define.h"

char* parse_file(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	char *buff = malloc(sizeof(char) * 255);
	
	for (int i = 0; i < 50; i++)
	{  
		char *return_fget = fgets(buff, 255, (FILE*)file);
		printf("%s", buff);
	}
	fclose(file);
	free(buff);
	return "";
}