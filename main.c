#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//user define function
void add_course();
void show_courses();
void show_question();
void add_question();
void delete_question();
void update_question();
void admin_menu();
void student_menu();
void exam_rules();
void student_exam();
void show_all_results();
void register_admin(const char *admin_role);
void login_admin();

//structures
typedef struct
{
    char student_id[20];
    char name[50];
    char password[50];
    char student_role[10];
} user;


typedef struct
{
    char admin_id[20];
    char name[50];
    char password[50];
    char admin_role[10];
} admin;


typedef struct
{
    char course_name[100];
    char course_code[20];
} Course;


typedef struct
{
    char course_code[20];
    char question[300];
    char check;
} MCQ;


typedef struct
{
    char question[300];
    char correct_answer;
    char student_answer;
} WrongAnswer;


typedef struct
{
    char student_id[20];
    char course_code[20];
    int score;
} ExamResult;



//register an user
void register_user(const char *student_role)
{
    FILE *file = fopen("user.txt", "a");
    if (file == NULL)
    {
        perror("Error: Unable to open user database for registration. Please try again later.\n");
        return;
    }
    user newuser;
    strcpy(newuser.student_role, student_role);

    printf("Enter %s's ID: ", student_role);
    fgets(newuser.student_id, sizeof(newuser.student_id), stdin);
    newuser.student_id[strcspn(newuser.student_id, "\n")] = '\0';

    printf("Enter %s's name: ", student_role);
    fgets(newuser.name, sizeof(newuser.name), stdin);
    newuser.name[strcspn(newuser.name, "\n")] = '\0';

    printf("Enter %s's password: ", student_role);
    fgets(newuser.password, sizeof(newuser.password), stdin);
    newuser.password[strcspn(newuser.password, "\n")] = '\0';

    
    fprintf(file, "%s %s %s\n", newuser.student_id, newuser.name, newuser.password);
    fclose(file);

    printf("\n%s's registration successful.\n\n", student_role);
}



//function to log in a user
int login_user(const char *student_role)
{
    FILE *file = fopen("user.txt", "r");
    if (file == NULL)
    {
        perror("Error: Unable to open user database for login. Please try again later.\n");
        return 0;
    }
    user newuser, saveduser;

    printf("\n");

    printf("Enter %s's ID: ", student_role);
    fgets(newuser.student_id, sizeof(newuser.student_id), stdin);
    newuser.student_id[strcspn(newuser.student_id, "\n")] = '\0';

    printf("Enter %s's password: ", student_role);
    fgets(newuser.password, sizeof(newuser.password), stdin);
    newuser.password[strcspn(newuser.password, "\n")] = '\0';

    char userName[10];

    int login = 0;
    while (fscanf(file, "%s %s %s\n", saveduser.student_id, userName, saveduser.password) != EOF)
    {

        if ((strcmp(newuser.student_id, saveduser.student_id) == 0) && (strcmp(newuser.password, saveduser.password) == 0))
        {
            login = 1;
            break;
            printf("\nLogin Successful.\n");
        }
    }
    fclose(file);

    if (login == 1)
    {
        student_menu();
    }
    else if (login == 0)
    {
        printf("Error Login");
    }

    
}




//function to registration a user
void register_admin(const char *admin_role)
{
    FILE *file = fopen("admin.txt", "a");
    if (file == NULL)
    {
        perror("Error: Unable to open admin database for registration. Please try again later.\n");
        return;
    }
    admin newadmin;
    strcpy(newadmin.admin_role, admin_role);

    printf("Enter %s's ID: ", admin_role);
    fgets(newadmin.admin_id, sizeof(newadmin.admin_id), stdin);
    newadmin.admin_id[strcspn(newadmin.admin_id, "\n")] = '\0';

    printf("Enter %s's name: ", admin_role);
    fgets(newadmin.name, sizeof(newadmin.name), stdin);
    newadmin.name[strcspn(newadmin.name, "\n")] = '\0';

    printf("Enter %s's password: ", admin_role);
    fgets(newadmin.password, sizeof(newadmin.password), stdin);
    newadmin.password[strcspn(newadmin.password, "\n")] = '\0';

    printf("%s %s %s\n", newadmin.admin_id, newadmin.name, newadmin.password);
    fprintf(file, "%s %s %s\n", newadmin.admin_id, newadmin.name, newadmin.password);
    fclose(file);

    printf("\n%s's registration successful.\n\n", admin_role);
}


