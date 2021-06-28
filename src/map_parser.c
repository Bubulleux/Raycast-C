#include "define.h"

char* parse_file(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	char *buff = malloc(sizeof(char) * 255);
	
	for (int i = 0; i < 100; i++)
	{  
		char *return_fget = fgets(buff, 255, (FILE*)file);
		if (return_fget == NULL) break;
		parse_line(buff);
		printf("%s", buff);
		
	}
	printf("\n");
	fclose(file);
	free(buff);
	return "";
}

char** parse_line(char* line)
{
	bool in_word = false;
	int word_count = 0;
	char **out_char = NULL;
	for (int i = 0; i < 30; i++)
	{
		char cur_char = line[i];
		if (i == 0 && cur_char == 0x21) return NULL;
		if (cur_char == 0x3B || cur_char == 0x0) break;


		if (!in_word && cur_char != 0x20) 
		{
			in_word = true;
			word_count++;
		}
		else if (in_word && cur_char == 0x20)
		{
			in_word = false;
		}
	}
	out_char = malloc(sizeof(char*) * word_count);
	
	in_word = false;
	int word_size;
	for (int i = 0; i < 30; i++)
	{
		char cur_char = line[i];
		if (cur_char == 0x3B || cur_char == 0x0)
		{

			break;
		}


		if (!in_word && cur_char != 0x20) 
		{
			in_word = true;
		}
		else if (in_word)
		{
			word_count++;
			if (cur_char == 0x20)
			{
				in_word = false;
			}
		}
	}
	printf("%d  ", word_count);
}