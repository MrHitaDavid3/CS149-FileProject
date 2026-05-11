int main() {
    pthread_t t_create, t_open, t_close, t_search;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t_create, NULL, create_thread, NULL);
    pthread_create(&t_open, NULL, open_thread, NULL);
    pthread_create(&t_close, NULL, close_thread, NULL);
    pthread_create(&t_search, NULL, search_thread, NULL);

    char cmd[20];

    while (1) {
        pthread_mutex_lock(&mutex);

        if (current_command != NONE) {
            pthread_mutex_unlock(&mutex);
            sleep(1);
            continue;
        }

        printf("\nCommands: create, open, close, search, exit\n");
        printf("Enter command: ");
        scanf("%s", cmd);

        if (strcmp(cmd, "exit") == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (strcmp(cmd, "close") != 0) {
            printf("Enter filename: ");
            scanf("%s", current_filename);
        }

        if (strcmp(cmd, "create") == 0) {
            current_command = CREATE;
        } 
        else if (strcmp(cmd, "open") == 0) {
            current_command = OPEN;
        } 
        else if (strcmp(cmd, "close") == 0) {
            current_command = CLOSE;
        } 
        else if (strcmp(cmd, "search") == 0) {
            current_command = SEARCH;
        } 
        else {
            printf("Invalid command.\n");
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_join(t_create, NULL);
    pthread_join(t_open, NULL);
    pthread_join(t_close, NULL);
    pthread_join(t_search, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
