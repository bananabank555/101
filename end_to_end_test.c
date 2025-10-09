#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

#define MAX_PROJECTS 100
#define MAX_LEN 100

char projectNames[MAX_PROJECTS][MAX_LEN];
char startDates[MAX_PROJECTS][MAX_LEN];
char endDates[MAX_PROJECTS][MAX_LEN];
char evaluationResults[MAX_PROJECTS][MAX_LEN];
int projectCount = 0;

void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ไม่สามารถเปิดไฟล์ %s\n", filename);
        return;
    }

    char line[512];
    projectCount = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && projectCount < MAX_PROJECTS) {
        char *token = strtok(line, ",");
        if (token) strcpy(projectNames[projectCount], token);

        token = strtok(NULL, ",");
        if (token) strcpy(startDates[projectCount], token);

        token = strtok(NULL, ",");
        if (token) strcpy(endDates[projectCount], token);

        token = strtok(NULL, "\n");
        if (token) {
            size_t len = strlen(token);
            if (len > 0 && token[len-1] == '\r') {
                token[len-1] = '\0';
            }
            strcpy(evaluationResults[projectCount], token);
        }

        projectCount++;
    }
    fclose(file);
    printf("โหลดข้อมูลจากไฟล์ %s สำเร็จ! จำนวน %d โครงการ\n", filename, projectCount);
}

void writeCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("ไม่สามารถเขียนไฟล์ %s\n", filename);
        return;
    }

    fprintf(file, "ProjectName,StartDate,EndDate,EvaluationResult\n");
    for (int i = 0; i < projectCount; i++) {
        fprintf(file, "%s,%s,%s,%s\n", projectNames[i], startDates[i], endDates[i], evaluationResults[i]);
    }

    fclose(file);
    printf("บันทึกข้อมูลลงไฟล์ %s สำเร็จ!\n", filename);
}

void addProject() {
    if (projectCount >= MAX_PROJECTS) {
        printf("ข้อมูลเต็มแล้ว\n");
        return;
    }

    printf("เพิ่มข้อมูลโครงการใหม่:\n");

    printf("ชื่อโครงการ: ");
    scanf(" %[^\n]", projectNames[projectCount]);

    printf("วันที่เริ่ม (DD-MM-YYYY): ");
    scanf(" %[^\n]", startDates[projectCount]);

    printf("วันที่สิ้นสุด (DD-MM-YYYY): ");
    scanf(" %[^\n]", endDates[projectCount]);

    printf("ผลการประเมิน: ");
    scanf(" %[^\n]", evaluationResults[projectCount]);

    projectCount++;
    printf("เพิ่มข้อมูลสำเร็จ!\n");
}

void showAllProjects() {
    if (projectCount == 0) {
        printf("ยังไม่มีข้อมูล\n");
        return;
    }

    printf("\n==== ข้อมูลโครงการทั้งหมด ====\n");
    for (int i = 0; i < projectCount; i++) {
        printf("%d. %s | %s - %s | %s\n", i+1, projectNames[i], startDates[i], endDates[i], evaluationResults[i]);
    }
}

void searchProject() {
    char keyword[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการหรือผลการประเมินที่ต้องการค้นหา: ");
    scanf(" %[^\n]", keyword);

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strstr(projectNames[i], keyword) != NULL || strstr(evaluationResults[i], keyword) != NULL) {
            printf("%d. %s | %s - %s | %s\n", i+1, projectNames[i], startDates[i], endDates[i], evaluationResults[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("ไม่พบข้อมูลที่ตรงกับคำค้นหา\n");
    }
}

void updateProject() {
    char name[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการที่ต้องการแก้ไข: ");
    scanf(" %[^\n]", name);

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projectNames[i], name) == 0) {
            found = 1;
            printf("แก้ไขข้อมูลโครงการ: %s\n", projectNames[i]);

            printf("วันที่เริ่มใหม่ (ปัจจุบัน: %s): ", startDates[i]);
            scanf(" %[^\n]", startDates[i]);

            printf("วันที่สิ้นสุดใหม่ (ปัจจุบัน: %s): ", endDates[i]);
            scanf(" %[^\n]", endDates[i]);

            printf("ผลการประเมินใหม่ (ปัจจุบัน: %s): ", evaluationResults[i]);
            scanf(" %[^\n]", evaluationResults[i]);

            printf("แก้ไขข้อมูลสำเร็จ!\n");
            break;
        }
    }
    if (!found) {
        printf("ไม่พบชื่อโครงการที่ระบุ\n");
    }
}

