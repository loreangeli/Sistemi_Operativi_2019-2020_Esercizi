
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
char* save_ptr=NULL;
char src[] = "1.2,3.5,4.2";
char *first = strtok_r(src, ",", &save_ptr);


do 
{
    char *part;
    printf("[%s]\n", first);
    part = strtok(first, ".");
    while (part != NULL)
    {
        printf(" [%s]\n", part);
        part = strtok(NULL, ".");
    }	

} while ((first = strtok_r(save_ptr, ",", &save_ptr)) != NULL);


    return 0;
}
