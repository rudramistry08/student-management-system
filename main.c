#include <stdio.h>
#include <string.h>

#define MAX 100

struct Student
{
    int rollNo;
    char name[50];
    float marks;
};

// ---------- GRADE CALCULATION ----------
char calculateGrade(float marks)
{
    if (marks >= 90) return 'A';
    else if (marks >= 75) return 'B';
    else if (marks >= 60) return 'C';
    else if (marks >= 40) return 'D';
    else return 'F';
}

// ---------- FIND STUDENT ----------
int findStudent(struct Student s[], int count, int roll)
{
    for (int i = 0; i < count; i++)
    {
        if (s[i].rollNo == roll)
            return i;
    }
    return -1;
}

// ---------- SAVE RECORDS ----------
void saveStudents(struct Student s[], int count)
{
    FILE *fp = fopen("students.dat", "wb");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to save records!\n");
        return;
    }

    if (fwrite(&count, sizeof(int), 1, fp) != 1 ||
        fwrite(s, sizeof(struct Student), count, fp) != (size_t)count)
    {
        printf("\n[ERROR] Record saving failed!\n");
    }
    else
    {
        printf("\n[SUCCESS] Records saved successfully!\n");
    }

    fclose(fp);
}

// ---------- LOAD RECORDS ----------
void loadStudents(struct Student s[], int *count)
{
    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL)
    {
        *count = 0;
        return;
    }

    if (fread(count, sizeof(int), 1, fp) != 1 ||
        *count < 0 || *count > MAX)
    {
        *count = 0;
        fclose(fp);
        printf("\n[ERROR] Invalid storage file!\n");
        return;
    }

    if (fread(s, sizeof(struct Student), *count, fp)
        != (size_t)*count)
    {
        *count = 0;
        printf("\n[ERROR] Unable to load all records!\n");
    }
    else
    {
        printf("\n[SUCCESS] %d student record(s) loaded!\n", *count);
    }

    fclose(fp);
}

