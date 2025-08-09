/* ExamBunny - exambunny.c
   Phase 1: includes, logo, structs, prototypes
   All storage uses TEXT files (no binary).
   Follow-up: I'll provide function implementations user-wise (admin/teacher/student).
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>       // timer for exam
#include <unistd.h>     // sleep (optional, unix). On Windows you may need <windows.h>

/* -------------------------
   Constants: filenames
   -------------------------*/
#define USER_FILE       "user.txt"        // students (id name password role)
#define ADMIN_FILE      "admin.txt"       // admins (id name password)
#define TEACHER_FILE    "teacher.txt"     // teachers (id name password vcode)
#define COURSES_FILE    "courses.txt"     // courses (code|name)
#define QUESTIONS_FILE  "questions.txt"   // questions (course_code|question|answer)
#define RESULTS_FILE    "results.txt"     // results (student_id|course_code|score)
#define VIDEO_FILE      "video_links.txt" // video links (course_code|title|url)
#define PDF_FILE        "pdf_links.txt"   // pdf links (course_code|title|url)
#define WEB_FILE        "web_links.txt"   // web pages (course_code|title|url)
#define COUNSEL_FILE    "counseling.txt"  // counseling messages (student_id|teacher_id|message|time)

/* -------------------------
   Utility constants
   -------------------------*/
#define MAX_LINE 512
#define MAX_NAME 100
#define MAX_ID   50
#define MAX_PASS 50
#define MAX_Q    300
#define VERIF_CODE_LEN 20

/* -------------------------
   Logo / Header function
   -------------------------*/
void print_logo()
{
    printf("\n");
    printf("====================================================\n");
    printf("  ______                      ____                      \n");
    printf(" |  ____|                    |  _ \\                     \n");
    printf(" | |__   _ __   ___  _ __ ___| |_) | ___  ___  ___ ___  \n");
    printf(" |  __| | '_ \\ / _ \\| '__/ _ \\  _ < / _ \\/ __|/ __/ _ \\ \n");
    printf(" | |____| | | | (_) | | |  __/ |_) |  __/\\__ \\ (_|  __/ \n");
    printf(" |______|_| |_|\\___/|_|  \\___|____/ \\___||___/\\___\\___| \n");
    printf("\n");
    printf("                Welcome to ExamBunny\n");
    printf("        Lightweight Online Education System (TXT)\n");
    printf("====================================================\n\n");
}

/* -------------------------
   Struct declarations
   -------------------------*/

/* Generic user (students) */
typedef struct {
    char student_id[MAX_ID];
    char name[MAX_NAME];
    char password[MAX_PASS];
    char role[20];   /* e.g. "student" */
} User;

/* Admin */
typedef struct {
    char admin_id[MAX_ID];
    char name[MAX_NAME];
    char password[MAX_PASS];
} Admin;

/* Teacher */
typedef struct {
    char teacher_id[MAX_ID];
    char name[MAX_NAME];
    char password[MAX_PASS];
    char vcode[VERIF_CODE_LEN];   /* verification code */
} Teacher;

/* Course */
typedef struct {
    char course_code[50];
    char course_name[150];
} Course;

/* MCQ (True/False simplified) stored in text as:
   course_code|question|answer(A/B)
*/
typedef struct {
    char course_code[50];
    char question[MAX_Q];
    char answer; /* 'A' or 'B' */
} MCQ;

/* Exam result */
typedef struct {
    char student_id[MAX_ID];
    char course_code[50];
    int score;
    char timestamp[50];
} ExamResult;

/* Media link (video/pdf/web) */
typedef struct {
    char course_code[50];
    char title[150];
    char url[300];
} MediaLink;

/* Counseling message */
typedef struct {
    char student_id[MAX_ID];
    char teacher_id[MAX_ID]; /* optional: if student picks a teacher, else "any" */
    char message[500];
    char time_sent[50];
} CounselMsg;

/* -------------------------
   Function prototypes
   (I will implement these step-by-step as you requested)
   Grouped by purpose / user
   -------------------------*/

/* --- General utilities --- */
void pause_and_continue();
void clear_stdin();
void write_line_safe(FILE *fp, const char *line);
void get_timestamp(char *buf, int len);

/* --- Registration & Login --- */
/* Student */
void register_user(const char *role);
int login_user(const char *role); /* returns 1 on success (and sets some global/session) */
/* Admin */
void register_admin(const char *role); /* NOTE: later admin registration may be disabled per requirement */
int login_admin();
/* Teacher */
void register_teacher(); /* admins create teachers or teachers self-register (decide later) */
int login_teacher();

/* --- Admin features --- */
void admin_menu();
void admin_view_all_teachers();
void admin_view_all_students();
void admin_view_all_results();
void admin_view_courses();
void admin_update_course(); /* edit course name */

/* --- Teacher features --- */
void teacher_menu(const char *teacher_id);
void teacher_view_courses();
void teacher_add_question(const char *teacher_id);
void teacher_update_question(const char *teacher_id);
void teacher_delete_question(const char *teacher_id);
void teacher_add_video(const char *teacher_id);
void teacher_add_pdf(const char *teacher_id);
void teacher_add_webpage(const char *teacher_id);
void teacher_view_results_of_their_courses(const char *teacher_id);
void teacher_view_counseling_messages(const char *teacher_id);

/* --- Student features --- */
void student_menu(const char *student_id);
void student_view_courses();
void student_exam(const char *student_id); /* includes timer logic */
void student_view_media(); /* videos/pdfs/web */
void student_view_my_results(const char *student_id);
void student_send_counsel(const char *student_id);

/* --- Course & Question management (text based) --- */
int add_course_text(const Course *c);                 /* append course */
int load_courses(Course courses[], int max);         /* returns count */
int save_courses(Course courses[], int count);       /* overwrite file */
int add_question_text(const MCQ *q);                 /* append question */
int load_questions(MCQ questions[], int max);        /* returns count */
int save_questions(MCQ questions[], int count);      /* overwrite file */

/* --- Media management --- */
int add_media(const char *filename, const MediaLink *m);
int list_media(const char *filename, MediaLink list[], int max);

/* --- Results --- */
int save_result(const ExamResult *r);
int load_results(ExamResult results[], int max);

/* --- Counseling --- */
int save_counsel(const CounselMsg *c);
int load_counsel_for_teacher(const char *teacher_id, CounselMsg out[], int max);

/* -------------------------
   End of Phase 1 (skeleton)
   Next: implement the utility functions + registration/login (step 2)
   -------------------------*/



   /* -------------------------
   Part-2: Utilities & File helpers
   (paste this AFTER the prototypes in your exambunny.c)
   -------------------------*/

#include <errno.h>

/* ---------- small helpers ---------- */

/* pause and wait for Enter */
void pause_and_continue()
{
    printf("\nPress Enter to continue...");
    fflush(stdout);
    int c = getchar();
    if (c != '\n') { /* if leftover, clear rest of line */
        clear_stdin();
    }
}

/* clear stdin buffer up to newline */
void clear_stdin()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
}

