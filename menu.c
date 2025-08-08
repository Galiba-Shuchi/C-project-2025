void admin_menu()
{
    int choice;
    while (1)
    {
        printf("\n    Admin Menu    \n\n");
        printf("1. Add Courses\n");
        printf("2. Show Courses\n");
        printf("3. Show Questions\n");
        printf("4. Add Question\n");
        printf("5. Update Question\n");
        printf("6. Delete Question\n");
        printf("7. View All Exam Results\n");
        printf("8. Logout\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            add_course();
            break;
        case 2:
            show_courses();
            break;
        case 3:
            show_question(1);
            break;
        case 4:
            add_question();
            break;
        case 5:
            printf("Update questions\n");
            update_question();
            break;
        case 6:
            printf("Delete questions\n");
            delete_question();
            break;
        case 7:
            show_all_results();
            break;
        case 8:
            printf("Logging oUT\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}
