#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "globals.h"

static char *initial_text = NULL;

int prefill_hook(void) {
    if (initial_text) {
        rl_insert_text(initial_text);
        initial_text = NULL;
    }
    return 0;
}

void *open_thread(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != OPEN) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        strncpy(global_buffer, files[current_file_index].content, MAX_CONTENT - 1);
        global_buffer[MAX_CONTENT - 1] = '\0';

        printf("Edit file and press. Enter to save and close. Clear input and ctrl + D to close without saving.\n");

        initial_text = global_buffer;
        rl_startup_hook = prefill_hook;
        
        char *edited = readline("> ");

        rl_startup_hook = NULL;

        if (edited == NULL) {
            current_command = NONE;

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
