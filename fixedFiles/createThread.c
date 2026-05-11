#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"

void *create_thread(void *arg) {
    (void)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != CREATE) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        int exists = 0;

        for (int i = 0; i < file_count; i++) {
            if (strcmp(files[i].name, current_filename) == 0) {
                exists = 1;
                break;
            }
        }

        if (exists) {
            printf("File '%s' already exists.\n", current_filename);
        } else if (file_count >= MAX_FILES) {
            printf("File system full. Cannot create '%s'.\n", current_filename);
        } else {
            strncpy(files[file_count].name, current_filename, MAX_FILENAME - 1);
            files[file_count].name[MAX_FILENAME - 1] = '\0';

            files[file_count].content[0] = '\0';
            files[file_count].size = 0;

            printf("File '%s' created successfully at index %d.\n",
                   files[file_count].name,
                   file_count);

            file_count++;
        }

        current_command = NONE;
        current_filename[0] = '\0';
        current_file_index = -1;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}