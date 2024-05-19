#include "data.h"

#define MAX_OP_IN_LINE 7

char encrpt[MAX_OP_IN_LINE];

void line_encryption(unsigned short int num, FILE *ob_file)
{
    static int linCounter = 100;
    int i, j, tmp;
    char ob[MAX_OP_IN_LINE];

    if (linCounter > 1000)
    {
        fprintf(ob_file, "%d ", linCounter);
    }
    else
        fprintf(ob_file, "0%d ", linCounter);

    for (i = 0; i < 7; i++)
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
    linCounter++;
    for (j = strlen(ob) - 1; j > -1; j--)
    {
        fprintf(ob_file, "%c", ob[j]);
    }
    fprintf(ob_file, "\n");
}

int encryption(int **array, char *fileName, int instLength, int dataLength)
{
    int i;
    FILE *ob_file;
    char *curFileName;

    curFileName = (char *)malloc(strlen(fileName) + 4);
    sprintf(curFileName, "%s%s", fileName, ".ob");

    if ((ob_file = fopen(curFileName, "r")))
        rewind(ob_file);

    if ((ob_file = fopen(curFileName, "w+")) == NULL)
    {
        perror("Error: ");
        return -1;
    }
    /*
    print the title of file
    amount of instructions ,  amount of data
    */
    fprintf(ob_file, "  %d %d\n", instLength, dataLength);

    for (i = 0; i < (instLength + dataLength); i++)
    {
        line_encryption(**(array), ob_file);
        (*array)++;
    }
    printf("Running the encryption for %s! \n", fileName);
    free(curFileName);
    fclose(ob_file);
    return 0;
}
