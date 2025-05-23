/* 
    Name: Omkar Irappanavar
    Date: 21/09/2024
    Description: mpeg (mp3 not opening) tag Reader/Editor
*/

#include <stdio.h>
#include <string.h>
#include "common.h"

unsigned int e_data_size;
char temp_tag[5], data[50], flag[4];

Status edit_info(details *info)
{
    // info->duplicate_fname = "duplicate.mpeg";   // taking extra file to copy data
    info->duplicate_fname = "duplicate.mp3";   // taking extra file to copy data


    if (!(e_open_files(info)))  // calling function to open the required files
    {
        return failure;
    }

    if (!(e_check_size(info->fptr_source_fname)))   // calling function to check the size of file
    {
        return failure;
    }

    if (!(e_check_version(info->fptr_source_fname)))    // calling function to check the version
    {
        return failure;
    }

    printf(YELLOW "\n----------------- SELECTED EDIT DETAILS ----------------------\n\n" RESET);
    printf(MAGENTA "------------- SELECTED EDIT OPTION -------------\n" RESET);

    // fseek(info->fptr_source_fname, 10, SEEK_SET);
    rewind(info->fptr_source_fname);
    fread(data, 10, 1, info->fptr_source_fname);
    fwrite(data, 10, 1, info->fptr_duplicate_fname);

    for (int i = 0; i < 6; i++)
    {

        e_read_tag(info);   // calling function to read the tags from file

        fwrite(temp_tag, 4, 1, info->fptr_duplicate_fname);  // copying the tag into duplicate file

        e_read_size(info);      // calling function to read the data size 


        e_read_data(info);      // calling function to read the data from file
        if (strcmp(temp_tag, info->tag) == 0)
        {
            strcpy(data, info->change_data);
            e_data_size = strlen(info->change_data);
            e_data_size = e_data_size + 1;
        
            /* based on user given command printing info by matching the tags*/
            if (strcmp(info->tag, "TIT2") == 0)
            {
                printf(CYAN "-------------- CHANGE THE TITLE --------------\n\n" RESET);
                printf("TITLE\t: %s\n", data);
                printf(MAGENTA "------------------------------------------------\n" RESET);
            }
            else if (strcmp(info->tag, "TPE1") == 0)
            {
                printf(CYAN"-------------- CHANGE THE ARTIST --------------\n\n"RESET);
                printf("ARTIST\t: %s\n", data);
                printf(MAGENTA"------------------------------------------------\n"RESET);
            }
            else if (strcmp(info->tag, "TALB") == 0)
            {
                printf(CYAN"-------------- CHANGE THE Album --------------\n\n"RESET);
                printf("ALBUM\t: %s\n", data);
                printf(MAGENTA"------------------------------------------------\n"RESET);
            }
            else if (strcmp(info->tag, "TYER") == 0)
            {
                printf(CYAN"-------------- CHANGE THE YEAR --------------\n\n"RESET);
                printf("YEAR\t: %s\n", data);
                printf(MAGENTA"------------------------------------------------\n"RESET);
            }
            else if (strcmp(info->tag, "TCON") == 0)
            {
                printf(CYAN"-------------- CHANGE THE MUSIC --------------\n\n"RESET);
                printf("MUSIC\t: %s\n", data);
                printf(MAGENTA"------------------------------------------------\n"RESET);
            }
            else if (strcmp(info->tag, "COMM") == 0)
            {
                printf(CYAN"-------------- CHANGE THE COMMENT --------------\n\n"RESET);
                printf("COMMENT\t: %s\n", data);
                printf(MAGENTA"------------------------------------------------\n"RESET);
            }
        }

        /* reversing the size */
        e_data_size = ((e_data_size & 0xFF000000) >> 24) | // Move byte 1 to byte 4
                      ((e_data_size & 0x00FF0000) >> 8) |  // Move byte 2 to byte 3
                      ((e_data_size & 0x0000FF00) << 8) |  // Move byte 3 to byte 2
                      ((e_data_size & 0x000000FF) << 24);  // Move byte 4 to byte 1


        fwrite(&e_data_size, 4, 1, info->fptr_duplicate_fname);
        fwrite(flag, 3, 1, info->fptr_duplicate_fname);

        /* reversing the size */
        e_data_size = ((e_data_size & 0xFF000000) >> 24) | // Move byte 1 to byte 4
                      ((e_data_size & 0x00FF0000) >> 8) |  // Move byte 2 to byte 3
                      ((e_data_size & 0x0000FF00) << 8) |  // Move byte 3 to byte 2
                      ((e_data_size & 0x000000FF) << 24);  // Move byte 4 to byte 1


        for (int i = 0; i < e_data_size-1; i++)
        {
            fprintf(info->fptr_duplicate_fname, "%c", data[i]);
        }
    }

    e_copy_data(info->fptr_source_fname, info->fptr_duplicate_fname);   // calling function to copy remaining data from file

    fclose(info->fptr_duplicate_fname); // closing the files
    fclose(info->fptr_source_fname);

    info->fptr_duplicate_fname = fopen(info->duplicate_fname, "r"); // opening file in read mode
    info->fptr_source_fname = fopen(info->source_fname, "w");       // opening file in write mode

    e_copy_data(info->fptr_duplicate_fname, info->fptr_source_fname);   // calling function to copy remaining data from file
    fclose(info->fptr_duplicate_fname);
    fclose(info->fptr_source_fname);
}

