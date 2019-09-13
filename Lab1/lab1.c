#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "sb.h"
#include <stdbool.h>
#include <string.h>

static const char INVALID_OPTION_BEGIN[] = "invalid option -- '";
static const char INVALID_OPTION_END[] = "'\n";

/* Typedefs. */
typedef struct
{
	/* Кол-во уже выведенных строк. */
	int count_print;

	/* Наличие параметра -E в аргументах командой строки. */
	bool param_E_is_enable;

	/* Наличие параметра -n в аргументах командой строки. */
	bool param_n_is_enable;

	/* Наличие параметра -s в аргументах командой строки. */
	bool param_s_is_enable;

	/* Состояние наличия пустой строки при предыдущем выводе. */
	bool param_s_prev_string_is_empty;
} PrintCatInfo_t;

/* Private variables */

/* Глобальная переменная, для хранения информации о параметров вызова команды и их реализации. */
PrintCatInfo_t g_PrintInfo;

/* Function prototypes. */
void ReadAndPrintFromStandardInputAsCat();

void FindAndSetCatParamsFromArgv(int argc, char const *argv[]);

void PrintWithCatParams(const char* string);

bool IsCatParam(char const* string);

bool ArgvIsEmtyOrOnlyCatParam(int argc, char const *argv[]);

void ReadAndPrintFromFileAsCat(const char* path);

void ReadAndPrintFromStreamAsCat(int p_stream);

/**
* @brief Имплементация команды cat на Си.
* Имплментация обрабатывает следующие опции базовой команды:
*	1) -E/-e - показывать символ $ в конце каждой строки;
*	2) -n - нумеровать все строки;
*	3) -s - удалять пустые повторяющиеся строки;
* Имплементация способна рабоать с неограниченным входным количеством файлов и стандартными входными потоками.
* Входной поток определяется символом '-'.
* Обработка ошибок осуществляется с помощью библиоткеи error.h
* @param argc кол-во аргументов, передаваемых при вызове.
* @param *argv[] список входных параметров в виде строк.
* @retval Состояния успешности выполнения.
*/
int main(int argc, char const *argv[])
{
	/* code */

	// Test command: ./my_cat -e -s -n TestFiles/file1.txt - TestFiles/file2.txt
	FindAndSetCatParamsFromArgv(argc, argv);
	g_PrintInfo.count_print = 1;

	if (ArgvIsEmtyOrOnlyCatParam(argc, argv))
	{
		// Если нет входных файлов, то требуется выполнить только чтение из стандартного входного потока.
		ReadAndPrintFromStandardInputAsCat();
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (!IsCatParam(argv[i]))
			{
				if (strcmp(argv[i], "-") == 0)
				{
					ReadAndPrintFromStandardInputAsCat();
				}
				else
				{
					const char* param = argv[i];

					if (param[0] == '-')
					{
						write(STDOUT_FILENO, INVALID_OPTION_BEGIN, strlen(INVALID_OPTION_BEGIN));
						write(STDOUT_FILENO, &param[1], strlen(param) - 1);
						write(STDOUT_FILENO, INVALID_OPTION_END, strlen(INVALID_OPTION_END));
					}
					else
					{
						ReadAndPrintFromFileAsCat(argv[i]);
					}
				}
			}
		}
	}
	
	return 0;
}

/**
* @brief Выполняет проверку параметров командной строки на пустоту или наличие только Cat параметров.
* @param argc кол-во аргументов командной строки.
* @param argv коллекция аргументов командной строки.
* @retval Состояние проверки. True - параметры командной строки пустые или содержат только cat параметры,
* false - в параметрах есть что то еще.
*/
bool ArgvIsEmtyOrOnlyCatParam(int argc, char const *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (!IsCatParam(argv[i]))
		{
			return false;
		}
	}

	return true;
}

/**
* @brief Определяет соответсвие строки параметру команды Cat из списка перечисленных в заголовке файла.
* @param string Строка, над которой требуется проверка.
* @retval Соответсвия строки параметру команды cat. True - строка является параметров, false - не является.
*/
bool IsCatParam(char const* string)
{
	return strcmp(string, "-s") == 0 || strcmp(string, "-e") == 0 ||
		strcmp(string, "-E") == 0 || strcmp(string, "-n") == 0;
}

