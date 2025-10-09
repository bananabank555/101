#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

// Forward declarations
void resetState();
void addTestProject(const char* name, const char* start, const char* end, const char* result);
void writeCSV(const char *filename);
void readCSV(const char *filename);
char* strcasestr_custom(const char* haystack, const char* needle);

// --- Core Logic Functions (simplified for testing) ---

void resetState() {
    projectCount = 0;
}

void addTestProject(const char* name, const char* start, const char* end, const char* result) {
    if (projectCount < MAX_PROJECTS) {
        strncpy(projects[projectCount].name, name, MAX_LEN - 1);
        projects[projectCount].name[MAX_LEN - 1] = '\0';
        strncpy(projects[projectCount].startDate, start, MAX_LEN - 1);
        projects[projectCount].startDate[MAX_LEN - 1] = '\0';
        strncpy(projects[projectCount].endDate, end, MAX_LEN - 1);
        projects[projectCount].endDate[MAX_LEN - 1] = '\0';
        strncpy(projects[projectCount].evaluationResult, result, MAX_LEN - 1);
        projects[projectCount].evaluationResult[MAX_LEN - 1] = '\0';
        projectCount++;
    }
}

void writeCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Test Error: Cannot write to file %s\n", filename);
        return;
    }
    fprintf(file, "ProjectName,StartDate,EndDate,EvaluationResult\n");
    for (int i = 0; i < projectCount; i++) {
        fprintf(file, "%s,%s,%s,%s\n", projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }
    fclose(file);
}

void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return;
    }
    char line[512];
    projectCount = 0;
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file) && projectCount < MAX_PROJECTS) {
        line[strcspn(line, "\r\n")] = 0;
        char *p = strtok(line, ",");
        if(p) strncpy(projects[projectCount].name, p, MAX_LEN);
        p = strtok(NULL, ",");
        if(p) strncpy(projects[projectCount].startDate, p, MAX_LEN);
        p = strtok(NULL, ",");
        if(p) strncpy(projects[projectCount].endDate, p, MAX_LEN);
        p = strtok(NULL, ",");
        if(p) strncpy(projects[projectCount].evaluationResult, p, MAX_LEN);
        projectCount++;
    }
    fclose(file);
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


// --- Test Assertion Helpers ---

void assert_equals_int(int expected, int actual, const char* test_name) {
    if (expected == actual) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s: Expected %d, but got %d\n", test_name, expected, actual);
    }
}

void assert_equals_str(const char* expected, const char* actual, const char* test_name) {
    if (strcmp(expected, actual) == 0) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s: Expected '%s', but got '%s'\n", test_name, expected, actual);
    }
}

// --- Test Cases ---

void test_add_write_read() {
    printf("\n--- Test: Add, Write, and Read CSV ---\n");
    resetState();
    addTestProject("Project Alpha", "01-01-2024", "31-01-2024", "Good");
    addTestProject("Project Beta", "15-02-2024", "28-02-2024", "Excellent");

    assert_equals_int(2, projectCount, "Project count after adding two projects");
    assert_equals_str("Project Alpha", projects[0].name, "First project's name");

    const char* test_filename = "e2e_test_projects.csv";
    writeCSV(test_filename);

    printf("Resetting state and reading from CSV...\n");
    resetState();
    readCSV(test_filename);

    assert_equals_int(2, projectCount, "Project count after reading from CSV");
    assert_equals_str("Project Beta", projects[1].name, "Second project's name after reading");
    assert_equals_str("Excellent", projects[1].evaluationResult, "Second project's evaluation after reading");

    remove(test_filename);
}

void test_search() {
    printf("\n--- Test: Search Project ---\n");
    resetState();
    addTestProject("FindMe", "01-01-2024", "01-01-2024", "Result A");
    addTestProject("Another Project", "02-02-2024", "02-02-2024", "AlsoFindMe");
    addTestProject("No Match", "03-03-2024", "03-03-2024", "Result B");

    int found_count = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcasestr_custom(projects[i].name, "FindMe") != NULL || strcasestr_custom(projects[i].evaluationResult, "FindMe") != NULL) {
            found_count++;
        }
    }
    assert_equals_int(2, found_count, "Searching for keyword 'FindMe'");
}

void test_delete() {
    printf("\n--- Test: Delete Project ---\n");
    resetState();
    addTestProject("Project One", "d1", "d2", "r1");
    addTestProject("ToDelete", "d3", "d4", "r2");
    addTestProject("Project Three", "d5", "d6", "r3");

    int delete_idx = -1;
    for(int i=0; i<projectCount; i++) {
        if(strcmp(projects[i].name, "ToDelete") == 0) {
            delete_idx = i;
            break;
        }
    }

    if (delete_idx != -1) {
        for (int i = delete_idx; i < projectCount - 1; i++) {
            projects[i] = projects[i+1];
        }
        projectCount--;
    }

    assert_equals_int(2, projectCount, "Project count after deletion");
    assert_equals_str("Project Three", projects[1].name, "Correct project is at index 1 after deletion");
}

// --- Main Test Runner ---

int main() {
    printf("===== Running End-to-End Tests =====\n");

    test_add_write_read();
    test_search();
    test_delete();

    printf("\n===== All Tests Finished =====\n");

    return 0;
}