/* function to open the required files */
Status e_open_files(details *info)
{
    info->fptr_duplicate_fname = fopen(info->duplicate_fname, "w"); // opening the file in write mode
    if (info->fptr_duplicate_fname == NULL)
    {
        printf(RED "INFO: ERROR: %s file not opened\n" RESET, info->duplicate_fname);
        return failure;
    }

    info->fptr_source_fname = fopen(info->source_fname, "r");   // opening the file in read mode
    if (info->fptr_source_fname == NULL)
    {
        printf(RED "INFO: ERROR: %s file not opened\n" RESET, info->source_fname);
        return failure;
    }

    return success;
}

/* function to check the size of file (checking file empty or not)*/
Status e_check_size(FILE *fptr)
{
    // printf("INFO: Checking mpeg file size\n");
    fseek(fptr, 0, SEEK_END);   // setting the file cursor at end of file
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
Status e_check_version(FILE *fptr_source_fname)
{
    // printf("INFO: Checking version\n");
    char buf[4];

    fread(buf, 3, 1, fptr_source_fname);
    buf[3] = '\0';
    if (strcmp(buf, "ID3") != 0)
    {
        printf(RED "INFO: Error: %s version is Invalid\n" RESET, buf);
        return failure;
    }

    fread(&e_data_size, 2, 1, fptr_source_fname);
    
    if (e_data_size != 3)
    {
        printf(RED "INFO: ERROR: %x Version is different\n" RESET, e_data_size);
        return failure;
    }

    // printf("INFO: Done.\n");
    return success;
}

/* function to read tags from file */
Status e_read_tag(details *info)
{

    fread(temp_tag, 4, 1, info->fptr_source_fname);
    temp_tag[4] = '\0';

    return success;
}

/* function to read the data size from file */
Status e_read_size(details *info)
{
    fread(&e_data_size, 4, 1, info->fptr_source_fname);
    // printf("%x\t", e_data_size);

    // reversing the size
    e_data_size = ((e_data_size & 0xFF000000) >> 24) | // Move byte 1 to byte 4
                  ((e_data_size & 0x00FF0000) >> 8) |  // Move byte 2 to byte 3
                  ((e_data_size & 0x0000FF00) << 8) |  // Move byte 3 to byte 2
                  ((e_data_size & 0x000000FF) << 24);  // Move byte 4 to byte 1

    // printf("%x\n",e_data_size);
    return success;
}

/* function to read data from file */
Status e_read_data(details *info)
{
    fread(flag, 3, 1, info->fptr_source_fname); // reading 3 bytes of flag from file(skipping)

    fread(data, e_data_size-1, 1, info->fptr_source_fname);

    return success;
}

/* function to copy the data from file */
Status e_copy_data(FILE *fptr_source, FILE *fptr_dest)
{
    while (!feof(fptr_source))
    {
        fread(data, 50, 1, fptr_source);
        fwrite(data, 50, 1, fptr_dest);
    }
}