/* get current timestamp as "YYYY-MM-DD HH:MM:SS" */
void get_timestamp(char *buf, int len)
{
    time_t t = time(NULL);
    struct tm tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm);
}

/* make field safe for text-file by removing '|' and newlines (replace with space) */
void sanitize_field(char *s)
{
    for (int i = 0; s[i]; i++) {
        if (s[i] == '|' || s[i] == '\n' || s[i] == '\r') s[i] = ' ';
    }
}

/* convenience: write a line with newline */
void write_line_safe(FILE *fp, const char *line)
{
    fprintf(fp, "%s\n", line);
}

/* ---------- Course file helpers ---------- */

/* append course to COURSES_FILE */
int add_course_text(const Course *c)
{
    FILE *fp = fopen(COURSES_FILE, "a");
    if (!fp) return 0;
    char cc[64], cn[256];
    strncpy(cc, c->course_code, sizeof(cc)-1); cc[sizeof(cc)-1]=0;
    strncpy(cn, c->course_name, sizeof(cn)-1); cn[sizeof(cn)-1]=0;
    sanitize_field(cc); sanitize_field(cn);
    fprintf(fp, "%s|%s\n", cc, cn);
    fclose(fp);
    return 1;
}

/* load all courses into array, return count */
int load_courses(Course courses[], int max)
{
    FILE *fp = fopen(COURSES_FILE, "r");
    if (!fp) return 0;
    char line[MAX_LINE];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < max) {
        line[strcspn(line, "\n")] = '\0';
        char *sep = strchr(line, '|');
        if (!sep) continue;
        *sep = '\0';
        strncpy(courses[cnt].course_code, line, sizeof(courses[cnt].course_code)-1);
        courses[cnt].course_code[sizeof(courses[cnt].course_code)-1] = 0;
        strncpy(courses[cnt].course_name, sep + 1, sizeof(courses[cnt].course_name)-1);
        courses[cnt].course_name[sizeof(courses[cnt].course_name)-1] = 0;
        cnt++;
    }
    fclose(fp);
    return cnt;
}

/* overwrite courses file with given array */
int save_courses(Course courses[], int count)
{
    FILE *fp = fopen(COURSES_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        char cc[64], cn[256];
        strncpy(cc, courses[i].course_code, sizeof(cc)-1); cc[sizeof(cc)-1]=0;
        strncpy(cn, courses[i].course_name, sizeof(cn)-1); cn[sizeof(cn)-1]=0;
        sanitize_field(cc); sanitize_field(cn);
        fprintf(fp, "%s|%s\n", cc, cn);
    }
    fclose(fp);
    return 1;
}

/* ---------- Question file helpers ---------- */

/* append MCQ to QUESTIONS_FILE */
int add_question_text(const MCQ *q)
{
    FILE *fp = fopen(QUESTIONS_FILE, "a");
    if (!fp) return 0;
    char cc[64], ques[MAX_Q];
    strncpy(cc, q->course_code, sizeof(cc)-1); cc[sizeof(cc)-1]=0;
    strncpy(ques, q->question, sizeof(ques)-1); ques[sizeof(ques)-1]=0;
    sanitize_field(cc); sanitize_field(ques);
    fprintf(fp, "%s|%s|%c\n", cc, ques, (char)toupper(q->answer));
    fclose(fp);
    return 1;
}

/* load questions into array, return count */
int load_questions(MCQ questions[], int max)
{
    FILE *fp = fopen(QUESTIONS_FILE, "r");
    if (!fp) return 0;
    char line[MAX_LINE];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < max) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        strncpy(questions[cnt].course_code, line, sizeof(questions[cnt].course_code)-1);
        questions[cnt].course_code[sizeof(questions[cnt].course_code)-1]=0;
        strncpy(questions[cnt].question, p1 + 1, sizeof(questions[cnt].question)-1);
        questions[cnt].question[sizeof(questions[cnt].question)-1]=0;
        char ans = p2 + 1 && p2[1] ? p2[1] : p2[0+1]; /* safe pickup */
        if (p2[1] == '\0') ans = '\0';
        questions[cnt].answer = toupper( (p2 + 1 && p2[1]) ? p2[1] : p2[0+1] );
        /* safer: find first non-space char after p2 */
        char *after = p2 + 1;
        while (*after == ' ' && *after) after++;
        questions[cnt].answer = *after ? (char)toupper(*after) : ' ';
        cnt++;
    }
    fclose(fp);
    return cnt;
}

/* overwrite questions file */
int save_questions(MCQ questions[], int count)
{
    FILE *fp = fopen(QUESTIONS_FILE, "w");
    if (!fp) return 0;
    for (int i = 0; i < count; i++) {
        char cc[64], ques[MAX_Q];
        strncpy(cc, questions[i].course_code, sizeof(cc)-1); cc[sizeof(cc)-1]=0;
        strncpy(ques, questions[i].question, sizeof(ques)-1); ques[sizeof(ques)-1]=0;
        sanitize_field(cc); sanitize_field(ques);
        fprintf(fp, "%s|%s|%c\n", cc, ques, toupper(questions[i].answer));
    }
    fclose(fp);
    return 1;
}

/* ---------- Media helpers (video/pdf/web) ---------- */

/* generic add media line to filename */
int add_media(const char *filename, const MediaLink *m)
{
    FILE *fp = fopen(filename, "a");
    if (!fp) return 0;
    char cc[64], title[256], url[400];
    strncpy(cc, m->course_code, sizeof(cc)-1); cc[sizeof(cc)-1]=0;
    strncpy(title, m->title, sizeof(title)-1); title[sizeof(title)-1]=0;
    strncpy(url, m->url, sizeof(url)-1); url[sizeof(url)-1]=0;
    sanitize_field(cc); sanitize_field(title); sanitize_field(url);
    fprintf(fp, "%s|%s|%s\n", cc, title, url);
    fclose(fp);
    return 1;
}

/* list media items from filename into list[], return count */
int list_media(const char *filename, MediaLink list[], int max)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char line[MAX_LINE];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < max) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        strncpy(list[cnt].course_code, line, sizeof(list[cnt].course_code)-1);
        list[cnt].course_code[sizeof(list[cnt].course_code)-1] = 0;
        strncpy(list[cnt].title, p1 + 1, sizeof(list[cnt].title)-1);
        list[cnt].title[sizeof(list[cnt].title)-1] = 0;
        strncpy(list[cnt].url, p2 + 1, sizeof(list[cnt].url)-1);
        list[cnt].url[sizeof(list[cnt].url)-1] = 0;
        cnt++;
    }
    fclose(fp);
    return cnt;
}

/* ---------- Results helpers ---------- */

/* append result to RESULTS_FILE */
int save_result(const ExamResult *r)
{
    FILE *fp = fopen(RESULTS_FILE, "a");
    if (!fp) return 0;
    char ts[64];
    if (r->timestamp[0]) strncpy(ts, r->timestamp, sizeof(ts)-1);
    else { get_timestamp(ts, sizeof(ts)); }
    sanitize_field(ts);
    fprintf(fp, "%s|%s|%d|%s\n", r->student_id, r->course_code, r->score, ts);
    fclose(fp);
    return 1;
}

