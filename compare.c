#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILE1_PATH "config-new"
#define FILE2_PATH "config-old"
#define FILE3_PATH "compare_out1"                   
#define DATA_LEN 80

int main(int argc, char **argv)
{
    FILE *file1, *file2, *file3;
    char *data1, *data2;
    char *str_idx = NULL;
    bool is_find = false;
    data1 = malloc(DATA_LEN);
    if (NULL == data1) {
        printf("malloc data1 failed!\n");
    }
    data2 = malloc(DATA_LEN);
    if (NULL == data2) {
        printf("malloc data2 failed!\n");
        goto free_data1;
    }
    file1 = fopen (FILE1_PATH, "r");
    if (NULL == file1) {
        printf("open file1 error:%s\n", FILE1_PATH); 
        goto free_data2;
    }
    file2 = fopen (FILE2_PATH, "r");
    if (NULL == file2) {
        printf("open file2 error:%s\n", FILE2_PATH); 
        goto close_file1;
    }
    file3 = fopen (FILE3_PATH, "w+");
    if (NULL == file3) {
        printf("open file3 error:%s\n", FILE3_PATH); 
        goto close_file2;
    }
    sprintf(data1, "file:%s have but file:%s dont have!\n", FILE1_PATH, FILE2_PATH);
    fwrite(data1, strlen(data1), 1, file3);
    memset(data1, 0, DATA_LEN);
    memset(data2, 0, DATA_LEN);
    while (fgets(data1, DATA_LEN, file1)) {
        str_idx = strchr(data1, '#');
        if (str_idx)
            continue;
        str_idx = strchr(data1, '=');
        if (str_idx) {
            if (*(str_idx+1) == 'n')
                continue;
            while (fgets(data2, DATA_LEN, file2)) {
                if (0 == strncmp(data2, data1, str_idx-data1)){
                    if (0 == strcmp(data2, data1)) {
                        is_find = true;
                        break;
                    }
                }
            } 
            fseek(file2, 0, SEEK_SET);
            if (true == is_find)
                is_find = false;
            else
                fwrite(data1, strlen(data1), 1, file3);
        }
        memset(data1, 0, DATA_LEN);
        memset(data2, 0, DATA_LEN);
    }

close_file3:
    fclose(file3);
close_file2:
    fclose(file2);
close_file1:
    fclose(file1);
free_data2:
    free(data2);
free_data1:
    free(data1);
    return 0;
}
