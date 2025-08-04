#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

typedef struct
{
    char student_id[20];
    char name[50]; 
    char password[50]; 
    char role[10];
    
} user;

typedef struct
{ 
    char course_name[100];
    char course_code[20];
    
} Course;

typedef struct
{
    char course_code[20];
    char question[300];
    char optionA[100];
    char optionB[100]; 
    char optionC[100]; 
    char optionD[100]; 
    char check;
} MCQ;

typedef struct
{
char student_id[20];
char course_code[20]; 
int score;
} ExamResult;
