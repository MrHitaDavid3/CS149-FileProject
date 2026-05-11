#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"

void *search_thread(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != SEARCH) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        int found = -1;

        for (int i = 0; i < file_count; i++) {
            if (strcmp(files[i].name, current_filename) == 0) {
                found = i;
                break;
            }
        }

        current_file_index = found;

        if (found == -1) {
            printf("File '%s' not found.\n", current_filename);
        } else {
            printf("File '%s' found at index %d.\n", current_filename, found);
        }

        if (search_requested_by_open) {
            current_command = OPEN;
            pthread_cond_signal(&cond);
        } else {
            current_command = NONE;
            current_filename[0] = '\0';
            current_file_index = -1;
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}