//
void login_admin()
{
    FILE *file = fopen("admin.txt", "r");

    admin newadmin, savedadmin;

    printf("\n");

    printf("Enter Admin's ID: ");
    fgets(newadmin.admin_id, sizeof(newadmin.admin_id), stdin);
    newadmin.admin_id[strcspn(newadmin.admin_id, "\n")] = '\0';

    

    printf("Enter Admin's password: ");
    fgets(newadmin.password, sizeof(newadmin.password), stdin);
    newadmin.password[strcspn(newadmin.password, "\n")] = '\0';

    char adminName[10];

    int login = 0;
    while (fscanf(file, "%s %s %s\n", savedadmin.admin_id, adminName, savedadmin.password) != EOF)
    {

        if ((strcmp(newadmin.admin_id, savedadmin.admin_id) == 0) && (strcmp(newadmin.password, savedadmin.password) == 0))
        {
            login = 1;
            break;
        }
    }
    fclose(file);

    if (login == 1)
    {
        printf("\nLogin Successful.\n");
        admin_menu();
    }
    else if (login == 0)
    {
        printf("Error Login");
    }

   
}



//
void add_course()
{
    Course course;
    FILE *file = fopen("courses.bin", "ab");
    if (!file)
    {
        perror("Error: Unable to open courses file to add new course. Please try again later.");
        return;
    }
    printf("Enter course name: ");
    fgets(course.course_name, sizeof(course.course_name), stdin);
    course.course_name[strcspn(course.course_name, "\n")] = '\0';

    printf("Enter course code: ");
    fgets(course.course_code, sizeof(course.course_code), stdin);
    course.course_code[strcspn(course.course_code, "\n")] = '\0';

    fwrite(&course, sizeof(Course), 1, file);
    fclose(file);
    printf("Course added successfully.\n");
}




//
void show_courses()
{
    Course course;
    FILE *file = fopen("courses.bin", "rb");
    if (!file)
    {
        perror("No courses found or unable to open courses file.\n");
        return;
    }
    int i = 1;
    printf("\nAvailable courses:\n");
    while (fread(&course, sizeof(Course), 1, file) == 1)
    {
        printf("%d. Course Name: %s\n", i++, course.course_name);
        printf("    Course code: %s\n", course.course_code);
    }
    fclose(file);
}




//
void add_question()
{
    MCQ qus;
    FILE *file = fopen("question.bin", "ab");
    if (file == NULL)
    {
        perror("Error: Unable to open question file for adding question. Please try again later.\n");
        return;
    }
    printf("Enter course code: ");
    fgets(qus.course_code, sizeof(qus.course_code), stdin);
    qus.course_code[strcspn(qus.course_code, "\n")] = '\0';

    printf("Enter the question:\n");
    fgets(qus.question, sizeof(qus.question), stdin);
    qus.question[strcspn(qus.question, "\n")] = '\0';

    printf("Option\nA. True\nB. False\n");
    printf("Enter your answer: ");
    scanf(" %c", &qus.check);
    getchar();

    qus.check = toupper(qus.check);
    if (qus.check != 'A' && qus.check != 'B')
    {
        printf("Invalid Input! Please Enter A or B only.\n");
        fclose(file);
        return;
    }
    if (fwrite(&qus, sizeof(MCQ), 1, file) != 1)
    {
        perror("Write error");
        fclose(file);
        return;
    }
    fclose(file);
    printf("Question added successfully.\n");
}



//function for show question
void show_question(int is_admin)
{
    MCQ qus;
    FILE *file = fopen("question.bin", "rb");

    if (file == NULL)
    {
        perror("No questions found or unable to open question file.\n");
        return;
    }
    char course_code[20];
    printf("Enter course code to view questions: ");
    fgets(course_code, sizeof(course_code), stdin);
    course_code[strcspn(course_code, "\n")] = '\0';

    printf("\nQuestions for course code [%s]:\n", course_code);

    int question_number = 1, found = 0;

    while (fread(&qus, sizeof(MCQ), 1, file) == 1)
    {
        if (strcmp(qus.course_code, course_code) == 0)
        {
            printf("\n%d. %s\n", question_number++, qus.question);
            if (is_admin)
            {
                printf("Answer: %c\n", qus.check);
            }
            found++;
        }
    }
    if (!found)
    {
        printf("No questions stored for this course.\n");
    }

    fclose(file);
}



