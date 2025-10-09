#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_PROJECTS 100
#define MAX_LEN 100

typedef struct {
    char name[MAX_LEN];
    char startDate[MAX_LEN];
    char endDate[MAX_LEN];
    char evaluationResult[MAX_LEN];
} Project;
 
Project projects[MAX_PROJECTS];
int projectCount = 0;

void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line[512];
    projectCount = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && projectCount < MAX_PROJECTS) {
        char *token = strtok(line, ",");
        if (token) {
            strncpy(projects[projectCount].name, token, MAX_LEN - 1);
            projects[projectCount].name[MAX_LEN - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token) {
            strncpy(projects[projectCount].startDate, token, MAX_LEN - 1);
            projects[projectCount].startDate[MAX_LEN - 1] = '\0';
        }

        token = strtok(NULL, ",");
        if (token) {
            strncpy(projects[projectCount].endDate, token, MAX_LEN - 1);
            projects[projectCount].endDate[MAX_LEN - 1] = '\0';
        }

        token = strtok(NULL, "\n");
        if (token) {
            size_t len = strlen(token);
            if (len > 0 && (token[len-1] == '\r' || token[len-1] == '\n')) token[len-1] = '\0';
            strncpy(projects[projectCount].evaluationResult, token, MAX_LEN - 1);
            projects[projectCount].evaluationResult[MAX_LEN - 1] = '\0';
        }

        projectCount++;
    }
    fclose(file);
    printf("Loaded data from %s successfully! %d projects\n", filename, projectCount);
}

void writeCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Cannot write to file %s\n", filename);
        return;
    }

    fprintf(file, "ProjectName,StartDate,EndDate,EvaluationResult\n");
    for (int i = 0; i < projectCount; i++) {
        fprintf(file, "%s,%s,%s,%s\n", projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }

    fclose(file);
    printf("Saved data to %s successfully!\n", filename);
}

void addProject() {
    if (projectCount >= MAX_PROJECTS) {
        printf("Project limit reached.\n");
        return;
    }

    char buffer[MAX_LEN];

    printf("Add new project:\n");

    printf("Project Name: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].name, buffer, MAX_LEN - 1);
    projects[projectCount].name[MAX_LEN - 1] = '\0';

    printf("Start Date (DD-MM-YYYY): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].startDate, buffer, MAX_LEN - 1);
    projects[projectCount].startDate[MAX_LEN - 1] = '\0';

    printf("End Date (DD-MM-YYYY): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].endDate, buffer, MAX_LEN - 1);
    projects[projectCount].endDate[MAX_LEN - 1] = '\0';

    printf("Evaluation Result: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].evaluationResult, buffer, MAX_LEN - 1);
    projects[projectCount].evaluationResult[MAX_LEN - 1] = '\0';

    
    projectCount++;
    printf("Project added successfully!\n");
}

void showAllProjects() {
    if (projectCount == 0) {
        printf("No projects to display.\n");
        return;
    }

    printf("\n==== All Project Data ====\n");
    for (int i = 0; i < projectCount; i++) {
        printf("%d. %s | %s - %s | %s\n", i + 1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }
}

void searchProject() {
    char keyword[MAX_LEN];
    printf("Enter project name or evaluation result to search: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL) { printf("Error reading input.\n"); return; }
    keyword[strcspn(keyword, "\r\n")] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strstr(projects[i].name, keyword) != NULL || strstr(projects[i].evaluationResult, keyword) != NULL) {
            if (!found) {
                printf("\nSearch Results:\n");
            }
            printf("%d. %s | %s - %s | %s\n", i + 1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching projects found.\n");
    }
}

void updateProject() {
    char name[MAX_LEN];
    printf("Enter the name of the project to edit: ");
    if (fgets(name, sizeof(name), stdin) == NULL) { printf("Error reading input.\n"); return; }
    name[strcspn(name, "\r\n")] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, name) == 0) {
            found = 1;
            printf("Editing project: %s\n", projects[i].name);

            char buffer[MAX_LEN];

            printf("New start date (current: %s): ", projects[i].startDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); break; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].startDate, buffer, MAX_LEN - 1);
            projects[i].startDate[MAX_LEN - 1] = '\0';

            printf("New end date (current: %s): ", projects[i].endDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); break; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].endDate, buffer, MAX_LEN - 1);
            projects[i].endDate[MAX_LEN - 1] = '\0';

            printf("New evaluation result (current: %s): ", projects[i].evaluationResult);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); break; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].evaluationResult, buffer, MAX_LEN - 1);
            projects[i].evaluationResult[MAX_LEN - 1] = '\0';

            printf("Project updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Project with the specified name not found.\n");
    }
}

void deleteProject() {
    char name[MAX_LEN];
    printf("Enter the name of the project to delete: ");
    if (fgets(name, sizeof(name), stdin) == NULL) { printf("Error reading input.\n"); return; }
    name[strcspn(name, "\r\n")] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, name) == 0) {
            found = 1;
            for (int j = i; j < projectCount - 1; j++) {
                projects[j] = projects[j+1];
            }
            projectCount--;
            printf("Project deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Project with the specified name not found.\n");
    }
}

void displayMenu() {
    printf("\n==== Main Menu ====\n");
    printf("1. Read data from CSV file\n");
    printf("2. Add new project\n");
    printf("3. Search for a project\n");
    printf("4. Edit a project\n");
    printf("5. Delete a project\n");
    printf("6. Show all projects\n");
    printf("7. Save data to CSV file\n");
    printf("8. Exit program\n");
}

int main() {
    int choice;
    char filename[] = "projects.csv";

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            // Clear the invalid input from the buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while (getchar() != '\n'); // Consume the rest of the line

        switch (choice) {
            case 1:
                readCSV(filename);
                break;
            case 2:
                addProject();
                break;
            case 3:
                searchProject();
                break;
            case 4:
                updateProject();
                break;
            case 5:
                deleteProject();
                break;
            case 6:
                showAllProjects();
                break;
            case 7:
                writeCSV(filename);
                break;
            case 8:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}