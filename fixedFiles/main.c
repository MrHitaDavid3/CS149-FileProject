#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"

pthread_mutex_t mutex;
pthread_cond_t cond;

File files[MAX_FILES];
int file_count = 0;

Command current_command = NONE;
char current_filename[MAX_FILENAME] = "";
int current_file_index = -1;

char global_buffer[MAX_CONTENT] = "";

int search_requested_by_open = 0;

void *create_thread(void *arg);
void *open_thread(void *arg);
void *close_thread(void *arg);
void *search_thread(void *arg);

void clear_input_buffer(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int main(void) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t t_create;
    pthread_t t_open;
    pthread_t t_close;
    pthread_t t_search;

    pthread_create(&t_create, NULL, create_thread, NULL);
    pthread_create(&t_open, NULL, open_thread, NULL);
    pthread_create(&t_close, NULL, close_thread, NULL);
    pthread_create(&t_search, NULL, search_thread, NULL);

    char cmd_input[20];

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != NONE) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        printf("\nCommands: create, open, search, exit\n");
        printf("Enter command: ");

        if (scanf("%19s", cmd_input) != 1) {
            clear_input_buffer();
            pthread_mutex_unlock(&mutex);
            continue;
        }

        clear_input_buffer();

        if (strcmp(cmd_input, "exit") == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (strcmp(cmd_input, "create") == 0) {
            printf("Enter filename: ");

            if (scanf("%49s", current_filename) != 1) {
                clear_input_buffer();
                pthread_mutex_unlock(&mutex);
                continue;
            }

            clear_input_buffer();

            current_file_index = -1;
            current_command = CREATE;
        } else if (strcmp(cmd_input, "open") == 0) {
            printf("Enter filename: ");

            if (scanf("%49s", current_filename) != 1) {
                clear_input_buffer();
                pthread_mutex_unlock(&mutex);
                continue;
            }

            clear_input_buffer();

            current_file_index = -1;
            current_command = OPEN;
        } else if (strcmp(cmd_input, "search") == 0) {
            printf("Enter filename: ");

            if (scanf("%49s", current_filename) != 1) {
                clear_input_buffer();
                pthread_mutex_unlock(&mutex);
                continue;
            }

            clear_input_buffer();

            current_file_index = -1;
            search_requested_by_open = 0;
            current_command = SEARCH;
        } else {
            printf("Invalid command.\n");
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Program ended.\n");

    return 0;
}