//function for student take an exam
void student_exam()
{
    show_courses();

    int choice;
    printf("Do you want to take an exam?\n");
    printf("1.Yes\n");
    printf("2.No\n");
    scanf("%d", &choice);
    getchar();

    if (choice != 1)
    {
        printf("You chose not to take an exam.\n");
        return;
    }

    char student_id[20];
    printf("Enter your Student ID: ");
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = '\0';

    char given_code[20];
    printf("Enter the course code you want to take exam for: ");
    scanf("%s", given_code);
    getchar();

    FILE *file = fopen("question.bin", "rb");
    if (!file)
    {
        perror("Error: Unable to open question file. Please try again later.\n");
        return;
    }

    int total_question = 0, correct_answer = 0, wrong_count = 0;
    MCQ qus;

    while (fread(&qus, sizeof(MCQ), 1, file) == 1)
    {
        if (strcmp(qus.course_code, given_code) == 0)
        {
            total_question++;
            printf("\nQuestion %d: %s\n", total_question, qus.question);
            printf("A. True\nB. False\n");
            printf("Your Answer (A/B) : ");

            char ans;
            scanf(" %c", &ans);
            ans = toupper(ans);

            if (ans == qus.check)
            {
                printf("Correct!\n");
                correct_answer++;
            }
            else
            {
                printf("Wrong.\n");
                wrong_count++;
            }
        }
    }
    fclose(file);

    if (total_question == 0)
    {
        printf("No questions found for course %s\n", given_code);
        return;
    }

    printf("\nExam completed!\n");
    printf("Total Questions: %d\n", total_question);
    printf("Correct Answers: %d\n", correct_answer);
    printf("Wrong Answers: %d\n", wrong_count);

    ExamResult result;
    strcpy(result.student_id, student_id);
    strcpy(result.course_code, given_code);
    result.score = correct_answer;

    FILE *res_file = fopen("results.bin", "ab");
    if (res_file == NULL)
    {
        perror("Error! Result can't be saved.\n");
        return;
    }
    fwrite(&result, sizeof(ExamResult), 1, res_file);
    fclose(res_file);

    printf("Your result successfully saved.\n");
}




