#include <stdio.h>

void AddTable()
{
    printf("This is add menu");
}

void DeleteTable()
{
    printf("This is delete menu");
}
void ReadFile(){
    
}

void DisplayMenu()
{

    char menu;
    printf("a. Add Table\n");
    printf("b. Delete Table\n");
    printf("Please select Menu: ");
    scanf("%c", &menu);

    switch (menu)
    {
    case 'a':
        AddTable();
        break;

    case 'b':
        DeleteTable();
        break;

    default:
        printf("Bye Bye");
        break;
    }
}
int main()
{
    // printf("Hello World");
    DisplayMenu();

    FILE *file = fopen("evaluate.csv", "r");

    if (file == NULL)
    {
        printf("File %p\n", file);
        printf("File cannot opened.");

        return 1;
    }

    return 0;
}