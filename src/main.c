#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"

pthread_mutex_t mutex;
pthread_cond_t cond;

File files[MAX_FILES];
int file_count = 0;

Command current_command = NONE;
char current_filename[MAX_FILENAME];
int current_file_index = -1;

char global_buffer[MAX_CONTENT];

void *create_thread(void *arg);
void *open_thread(void *arg);
void *close_thread(void *arg);
void *search_thread(void *arg);

int main() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    
    pthread_t t_create, t_open, t_close, t_search;

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

        printf("\nCommands: create, open, exit\n");
        printf("Enter command: ");
        scanf("%19s", cmd_input);

        if (strcmp(cmd_input, "exit") == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        if (strcmp(cmd_input, "create") == 0) {
            printf("Enter filename: ");
            scanf("%49s", current_filename);
            current_command = CREATE;
        } 
        else if (strcmp(cmd_input, "open") == 0) {
            printf("Enter filename: ");
            scanf("%49s", current_filename);
            current_command = OPEN;
        } 
        else {
            printf("Invalid command.\n");
        }

        pthread_mutex_unlock(&mutex);
    }

    return 0;
}
