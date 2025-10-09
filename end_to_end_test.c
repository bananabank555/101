#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
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
        printf("ไม่สามารถเปิดไฟล์ %s\n", filename);
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

    printf("เพิ่มข้อมูลโครงการใหม่:\n");
    char buffer[MAX_LEN];

    printf("ชื่อโครงการ: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0'; 
    strncpy(projects[projectCount].name, buffer, MAX_LEN - 1);
    projects[projectCount].name[MAX_LEN - 1] = '\0';

    printf("วันที่เริ่ม (DD-MM-YYYY): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].startDate, buffer, MAX_LEN - 1);
    projects[projectCount].startDate[MAX_LEN - 1] = '\0';

    printf("วันที่สิ้นสุด (DD-MM-YYYY): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].endDate, buffer, MAX_LEN - 1);
    projects[projectCount].endDate[MAX_LEN - 1] = '\0';

    printf("ผลการประเมิน: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(projects[projectCount].evaluationResult, buffer, MAX_LEN - 1);
    projects[projectCount].evaluationResult[MAX_LEN - 1] = '\0';
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
        printf("%d. %s | %s - %s | %s\n", i+1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
    }
}

void searchProject() {
    char keyword[MAX_LEN];
    char buffer[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการหรือผลการประเมินที่ต้องการค้นหา: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(keyword, buffer, MAX_LEN - 1);
    keyword[MAX_LEN - 1] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcasestr_custom(projects[i].name, keyword) != NULL || strcasestr_custom(projects[i].evaluationResult, keyword) != NULL) {

            printf("%d. %s | %s - %s | %s\n", i+1, projects[i].name, projects[i].startDate, projects[i].endDate, projects[i].evaluationResult);
            found = 1;
        }
    }

    if (!found) {
        printf("ไม่พบข้อมูลที่ตรงกับคำค้นหา\n");
    }
}

void updateProject() {
    char name[MAX_LEN];
    char buffer[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการที่ต้องการแก้ไข: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    strncpy(name, buffer, MAX_LEN - 1);
    name[MAX_LEN - 1] = '\0';

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strcmp(projects[i].name, name) == 0) {
            found = 1;
            printf("แก้ไขข้อมูลโครงการ: %s\n", projects[i].name);

            printf("วันที่เริ่มใหม่ (ปัจจุบัน: %s): ", projects[i].startDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].startDate, buffer, MAX_LEN - 1);
            projects[i].startDate[MAX_LEN - 1] = '\0';

            printf("วันที่สิ้นสุดใหม่ (ปัจจุบัน: %s): ", projects[i].endDate);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].endDate, buffer, MAX_LEN - 1);
            projects[i].endDate[MAX_LEN - 1] = '\0';

            printf("ผลการประเมินใหม่ (ปัจจุบัน: %s): ", projects[i].evaluationResult);
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strncpy(projects[i].evaluationResult, buffer, MAX_LEN - 1);
            projects[i].evaluationResult[MAX_LEN - 1] = '\0';

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
    char buffer[MAX_LEN];
    printf("กรุณากรอกชื่อโครงการที่ต้องการลบ: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { printf("เกิดข้อผิดพลาดในการอ่านข้อมูล.\n"); return; }
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
    printf("9. รัน Unit Tests (ทดสอบการทำงานของฟังก์ชัน)\n");
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
    printf("\n--- Test: การอ่าน/เขียนไฟล์ CSV ---\n");
    resetState();

    FILE *tempFile = fopen(testFilename, "w");
    if (!tempFile) {
        printf("[FAIL] ไม่สามารถสร้างไฟล์ทดสอบ: %s\n", testFilename);
        return;
    }
    fprintf(tempFile, "ProjectName,StartDate,EndDate,EvaluationResult\n");
    fprintf(tempFile, "Project Alpha,01-01-2024,31-01-2024,Good\n");
    fprintf(tempFile, "Project Beta,15-02-2024,28-02-2024,Excellent\n");
    fclose(tempFile);

    readCSV(testFilename);

    if (projectCount == 2) {
        printf("[PASS] readCSV: โหลดจำนวนโครงการถูกต้อง (%d).\n", projectCount);
    } else {
        printf("[FAIL] readCSV: คาดหวัง 2 โครงการ, ได้ %d.\n", projectCount);
    }

    if (strcmp(projects[0].name, "Project Alpha") == 0 &&
        strcmp(projects[0].evaluationResult, "Good") == 0) {
        printf("[PASS] readCSV: ตรวจสอบข้อมูล (Project Alpha) ผ่าน.\n");
    } else {
        printf("[FAIL] readCSV: ตรวจสอบข้อมูล (Project Alpha) ไม่ผ่าน. Got: %s, %s\n", projects[0].name, projects[0].evaluationResult);
    }
    
    const char* writeFilename = "temp_output.csv";
    writeCSV(writeFilename);

    resetState();
    readCSV(writeFilename); 
    if (projectCount == 2 && strcmp(projects[1].name, "Project Beta") == 0 && strcmp(projects[1].evaluationResult, "Excellent") == 0) {
        printf("[PASS] writeCSV: เนื้อหาไฟล์ถูกเขียนและตรวจสอบผ่าน.\n");
    } else {
        printf("[FAIL] writeCSV: การตรวจสอบไฟล์ที่เขียนล้มเหลว. projectCount: %d, project name: %s, eval: %s\n", projectCount, projects[1].name, projects[1].evaluationResult);
    }
    
    remove(testFilename);
    remove(writeFilename);
}

void testDeleteLogic() {
    printf("\n--- Test: ตรรกะการลบข้อมูล (Delete Logic) ---\n");
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
            printf("[PASS] Delete Logic: จำนวนโครงการถูกต้องหลังการลบ (%d).\n", projectCount);
        } else {
            printf("[FAIL] Delete Logic: จำนวนโครงการไม่ถูกต้อง คาดหวัง 2, ได้ %d.\n", projectCount);
        }
        
        if (strcmp(projects[1].name, "P3") == 0) {
            printf("[PASS] Delete Logic: การเลื่อนข้อมูลในอาร์เรย์สำเร็จ (P3 อยู่ที่ดัชนี 1).\n");
        } else {
            printf("[FAIL] Delete Logic: การเลื่อนข้อมูลล้มเหลว คาดหวัง P3, ได้ %s.\n", projects[1].name);
        }
    } else {
        printf("[FAIL] Delete Logic: ไม่พบโครงการ P2_to_delete.\n");
    }
    
    resetState();
}

void testUpdateLogic() {
    printf("\n--- Test: ตรรกะการแก้ไขข้อมูล (Update Logic) ---\n");
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
            printf("[PASS] Update Logic: ข้อมูล Project X ได้รับการแก้ไขสำเร็จ.\n");
        } else {
            printf("[FAIL] Update Logic: ข้อมูล Project X แก้ไขไม่สำเร็จ. Got: %s, %s\n", projects[0].startDate, projects[0].evaluationResult);
        }
    } else {
        printf("[FAIL] Update Logic: ไม่พบ Project X.\n");
    }
    
    resetState();
}


void runTests() {
    printf("\n#################################\n");
    printf("### เริ่มการทดสอบ Unit Tests ###\n");
    printf("#################################\n");

    testCSV_IO("test_projects.csv"); 

    testDeleteLogic();
    
    testUpdateLogic();

    printf("\n#################################\n");
    printf("### เสร็จสิ้นการทดสอบทั้งหมด ###\n");
    printf("#################################\n");
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
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("กรุณาใส่ตัวเลขเท่านั้น\n");
            while (getchar() != '\n'); 
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
            case 9:
                runTests();
                break;
            default:
                printf("เลือกเมนูไม่ถูกต้อง\n");
        }
    }

    return 0;
}
