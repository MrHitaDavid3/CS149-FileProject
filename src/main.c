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
bool search_done = false;

void *create_thread(void *arg);
void *open_thread(void *arg);
void *close_thread(void *arg);
void *search_thread(void *arg);

bool read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) { // Deal with EOF
        clearerr(stdin);
        return false;
    }
    
    if (strchr(buffer, '\n') == NULL) { // input too long
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF); // clear remaining input
        printf("Invalid command.");
        return false;
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
    
    return true;
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

    char cmd_input[MAX_CMD_INPUT];

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != NONE) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        printf("\nCommands: create, open, search, exit\n");
        
        printf("Enter command: ");
        if (read_line(cmd_input, MAX_CMD_INPUT) == false) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        if (strcmp(cmd_input, "exit") == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        printf("Enter filename: ");
        if (read_line(current_filename, MAX_FILENAME) == false) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        if (strcmp(cmd_input, "create") == 0) {
            current_command = CREATE;
        } 
        else if (strcmp(cmd_input, "open") == 0) {
            search_done = false;
            current_command = SEARCH;
            while(!search_done) {
                pthread_cond_wait(&cond, &mutex);
            }
            if (current_file_index == -1){
                printf("File does not exist.\n");
            }
            else {
                current_command = OPEN;
            }
        } 
        else if (strcmp(cmd_input, "search") == 0) {
            search_done = false;
            current_command = SEARCH;
            while(!search_done) {
                pthread_cond_wait(&cond, &mutex);
            }
            if (current_file_index == -1){
                printf("File does not exist.\n");                
            }
            else {
                printf("File exists.\n");
            }
        } 
        else {
            printf("Invalid command.\n");
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Program ended.\n");

    return 0;
}