void deleteProject() {
    char name[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการที่ต้องการลบ: ");
    scanf(" %[^\n]", name);

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projectNames[i], name) == 0) {
            found = 1;
            for (int j = i; j < projectCount - 1; j++) {
                strcpy(projectNames[j], projectNames[j+1]);
                strcpy(startDates[j], startDates[j+1]);
                strcpy(endDates[j], endDates[j+1]);
                strcpy(evaluationResults[j], evaluationResults[j+1]);
            }
            projectCount--;
            printf("ลบข้อมูลสำเร็จ!\n");
            break;
        }
    }
    if (!found) {
        printf("ไม่พบชื่อโครงการที่ระบุ\n");
    }
}

void displayMenu() {
    printf("\n==== เมนูหลัก ====\n");
    printf("1. อ่านข้อมูลจากไฟล์ CSV\n");
    printf("2. เพิ่มข้อมูลโครงการใหม่\n");
    printf("3. ค้นหาข้อมูลโครงการ\n");
    printf("4. แก้ไขข้อมูลโครงการ\n");
    printf("5. ลบข้อมูลโครงการ\n");
    printf("6. แสดงข้อมูลทั้งหมด\n");
    printf("7. บันทึกข้อมูลลงไฟล์ CSV\n");
    printf("8. ออกจากโปรแกรม\n");
}

int runApp() {
    int choice;
    char filename[] = "projects.csv";

    while (1) {
        displayMenu();
        printf("เลือกเมนู: ");
        
        if (scanf("%d", &choice) != 1) {
             if (feof(stdin)) {
                 return 0; 
             }
             while (getchar() != '\n'); 
             printf("กรุณาใส่ตัวเลขเท่านั้น\n");
             continue;
        }

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
                printf("ออกจากโปรแกรม...\n");
                return 0;
            default:
                printf("เลือกเมนูไม่ถูกต้อง\n");
        }
    }
}

int checkOutput(const char* outputFilename, const char* expectedString, const char* testCaseName) {
    FILE *file = fopen(outputFilename, "r");
    if (!file) {
        printf("[FAIL] %s: ไม่สามารถเปิดไฟล์ Output เพื่อตรวจสอบ\n", testCaseName);
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, expectedString)) {
            fclose(file);
            printf("[PASS] %s: พบข้อความที่คาดหวัง '%s'\n", testCaseName, expectedString);
            return 1;
        }
    }

    fclose(file);
    printf("[FAIL] %s: ไม่พบข้อความที่คาดหวัง '%s'\n", testCaseName, expectedString);
    return 0;
}

