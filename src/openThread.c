#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "globals.h"

static int prefill_readline(const char *text, int state) {
    (void)text; (void)state;
    rl_insert_text(global_buffer);
    rl_redisplay();
    return 0;
}

void *open_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != OPEN) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        current_command = SEARCH;

        pthread_cond_wait(&cond, &mutex);

        if (current_file_index == -1) {
            current_command = NONE;
            current_filename[0] = '\0';
            current_file_index = -1;

            pthread_mutex_unlock(&mutex);
            continue;
        }

        strcpy(global_buffer, files[current_file_index].content);

        printf("Edit text below. Press Enter to save and close.\n");

        rl_startup_hook = prefill_readline;

        char *edited = readline("");

        rl_startup_hook = NULL;

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
