#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001); // ให้ Console แสดงผล UTF-8
    SetConsoleCP(65001);       // ให้ scanf ใช้ UTF-8
    printf("ทดสอบภาษาไทย\n");
    return 0;
}

#define MAX_PROJECTS 100
#define MAX_LEN 100

char projectNames[MAX_PROJECTS][MAX_LEN];
char startDates[MAX_PROJECTS][MAX_LEN];
char endDates[MAX_PROJECTS][MAX_LEN];
char evaluationResults[MAX_PROJECTS][MAX_LEN];
int projectCount = 0;

// อ่านข้อมูลจากไฟล์ CSV
void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("ไม่สามารถเปิดไฟล์ %s\n", filename);
        return;
    }

    char line[512];
    projectCount = 0;

    // ข้ามบรรทัด header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && projectCount < MAX_PROJECTS) {
        char *token = strtok(line, ",");
        if (token) strcpy(projectNames[projectCount], token);

        token = strtok(NULL, ",");
        if (token) strcpy(startDates[projectCount], token);

        token = strtok(NULL, ",");
        if (token) strcpy(endDates[projectCount], token);

        token = strtok(NULL, "\n");
        if (token) strcpy(evaluationResults[projectCount], token);

        projectCount++;
    }
    fclose(file);
    printf("โหลดข้อมูลจากไฟล์ %s สำเร็จ! จำนวน %d โครงการ\n", filename, projectCount);
}

// เขียนข้อมูลลงไฟล์ CSV
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

// เพิ่มข้อมูลโครงการใหม่
void addProject() {
    if (projectCount >= MAX_PROJECTS) {
        printf("ข้อมูลเต็มแล้ว\n");
        return;
    }

    printf("เพิ่มข้อมูลโครงการใหม่:\n");

    printf("ชื่อโครงการ: ");
    scanf(" %[^\n]", projectNames[projectCount]);

    printf("วันที่เริ่ม (YYYY-MM-DD): ");
    scanf(" %[^\n]", startDates[projectCount]);

    printf("วันที่สิ้นสุด (YYYY-MM-DD): ");
    scanf(" %[^\n]", endDates[projectCount]);

    printf("ผลการประเมิน: ");
    scanf(" %[^\n]", evaluationResults[projectCount]);

    projectCount++;
    printf("เพิ่มข้อมูลสำเร็จ!\n");
}

// แสดงข้อมูลทั้งหมด
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

// ค้นหาข้อมูลด้วยชื่อโครงการหรือผลการประเมิน
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

// อัปเดตข้อมูลโครงการตามชื่อ
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

// ลบข้อมูลโครงการตามชื่อ
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

// แสดงเมนู
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

int main() {
    int choice;
    char filename[] = "projects.csv";

    while (1) {
        displayMenu();
        printf("เลือกเมนู: ");
        scanf("%d", &choice);

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

    return 0;
}
