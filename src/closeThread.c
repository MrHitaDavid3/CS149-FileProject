#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "globals.h"

void *close_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != CLOSE) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        /* Copy the edited global buffer back into the file's content */
        strncpy(files[current_file_index].content, global_buffer, MAX_CONTENT - 1);
        files[current_file_index].content[MAX_CONTENT - 1] = '\0';
        files[current_file_index].size = (int)strlen(global_buffer);

        printf("File '%s' saved. Size: %d/%d characters used.\n",
               files[current_file_index].name,
               files[current_file_index].size,
               MAX_CONTENT);

        /* Reset globals and return control to main */
        global_buffer[0] = '\0';
        current_command = NONE;
        current_filename[0] = '\0';
        current_file_index = -1;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
