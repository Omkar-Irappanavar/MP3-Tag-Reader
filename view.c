/* 
    Name: Omkar Irappanavar
    Date: 21/09/2024
    Description: mpeg (mp3 not opening) tag Reader/Editor
*/


#include <stdio.h>
#include <string.h>
#include "common.h"

unsigned int data_size;

Status view_info(details *info)
{

    if (!(v_open_files(info)))  // calling function to open the files
    {
        return failure;
    }

    if (!(v_check_size(info->fptr_source_fname)))   // calling function to check size of file
    {
        return failure;
    }

    if (!(v_check_version(info->fptr_source_fname)))    // calling function to check the version of file content
    {
        return failure;
    }

    printf(YELLOW "\n----------------- SELECTED VIEW DETAILS ----------------------\n\n" RESET);
    printf(MAGENTA "--------------------------------------------------------------\n" RESET);
    printf(CYAN "\t MP3 TAG READER AND EDITOR FOR ID3v2\n" RESET);
    printf(MAGENTA "--------------------------------------------------------------\n" RESET);

    fseek(info->fptr_source_fname, 10, SEEK_SET);

    for (int i = 0; i < 6; i++)
    {
        if (!(v_read_tag(info->fptr_source_fname)))     // calling function to read tag from file
        {
            return failure;
        }
        v_read_size(info->fptr_source_fname);   // calling function to read the data size from file

        v_read_data(info->fptr_source_fname);   // calling function to read the data from file
    }

    printf(MAGENTA "--------------------------------------------------------------\n" RESET);

    return success;
}

/* function to open the files */
Status v_open_files(details *info)
{
    // printf("INFO: Opening the required file\n");
    info->fptr_source_fname = fopen(info->source_fname, "r");   // opening the source file in read mode
    if (info->fptr_source_fname == NULL)
    {
        printf(RED "INFO: ERROR: %s file is not opened\n" RESET, info->source_fname);
        return failure;
    }

    // printf("INFO: %s file opened successfully\n",info->source_fname);
    return success;
}

/* function to check the size of file (empty file or not)*/
Status v_check_size(FILE *fptr)
{
    // printf("INFO: Checking mpeg file size\n");
    fseek(fptr, 0, SEEK_END);   // setting cursor at end of file
    if (ftell(fptr) < 1)
    {
        // printf(RED "INFO: ERROR: mpeg file is empty\n" RESET);
        printf(RED "INFO: ERROR: mp3 file is empty\n" RESET);
        return failure;
    }

    // printf("INFO: Done.\n");
    rewind(fptr);
    return success;
}

/* function to check the version */
Status v_check_version(FILE *fptr_source_fname)
{
    // printf("INFO: Checking version\n");
    char buf[4];

    fread(buf, 3, 1, fptr_source_fname);
    buf[3] = '\0';
    if (strcmp(buf, "ID3") != 0)    // checking the version
    {
        printf(RED "INFO: Error: %s version is Invalid\n" RESET, buf);
        return failure;
    }

    fread(&data_size, 2, 1, fptr_source_fname);

    if (data_size != 3)
    {
        printf(RED "INFO: ERROR: %s Version is different\n" RESET, buf);
        return failure;
    }

    // printf("INFO: Done.\n");
    return success;
}

/* function to read the tags from file */
Status v_read_tag(FILE *fptr_source_fname)
{
    char buf[5];

    fread(buf, 4, 1, fptr_source_fname);
    buf[4] = '\0';

    if (strcmp(buf, "TIT2") == 0)
        printf("TITLE\t:\t");
    else if (strcmp(buf, "TPE1") == 0)
        printf("ARTIST\t:\t");
    else if (strcmp(buf, "TALB") == 0)
        printf("Album\t:\t");
    else if (strcmp(buf, "TYER") == 0)
    // else if (strcmp(buf, "TDRC") == 0)
        printf("YEAR\t:\t");
    else if (strcmp(buf, "TCON") == 0)
    // else if (strcmp(buf, "TCOM") == 0)  
        printf("MUSIC\t:\t");
    else if (strcmp(buf, "COMM") == 0)
    // else if (strcmp(buf, "TCOP") == 0)
        printf("COMMENT\t:\t");
    else
    {
        buf[4] = '\0';
        printf(RED "INFO: ERROR: %s tag not matched\n" RESET, buf);
        return failure;
    }

    return success;
}

/* function to read the size of data */
Status v_read_size(FILE *fptr_source_fname)
{
    fread(&data_size, 4, 1, fptr_source_fname);

    // reversing the data size
    data_size = ((data_size & 0xFF000000) >> 24) | // Move byte 1 to byte 4
                  ((data_size & 0x00FF0000) >> 8) |  // Move byte 2 to byte 3
                  ((data_size & 0x0000FF00) << 8) |  // Move byte 3 to byte 2
                  ((data_size & 0x000000FF) << 24);  // Move byte 4 to byte 1

    // printf("%x\t",data_size);

    return success;
}

/* function to read data based on size*/
Status v_read_data(FILE *fptr_souce_fname)
{
    fseek(fptr_souce_fname, 3, SEEK_CUR);
    char buf[data_size];
    fread(buf, data_size - 1, 1, fptr_souce_fname);
    buf[data_size - 1] = '\0';

    for (int i = 0; i < data_size; i++)
    {
        printf("%c", buf[i]);
    }

    printf("\n");

    return success;
}
