#include "define.h"

char* parse_file(char *file_name)
{
	FILE *file = fopen(file_name, "r");
	char *buff = malloc(sizeof(char) * 32);
	char *file_char = malloc(sizeof(char) * 32 * 50);
	
	for (int line = 0; line < 50; line++)
	{  
		char *return_fget = fgets(buff, 32, (FILE*)file);
		if (return_fget == NULL) break;
		for (int i = 0; i < 32; i++)
		{
			printf("%x ", buff[i]);
			file_char[line * 32 + i] = buff[i];
			if (buff[i] == 0xa) break;
		}
		printf("\n");
		printf("%s", buff);
	}

	int size;
	char *cleaned_file = clean_file(file_char, 32 * 50, &size);
	free(file_char);
	file_char = cleaned_file;

	printf("\n\n%s", file_char);
	printf("\n");

	parse_line(file_char, size);
	fclose(file);
	free(buff);
	return "";
}

char* clean_file(char* txt, int size, int *end_size_ptr)
{
	char *cleaned_file = malloc(sizeof(char) * size);
	int end_size = 0;
	for (int i = 0; i < size; i ++)
	{
		char c = txt[i];
		if (c != 0x20 && c != 0x0 && c != 0xa)
		{
			cleaned_file[end_size] = c;
			end_size += 1;
		}
	}
	char *resized_file = resize_char(cleaned_file, end_size);
	free(cleaned_file);
	*end_size_ptr = end_size;
	return resized_file;
}

char* resize_char(char *txt, int size)
{
	char* resized_txt = malloc(sizeof(char) * size);
    for (int i = 0; i < size; i ++) {resized_txt[i] = 0x0;}
	for (int i = 0; i < size; i ++)
	{
		resized_txt[i] = txt[i];
		//printf("%s\n", resized_txt);
	}
	//printf("\n&& %s %c\n", resized_txt, resized_txt[size]);
	return resized_txt;
}

void resize_char_ptr(char *src, char *ptr, int size)
{
    for (int i = 0; i < size; i ++)
    {
        ptr[i] = src[i];
    }
    //printf("&& %s\n", ptr);
}

char*** parse_line(char *txt, int size)
{
	int line_count = 0;
	int* elemente_count = NULL;
	count_txt_elemente(txt, size, &line_count, &elemente_count);
	for (int i = 0; i < line_count; i++)
	{
		//printf("\n%d", elemente_count[i]);
	}
    return txt_parsed;

	char ***txt_parsed = malloc(sizeof(char**) * line_count);

	for (int i = 0; i < line_count; i++) { txt_parsed[i] = malloc(sizeof(char*) * elemente_count[i]); }

	int cur_line = 0;
	int cur_elemente_index = 0;
	int cur_elemente_char_index = 0;
	char* elemente_buffer = malloc(sizeof(char) * 32);
	for (int j = 0; j < 32; j++) {elemente_buffer[j] = 0x0;}
	for (int i = 0; i < size; i++)
	{
		char cur_char = txt[i];

		if (cur_char == 0x3b || cur_char == 0x2c)
		{
			//printf("%d %d %d %s %x\n", cur_elemente_char_index, cur_line, cur_elemente_index, elemente_buffer, txt_parsed[cur_line][cur_elemente_index]);
			char *resized_ellemente = resize_char(elemente_buffer, cur_elemente_char_index);
            char *ptr_txt = malloc(sizeof(char) * 32);
            txt_parsed[cur_line][cur_elemente_index] = ptr_txt;
            resize_char_ptr(elemente_buffer, txt_parsed[cur_line][cur_elemente_index], 32);
			//printf("%d %s  %s\n",cur_elemente_char_index, resized_ellemente, elemente_buffer);
			free(elemente_buffer);
			elemente_buffer = malloc(sizeof(char) * 32);
			for (int j = 0; j < 32; j++) {elemente_buffer[j] = 0x0;}
		}
		
		if (cur_char == 0x3b)
		{
			cur_line ++;
			cur_elemente_index = 0;
			cur_elemente_char_index = 0;
		}
		else if (cur_char == 0x2c)
		{
			cur_elemente_index ++;
			cur_elemente_char_index = 0;
		}
		else
		{
			elemente_buffer[cur_elemente_char_index] = cur_char;
			cur_elemente_char_index ++;
			//printf("#%s\n", elemente_buffer);
		}
	}
	free(elemente_buffer);

	printf("\n %d %d\n", line_count, elemente_count[1]);
	for (int line = 0; line < line_count; line++)
	{
		for (int elemente = 0; elemente < elemente_count[line]; elemente ++)
		{
			printf("%s, ", txt_parsed[line][elemente]);
		}
		printf(";\n");
	}
	printf("parcing end");
	return txt_parsed;

}

void count_txt_elemente(char *txt, int txt_size, int *line_count_ptr, int **elemente_count_ptr)
{
	int line_count = 0;

	for (int i = 0; i < txt_size; i++)
	{
		if (txt[i] == 0x3b)
		{
			line_count += 1;
		}
	}

	int *elemente_count = malloc(sizeof(int) * line_count);

	for (int i = 0; i < txt_size; i++) { elemente_count[i] = 0; }

	int cur_line = 0;
	for (int i = 0; i < txt_size; i++)
	{
		if (txt[i] == 0x3b)
		{
			elemente_count[cur_line] += 1;
			cur_line += 1;
		}

		if (txt[i] == 0x2c)
		{
			elemente_count[cur_line] += 1;
		}
	}

	*line_count_ptr = line_count;
	*elemente_count_ptr = elemente_count;
}