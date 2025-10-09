#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* strcasestr_custom(const char* haystack, const char* needle);

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

int isValidDate(const char* dateStr) {
    int day, month, year;
    if (sscanf(dateStr, "%d-%d-%d", &day, &month, &year) != 3) {
        return 0;
    }

    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1) {
        return 0;
    }

    int daysInMonth[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (day > daysInMonth[month]) {
        return 0;
    }

    return 1;
}

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
        if (token) strncpy(projects[projectCount].name, token, MAX_LEN - 1);
        projects[projectCount].name[MAX_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (token) strncpy(projects[projectCount].startDate, token, MAX_LEN - 1);
        projects[projectCount].startDate[MAX_LEN - 1] = '\0';

        token = strtok(NULL, ",");
        if (token) strncpy(projects[projectCount].endDate, token, MAX_LEN - 1);
        projects[projectCount].endDate[MAX_LEN - 1] = '\0';

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

    printf("Add new project:\n");
    char buffer[MAX_LEN];

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
        printf("%d. %s | %s - %s | %s\n", i+1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }
}

void searchProject() {
    char keyword[MAX_LEN];
    char buffer[MAX_LEN];
    printf("Enter project name or evaluation result to search: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(keyword, buffer, MAX_LEN - 1);
    keyword[MAX_LEN - 1] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcasestr_custom(projects[i].name, keyword) != NULL || strcasestr_custom(projects[i].evaluationResult, keyword) != NULL) {
            if (!found) printf("\nSearch Results:\n");
            printf("%d. %s | %s - %s | %s\n", i+1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching projects found.\n");
    }
}

void updateProject() {
    char name[MAX_LEN];
    char buffer[MAX_LEN];
    printf("Enter the name of the project to edit: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(name, buffer, MAX_LEN - 1);
    name[MAX_LEN - 1] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, name) == 0) {
            found = 1;
            printf("Editing project: %s\n", projects[i].name);

            printf("New start date (current: %s): ", projects[i].startDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].startDate, buffer, MAX_LEN - 1);
            projects[i].startDate[MAX_LEN - 1] = '\0';

            printf("New end date (current: %s): ", projects[i].endDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].endDate, buffer, MAX_LEN - 1);
            projects[i].endDate[MAX_LEN - 1] = '\0';

            printf("New evaluation result (current: %s): ", projects[i].evaluationResult);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
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
    char buffer[MAX_LEN];
    printf("Enter the name of the project to delete: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("Error reading input.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(name, buffer, MAX_LEN - 1);
    name[MAX_LEN - 1] = '\0';

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
    printf("9. Run Unit Tests\n");
}

void resetState() {
    projectCount = 0;
}

void addTestProject(const char* name, const char* start, const char* end, const char* result) {
    if (projectCount < MAX_PROJECTS) {
        strncpy(projects[projectCount].name, name, MAX_LEN - 1); projects[projectCount].name[MAX_LEN - 1] = '\0';
        strncpy(projects[projectCount].startDate, start, MAX_LEN - 1); projects[projectCount].startDate[MAX_LEN - 1] = '\0';
        strncpy(projects[projectCount].endDate, end, MAX_LEN - 1); projects[projectCount].endDate[MAX_LEN - 1] = '\0';
        strncpy(projects[projectCount].evaluationResult, result, MAX_LEN - 1); projects[projectCount].evaluationResult[MAX_LEN - 1] = '\0';
        projectCount++;
    }
}

char* strcasestr_custom(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack; 
    for (; *haystack; ++haystack) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char* h, * n;
            for (h = haystack, n = needle; *h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n); ++h, ++n);
            if (!*n) return (char*)haystack; 
        }
    }
    return NULL;
}