/* load results into array, return count */
int load_results(ExamResult results[], int max)
{
    FILE *fp = fopen(RESULTS_FILE, "r");
    if (!fp) return 0;
    char line[MAX_LINE];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < max) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *p3 = strchr(p2 + 1, '|');
        /* p3 optional (timestamp) */
        strncpy(results[cnt].student_id, line, sizeof(results[cnt].student_id)-1);
        results[cnt].student_id[sizeof(results[cnt].student_id)-1]=0;
        strncpy(results[cnt].course_code, p1 + 1, sizeof(results[cnt].course_code)-1);
        results[cnt].course_code[sizeof(results[cnt].course_code)-1]=0;
        results[cnt].score = atoi(p2 + 1);
        if (p3) strncpy(results[cnt].timestamp, p3 + 1, sizeof(results[cnt].timestamp)-1);
        else results[cnt].timestamp[0] = '\0';
        cnt++;
    }
    fclose(fp);
    return cnt;
}

/* ---------- Counseling helpers ---------- */

/* save counseling message (student->teacher) */
int save_counsel(const CounselMsg *c)
{
    FILE *fp = fopen(COUNSEL_FILE, "a");
    if (!fp) return 0;
    char sid[MAX_ID], tid[MAX_ID], msg[512], ts[64];
    strncpy(sid, c->student_id, sizeof(sid)-1); sid[sizeof(sid)-1]=0;
    strncpy(tid, c->teacher_id, sizeof(tid)-1); tid[sizeof(tid)-1]=0;
    strncpy(msg, c->message, sizeof(msg)-1); msg[sizeof(msg)-1]=0;
    strncpy(ts, c->time_sent, sizeof(ts)-1); ts[sizeof(ts)-1]=0;
    sanitize_field(sid); sanitize_field(tid); sanitize_field(msg); sanitize_field(ts);
    fprintf(fp, "%s|%s|%s|%s\n", sid, tid, msg, ts);
    fclose(fp);
    return 1;
}

/* load counseling messages for a teacher_id (teacher_id or "any"), return count */
int load_counsel_for_teacher(const char *teacher_id, CounselMsg out[], int max)
{
    FILE *fp = fopen(COUNSEL_FILE, "r");
    if (!fp) return 0;
    char line[MAX_LINE];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < max) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *p3 = strchr(p2 + 1, '|');
        if (!p3) continue;
        *p3 = '\0';
        char *sid = line;
        char *tid = p1 + 1;
        char *msg = p2 + 1;
        char *time_sent = p3 + 1;
        if (strcmp(teacher_id, "any") == 0 || strcmp(teacher_id, tid) == 0) {
            strncpy(out[cnt].student_id, sid, sizeof(out[cnt].student_id)-1);
            out[cnt].student_id[sizeof(out[cnt].student_id)-1]=0;
            strncpy(out[cnt].teacher_id, tid, sizeof(out[cnt].teacher_id)-1);
            out[cnt].teacher_id[sizeof(out[cnt].teacher_id)-1]=0;
            strncpy(out[cnt].message, msg, sizeof(out[cnt].message)-1);
            out[cnt].message[sizeof(out[cnt].message)-1]=0;
            strncpy(out[cnt].time_sent, time_sent, sizeof(out[cnt].time_sent)-1);
            out[cnt].time_sent[sizeof(out[cnt].time_sent)-1]=0;
            cnt++;
        }
    }
    fclose(fp);
    return cnt;
}

/* -------------------------
   End of Part-2
   -------------------------*/



   /* -------------------------
   Part-3: Student - register, login, student menu & student-side features
   (paste this AFTER Part-2 in exambunny.c)
   -------------------------*/

/* current logged-in student id (used after successful login) */
char CURRENT_USER_ID[MAX_ID] = "";

/* -------------------------
   Student registration
   File format for USER_FILE: student_id|name|password|role
   -------------------------*/
void register_user(const char *role)
{
    User u;
    printf("\n--- Student Registration ---\n");
    printf("Enter Student ID: ");
    if (!fgets(u.student_id, sizeof(u.student_id), stdin)) return;
    u.student_id[strcspn(u.student_id, "\n")] = '\0';

    printf("Enter Name: ");
    if (!fgets(u.name, sizeof(u.name), stdin)) return;
    u.name[strcspn(u.name, "\n")] = '\0';

    printf("Enter Password: ");
    if (!fgets(u.password, sizeof(u.password), stdin)) return;
    u.password[strcspn(u.password, "\n")] = '\0';

    strncpy(u.role, role, sizeof(u.role)-1);
    u.role[sizeof(u.role)-1] = '\0';

    /* sanitize */
    sanitize_field(u.student_id);
    sanitize_field(u.name);
    sanitize_field(u.password);
    sanitize_field(u.role);

    /* append to file */
    FILE *fp = fopen(USER_FILE, "a");
    if (!fp) {
        perror("Unable to open user file for registration");
        return;
    }
    fprintf(fp, "%s|%s|%s|%s\n", u.student_id, u.name, u.password, u.role);
    fclose(fp);

    printf("\nRegistration successful for %s (ID: %s)\n", u.name, u.student_id);
    pause_and_continue();
}

/* -------------------------
   Student login
   Reads USER_FILE and matches id/password and role.
   On success sets CURRENT_USER_ID and returns 1.
   -------------------------*/
int login_user(const char *role)
{
    char id[MAX_ID], pass[MAX_PASS];
    printf("\n--- Student Login ---\n");
    printf("Enter Student ID: ");
    if (!fgets(id, sizeof(id), stdin)) return 0;
    id[strcspn(id, "\n")] = '\0';

    printf("Enter Password: ");
    if (!fgets(pass, sizeof(pass), stdin)) return 0;
    pass[strcspn(pass, "\n")] = '\0';

    /* open user file */
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        perror("Unable to open user file for login");
        return 0;
    }

    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        /* parse id|name|password|role */
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *file_id = line;
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *file_name = p1 + 1;
        char *p3 = strchr(p2 + 1, '|');
        if (!p3) continue;
        *p3 = '\0';
        char *file_pass = p2 + 1;
        char *file_role = p3 + 1;

        if (strcmp(file_id, id) == 0 && strcmp(file_pass, pass) == 0 && strcmp(file_role, role) == 0) {
            found = 1;
            strncpy(CURRENT_USER_ID, file_id, sizeof(CURRENT_USER_ID)-1);
            CURRENT_USER_ID[sizeof(CURRENT_USER_ID)-1] = '\0';
            printf("\nLogin successful. Welcome, %s!\n", file_name);
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("\nLogin failed. Check ID/password/role.\n");
        pause_and_continue();
        return 0;
    }
    pause_and_continue();
    return 1;
}

/* -------------------------
   Student Menu
   student_menu expects student_id (but will also read CURRENT_USER_ID)
   -------------------------*/
