#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[50];


// ======================= PASSWORD MASKING (MAC/LINUX/UNIX) =======================
void getPassword(char *password) {
    struct termios oldt, newt;
    int i = 0;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        ch = getchar();
        if (ch == '\n') break;
        else if (ch == 127 || ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}


// =========================== FUNCTION DECLARATIONS ===========================
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();
void addStudent();
void displayStudents();
void searchStudent();




// ================================== MAIN ==================================
int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("Exiting program...\n");
    }
    return 0;
}



// ========================== LOGIN SYSTEM ====================================
int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[20];

    int attempts = 0;

    while (attempts < 3) {
        printf("\n============ LOGIN SCREEN ============\n");
        printf("Username: ");
        scanf("%s", username);

        getchar(); // clear buffer

        printf("Password: ");
        getPassword(password);

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credentials file missing!\n");
            return 0;
        }

        int loginSuccess = 0;

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 &&
                strcmp(password, filePass) == 0) {

                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                fclose(fp);

                printf("\nLogin Successful!\n");
                return 1;
            }
        }

        fclose(fp);
        attempts++;
        printf("Wrong Username or Password! Attempts left: %d\n\n", 3 - attempts);
    }

    printf("Too many failed attempts!\n");
    return 0;
}



// ============================== MAIN MENU ==============================
void mainMenu() {
    int choice;

    while (1) {
        printf("\n================ MAIN MENU ================\n");
        printf("Logged in as: %s (%s)\n", currentUser, currentRole);
        printf("-------------------------------------------\n");

        if (strcmp(currentRole, "admin") == 0) {
            adminMenu();
            return; // important fix
        }
        else if (strcmp(currentRole, "user") == 0) {
            userMenu();
            return;
        }
        else if (strcmp(currentRole, "staff") == 0) {
            staffMenu();
            return;
        }
        else if (strcmp(currentRole, "guest") == 0) {
            guestMenu();
            return;
        }
        else {
            printf("Invalid role configuration!\n");
            return;
        }
    }
}



// ============================ ADMIN MENU ============================
void adminMenu() {
    int choice;

    while (1) {
        printf("\n============== ADMIN MENU ==============\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: return;  // return to mainMenu -> but mainMenu returns also
            default: printf("Invalid choice!\n");
        }
    }
}



// ============================ USER MENU ============================
void userMenu() {
    int choice;

    while (1) {
        printf("\n================ USER MENU ================\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}



// ============================ STAFF MENU ============================
void staffMenu() {
    int choice;

    while (1) {
        printf("\n================ STAFF MENU ================\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}



// ============================ GUEST MENU ============================
void guestMenu() {
    int choice;

    while (1) {
        printf("\n================ GUEST MENU ================\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: return;
            default: printf("Invalid choice!\n");
        }
    }
}



// ============================ ADD STUDENT ============================
void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("Error opening student file!\n");
        return;
    }

    printf("Enter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);

    fclose(fp);
    printf("Student Added Successfully!\n");
}



// ========================== DISPLAY STUDENTS ==========================
void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("\nROLL\tNAME\tMARKS\n");
    printf("--------------------------\n");

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}



// ============================ SEARCH STUDENT ============================
void searchStudent() {
    struct Student s;
    int r, found = 0;

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("Enter Roll No to Search: ");
    scanf("%d", &r);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == r) {
            printf("\nRecord Found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No such student found!\n");

    fclose(fp);
}