void testCSV_IO(const char* testFilename) {
    printf("\n--- Test: CSV File I/O ---\n");
    resetState();

    FILE *tempFile = fopen(testFilename, "w");
    if (!tempFile) {
        printf("[FAIL] Unable to create test file: %s\n", testFilename);
        return;
    }
    fprintf(tempFile, "ProjectName,StartDate,EndDate,EvaluationResult\n");
    fprintf(tempFile, "Project Alpha,01-01-2024,31-01-2024,Good\n");
    fprintf(tempFile, "Project Beta,15-02-2024,28-02-2024,Excellent\n");
    fclose(tempFile);

    readCSV(testFilename);

    if (projectCount == 2) {
        printf("[PASS] readCSV: Correct number of projects loaded (%d).\n", projectCount);
    } else {
        printf("[FAIL] readCSV: Expected 2 projects, but got %d.\n", projectCount);
    }

    if (strcmp(projects[0].name, "Project Alpha") == 0 &&
        strcmp(projects[0].evaluationResult, "Good") == 0) {
        printf("[PASS] readCSV: Data for 'Project Alpha' is correct.\n");
    } else {
        printf("[FAIL] readCSV: Data for 'Project Alpha' is incorrect. Got: %s, %s\n", projects[0].name, projects[0].evaluationResult);
    }
    
    const char* writeFilename = "temp_output.csv";
    writeCSV(writeFilename);

    resetState();
    readCSV(writeFilename);
    if (projectCount == 2 && strcmp(projects[1].name, "Project Beta") == 0 && strcmp(projects[1].evaluationResult, "Excellent") == 0) {
        printf("[PASS] writeCSV: File content written and verified successfully.\n");
    } else {
        printf("[FAIL] writeCSV: Verification of written file failed. projectCount: %d, name: %s, eval: %s\n", projectCount, projects[1].name, projects[1].evaluationResult);
    }
    
    remove(testFilename);
    remove(writeFilename);
}

void testDeleteLogic() {
    printf("\n--- Test: Delete Logic ---\n");
    resetState();
    addTestProject("P1", "d1", "d2", "r1");
    addTestProject("P2_to_delete", "d3", "d4", "r2");
    addTestProject("P3", "d5", "d6", "r3");
    
    int indexToDelete = -1;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, "P2_to_delete") == 0) {
            indexToDelete = i;
            break;
        }
    }
    
    if (indexToDelete != -1) {
        for (int j = indexToDelete; j < projectCount - 1; j++) { projects[j] = projects[j+1]; } 
        projectCount--;
        
        if (projectCount == 2) {
            printf("[PASS] Delete Logic: Project count is correct after deletion (%d).\n", projectCount);
        } else {
            printf("[FAIL] Delete Logic: Incorrect project count. Expected 2, got %d.\n", projectCount);
        }
        
        if (strcmp(projects[1].name, "P3") == 0) {
            printf("[PASS] Delete Logic: Array shifting is correct ('P3' is now at index 1).\n");
        } else {
            printf("[FAIL] Delete Logic: Array shifting failed. Expected 'P3', got '%s'.\n", projects[1].name);
        }
    } else {
        printf("[FAIL] Delete Logic: Test project 'P2_to_delete' not found.\n");
    }
    
    resetState();
}

void testUpdateLogic() {
    printf("\n--- Test: Update Logic ---\n");
    resetState();
    addTestProject("Project X", "01-01", "10-01", "Pending");
    
    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, "Project X") == 0) {
            found = 1;
            strncpy(projects[i].startDate, "05-01", MAX_LEN - 1); projects[i].startDate[MAX_LEN - 1] = '\0';
            strncpy(projects[i].evaluationResult, "Completed", MAX_LEN - 1); projects[i].evaluationResult[MAX_LEN - 1] = '\0';
            break;
        }
    }

    if (found) {
        if (strcmp(projects[0].startDate, "05-01") == 0 && strcmp(projects[0].evaluationResult, "Completed") == 0) {
            printf("[PASS] Update Logic: 'Project X' data updated successfully.\n");
        } else {
            printf("[FAIL] Update Logic: 'Project X' data not updated correctly. Got: %s, %s\n", projects[0].startDate, projects[0].evaluationResult);
        }
    } else {
        printf("[FAIL] Update Logic: Test project 'Project X' not found.\n");
    }
    
    resetState();
}


void runTests() {
    printf("\n#################################\n");
    printf("### Starting Unit Tests ###\n");
    printf("#################################\n");

    testCSV_IO("test_projects.csv"); 

    testDeleteLogic();
    
    testUpdateLogic();

    printf("\n#################################\n");
    printf("### All Tests Finished ###\n");
    printf("#################################\n");
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
            case 9:
                runTests();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}