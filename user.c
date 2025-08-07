// Function to register a new user (student or admin)
void register_user(const char *role) {
    FILE *file = fopen("user.bin", "rb");
    if (file == NULL) {
        printf("No existing user found. Proceeding with registration...\n");
    }

    char check_id[20];
    printf("Enter %s's ID: ", role);
    fgets(check_id, sizeof(check_id), stdin);
    check_id[strcspn(check_id, "\n")] = '\0'; // Remove newline character

    user temp;
    if (file != NULL) {
        while (fread(&temp, sizeof(user), 1, file) == 1) {
            if (strcmp(temp.student_id, check_id) == 0 && strcmp(temp.role, role) == 0) {
                printf("\nError: This ID is already registered as a %s!\n", role);
                fclose(file);
                return;
            }
        }
        fclose(file);
    }

    // Registration starts
    file = fopen("user.bin", "ab");
    if (file == NULL) {
        perror("Error: Unable to open user database for writing.");
        return;
    }

    user newuser;
    strcpy(newuser.role, role);
    strcpy(newuser.student_id, check_id);

    printf("Enter %s's name: ", role);
    fgets(newuser.name, sizeof(newuser.name), stdin);
    newuser.name[strcspn(newuser.name, "\n")] = '\0';

    printf("Enter %s's password: ", role);
    fgets(newuser.password, sizeof(newuser.password), stdin);
    newuser.password[strcspn(newuser.password, "\n")] = '\0';

    fwrite(&newuser, sizeof(user), 1, file);
    fflush(file); // Ensure data is written to disk
    fclose(file);
    printf("\n%s's registration successful.\n\n", role);
}


int login_user(const char *role) {
    FILE *file = fopen("user.bin", "rb");
    if (file == NULL) {
        perror("Error: Unable to open user database for login. Please try again later.");
        return 0;
    }

    user newuser, saveduser;

    printf("\nEnter %s's ID: ", role);
    fgets(newuser.student_id, sizeof(newuser.student_id), stdin);
    newuser.student_id[strcspn(newuser.student_id, "\n")] = '\0';

    printf("Enter %s's password: ", role);
    fgets(newuser.password, sizeof(newuser.password), stdin);
    newuser.password[strcspn(newuser.password, "\n")] = '\0';

    while (fread(&saveduser, sizeof(user), 1, file)) {
        if ((strcmp(newuser.student_id, saveduser.student_id) == 0) &&
            (strcmp(newuser.password, saveduser.password) == 0) &&
            (strcmp(role, saveduser.role) == 0)) {
            printf("\nLogin Successful.\n");
            fclose(file);
            return 1;
        }
    }

    printf("\nLogin failed. Incorrect credentials or role.\n");
    fclose(file);
    return 0;
}

