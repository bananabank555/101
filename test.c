#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

#define MAX_PROJECTS 100
#define MAX_LEN 100

// ใช้ struct เพื่อจัดกลุ่มข้อมูลให้เป็นระเบียบและจัดการง่ายขึ้น
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
        printf("ไม่สามารถเปิดไฟล์ %s\n", filename);
    }

    char line[512];
    projectCount = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && projectCount < MAX_PROJECTS) {
        char *token = strtok(line, ",");
        // ใช้ strncpy เพื่อความปลอดภัย ป้องกัน Buffer Overflow
        if (token) {
            strncpy(projects[projectCount].name, token, MAX_LEN - 1);
            projects[projectCount].name[MAX_LEN - 1] = '\0'; // Ensure null-termination
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
        fprintf(file, "%s,%s,%s,%s\n", projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }

    fclose(file);
    printf("บันทึกข้อมูลลงไฟล์ %s สำเร็จ!\n", filename);
}

void addProject() {
    if (projectCount >= MAX_PROJECTS) {
        printf("ข้อมูลเต็มแล้ว\n");
        return;
    }

    // ใช้ Buffer และ fgets เพื่อการรับ Input ที่ปลอดภัย
    char buffer[MAX_LEN * 2]; // Buffer ขนาดใหญ่ขึ้นเล็กน้อยเพื่อตรวจจับ input ที่ยาวเกิน

    printf("เพิ่มข้อมูลโครงการใหม่:\n");

    printf("ชื่อโครงการ: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].name, buffer, MAX_LEN - 1);
    projects[projectCount].name[MAX_LEN - 1] = '\0';

    printf("วันที่เริ่ม (DD-MM-YYYY): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].startDate, buffer, MAX_LEN - 1);
    projects[projectCount].startDate[MAX_LEN - 1] = '\0';

    printf("วันที่สิ้นสุด (DD-MM-YYYY): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].endDate, buffer, MAX_LEN - 1);
    projects[projectCount].endDate[MAX_LEN - 1] = '\0';

    printf("ผลการประเมิน: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].evaluationResult, buffer, MAX_LEN - 1);
    projects[projectCount].evaluationResult[MAX_LEN - 1] = '\0';

    // ตรวจสอบว่าชื่อโครงการซ้ำหรือไม่ (ตัวอย่าง business logic เพิ่มเติม)
    // for (int i = 0; i < projectCount; i++) { ... }
    
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
        printf("%d. %s | %s - %s | %s\n", i + 1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }
}

void searchProject() {
    char keyword[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการหรือผลการประเมินที่ต้องการค้นหา: ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL) return;
    keyword[strcspn(keyword, "\r\n")] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        // strstr เป็น case-sensitive, ถ้าต้องการไม่สน case ต้องใช้ strcasestr หรือเขียนฟังก์ชันเอง
        if (strstr(projects[i].name, keyword) != NULL || strstr(projects[i].evaluationResult, keyword) != NULL) {
            if (!found) {
                printf("\nผลการค้นหา:\n");
            }
            printf("%d. %s | %s - %s | %s\n", i + 1, projects[i].name, projects[i].endDate, projects[i].endDate, projects[i].evaluationResult);
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
    if (fgets(name, sizeof(name), stdin) == NULL) return;
    name[strcspn(name, "\r\n")] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, name) == 0) {
            found = 1;
            printf("แก้ไขข้อมูลโครงการ: %s\n", projects[i].name);

            char buffer[MAX_LEN];

            printf("วันที่เริ่มใหม่ (ปัจจุบัน: %s): ", projects[i].startDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].startDate, buffer, MAX_LEN - 1);

            printf("วันที่สิ้นสุดใหม่ (ปัจจุบัน: %s): ", projects[i].endDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].endDate, buffer, MAX_LEN - 1);

            printf("ผลการประเมินใหม่ (ปัจจุบัน: %s): ", projects[i].evaluationResult);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].evaluationResult, buffer, MAX_LEN - 1);

            // หมายเหตุ: โค้ดนี้ยังไม่รองรับชื่อโครงการซ้ำ
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
    if (fgets(name, sizeof(name), stdin) == NULL) return;
    name[strcspn(name, "\r\n")] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, name) == 0) {
            found = 1;
            // เลื่อนข้อมูลในอาร์เรย์ (struct assignment ทำให้ง่ายขึ้น)
            for (int j = i; j < projectCount - 1; j++) {
                projects[j] = projects[j+1];
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

int main() {
    system("chcp 65001");
    setlocale(LC_ALL, "Thai"); 
    SetConsoleOutputCP(65001); 
    SetConsoleCP(65001);       

    int choice;
    char filename[] = "projects.csv";

    while (1) {
        displayMenu();
        printf("เลือกเมนู: ");
        
        // ปรับปรุงการรับเมนูให้ดีขึ้น
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // เคลียร์ input buffer ที่ผิดพลาด
            printf("กรุณาใส่ตัวเลขเท่านั้น\n");
            continue;
        }
        // เคลียร์ \n ที่เหลือจากการกด Enter หลังใส่ตัวเลข
        while (getchar() != '\n');

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