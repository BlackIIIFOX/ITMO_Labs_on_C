#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <ctype.h>
#include <pthread.h> 

#define COUNT_LETTERS 26
#define TIME_WAIT_PRINT_MILLISECOND 100
#define TIME_WAIT_THREADS_MILLISECOND 20

static pthread_mutex_t mutex;

char letters[COUNT_LETTERS] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

void print_letters();
void* invert_letters();
void* change_case();
int to_microsecond(int milliscond);

int main()
{
	pthread_mutex_init(&mutex, NULL);

	pthread_t id_thread_invert; 
	pthread_t id_change_case;
	pthread_create(&id_thread_invert, NULL, invert_letters, NULL); 
	pthread_create(&id_change_case, NULL, change_case, NULL); 

	while (1)
	{
		print_letters();
		usleep(to_microsecond(TIME_WAIT_PRINT_MILLISECOND));
	}

	//pthread_join(id_thread_invert, NULL);
	//pthread_join(id_change_case, NULL);

	return 0;
}

int to_microsecond(int milliscond)
{
	return milliscond * 1000;
}

void print_letters()
{
	for (int i = 0; i < COUNT_LETTERS; i++)
	{
        printf("%c ", letters[i]);
	}

	printf("\n");
}

void* invert_letters()
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		for (int i = 0; i < COUNT_LETTERS/2; i++)
		{
			for (int j = 0; j < 10000000; j++);
			char tmp_char = letters[i];
			letters[i] = letters[COUNT_LETTERS - i - 1];
			letters[COUNT_LETTERS - i - 1] = tmp_char;
		}
		pthread_mutex_unlock(&mutex);

		usleep(to_microsecond(TIME_WAIT_THREADS_MILLISECOND));
	}
}

void* change_case()
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		for (int i = 0; i < COUNT_LETTERS; i++)
		{
			for (int j = 0; j < 10000000; j++);
			if (isupper(letters[i]) != 0)
			{
				letters[i] = tolower(letters[i]);
			}
			else
			{
				letters[i] = toupper(letters[i]);
			}
		}
		pthread_mutex_unlock(&mutex);

		usleep(to_microsecond(TIME_WAIT_THREADS_MILLISECOND));
	}
}