void student_menu(const char *student_id_param)
{
    char student_id_local[MAX_ID];
    if (student_id_param && student_id_param[0]) {
        strncpy(student_id_local, student_id_param, sizeof(student_id_local)-1);
        student_id_local[sizeof(student_id_local)-1] = '\0';
    } else {
        strncpy(student_id_local, CURRENT_USER_ID, sizeof(student_id_local)-1);
        student_id_local[sizeof(student_id_local)-1] = '\0';
    }

    int choice;
    while (1) {
        system("clear || cls"); /* try to clear terminal, cross-platform */
        print_logo();
        printf("Student Menu (ID: %s)\n", student_id_local);
        printf("1. View Courses\n");
        printf("2. Exam Instructions\n");
        printf("3. Take Exam\n");
        printf("4. View Video Lectures\n");
        printf("5. View PDF Files\n");
        printf("6. View Web Pages\n");
        printf("7. View My Results\n");
        printf("8. Send Counseling Message\n");
        printf("9. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            printf("Invalid input. Try again.\n");
            pause_and_continue();
            continue;
        }
        clear_stdin();

        switch (choice) {
            case 1:
                student_view_courses();
                break;
            case 2:
                exam_rules();
                pause_and_continue();
                break;
            case 3:
                student_exam(student_id_local);
                break;
            case 4:
                /* videos */
                {
                    MediaLink list[200];
                    int c = list_media(VIDEO_FILE, list, 200);
                    if (c == 0) {
                        printf("\nNo video lectures found.\n");
                    } else {
                        printf("\nVideo Lectures:\n");
                        for (int i = 0; i < c; i++) {
                            printf("%d) [%s] %s\n    URL: %s\n", i+1, list[i].course_code, list[i].title, list[i].url);
                        }
                    }
                    pause_and_continue();
                }
                break;
            case 5:
                /* pdfs */
                {
                    MediaLink list[200];
                    int c = list_media(PDF_FILE, list, 200);
                    if (c == 0) {
                        printf("\nNo PDF files found.\n");
                    } else {
                        printf("\nPDF Files:\n");
                        for (int i = 0; i < c; i++) {
                            printf("%d) [%s] %s\n    URL: %s\n", i+1, list[i].course_code, list[i].title, list[i].url);
                        }
                    }
                    pause_and_continue();
                }
                break;
            case 6:
                /* web pages */
                {
                    MediaLink list[200];
                    int c = list_media(WEB_FILE, list, 200);
                    if (c == 0) {
                        printf("\nNo web pages found.\n");
                    } else {
                        printf("\nWeb Pages:\n");
                        for (int i = 0; i < c; i++) {
                            printf("%d) [%s] %s\n    URL: %s\n", i+1, list[i].course_code, list[i].title, list[i].url);
                        }
                    }
                    pause_and_continue();
                }
                break;
            case 7:
                student_view_my_results(student_id_local);
                pause_and_continue();
                break;
            case 8:
                student_send_counsel(student_id_local);
                pause_and_continue();
                break;
            case 9:
                printf("Logging out...\n");
                strncpy(CURRENT_USER_ID, "", sizeof(CURRENT_USER_ID));
                pause_and_continue();
                return;
            default:
                printf("Invalid choice. Try again.\n");
                pause_and_continue();
        }
    }
}

/* -------------------------
   Student helper: view courses
   -------------------------*/
void student_view_courses()
{
    Course courses[200];
    int c = load_courses(courses, 200);
    if (c == 0) {
        printf("\nNo courses available.\n");
    } else {
        printf("\nAvailable Courses:\n");
        for (int i = 0; i < c; i++) {
            printf("%d) %s  [%s]\n", i+1, courses[i].course_name, courses[i].course_code);
        }
    }
}

/* -------------------------
   Student: view my results (filter by student_id)
   -------------------------*/
void student_view_my_results(const char *student_id)
{
    ExamResult results[500];
    int c = load_results(results, 500);
    int found = 0;
    printf("\nMy Exam Results (ID: %s)\n", student_id);
    for (int i = 0; i < c; i++) {
        if (strcmp(results[i].student_id, student_id) == 0) {
            printf("Course: %s  | Score: %d  | Time: %s\n", results[i].course_code, results[i].score,
                   results[i].timestamp[0] ? results[i].timestamp : "N/A");
            found++;
        }
    }
    if (!found) {
        printf("No results found for your ID.\n");
    }
}

/* -------------------------
   Student: send counseling message
   Prompts student for teacher_id (or 'any') and message, saves to COUNSEL_FILE
   -------------------------*/
void student_send_counsel(const char *student_id)
{
    CounselMsg c;
    char tmp[600];

    strncpy(c.student_id, student_id, sizeof(c.student_id)-1);
    c.student_id[sizeof(c.student_id)-1] = '\0';

    printf("\nSend Counseling Message\n");
    printf("Enter Teacher ID to send to (or type 'any' to send to all teachers): ");
    if (!fgets(c.teacher_id, sizeof(c.teacher_id), stdin)) return;
    c.teacher_id[strcspn(c.teacher_id, "\n")] = '\0';
    if (strlen(c.teacher_id) == 0) strncpy(c.teacher_id, "any", sizeof(c.teacher_id)-1);

    printf("Enter your message (single line): ");
    if (!fgets(tmp, sizeof(tmp), stdin)) return;
    tmp[strcspn(tmp, "\n")] = '\0';
    strncpy(c.message, tmp, sizeof(c.message)-1);
    c.message[sizeof(c.message)-1] = '\0';

    get_timestamp(c.time_sent, sizeof(c.time_sent));
    if (save_counsel(&c)) {
        printf("\nMessage sent successfully.\n");
    } else {
        printf("\nFailed to send message.\n");
    }
}

/* -------------------------
   End of Part-3 (Student)
   -------------------------*/


   /* -------------------------
   Part-4: Admin login & Admin menu (view-only, no registration)
   File format for ADMIN_FILE: admin_id|name|password
   -------------------------*/

/* Admin login */
int login_admin()
{
    char id[MAX_ID], pass[MAX_PASS];
    printf("\n--- Admin Login ---\n");
    printf("Enter Admin ID: ");
    if (!fgets(id, sizeof(id), stdin)) return 0;
    id[strcspn(id, "\n")] = '\0';

    printf("Enter Password: ");
    if (!fgets(pass, sizeof(pass), stdin)) return 0;
    pass[strcspn(pass, "\n")] = '\0';

    FILE *fp = fopen(ADMIN_FILE, "r");
    if (!fp) {
        perror("Unable to open admin file for login");
        return 0;
    }
    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *file_id = line;
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *file_name = p1 + 1;
        char *file_pass = p2 + 1;
        if (strcmp(file_id, id) == 0 && strcmp(file_pass, pass) == 0) {
            found = 1;
            printf("\nLogin successful. Welcome, %s!\n", file_name);
            break;
        }
    }
    fclose(fp);
    if (!found) {
        printf("\nLogin failed. Check ID/password.\n");
        pause_and_continue();
        return 0;
    }
    pause_and_continue();
    return 1;
}