//function for admin menu
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
            printf("Update questions.\n");
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
            printf("Logging out\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}


//function for student menu
void student_menu()
{
    int choice;
    while (1)
    {
        printf("\n   Student Menu    \n");
        printf("1. View Courses\n");
        printf("2. Exam Instructions\n");
        printf("3. Take exam\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            show_courses();
            break;
        case 2:
            printf("Exam instructions: \n");
                           exam_rules();
            break;
        case 3:
            student_exam();
            break;
        case 4:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}


//function for update question
void update_question()
{
    FILE *file = fopen("question.bin", "rb");
    if (file == NULL)
    {
        perror("Error: Could not open question file.");
        return;
    }
    MCQ questions[100];
    int total = 0;

    while (fread(&questions[total], sizeof(MCQ), 1, file) == 1)
    {
        total++;
    }
    fclose(file);

    char course_code[20];
    printf("Enter course code to update questions: ");
    fgets(course_code, sizeof(course_code), stdin);
    course_code[strcspn(course_code, "\n")] = '\0';

    int map[100];
    int shown = 0;
    for (int i = 0; i < total; i++)
    {
        if (strcmp(questions[i].course_code, course_code) == 0)
        {
            printf("%d. %s (Answer: %c)\n", shown + 1, questions[i].question, questions[i].check);
            map[shown++] = i;
        }
    }

    if (shown == 0)
    {
        printf("No questions found for course code: %s\n", course_code);
        return;
    }

    int choice;
    printf("Enter question number to update (1 - %d): ", shown);
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > shown)
    {
        printf("Invalid question number.\n");
        return;
    }

    int idx = map[choice - 1];
    printf("Enter updated question: ");
    fgets(questions[idx].question, sizeof(questions[idx].question), stdin);
    questions[idx].question[strcspn(questions[idx].question, "\n")] = '\0';

    printf("Option:\nA. True\nB. False\n");
    printf("Enter correct answer (A/B): ");
    scanf(" %c", &questions[idx].check);
    getchar();

    questions[idx].check = toupper(questions[idx].check);
    if (questions[idx].check != 'A' && questions[idx].check != 'B')
    {
        printf("Invalid input. Only A or B allowed.\n");
        return;
    }
    file = fopen("question.bin", "wb");
    if (file == NULL)
    {
        perror("Error: Could not write to question file.");
        return;
    }

    fwrite(questions, sizeof(MCQ), total, file);
    fclose(file);

    printf("Question updated successfully.\n");
}




//function for delet question
void delete_question()
{
    FILE *file = fopen("question.bin", "rb");
    if (file == NULL)
    {
        perror("Error: Could not open question file.");
        return;
    }

    MCQ questions[100];
    int total = 0;
    while (fread(&questions[total], sizeof(MCQ), 1, file) == 1)
        total++;
    fclose(file);

    char course_code[20];
    printf("Enter course code to delete question: ");
    fgets(course_code, sizeof(course_code), stdin);
    course_code[strcspn(course_code, "\n")] = '\0';

    int map[100], shown = 0;
    for (int i = 0; i < total; i++)
        if (strcmp(questions[i].course_code, course_code) == 0)
        {
            printf("%d. %s (Answer: %c)\n", shown + 1,
                   questions[i].question, questions[i].check);
            map[shown++] = i; 
        }

    if (shown == 0)
    {
        printf("No questions found for this course.\n");
        return;
    }

    int choice;
    printf("Enter question number to delete (1 %d): ", shown);
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > shown)
    {
        printf("Invalid question number.\n");
        return;
    }

    int idx = map[choice - 1];

    for (int i = idx; i < total - 1; i++)
        questions[i] = questions[i + 1];
    total--;

    file = fopen("question.bin", "wb");
    if (file == NULL)
    {
        perror("Error: Could not write to question file.");
        return;
    }
    fwrite(questions, sizeof(MCQ), total, file);
    fclose(file);

    printf("Question deleted successfully.\n");
}



//
void exam_rules()
{
    printf("\n      Exam Instructions \n");
    printf("1. Each exam is based on a specific course code.\n");
    printf("2. Only 'A' (True) or 'B' (False) are accepted answers.\n");
    printf("3. Make sure to read the question carefully before answering.\n");
    printf("4. You will see whether your answer is correct or not immediately.\n");
    printf("5. Final result will show total, correct, and wrong answers.\n");
    printf("6. You cannot retake the same exam unless the admin resets it.\n");
    printf("7. Don't press extra keys or enter wrong course code.\n");
    printf("8. Best of luck!\n");
    printf("\n\n");
}



//
void show_all_results()
{
    ExamResult result;
    FILE *file = fopen("results.bin", "rb");
    if (!file)
    {
        printf("No results found or unable to open results file.\n");
        return;
    }

    int found = 0;

    printf("\n      Exam Results \n");

    while (fread(&result, sizeof(ExamResult), 1, file) == 1)
    {
        printf("Student ID: %s\n", result.student_id);
        printf("Course Code: %s\n", result.course_code);
        printf("Score: %d\n", result.score);
        printf("\n\n");
        found++;
    }
    if (!found)
    {
        printf("No exam results found.\n");
    }

    fclose(file);
}




//main function
int main()
{

    int user_choice, action_choice;

    while (1)
    {
        printf("\nChoose user type:\n");
        printf("1. Student\n");
        printf("2. Admin\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &user_choice);
        getchar();

        if (user_choice == 3)
        {
            printf("Exiting system.\n");
            break;
        }

        const char *role = (user_choice == 1) ? "student" : "admin";

        switch (user_choice)
        {
        case 1:
            int choose1;
            printf("\n----------Student---------\n");
            printf("\nChoose action:\n");
            printf("1. Register\n");
            printf("2. Login\n");
            printf("Enter your choice: ");
            scanf("%d", &choose1);
            getchar();

            switch (choose1)
            {
            case 1:
                register_user(role);
                break;
            case 2:
                if (login_user(role))
                {
                    if (strcmp(role, "Student") == 0)
                        student_menu();
                    else if (strcmp(role, "Admin") == 0)
                        admin_menu();
                }
                break;
            default:
                printf("Invalid action choice.\n");
            }
            break;

        case 2:
            printf("\n----------Admin---------\n");
            int choose2;
            printf("\nChoose action:\n");
            printf("1. Register\n");
            printf("2. Login\n");
            printf("Enter your choice: ");
            scanf("%d", &choose2);
            getchar();

            switch (choose2)
            {
            case 1:
                register_admin(role);
                break;
            case 2:
                login_admin();
                break;
            default:
                printf("Invalid action choice.\n");
            }
            break;
        default:
            printf("Invalid user type.\n");
        }
    }

    return 0;
}



