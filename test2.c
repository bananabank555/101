#include <stdio.h>
#include <string.h>

#define max100

char projectname[max100]
char startdate[max][20]
char enddate[max][20]
char result[max][50]
int count = 0;

void displaymanu(){
    printf("\n====เมนูproject====\n");
    printf("เพิ่มโครงการสุดจ๊าบ\n");
    printf("การแสดงข้อมูลล\n");
    printf("ค้นหาโครงการ เจอมั้ยจ้ะ\n");
    printf("แก้ไข และวานิช\n");
    printf("โครงการถูกลบ แต่เขาไม่เคยถูกลบเลย\n");
    printf("ออกจากโครงการ ออกละ\n");
    printf("เลือกเมนู\n");
void addproject(){
    if (count>=max){
        printf("เต็มแล้ว เพิ่มกว่านี้มีอ้วก\n");
        return;
    }
}
}
int main()
{
    printf("Hello World");

    return 0;
}