/* View all teachers */
void admin_view_all_teachers()
{
    FILE *fp = fopen(TEACHER_FILE, "r");
    if (!fp) {
        printf("\nNo teachers found.\n");
        return;
    }
    char line[MAX_LINE];
    int i = 1;
    printf("\n--- Teachers List ---\n");
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *id = line;
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *name = p1 + 1;
        /* password field skip */
        printf("%d) %s  [%s]\n", i++, name, id);
    }
    fclose(fp);
}

/* View all students */
void admin_view_all_students()
{
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        printf("\nNo students found.\n");
        return;
    }
    char line[MAX_LINE];
    int i = 1;
    printf("\n--- Students List ---\n");
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *id = line;
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *name = p1 + 1;
        printf("%d) %s  [%s]\n", i++, name, id);
    }
    fclose(fp);
}

/* View all results */
void admin_view_all_results()
{
    ExamResult results[500];
    int c = load_results(results, 500);
    if (c == 0) {
        printf("\nNo results found.\n");
        return;
    }
    printf("\n--- All Exam Results ---\n");
    for (int i = 0; i < c; i++) {
        printf("%d) Student: %s | Course: %s | Score: %d | Time: %s\n",
               i+1, results[i].student_id, results[i].course_code,
               results[i].score,
               results[i].timestamp[0] ? results[i].timestamp : "N/A");
    }
}

/* View courses */
void admin_view_courses()
{
    student_view_courses(); /* reuse student view */
    pause_and_continue();
}

/* Update course name */
void admin_update_course()
{
    Course courses[200];
    int c = load_courses(courses, 200);
    if (c == 0) {
        printf("\nNo courses to update.\n");
        pause_and_continue();
        return;
    }
    printf("\n--- Update Course ---\n");
    for (int i = 0; i < c; i++) {
        printf("%d) %s [%s]\n", i+1, courses[i].course_name, courses[i].course_code);
    }
    printf("Enter course number to update: ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > c) {
        clear_stdin();
        printf("Invalid choice.\n");
        pause_and_continue();
        return;
    }
    clear_stdin();
    printf("Enter new course name: ");
    char newname[150];
    if (!fgets(newname, sizeof(newname), stdin)) return;
    newname[strcspn(newname, "\n")] = '\0';
    strncpy(courses[choice-1].course_name, newname, sizeof(courses[choice-1].course_name)-1);
    courses[choice-1].course_name[sizeof(courses[choice-1].course_name)-1] = '\0';
    if (save_courses(courses, c)) {
        printf("\nCourse updated successfully.\n");
    } else {
        printf("\nFailed to update course.\n");
    }
    pause_and_continue();
}

/* Admin menu */
void admin_menu()
{
    int choice;
    while (1) {
        system("clear || cls");
        print_logo();
        printf("Admin Menu\n");
        printf("1. View All Teachers\n");
        printf("2. View All Students\n");
        printf("3. View All Results\n");
        printf("4. View Courses\n");
        printf("5. Update Course\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            printf("Invalid input.\n");
            pause_and_continue();
            continue;
        }
        clear_stdin();
        switch (choice) {
            case 1:
                admin_view_all_teachers();
                pause_and_continue();
                break;
            case 2:
                admin_view_all_students();
                pause_and_continue();
                break;
            case 3:
                admin_view_all_results();
                pause_and_continue();
                break;
            case 4:
                admin_view_courses();
                break;
            case 5:
                admin_update_course();
                break;
            case 6:
                printf("Logging out...\n");
                pause_and_continue();
                return;
            default:
                printf("Invalid choice.\n");
                pause_and_continue();
        }
    }
}

/* -------------------------
   End of Part-4 (Admin)
   -------------------------*/


   /* -------------------------
   Part-5: Teacher registration/login (with verification code) + Teacher menu skeleton
   File format for TEACHER_FILE: teacher_id|name|password|verification_code
   -------------------------*/

/* Teacher registration
   এই ফাংশন admin বা নিজেই চালাতে পারে (তুমি main()-এ control দেবে কিভাবে হবে)
*/
void register_teacher()
{
    Teacher t;
    printf("\n--- Teacher Registration ---\n");
    printf("Enter Teacher ID: ");
    if (!fgets(t.teacher_id, sizeof(t.teacher_id), stdin)) return;
    t.teacher_id[strcspn(t.teacher_id, "\n")] = '\0';

    printf("Enter Name: ");
    if (!fgets(t.name, sizeof(t.name), stdin)) return;
    t.name[strcspn(t.name, "\n")] = '\0';

    printf("Enter Password: ");
    if (!fgets(t.password, sizeof(t.password), stdin)) return;
    t.password[strcspn(t.password, "\n")] = '\0';

    printf("Enter Special Verification Code: ");
    if (!fgets(t.vcode, sizeof(t.vcode), stdin)) return;
    t.vcode[strcspn(t.vcode, "\n")] = '\0';

    sanitize_field(t.teacher_id);
    sanitize_field(t.name);
    sanitize_field(t.password);
    sanitize_field(t.vcode);

    FILE *fp = fopen(TEACHER_FILE, "a");
    if (!fp) {
        perror("Unable to open teacher file");
        return;
    }
    fprintf(fp, "%s|%s|%s|%s\n", t.teacher_id, t.name, t.password, t.vcode);
    fclose(fp);

    printf("\nTeacher registration successful.\n");
    pause_and_continue();
}

/* Teacher login */
int login_teacher(char *out_teacher_id)
{
    char id[MAX_ID], pass[MAX_PASS], vcode[VERIF_CODE_LEN];
    printf("\n--- Teacher Login ---\n");
    printf("Enter Teacher ID: ");
    if (!fgets(id, sizeof(id), stdin)) return 0;
    id[strcspn(id, "\n")] = '\0';

    printf("Enter Password: ");
    if (!fgets(pass, sizeof(pass), stdin)) return 0;
    pass[strcspn(pass, "\n")] = '\0';

    printf("Enter Special Verification Code: ");
    if (!fgets(vcode, sizeof(vcode), stdin)) return 0;
    vcode[strcspn(vcode, "\n")] = '\0';

    FILE *fp = fopen(TEACHER_FILE, "r");
    if (!fp) {
        perror("Unable to open teacher file");
        return 0;
    }
    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *p1 = strchr(line, '|');
        if (!p1) continue;
        *p1 = '\0';
        char *file_id = line;
        char *p2 = strchr(p1 + 1, '|');
        if (!p2) continue;
        *p2 = '\0';
        char *file_name = p1 + 1;
        char *p3 = strchr(p2 + 1, '|');
        if (!p3) continue;
        *p3 = '\0';
        char *file_pass = p2 + 1;
        char *file_vcode = p3 + 1;
        if (strcmp(file_id, id) == 0 &&
            strcmp(file_pass, pass) == 0 &&
            strcmp(file_vcode, vcode) == 0) {
            found = 1;
            strncpy(out_teacher_id, file_id, MAX_ID-1);
            out_teacher_id[MAX_ID-1] = '\0';
            printf("\nLogin successful. Welcome, %s!\n", file_name);
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("\nLogin failed. Check ID/password/verification code.\n");
        pause_and_continue();
        return 0;
    }
    pause_and_continue();
    return 1;
}

