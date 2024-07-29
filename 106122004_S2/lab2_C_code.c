#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for grade points
#define GRADE_S 10
#define GRADE_A 9
#define GRADE_B 8
#define GRADE_C 7
#define GRADE_D 6
#define GRADE_E 5
#define GRADE_F 0

// Structure to hold course details
struct Course {
    char name[20];
    int credits;
    char grade;
};

// Structure to hold student details
struct Student {
    int rollno;
    char name[30];
    char department[30];
    struct Course courses[4]; // Max 4 courses per student
    int num_courses; // Actual number of courses
    float gpa;
};

// Function to write student records to file
void writeStudentsToFile(struct Student students[], int count) {
    FILE *fp = fopen("students.dat", "wb");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fwrite(students, sizeof(struct Student), count, fp);
    fclose(fp);
}

// Function to read student records from file
int readStudentsFromFile(struct Student students[]) {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        return 0; // Return 0 if file doesn't exist
    }
    int count = 0;
    while (fread(&students[count], sizeof(struct Student), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

// Function to insert student records
void insertStudent(struct Student students[], int *count) {
    if (*count >= 50) {
        printf("Maximum limit of students reached.\n");
        return;
    }
    
    struct Student newStudent;
    printf("Enter Roll No: ");
    scanf("%d", &newStudent.rollno);
    printf("Enter Name: ");
    scanf(" %[^\n]", newStudent.name);
    printf("Enter Department: ");
    scanf(" %[^\n]", newStudent.department);
    printf("Enter number of courses (3 or 4): ");
    scanf("%d", &newStudent.num_courses);
    if (newStudent.num_courses < 3 || newStudent.num_courses > 4) {
        printf("Invalid number of courses. Must be 3 or 4.\n");
        return;
    }
    
    for (int i = 0; i < newStudent.num_courses; i++) {
        printf("Enter Course Name: ");
        scanf(" %[^\n]", newStudent.courses[i].name);
        printf("Enter Credits: ");
        scanf("%d", &newStudent.courses[i].credits);
        printf("Enter Grade (S/A/B/C/D/E/F): ");
        scanf(" %c", &newStudent.courses[i].grade);
    }
    
    students[*count] = newStudent;
    (*count)++;
    printf("Student record inserted successfully.\n");
}

// Function to calculate GPA for all students
void createGPA(struct Student students[], int count) {
    for (int i = 0; i < count; i++) {
        float totalCredits = 0.0;
        float totalGradePoints = 0.0;
        
        for (int j = 0; j < students[i].num_courses; j++) {
            switch (students[i].courses[j].grade) {
                case 'S':
                    totalGradePoints += GRADE_S * students[i].courses[j].credits;
                    break;
                case 'A':
                    totalGradePoints += GRADE_A * students[i].courses[j].credits;
                    break;
                case 'B':
                    totalGradePoints += GRADE_B * students[i].courses[j].credits;
                    break;
                case 'C':
                    totalGradePoints += GRADE_C * students[i].courses[j].credits;
                    break;
                case 'D':
                    totalGradePoints += GRADE_D * students[i].courses[j].credits;
                    break;
                case 'E':
                    totalGradePoints += GRADE_E * students[i].courses[j].credits;
                    break;
                case 'F':
                    totalGradePoints += GRADE_F * students[i].courses[j].credits;
                    break;
                default:
                    printf("Invalid grade entered.\n");
                    break;
            }
            totalCredits += students[i].courses[j].credits;
        }
        
        if (totalCredits > 0) {
            students[i].gpa = totalGradePoints / totalCredits;
        } else {
            students[i].gpa = 0.0;
        }
        
        printf("GPA calculated for Roll No %d: %.2f\n", students[i].rollno, students[i].gpa);
    }
}
// Function to delete a course for a student with four courses
void deleteCourse(struct Student students[], int count, int rollno) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollno == rollno && students[i].num_courses == 4) {
            printf("Current Courses for Roll No %d:\n", rollno);
            for (int j = 0; j < students[i].num_courses; j++) {
                printf("%d. %s\n", j + 1, students[i].courses[j].name);
            }
            printf("Enter the course index to delete (1-%d): ", students[i].num_courses);
            int index;
            scanf("%d", &index);
            if (index < 1 || index > students[i].num_courses) {
                printf("Invalid index entered.\n");
                return;
            }
            
            // Move courses back to fill the gap
            for (int k = index - 1; k < students[i].num_courses - 1; k++) {
                strcpy(students[i].courses[k].name, students[i].courses[k + 1].name);
                students[i].courses[k].credits = students[i].courses[k + 1].credits;
                students[i].courses[k].grade = students[i].courses[k + 1].grade;
            }
            
            students[i].num_courses--;
            printf("Course deleted successfully.\n");
            return;
        }
    }
    printf("Student with Roll No %d either does not exist or does not have four courses.\n", rollno);
}