void runEndToEndTest() {
    printf("\n#################################\n");
    printf("### เริ่ม End-to-End Test ###\n");
    printf("#################################\n");

    const char* inputFilename = "e2e_input.txt";
    const char* outputFilename = "e2e_output.txt";
    const char* dataFilename = "e2e_data.csv";
    
    FILE *inputFile = fopen(inputFilename, "w");
    if (!inputFile) {
        printf("[ERROR] ไม่สามารถสร้างไฟล์ Input ได้\n");
        return;
    }

    fprintf(inputFile, "2\n");
    fprintf(inputFile, "โครงการ Alpha\n");
    fprintf(inputFile, "01-01-2024\n");
    fprintf(inputFile, "31-01-2024\n");
    fprintf(inputFile, "Good\n");

    fprintf(inputFile, "2\n");
    fprintf(inputFile, "โครงการ Beta\n");
    fprintf(inputFile, "01-02-2024\n");
    fprintf(inputFile, "28-02-2024\n");
    fprintf(inputFile, "Pending\n");

    fprintf(inputFile, "6\n");

    fprintf(inputFile, "3\n");
    fprintf(inputFile, "Alpha\n");

    fprintf(inputFile, "4\n");
    fprintf(inputFile, "โครงการ Beta\n");
    fprintf(inputFile, "01-02-2024\n");
    fprintf(inputFile, "31-03-2024\n");
    fprintf(inputFile, "Completed\n");

    fprintf(inputFile, "5\n");
    fprintf(inputFile, "โครงการ Alpha\n");

    fprintf(inputFile, "6\n");
    
    fprintf(inputFile, "7\n");

    fprintf(inputFile, "8\n");

    fclose(inputFile);

    freopen(inputFilename, "r", stdin);
    freopen(outputFilename, "w", stdout);
    
    system("chcp 65001");
    setlocale(LC_ALL, "Thai"); 
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);

    runApp();

    fclose(stdin);
    fclose(stdout);
    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);

    printf("\n==== ผลลัพธ์ End-to-End Test ====\n");
    
    checkOutput(outputFilename, "เพิ่มข้อมูลสำเร็จ!", "Test 1 (Add)");
    
    checkOutput(outputFilename, "โครงการ Alpha | 01-01-2024 - 31-01-2024 | Good", "Test 2 (Search)");

    checkOutput(outputFilename, "แก้ไขข้อมูลสำเร็จ!", "Test 3 (Update - Message)");
    
    checkOutput(outputFilename, "ลบข้อมูลสำเร็จ!", "Test 4 (Delete)");
    
    checkOutput(outputFilename, "1. โครงการ Beta | 01-02-2024 - 31-03-2024 | Completed", "Test 5 (Final Data Check)");
    
    checkOutput(outputFilename, "บันทึกข้อมูลลงไฟล์ e2e_data.csv สำเร็จ!", "Test 6 (Write CSV)");

    remove(inputFilename);
    remove(outputFilename);
    remove(dataFilename);

    printf("\n#################################\n");
    printf("### End-to-End Test เสร็จสิ้น ###\n");
    printf("#################################\n");
}


int main() {
    system("chcp 65001");
    setlocale(LC_ALL, "Thai"); 
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);

    int choice;

    while (1) {
        printf("\n==== เมนูหลัก ====\n");
        printf("1. อ่านข้อมูลจากไฟล์ CSV\n");
        printf("2. เพิ่มข้อมูลโครงการใหม่\n");
        printf("3. ค้นหาข้อมูลโครงการ\n");
        printf("4. แก้ไขข้อมูลโครงการ\n");
        printf("5. ลบข้อมูลโครงการ\n");
        printf("6. แสดงข้อมูลทั้งหมด\n");
        printf("7. บันทึกข้อมูลลงไฟล์ CSV\n");
        printf("8. ออกจากโปรแกรม\n");
        printf("9. รัน End-to-End Test (E2E)\n");
        printf("เลือกเมนู: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("กรุณาใส่ตัวเลขเท่านั้น\n");
            continue;
        }

        if (choice == 9) {
            runEndToEndTest();
        } else if (choice >= 1 && choice <= 8) {
            
            char filename[] = "projects.csv";
            
            switch (choice) {
                case 1: readCSV(filename); break;
                case 2: addProject(); break;
                case 3: searchProject(); break;
                case 4: updateProject(); break;
                case 5: deleteProject(); break;
                case 6: showAllProjects(); break;
                case 7: writeCSV(filename); break;
                case 8: printf("ออกจากโปรแกรม...\n"); return 0;
            }
        } else {
            printf("เลือกเมนูไม่ถูกต้อง\n");
        }
    }

    return 0;
}