/* Teacher menu skeleton
   আসল কাজগুলো (add/update/delete question, add media etc.) পরে আলাদা Part-7+ এ করব
*/
void teacher_menu(const char *teacher_id)
{
    int choice;
    while (1) {
        system("clear || cls");
        print_logo();
        printf("Teacher Menu (ID: %s)\n", teacher_id);
        printf("1. View Courses\n");
        printf("2. Add Question\n");
        printf("3. Update Question\n");
        printf("4. Delete Question\n");
        printf("5. Add Video Lecture\n");
        printf("6. Add PDF\n");
        printf("7. Add Web Page\n");
        printf("8. View My Course Results\n");
        printf("9. View Counseling Messages\n");
        printf("10. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            printf("Invalid input.\n");
            pause_and_continue();
            continue;
        }
        clear_stdin();

        switch (choice) {
            case 1:
                teacher_view_courses(); /* will implement later (reuse student_view_courses) */
                pause_and_continue();
                break;
            case 2:
                teacher_add_question(teacher_id);
                pause_and_continue();
                break;
            case 3:
                teacher_update_question(teacher_id);
                pause_and_continue();
                break;
            case 4:
                teacher_delete_question(teacher_id);
                pause_and_continue();
                break;
            case 5:
                teacher_add_video(teacher_id);
                pause_and_continue();
                break;
            case 6:
                teacher_add_pdf(teacher_id);
                pause_and_continue();
                break;
            case 7:
                teacher_add_webpage(teacher_id);
                pause_and_continue();
                break;
            case 8:
                teacher_view_results_of_their_courses(teacher_id);
                pause_and_continue();
                break;
            case 9:
                teacher_view_counseling_messages(teacher_id);
                pause_and_continue();
                break;
            case 10:
                printf("Logging out...\n");
                pause_and_continue();
                return;
            default:
                printf("Invalid choice.\n");
                pause_and_continue();
        }
    }
}



/* -------------------------
   Part-6: Course Management
   -------------------------*/

/* Teacher view courses (same as student_view_courses but labeled differently) */
void teacher_view_courses()
{
    Course courses[200];
    int c = load_courses(courses, 200);
    if (c == 0) {
        printf("\nNo courses available.\n");
    } else {
        printf("\nAvailable Courses:\n");
        for (int i = 0; i < c; i++) {
            printf("%d) %s  [%s]\n", i+1, courses[i].course_name, courses[i].course_code);
        }
    }
}

/* Add course (for Teacher or Admin) */
void add_course_interactive()
{
    Course c;
    printf("\n--- Add New Course ---\n");
    printf("Enter course code: ");
    if (!fgets(c.course_code, sizeof(c.course_code), stdin)) return;
    c.course_code[strcspn(c.course_code, "\n")] = '\0';

    printf("Enter course name: ");
    if (!fgets(c.course_name, sizeof(c.course_name), stdin)) return;
    c.course_name[strcspn(c.course_name, "\n")] = '\0';

    if (strlen(c.course_code) == 0 || strlen(c.course_name) == 0) {
        printf("Course code/name cannot be empty.\n");
        pause_and_continue();
        return;
    }

    if (add_course_text(&c)) {
        printf("\nCourse added successfully.\n");
    } else {
        printf("\nFailed to add course.\n");
    }
    pause_and_continue();
}

/* Optional: Delete course (Admin only if you want this feature) */
void admin_delete_course()
{
    Course courses[200];
    int c = load_courses(courses, 200);
    if (c == 0) {
        printf("\nNo courses to delete.\n");
        pause_and_continue();
        return;
    }
    printf("\n--- Delete Course ---\n");
    for (int i = 0; i < c; i++) {
        printf("%d) %s [%s]\n", i+1, courses[i].course_name, courses[i].course_code);
    }
    printf("Enter course number to delete: ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > c) {
        clear_stdin();
        printf("Invalid choice.\n");
        pause_and_continue();
        return;
    }
    clear_stdin();

    for (int i = choice - 1; i < c - 1; i++) {
        courses[i] = courses[i+1];
    }
    c--;

    if (save_courses(courses, c)) {
        printf("\nCourse deleted successfully.\n");
    } else {
        printf("\nFailed to delete course.\n");
    }
    pause_and_continue();
}

/* Updated Admin menu to include Add/Delete Course */
void admin_menu()
{
    int choice;
    while (1) {
        system("clear || cls");
        print_logo();
        printf("Admin Menu\n");
        printf("1. View All Teachers\n");
        printf("2. View All Students\n");
        printf("3. View All Results\n");
        printf("4. View Courses\n");
        printf("5. Update Course\n");
        printf("6. Add Course\n");
        printf("7. Delete Course\n");
        printf("8. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            printf("Invalid input.\n");
            pause_and_continue();
            continue;
        }
        clear_stdin();
        switch (choice) {
            case 1:
                admin_view_all_teachers();
                pause_and_continue();
                break;
            case 2:
                admin_view_all_students();
                pause_and_continue();
                break;
            case 3:
                admin_view_all_results();
                pause_and_continue();
                break;
            case 4:
                admin_view_courses();
                break;
            case 5:
                admin_update_course();
                break;
            case 6:
                add_course_interactive();
                break;
            case 7:
                admin_delete_course();
                break;
            case 8:
                printf("Logging out...\n");
                pause_and_continue();
                return;
            default:
                printf("Invalid choice.\n");
                pause_and_continue();
        }
    }
}

/* Updated Teacher menu to include Add Course if needed */
void teacher_menu(const char *teacher_id)
{
    int choice;
    while (1) {
        system("clear || cls");
        print_logo();
        printf("Teacher Menu (ID: %s)\n", teacher_id);
        printf("1. View Courses\n");
        printf("2. Add Course\n");
        printf("3. Add Question\n");
        printf("4. Update Question\n");
        printf("5. Delete Question\n");
        printf("6. Add Video Lecture\n");
        printf("7. Add PDF\n");
        printf("8. Add Web Page\n");
        printf("9. View My Course Results\n");
        printf("10. View Counseling Messages\n");
        printf("11. Logout\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            printf("Invalid input.\n");
            pause_and_continue();
            continue;
        }
        clear_stdin();

        switch (choice) {
            case 1:
                teacher_view_courses();
                pause_and_continue();
                break;
            case 2:
                add_course_interactive();
                break;
            case 3:
                teacher_add_question(teacher_id);
                pause_and_continue();
                break;
            case 4:
                teacher_update_question(teacher_id);
                pause_and_continue();
                break;
            case 5:
                teacher_delete_question(teacher_id);
                pause_and_continue();
                break;
            case 6:
                teacher_add_video(teacher_id);
                pause_and_continue();
                break;
            case 7:
                teacher_add_pdf(teacher_id);
                pause_and_continue();
                break;
            case 8:
                teacher_add_webpage(teacher_id);
                pause_and_continue();
                break;
            case 9:
                teacher_view_results_of_their_courses(teacher_id);
                pause_and_continue();
                break;
            case 10:
                teacher_view_counseling_messages(teacher_id);
                pause_and_continue();
                break;
            case 11:
                printf("Logging out...\n");
                pause_and_continue();
                return;
            default:
                printf("Invalid choice.\n");
                pause_and_continue();
        }
    }
}