// Function to insert a new course for a student with four courses
void insertCourse(struct Student students[], int count, int rollno) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollno == rollno && students[i].num_courses == 4) {
            printf("Current Courses for Roll No %d:\n", rollno);
            for (int j = 0; j < students[i].num_courses; j++) {
                printf("%d. %s\n", j + 1, students[i].courses[j].name);
            }
            printf("Enter the new course name: ");
            scanf(" %[^\n]", students[i].courses[students[i].num_courses].name);
            printf("Enter credits for the new course: ");
            scanf("%d", &students[i].courses[students[i].num_courses].credits);
            printf("Enter grade for the new course (S/A/B/C/D/E/F): ");
            scanf(" %c", &students[i].courses[students[i].num_courses].grade);
            
            students[i].num_courses++;
            printf("New course inserted successfully.\n");
            return;
        }
    }
    printf("Student with Roll No %d either does not exist or does not have four courses.\n", rollno);
}

// Function to update course name for two different students
void updateCourseName(struct Student students[], int count, char *oldName, char *newName) {
    int updated = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < students[i].num_courses; j++) {
            if (strcmp(students[i].courses[j].name, oldName) == 0) {
                strcpy(students[i].courses[j].name, newName);
                updated++;
            }
        }
    }
    printf("%d students updated with the new course name.\n", updated);
}

// Function to calculate GPA of all students
void calculateGPA(struct Student students[], int count) {
    for (int i = 0; i < count; i++) {
        float totalCredits = 0.0;
        float totalGradePoints = 0.0;
        
        for (int j = 0; j < students[i].num_courses; j++) {
            switch (students[i].courses[j].grade) {
                case 'S':
                    totalGradePoints += GRADE_S * students[i].courses[j].credits;
                    break;
                case 'A':
                    totalGradePoints += GRADE_A * students[i].courses[j].credits;
                    break;
                case 'B':
                    totalGradePoints += GRADE_B * students[i].courses[j].credits;
                    break;
                case 'C':
                    totalGradePoints += GRADE_C * students[i].courses[j].credits;
                    break;
                case 'D':
                    totalGradePoints += GRADE_D * students[i].courses[j].credits;
                    break;
                case 'E':
                    totalGradePoints += GRADE_E * students[i].courses[j].credits;
                    break;
                case 'F':
                    totalGradePoints += GRADE_F * students[i].courses[j].credits;
                    break;
                default:
                    printf("Invalid grade entered.\n");
                    break;
            }
            totalCredits += students[i].courses[j].credits;
        }
        
        if (totalCredits > 0) {
            students[i].gpa = totalGradePoints / totalCredits;
        } else {
            students[i].gpa = 0.0;
        }
        
        printf("GPA calculated for Roll No %d: %.2f\n", students[i].rollno, students[i].gpa);
    }
}

// Function to upgrade grade point for a student who has secured '7' in a course
void upgradeGradePoint(struct Student students[], int count, int rollno) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollno == rollno) {
            for (int j = 0; j < students[i].num_courses; j++) {
                if (students[i].courses[j].grade == '7') {
                    students[i].courses[j].grade = 'S'; // Upgrade to grade 'S' (10 points)
                }
            }
            printf("Grade points upgraded successfully for Roll No %d.\n", rollno);
            return;
        }
    }
    printf("Student with Roll No %d not found.\n", rollno);
}