/**
* @brief Выполняет поиск в параметрах аргументов командной строки параметров команды cat из списка перечисленных в заголовке файла.
* Если параметр обнаружен, то устанавливает соответствующий флаг в глобальную переменную g_PrintInfo.
* @param argc кол-во аргументов командной строки.
* @param argv коллекция аргументов командной строки.
* @retval None.
*/
void FindAndSetCatParamsFromArgv(int argc, char const *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (IsCatParam(argv[i]))
		{
			if (strcmp(argv[i], "-n") == 0)
			{
				g_PrintInfo.param_n_is_enable = true;
			}

			if (strcmp(argv[i], "-s") == 0)
			{
				g_PrintInfo.param_s_is_enable = true;
			}

			if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "-E") == 0)
			{
				g_PrintInfo.param_E_is_enable = true;
			}
		}
	}
}

/**
* @brief Вывод строки с установленными параметрами.
* @param string Строка для вывода.
* @retval None.
*/
void PrintWithCatParams(const char* string)
{
	if (g_PrintInfo.param_s_is_enable)
	{
		if (strlen(string) == 0)
		{
			if (g_PrintInfo.param_s_prev_string_is_empty)
			{
				return;
			}
			else
			{
				g_PrintInfo.param_s_prev_string_is_empty = true;
			}
		}
		else
		{
			g_PrintInfo.param_s_prev_string_is_empty = false;
		}
	}

	if (g_PrintInfo.param_n_is_enable)
	{
		write(STDOUT_FILENO, "     ", 5);
		char count_print_string[10];
		sprintf(count_print_string, "%d", g_PrintInfo.count_print);
		write(STDOUT_FILENO, count_print_string, strlen(count_print_string));
		write(STDOUT_FILENO, "  ", 2);
	}

	write(STDOUT_FILENO, string, strlen(string));
	if (g_PrintInfo.param_E_is_enable)
	{
		write(STDOUT_FILENO, "$", 1);
	}

	write(STDOUT_FILENO, "\n", 1);
	g_PrintInfo.count_print++;
}

/**
* @brief Выполняет чтение из стандартного входного потока и выводит его на печать, как в Cat.
* @retval None.
*/
void ReadAndPrintFromStandardInputAsCat()
{
	ReadAndPrintFromStreamAsCat(STDIN_FILENO);
}

/**
* @brief Выполняет чтение из файла и выводит его содержимое на печать, как в Cat.
* @param path путь к файлу.
* @retval None.
*/
void ReadAndPrintFromFileAsCat(const char* path)
{
	int p_file = open(path, O_RDONLY);
	if (p_file < 0)
	{
		perror(path);
		return;
	}

	ReadAndPrintFromStreamAsCat(p_file);

	close(p_file);
}

/**
* @brief Выполняет чтение из входного потока и выводит их на печать, как в Cat.
* @param file_descriptor файловый дескриптор потока.
* @retval None. 
*/
void ReadAndPrintFromStreamAsCat(int file_descriptor)
{
	StringBuilder* stringBuilderInputRead = sb_create();
	while(1)
	{
		char ch;
		int stateRead = read(file_descriptor, &ch, 1);

		if (stateRead == 0)
		{
			// 0 - завершение ввода в поток с помощью ctrl-D.
			break;
		}
		else
		{
			if (ch == '\n')
			{
				// Если встречается символ перевода на следующую строку, то выполняем печать и очищаем StringBuilder.
				PrintWithCatParams(sb_concat(stringBuilderInputRead));
				sb_reset(stringBuilderInputRead);
			}
			else
			{
				// Если очередной символ, а не перевод строки, то формируем строку и добавляем в StringBuilder.
				char stringArray[] = { ch, '\0'};
				sb_append(stringBuilderInputRead, stringArray);
			}
		}
	}

	sb_free(stringBuilderInputRead);
}