/* -------------------------
   Part-7: Question Management (Teacher only)
   -------------------------*/

/* Teacher: Add Question */
void teacher_add_question(const char *teacher_id)
{
    MCQ q;
    printf("\n--- Add Question ---\n");
    printf("Enter course code: ");
    if (!fgets(q.course_code, sizeof(q.course_code), stdin)) return;
    q.course_code[strcspn(q.course_code, "\n")] = '\0';

    printf("Enter question: ");
    if (!fgets(q.question, sizeof(q.question), stdin)) return;
    q.question[strcspn(q.question, "\n")] = '\0';

    printf("Enter correct answer (A/B): ");
    char ans;
    scanf(" %c", &ans);
    clear_stdin();
    q.answer = toupper(ans);
    if (q.answer != 'A' && q.answer != 'B') {
        printf("Invalid answer. Only A or B allowed.\n");
        pause_and_continue();
        return;
    }

    if (add_question_text(&q)) {
        printf("\nQuestion added successfully.\n");
    } else {
        printf("\nFailed to add question.\n");
    }
}

/* Teacher: View Questions by course */
void view_questions_by_course(const char *course_code, int show_answers)
{
    MCQ questions[500];
    int c = load_questions(questions, 500);
    int found = 0;
    for (int i = 0; i < c; i++) {
        if (strcmp(questions[i].course_code, course_code) == 0) {
            printf("%d) %s", i+1, questions[i].question);
            if (show_answers) printf("  [Ans: %c]", questions[i].answer);
            printf("\n");
            found++;
        }
    }
    if (!found) {
        printf("No questions found for course %s.\n", course_code);
    }
}

/* Teacher: Update Question */
void teacher_update_question(const char *teacher_id)
{
    char code[50];
    printf("\nEnter course code to update question: ");
    if (!fgets(code, sizeof(code), stdin)) return;
    code[strcspn(code, "\n")] = '\0';

    MCQ questions[500];
    int c = load_questions(questions, 500);
    int indexes[500], cnt = 0;

    for (int i = 0; i < c; i++) {
        if (strcmp(questions[i].course_code, code) == 0) {
            printf("%d) %s [Ans: %c]\n", cnt+1, questions[i].question, questions[i].answer);
            indexes[cnt++] = i;
        }
    }
    if (cnt == 0) {
        printf("No questions found for %s.\n", code);
        pause_and_continue();
        return;
    }

    printf("Enter question number to update: ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > cnt) {
        clear_stdin();
        printf("Invalid choice.\n");
        pause_and_continue();
        return;
    }
    clear_stdin();

    int idx = indexes[choice-1];
    printf("Enter new question text: ");
    if (!fgets(questions[idx].question, sizeof(questions[idx].question), stdin)) return;
    questions[idx].question[strcspn(questions[idx].question, "\n")] = '\0';

    printf("Enter new answer (A/B): ");
    char ans;
    scanf(" %c", &ans);
    clear_stdin();
    questions[idx].answer = toupper(ans);
    if (questions[idx].answer != 'A' && questions[idx].answer != 'B') {
        printf("Invalid answer.\n");
        pause_and_continue();
        return;
    }

    if (save_questions(questions, c)) {
        printf("\nQuestion updated successfully.\n");
    } else {
        printf("\nFailed to update question.\n");
    }
    pause_and_continue();
}

/* Teacher: Delete Question */
void teacher_delete_question(const char *teacher_id)
{
    char code[50];
    printf("\nEnter course code to delete question: ");
    if (!fgets(code, sizeof(code), stdin)) return;
    code[strcspn(code, "\n")] = '\0';

    MCQ questions[500];
    int c = load_questions(questions, 500);
    int indexes[500], cnt = 0;

    for (int i = 0; i < c; i++) {
        if (strcmp(questions[i].course_code, code) == 0) {
            printf("%d) %s [Ans: %c]\n", cnt+1, questions[i].question, questions[i].answer);
            indexes[cnt++] = i;
        }
    }
    if (cnt == 0) {
        printf("No questions found for %s.\n", code);
        pause_and_continue();
        return;
    }

    printf("Enter question number to delete: ");
    int choice;
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > cnt) {
        clear_stdin();
        printf("Invalid choice.\n");
        pause_and_continue();
        return;
    }
    clear_stdin();

    int del_index = indexes[choice-1];
    for (int i = del_index; i < c-1; i++) {
        questions[i] = questions[i+1];
    }
    c--;

    if (save_questions(questions, c)) {
        printf("\nQuestion deleted successfully.\n");
    } else {
        printf("\nFailed to delete question.\n");
    }
    pause_and_continue();
}



/* -------------------------
   Part-8: Exam Engine (Student only, Timer support)
   -------------------------*/

/* Helper to get current timestamp string */
void get_timestamp(char *buf, size_t sz) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, sz, "%Y-%m-%d %H:%M:%S", t);
}

/* Take Exam */
void student_take_exam(const char *student_id, const char *student_name)
{
    char course_code[50];
    printf("\nEnter course code to take exam: ");
    if (!fgets(course_code, sizeof(course_code), stdin)) return;
    course_code[strcspn(course_code, "\n")] = '\0';

    MCQ questions[500];
    int qcount = load_questions(questions, 500);
    if (qcount == 0) {
        printf("No questions available.\n");
        pause_and_continue();
        return;
    }

    MCQ filtered[200];
    int count = 0;
    for (int i = 0; i < qcount; i++) {
        if (strcmp(questions[i].course_code, course_code) == 0) {
            filtered[count++] = questions[i];
        }
    }
    if (count == 0) {
        printf("No questions found for this course.\n");
        pause_and_continue();
        return;
    }

    int correct = 0, wrong = 0;
    printf("\nExam starting for course %s...\n", course_code);
    printf("You have 30 seconds per question.\n");
    pause_and_continue();

    for (int i = 0; i < count; i++) {
        system("clear || cls");
        printf("Question %d/%d:\n%s\n", i+1, count, filtered[i].question);

        time_t start_time = time(NULL);
        char ans;
        int answered = 0;
        while (1) {
            if (difftime(time(NULL), start_time) >= 30) {
                printf("\nTime's up! Skipping question.\n");
                ans = '\0'; /* no answer */
                break;
            }
            printf("Enter answer (A/B): ");
            if (scanf(" %c", &ans) == 1) {
                ans = toupper(ans);
                if (ans == 'A' || ans == 'B') {
                    answered = 1;
                    break;
                } else {
                    printf("Invalid option.\n");
                }
            }
            clear_stdin();
        }
        clear_stdin();

        if (answered) {
            if (ans == filtered[i].answer) {
                printf("Correct!\n");
                correct++;
            } else {
                printf("Wrong. Correct answer: %c\n", filtered[i].answer);
                wrong++;
            }
        } else {
            wrong++;
        }
        sleep(1);
    }

    printf("\nExam finished!\n");
    printf("Correct: %d, Wrong: %d, Total: %d\n", correct, wrong, count);

    /* Save result to file */
    char timestamp[30];
    get_timestamp(timestamp, sizeof(timestamp));
    FILE *fp = fopen("results.txt", "a");
    if (!fp) {
        perror("Unable to save result");
    } else {
        fprintf(fp, "%s|%s|%s|%s|%d|%d|%d\n",
                student_id, student_name, course_code, timestamp,
                correct, wrong, count);
        fclose(fp);
    }

    pause_and_continue();
}