// Function to calculate updated GPA of a student after upgrading grade points
void calculateUpdatedGPA(struct Student students[], int count, int rollno) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollno == rollno) {
            float totalCredits = 0.0;
            float totalGradePoints = 0.0;
            
            for (int j = 0; j < students[i].num_courses; j++) {
                switch (students[i].courses[j].grade) {
                    case 'S':
                        totalGradePoints += GRADE_S * students[i].courses[j].credits;
                        break;
                    case 'A':
                        totalGradePoints += GRADE_A * students[i].courses[j].credits;
                        break;
                    case 'B':
                        totalGradePoints += GRADE_B * students[i].courses[j].credits;
                        break;
                    case 'C':
                        totalGradePoints += GRADE_C * students[i].courses[j].credits;
                        break;
                    case 'D':
                        totalGradePoints += GRADE_D * students[i].courses[j].credits;
                        break;
                    case 'E':
                        totalGradePoints += GRADE_E * students[i].courses[j].credits;
                        break;
                    case 'F':
                        totalGradePoints += GRADE_F * students[i].courses[j].credits;
                        break;
                    default:
                        printf("Invalid grade entered.\n");
                        break;
                }
                totalCredits += students[i].courses[j].credits;
            }
            
            if (totalCredits > 0) {
                students[i].gpa = totalGradePoints / totalCredits;
            } else {
                students[i].gpa = 0.0;
            }
            
            printf("Updated GPA calculated for Roll No %d: %.2f\n", students[i].rollno, students[i].gpa);
            return;
        }
    }
    printf("Student with Roll No %d not found.\n", rollno);
}

// Function to generate grade report for a student given roll no. or name
void generateGradeReport(struct Student students[], int count, int rollno) {
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].rollno == rollno || strcmp(students[i].name, rollno) == 0) {
            printf("Grade Report for Roll No %d - Name: %s\n", students[i].rollno, students[i].name);
            for (int j = 0; j < students[i].num_courses; j++) {
                printf("%d. Course: %s, Credits: %d, Grade: %c\n", j + 1, students[i].courses[j].name,
                       students[i].courses[j].credits, students[i].courses[j].grade);
            }
            printf("GPA: %.2f\n", students[i].gpa);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Student with Roll No %d or name not found.\n", rollno);
    }
}

int main() {
    struct Student students[50]; // Array to hold student records
    int count = 0; // Number of students currently in the array
    
    int choice;
    
    // Read existing student records from file, if any
    count = readStudentsFromFile(students);
    
    do {
        printf("\nMenu:");
        printf("\n1. Insert Student Records");
        printf("\n2. Create GPA for All Students");
        printf("\n3. Delete a Course for a Student");
        printf("\n4. Insert a New Course for a Student");
        printf("\n5. Update Course Name for Two Students");
        printf("\n6. Calculate GPA of All Students");
        printf("\n7. Upgrade Grade Point for a Student");
        printf("\n8. Calculate Updated GPA for a Student");
        printf("\n9. Generate Grade Report for a Student");
        printf("\n0. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                insertStudent(students, &count);
                break;
            case 2:
                createGPA(students, count);
                break;
            case 3:
                {
                    int rollno;
                    printf("Enter Roll No to delete course: ");
                    scanf("%d", &rollno);
                    deleteCourse(students, count, rollno);
                }
                break;
            case 4:
                {
                    int rollno;
                    printf("Enter Roll No to insert course: ");
                    scanf("%d", &rollno);
                    insertCourse(students, count, rollno);
                }
                break;
            case 5:
                {
                    char oldName[20], newName[20];
                    printf("Enter Old Course Name: ");
                    scanf(" %[^\n]", oldName);
                    printf("Enter New Course Name: ");
                    scanf(" %[^\n]", newName);
                    updateCourseName(students, count, oldName, newName);
                }
                break;
            case 6:
                calculateGPA(students, count);
                break;
            case 7:
                {
                    int rollno;
                    printf("Enter Roll No to upgrade grade point: ");
                    scanf("%d", &rollno);
                    upgradeGradePoint(students, count, rollno);
                }
                break;
            case 8:
                {
                    int rollno;
                    printf("Enter Roll No to calculate updated GPA: ");
                    scanf("%d", &rollno);
                    calculateUpdatedGPA(students, count, rollno);
                }
                break;
            case 9:
                {
                    int rollno;
                    printf("Enter Roll No or Name to generate grade report: ");
                    scanf("%d", &rollno);
                    generateGradeReport(students, count, rollno);
                }
                break;
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please enter again.\n");
        }
        
    } while (choice != 0);
    
    // Save student records to file before exiting
    writeStudentsToFile(students, count);
    
    return 0;
}

