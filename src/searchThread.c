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
        search_done = true;
        current_command = NONE;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