/* View student's own results */
void student_view_my_results(const char *student_id)
{
    FILE *fp = fopen("results.txt", "r");
    if (!fp) {
        printf("No results found.\n");
        pause_and_continue();
        return;
    }
    char line[MAX_LINE];
    int found = 0;
    printf("\n--- Your Exam Results ---\n");
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        char sid[50], sname[100], course[50], date[30];
        int correct, wrong, total;
        if (sscanf(line, "%49[^|]|%99[^|]|%49[^|]|%29[^|]|%d|%d|%d",
                   sid, sname, course, date, &correct, &wrong, &total) == 7) {
            if (strcmp(sid, student_id) == 0) {
                printf("Course: %s | Date: %s | Score: %d/%d (Wrong: %d)\n",
                       course, date, correct, total, wrong);
                found++;
            }
        }
    }
    fclose(fp);
    if (!found) printf("No results found for your ID.\n");
    pause_and_continue();
}


/* -------------------------
   Part-9: Media & Counseling Management
   -------------------------*/

/* Teacher add video lecture */
void teacher_add_video(const char *teacher_id)
{
    MediaLink m;
    printf("\n--- Add Video Lecture ---\n");
    printf("Enter course code: ");
    if (!fgets(m.course_code, sizeof(m.course_code), stdin)) return;
    m.course_code[strcspn(m.course_code, "\n")] = '\0';

    printf("Enter video title: ");
    if (!fgets(m.title, sizeof(m.title), stdin)) return;
    m.title[strcspn(m.title, "\n")] = '\0';

    printf("Enter video URL: ");
    if (!fgets(m.url, sizeof(m.url), stdin)) return;
    m.url[strcspn(m.url, "\n")] = '\0';

    if (add_media(VIDEO_FILE, &m))
        printf("\nVideo lecture added successfully.\n");
    else
        printf("\nFailed to add video lecture.\n");

    pause_and_continue();
}

/* Teacher add PDF */
void teacher_add_pdf(const char *teacher_id)
{
    MediaLink m;
    printf("\n--- Add PDF ---\n");
    printf("Enter course code: ");
    if (!fgets(m.course_code, sizeof(m.course_code), stdin)) return;
    m.course_code[strcspn(m.course_code, "\n")] = '\0';

    printf("Enter PDF title: ");
    if (!fgets(m.title, sizeof(m.title), stdin)) return;
    m.title[strcspn(m.title, "\n")] = '\0';

    printf("Enter PDF URL: ");
    if (!fgets(m.url, sizeof(m.url), stdin)) return;
    m.url[strcspn(m.url, "\n")] = '\0';

    if (add_media(PDF_FILE, &m))
        printf("\nPDF added successfully.\n");
    else
        printf("\nFailed to add PDF.\n");

    pause_and_continue();
}

/* Teacher add Web Page */
void teacher_add_webpage(const char *teacher_id)
{
    MediaLink m;
    printf("\n--- Add Web Page ---\n");
    printf("Enter course code: ");
    if (!fgets(m.course_code, sizeof(m.course_code), stdin)) return;
    m.course_code[strcspn(m.course_code, "\n")] = '\0';

    printf("Enter page title: ");
    if (!fgets(m.title, sizeof(m.title), stdin)) return;
    m.title[strcspn(m.title, "\n")] = '\0';

    printf("Enter page URL: ");
    if (!fgets(m.url, sizeof(m.url), stdin)) return;
    m.url[strcspn(m.url, "\n")] = '\0';

    if (add_media(WEB_FILE, &m))
        printf("\nWeb page added successfully.\n");
    else
        printf("\nFailed to add web page.\n");

    pause_and_continue();
}

/* Teacher view counseling messages */
void teacher_view_counseling_messages(const char *teacher_id)
{
    CounselMsg msgs[200];
    int count = load_counsel_for_teacher(teacher_id, msgs, 200);
    if (count == 0)
    {
        printf("\nNo counseling messages found.\n");
    }
    else
    {
        printf("\n--- Counseling Messages ---\n");
        for (int i = 0; i < count; i++)
        {
            printf("%d) From Student [%s] at %s\n   Message: %s\n",
                   i+1, msgs[i].student_id, msgs[i].time_sent, msgs[i].message);
        }
    }
    pause_and_continue();
}



/* -------------------------
   MAIN FUNCTION (Final, polished)
   -------------------------*/
int main()
{
    int choice;
    char tid[MAX_ID];  // teacher id holder

    while (1)
    {
        system("clear || cls"); // clear terminal
        print_logo();

        printf("========= Main Menu =========\n");
        printf("1. Admin Login\n");
        printf("2. Teacher Login\n");
        printf("3. Student Login\n");
        printf("4. Student Registration\n");
        printf("5. Teacher Registration\n");
        printf("6. Exit\n");
        printf("=============================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            clear_stdin();
            printf("\nInvalid input! Please enter a number.\n");
            pause_and_continue();
            continue;
        }
        clear_stdin();

        switch (choice)
        {
            case 1: // Admin Login
                if (login_admin())
                {
                    printf("\nWelcome, Admin! Loading your dashboard...\n");
                    pause_and_continue();
                    admin_menu();
                }
                break;

            case 2: // Teacher Login
                if (login_teacher(tid))
                {
                    printf("\nWelcome Teacher [%s]! Loading your dashboard...\n", tid);
                    pause_and_continue();
                    teacher_menu(tid);
                }
                break;

            case 3: // Student Login
                if (login_user("student"))
                {
                    printf("\nWelcome Student [%s]! Loading your portal...\n", CURRENT_USER_ID);
                    pause_and_continue();
                    student_menu(CURRENT_USER_ID);
                }
                break;

            case 4: // Student Registration
                register_user("student");
                break;

            case 5: // Teacher Registration
                register_teacher();
                break;

            case 6: // Exit
                printf("\nThank you for using ExamBunny!\n");
                printf("Wishing you success in your learning journey. Goodbye!\n\n");
                return 0;

            default:
                printf("\nInvalid choice! Please try again.\n");
                pause_and_continue();
        }
    }
}