// ---------- ADD STUDENT ----------
void addStudent(struct Student s[], int *count)
{
    int roll;
    float marks;

    if (*count >= MAX)
    {
        printf("\n[ERROR] Student limit reached!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    if (scanf("%d", &roll) != 1)
    {
        printf("\n[ERROR] Invalid roll number!\n");
        return;
    }

    if (findStudent(s, *count, roll) != -1)
    {
        printf("\n[ERROR] Roll number already exists!\n");
        return;
    }

    printf("Enter Student Name: ");
    scanf(" %49[^\n]", s[*count].name);

    printf("Enter Marks: ");
    if (scanf("%f", &marks) != 1 || marks < 0 || marks > 100)
    {
        printf("\n[ERROR] Marks must be between 0 and 100!\n");
        return;
    }

    s[*count].rollNo = roll;
    s[*count].marks = marks;
    (*count)++;

    printf("\n[SUCCESS] Student added successfully!\n");
    saveStudents(s, *count);
}

// ---------- DISPLAY STUDENTS ----------
void displayStudents(struct Student s[], int count)
{
    if (count == 0)
    {
        printf("\n[INFO] No student records available.\n");
        return;
    }

    printf("\n");
    printf("========================================================\n");
    printf("                    STUDENT RECORDS                     \n");
    printf("========================================================\n");

    printf("%-10s %-22s %-12s %-10s\n",
           "ROLL NO", "NAME", "MARKS", "GRADE");

    printf("--------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-22s %-12.2f %-10c\n",
               s[i].rollNo,
               s[i].name,
               s[i].marks,
               calculateGrade(s[i].marks));
    }

    printf("========================================================\n");
    printf("Total Records: %d\n", count);
}

// ---------- SEARCH STUDENT ----------
void searchStudent(struct Student s[], int count)
{
    int choice, roll, found = 0;
    char searchName[50];

    if (count == 0)
    {
        printf("\n[INFO] No records available.\n");
        return;
    }

    printf("\n========== SEARCH STUDENT ==========\n");
    printf("1. Search by Roll Number\n");
    printf("2. Search by Student Name\n");
    printf("3. Back to Main Menu\n");
    printf("Enter Choice: ");

    if (scanf("%d", &choice) != 1)
        return;

    if (choice == 1)
    {
        printf("Enter Roll Number: ");
        if (scanf("%d", &roll) != 1)
            return;

        int index = findStudent(s, count, roll);

        if (index != -1)
        {
            printf("\nStudent Found!\n");
            printf("Roll Number : %d\n", s[index].rollNo);
            printf("Name        : %s\n", s[index].name);
            printf("Marks       : %.2f\n", s[index].marks);
            printf("Grade       : %c\n",
                   calculateGrade(s[index].marks));
        }
        else
        {
            printf("\n[INFO] Student not found!\n");
        }
    }
    else if (choice == 2)
    {
        printf("Enter Name or Part of Name: ");
        scanf(" %49[^\n]", searchName);

        printf("\nSearch Results:\n");

        for (int i = 0; i < count; i++)
        {
            if (strstr(s[i].name, searchName) != NULL)
            {
                printf("\nRoll Number : %d\n", s[i].rollNo);
                printf("Name        : %s\n", s[i].name);
                printf("Marks       : %.2f\n", s[i].marks);
                printf("Grade       : %c\n",
                       calculateGrade(s[i].marks));
                found = 1;
            }
        }

        if (!found)
            printf("\n[INFO] No matching student found!\n");
    }
    else if (choice == 3)
    {
        printf("\nReturning to main menu...\n");
    }
    else
    {
        printf("\n[ERROR] Invalid search option!\n");
    }
}

// ---------- CALCULATE GRADE ----------
void studentGrade(struct Student s[], int count)
{
    int roll, index;

    printf("\nEnter Roll Number: ");
    if (scanf("%d", &roll) != 1)
        return;

    index = findStudent(s, count, roll);

    if (index == -1)
    {
        printf("\n[INFO] Student not found!\n");
        return;
    }

    printf("\nStudent: %s\n", s[index].name);
    printf("Marks  : %.2f\n", s[index].marks);
    printf("Grade  : %c\n", calculateGrade(s[index].marks));
}

// ---------- UPDATE STUDENT ----------
void updateStudent(struct Student s[], int count)
{
    int roll, index;
    char newName[50];
    float newMarks;

    printf("\nEnter Roll Number to Update: ");
    if (scanf("%d", &roll) != 1)
        return;

    index = findStudent(s, count, roll);

    if (index == -1)
    {
        printf("\n[INFO] Student not found!\n");
        return;
    }

    printf("Enter New Name: ");
    scanf(" %49[^\n]", newName);

    printf("Enter New Marks: ");
    if (scanf("%f", &newMarks) != 1 ||
        newMarks < 0 || newMarks > 100)
    {
        printf("\n[ERROR] Invalid marks! Update cancelled.\n");
        return;
    }

    strcpy(s[index].name, newName);
    s[index].marks = newMarks;

    printf("\n[SUCCESS] Student updated!\n");
    saveStudents(s, count);
}

// ---------- DELETE STUDENT ----------
void deleteStudent(struct Student s[], int *count)
{
    int roll, index;

    printf("\nEnter Roll Number to Delete: ");
    if (scanf("%d", &roll) != 1)
        return;

    index = findStudent(s, *count, roll);

    if (index == -1)
    {
        printf("\n[INFO] Student not found!\n");
        return;
    }

    for (int i = index; i < *count - 1; i++)
        s[i] = s[i + 1];

    (*count)--;

    printf("\n[SUCCESS] Student deleted!\n");
    saveStudents(s, *count);
}

// ---------- SORT BY ROLL ----------
void sortByRoll(struct Student s[], int count)
{
    struct Student temp;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (s[j].rollNo > s[j + 1].rollNo)
            {
                temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }

    printf("\n[SUCCESS] Sorted by Roll Number!\n");
    saveStudents(s, count);
}

// ---------- SORT BY MARKS ----------
void sortByMarks(struct Student s[], int count)
{
    struct Student temp;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (s[j].marks < s[j + 1].marks)
            {
                temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }

    printf("\n[SUCCESS] Sorted by Marks!\n");
    saveStudents(s, count);
}

// ---------- CLASS STATISTICS ----------
void classStatistics(struct Student s[], int count)
{
    if (count == 0)
    {
        printf("\n[INFO] No student records available.\n");
        return;
    }

    float total = 0;
    float highest = s[0].marks;
    float lowest = s[0].marks;

    int highIndex = 0, lowIndex = 0;
    int passed = 0, failed = 0;

    for (int i = 0; i < count; i++)
    {
        total += s[i].marks;

        if (s[i].marks > highest)
        {
            highest = s[i].marks;
            highIndex = i;
        }

        if (s[i].marks < lowest)
        {
            lowest = s[i].marks;
            lowIndex = i;
        }

        if (s[i].marks >= 40)
            passed++;
        else
            failed++;
    }

    printf("\n================================================\n");
    printf("                 CLASS STATISTICS                \n");
    printf("================================================\n");

    printf("Total Students : %d\n", count);
    printf("Average Marks  : %.2f\n", total / count);

    printf("\nHIGHEST SCORER\n");
    printf("Name  : %s\n", s[highIndex].name);
    printf("Roll  : %d\n", s[highIndex].rollNo);
    printf("Marks : %.2f\n", highest);

    printf("\nLOWEST SCORER\n");
    printf("Name  : %s\n", s[lowIndex].name);
    printf("Roll  : %d\n", s[lowIndex].rollNo);
    printf("Marks : %.2f\n", lowest);

    printf("\nPassed Students : %d\n", passed);
    printf("Failed Students : %d\n", failed);

    printf("================================================\n");
}

// ---------- MAIN MENU ----------
void displayMenu(void)
{
    printf("\n");
    printf("================================================\n");
    printf("             STUDENT MANAGEMENT SYSTEM           \n");
    printf("================================================\n");
    printf("  1. Add Student\n");
    printf("  2. Display Students\n");
    printf("  3. Search Student\n");
    printf("  4. Calculate Grade\n");
    printf("  5. Update Student\n");
    printf("  6. Delete Student\n");
    printf("  7. Exit Application\n");
    printf("  8. Sort by Roll Number\n");
    printf("  9. Sort by Marks\n");
    printf(" 10. Class Statistics\n");
    printf("================================================\n");
    printf(" Enter Your Choice: ");
}

// ---------- MAIN FUNCTION ----------
int main(void)
{
    struct Student s[MAX];
    int count = 0;
    int choice;

    printf("\n");
    printf("************************************************\n");
    printf("       WELCOME TO STUDENT MANAGEMENT SYSTEM      \n");
    printf("************************************************\n");

    loadStudents(s, &count);

    do
    {
        displayMenu();

        if (scanf("%d", &choice) != 1)
        {
            printf("\n[ERROR] Please enter a valid number!\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
                addStudent(s, &count);
                break;

            case 2:
                displayStudents(s, count);
                break;

            case 3:
                searchStudent(s, count);
                break;

            case 4:
                studentGrade(s, count);
                break;

            case 5:
                updateStudent(s, count);
                break;

            case 6:
                deleteStudent(s, &count);
                break;

            case 7:
                printf("\n************************************************\n");
                printf("       THANK YOU FOR USING OUR APPLICATION       \n");
                printf("************************************************\n");
                break;

            case 8:
                sortByRoll(s, count);
                break;

            case 9:
                sortByMarks(s, count);
                break;

            case 10:
                classStatistics(s, count);
                break;

            default:
                printf("\n[ERROR] Invalid menu choice!\n");
        }

    } while (choice != 7);

    return 0;
} 
