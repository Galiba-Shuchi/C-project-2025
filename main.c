// MAIN FUNCTION
int main() {
    int user_choice, action_choice;

    while (1) {
        printf("\n=====================================\n");
        printf("        🖥️  ONLINE EXAM SYSTEM\n");
        printf("=====================================\n");
        printf("Choose user type:\n");
        printf("  1. Student 👨‍🎓\n");
        printf("  2. Admin 🧑‍💼\n");
        printf("  3. Exit 🚪\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &user_choice);
        getchar();  // consume newline

        if (user_choice == 3) {
            printf("\nThank you for using the Online Exam System.\n");
            break;
        }

        if (user_choice != 1 && user_choice != 2) {
            printf("\n❌ Invalid user type. Please try again.\n");
            continue;
        }

        const char *role = (user_choice == 1) ? "student" : "admin";

        printf("\n=====================================\n");
        printf("        👤 %s Portal\n", role);
        printf("=====================================\n");
        printf("Choose action:\n");
        printf("  1. Register 📝\n");
        printf("  2. Login 🔐\n");
        printf("Enter your choice (1-2): ");
        scanf("%d", &action_choice);
        getchar();

        switch (action_choice) {
            case 1:
                register_user(role);
                break;

            case 2:
                if (login_user(role)) {
                    if (strcmp(role, "student") == 0)
                        student_menu();
                    else
                        admin_menu();
                } else {
                    printf("\n❌ Login failed. Please try again.\n");
                }
                break;

            default:
                printf("\n❌ Invalid action choice.\n");
        }
    }

    return 0;
}
