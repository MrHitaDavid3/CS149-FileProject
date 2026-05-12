#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"

void *close_thread(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != CLOSE) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        strncpy(files[current_file_index].content, global_buffer, MAX_CONTENT - 1);
        files[current_file_index].content[MAX_CONTENT - 1] = '\0';

        files[current_file_index].size = (int)strlen(files[current_file_index].content);

        printf(
            "File '%s' saved and closed. %d/%d characters used.\n",
            files[current_file_index].name,
            files[current_file_index].size,
            MAX_CONTENT - 1
        );

        current_command = NONE;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
