#include "data.h"

#define MAX_OP_IN_LINE 7 /*Amount of cipher keys*/

void line_encryption(unsigned short int num, FILE *ob_file, int plug)
{
    static int linCounter = 100;
    int i, j, tmp;
    char ob[MAX_OP_IN_LINE];

    if (linCounter + plug > 1000) /*print the line number of the given wordcode*/
    {
        fprintf(ob_file, "%d ", (linCounter + plug));
    }
    else
        fprintf(ob_file, "0%d ", (linCounter + plug));

    for (i = 0; i < 7; i++) /*encrypting the number and insert to array*/
    {
        tmp = num & 0x3;
        switch (tmp)
        {
        case 0:
            ob[i] = '*';
            num = num >> 2;
            break;
        case 1:
            ob[i] = '#';
            num = num >> 2;
            break;
        case 2:
            ob[i] = '%';
            num = num >> 2;
            break;
        case 3:
            ob[i] = '!';
            num = num >> 2;
            break;
        default:
            break;
        }
    }
    /*print the array from end to start into encryption file*/
    for (j = strlen(ob) - 1; j > -1; j--)
    {
        fprintf(ob_file, "%c", ob[j]);
    }
    fprintf(ob_file, "\n");
}

int encryption(int **array, char *fileName, int instLength, int dataLength)
{
    int i, *tmp;
    FILE *ob_file;
    char *curFileName;

    curFileName = (char *)malloc(strlen(fileName) + 4);
    sprintf(curFileName, "%s%s", fileName, ".ob");

    printf("Running the encryption for %s! \n", fileName);
    if ((ob_file = fopen(curFileName, "r"))) /*if the file been created overwrite the file*/
        rewind(ob_file);

    if ((ob_file = fopen(curFileName, "w+")) == NULL)
    {
        perror("Error: ");
        return -1;
    }

    fprintf(ob_file, "  %d %d\n", instLength, dataLength); /* print the title of file amount of instructions ,  amount of data*/
    tmp = (*array);
    for (i = 0; i < (instLength + dataLength); i++) /*encrypting line by line. */
    {
        line_encryption(**(array), ob_file, i);
        (*array)++;
    }
    (*array) = tmp;
    free(curFileName);
    fclose(ob_file);
    return 0;
}
