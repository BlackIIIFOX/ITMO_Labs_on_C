/*
	my_xargs -- утилита, имплементирующая стандартную xargs с урезанными возможностями.
	Предоставляет пользователю построение и исполнения команд из стандартного поток, 
	входными аргументами, которых являются результаты предыдущей команды.
	Пример: 
		cmd вход: echo A B C | my_xargs echo
		cmd выход: A B C
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

char* get_all_pipe_params()
{
	char *string;
    char ch;
    string = malloc(sizeof(char)); //allocating memory

    string[0]='\0';

    while(1)
    {
    	int read_success = read(STDIN_FILENO, &ch, 1);

    	if (!read_success || ch == '\n' || ch == EOF)
    	{
	    	break;
    	}

    	int current_length = strlen(string);
    	string = realloc(string, (current_length + 2) * sizeof(char)); //reallocating memory
	    string[current_length] = ch; //type casting `int` to `char`
	    string[current_length + 1] = '\0'; //inserting null character at the end
    }

    return string;
}

char* get_base_command_with_params(int argc, char const *argv[])
{
	char* command = calloc(1000, sizeof(char));

	if (argc == 1)
	{
		strcat(command, "echo");
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			strcat(command, argv[i]);
			strcat(command, " ");
		}
	}

	command = realloc(command, (strlen(command) + 1) * sizeof(char)); //reallocating memory
	return command;
}

int main(int argc, char const *argv[])
{
	char* pipe_params = get_all_pipe_params(); 
	char* base_cammand = get_base_command_with_params(argc, argv);

	char* cmd = calloc((strlen(base_cammand) + 1 + strlen(pipe_params) + 1), sizeof(char));
	strcat(cmd, base_cammand);
	strcat(cmd, " ");
	strcat(cmd, pipe_params);
	system(cmd);

	return 0;
}
