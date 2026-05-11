#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>

#define MAX_FILES 100
#define MAX_FILENAME 50
#define MAX_CONTENT 1024

typedef enum {
    NONE,
    CREATE,
    OPEN,
    CLOSE,
    SEARCH
} Command;

typedef struct {
    char name[MAX_FILENAME];
    char content[MAX_CONTENT];
    int size;
} File;

/* Shared globals */
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

extern File files[MAX_FILES];
extern int file_count;

extern Command current_command;
extern char current_filename[MAX_FILENAME];
extern int current_file_index;

extern char global_buffer[MAX_CONTENT];

#endif
