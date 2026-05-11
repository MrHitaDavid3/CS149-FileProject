#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "globals.h"

void *open_thread(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != OPEN) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        search_requested_by_open = 1;
        current_command = SEARCH;

        pthread_cond_wait(&cond, &mutex);

        search_requested_by_open = 0;

        if (current_file_index == -1) {
            current_command = NONE;
            current_filename[0] = '\0';
            current_file_index = -1;

            pthread_mutex_unlock(&mutex);
            continue;
        }

        strncpy(global_buffer, files[current_file_index].content, MAX_CONTENT - 1);
        global_buffer[MAX_CONTENT - 1] = '\0';

        printf("\nOpening file '%s'.\n", files[current_file_index].name);

        if (strlen(global_buffer) > 0) {
            printf("Current file content:\n%s\n", global_buffer);
        } else {
            printf("Current file content is empty.\n");
        }

        printf("Enter new content and press Enter to save and close.\n");

        char *edited = readline("> ");

        if (edited == NULL) {
            printf("Edit cancelled.\n");

            current_command = NONE;
            current_filename[0] = '\0';
            current_file_index = -1;

            pthread_mutex_unlock(&mutex);
            continue;
        }

        if (strlen(edited) >= MAX_CONTENT) {
            printf("Edited text too long. Truncating.\n");
        }

        strncpy(global_buffer, edited, MAX_CONTENT - 1);
        global_buffer[MAX_CONTENT - 1] = '\0';

        free(edited);

        current_command = CLOSE;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}