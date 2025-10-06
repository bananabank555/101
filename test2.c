#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROJECTS 100
#define MAX_FIELD 100

char data[MAX_PROJECTS][4][MAX_FIELD];
int projectCount = 0;

void splitLine(char *line, char fields[4][MAX_FIELD]) {
    int i = 0, j = 0, field = 0;
    while (line[i] != '\0' && field < 4) {
        if (line[i] == ',') {
            fields[field][j] = '\0';
            field++;
            j = 0;
        } else {
            fields[field][j++] = line[i];
        }
        i++;
    }
    fields[field][j] = '\0';
}

// อ่านข้อมูลไฟล์ CSV
void readCSV(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("ไม่สามารถเปิดไฟล์ %s ได้\n", filename);
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp);

    projectCount = 0;
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char fields[4][MAX_FIELD];
        splitLine(line, fields);

        for (int i = 0; i < 4; i++) {
            strcpy(data[projectCount][i], fields[i]);
        }
        projectCount++;
    }

    fclose(fp);
    printf("อ่านข้อมูลแล้ว %d รายการ\n", projectCount);
}

// แสดงข้อมูลทั้งหมด
void displayAll() {
    printf("\n===== ข้อมูลทั้งหมด =====\n");
    for (int i = 0; i < projectCount; i++) {
        printf("%d. %s | %s - %s | %s\n", i + 1,
               data[i][0], data[i][1], data[i][2], data[i][3]);
    }
    printf("==========================\n");
}

// เพิ่มโครงการ
void addProject() {
    if (projectCount >= MAX_PROJECTS) {
        printf("เกินจำนวนสูงสุด\n");
        return;
    }

    printf("ชื่อโครงการ: ");
    scanf(" %[^\n]", data[projectCount][0]);

    printf("วันเริ่มต้น (yyyy-mm-dd): ");
    scanf(" %[^\n]", data[projectCount][1]);

    printf("วันสิ้นสุด (yyyy-mm-dd): ");
    scanf(" %[^\n]", data[projectCount][2]);

    printf("ผลการประเมิน: ");
    scanf(" %[^\n]", data[projectCount][3]);

    projectCount++;
    printf("เพิ่มข้อมูลเรียบร้อย\n");
}

// ค้นหาโครงการ
void searchProject() {
    char keyword[MAX_FIELD];
    printf("พิมพ์คำค้นหา: ");
    scanf(" %[^\n]", keyword);

    int found = 0;
    for (int i = 0; i < projectCount; i++) {
        if (strstr(data[i][0], keyword) || strstr(data[i][3], keyword)) {
            printf("%d. %s | %s - %s | %s\n", i + 1,
                   data[i][0], data[i][1], data[i][2], data[i][3]);
            found = 1;
        }
    }
    if (!found) {
        printf("ไม่พบข้อมูล\n");
    }
}

// แก้ไขผลประเมิน
void updateProject() {
    displayAll();
    int index;
    printf("เลือกหมายเลขที่จะแก้ไข: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > projectCount) {
        printf("หมายเลขไม่ถูกต้อง\n");
        return;
    }

    printf("ผลประเมินใหม่: ");
    scanf(" %[^\n]", data[index - 1][3]);
    printf("อัปเดตเรียบร้อย\n");
}

// ลบโครงการ
void deleteProject() {
    displayAll();
    int index;
    printf("เลือกหมายเลขที่จะลบ: ");
    scanf("%d", &index);

    if (index < 1 || index > projectCount) {
        printf("หมายเลขไม่ถูกต้อง\n");
        return;
    }

    for (int i = index - 1; i < projectCount - 1; i++) {
        for (int j = 0; j < 4; j++) {
            strcpy(data[i][j], data[i + 1][j]);
        }
    }

    projectCount--;
    printf("ลบเรียบร้อย\n");
}

// บันทึกข้อมูลลงไฟล์
void writeCSV(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("ไม่สามารถเขียนไฟล์ได้\n");
        return;
    }

    fprintf(fp, "ProjectName,StartDate,EndDate,EvaluationResult\n");
    for (int i = 0; i < projectCount; i++) {
        fprintf(fp, "%s,%s,%s,%s\n",
                data[i][0], data[i][1], data[i][2], data[i][3]);
    }

    fclose(fp);
    printf("บันทึกข้อมูลเรียบร้อย\n");
}

// เมนูหลัก
void displayMenu() {
    printf("\n===== เมนู =====\n");
    printf("1. อ่านจากไฟล์ CSV\n");
    printf("2. แสดงข้อมูลทั้งหมด\n");
    printf("3. เพิ่มข้อมูลใหม่\n");
    printf("4. ค้นหาโครงการ\n");
    printf("5. แก้ไขผลการประเมิน\n");
    printf("6. ลบโครงการ\n");
    printf("7. บันทึกลงไฟล์ CSV\n");
    printf("0. ออกจากโปรแกรม\n");
    printf("เลือก: ");
}

int main() {
    int choice;
    char filename[] = "projects.csv";

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: readCSV(filename); break;
            case 2: displayAll(); break;
            case 3: addProject(); break;
            case 4: searchProject(); break;
            case 5: updateProject(); break;
            case 6: deleteProject(); break;
            case 7: writeCSV(filename); break;
            case 0: printf("จบการทำงาน\n"); break;
            default: printf("เมนูไม่ถูกต้อง\n");
        }
    } while (choice != 0);

    return 0;
}
