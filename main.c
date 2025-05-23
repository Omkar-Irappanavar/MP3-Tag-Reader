/* 
    Name: Omkar Irappanavar
    Date: 21/09/2024
    Description: mp3 tag Reader/Editor
*/

#include <stdio.h>
#include <string.h>
#include "common.h"

int main(int argc, char *argv[])
{

    details info;

    if (!(validate_args(argc, argv, &info)))    // calling function to validate command line arguments
    {
        return failure;
    }
    else
    {
        if (strcmp(argv[1], "-v") == 0)     // view method validations
        {
            if(argc > 2)
            {
                info.source_fname = argv[2];
            }
            else
            {
                // info.source_fname = "sample1.mpeg"; 
                info.source_fname = "sample.mp3"; 
            }

            if (view_info(&info) == failure)
            {
                return failure;
            }
        }
        else if (strcmp(argv[1], "-e") == 0)
        {
            if( strcmp(argv[2], "-t") == 0 )
            {
                strcpy(info.tag, "TIT2");
                strcpy(info.change_data, argv[3]);
            }
            else if( strcmp(argv[2], "-a") == 0 )
            {
                strcpy(info.tag, "TPE1");
                strcpy(info.change_data, argv[3]);
            }
            else if( strcmp(argv[2], "-A") == 0 )
            {
                strcpy(info.tag, "TALB");
                strcpy(info.change_data, argv[3]);
            }
            else if( strcmp(argv[2], "-y") == 0 )
            {
                strcpy(info.tag, "TYER");
                strcpy(info.change_data, argv[3]);
            }
            else if( strcmp(argv[2], "-m") == 0 )
            {
                strcpy(info.tag, "TCON");
                strcpy(info.change_data, argv[3]);
            }
            else if( strcmp(argv[2], "-c") == 0 )
            {
                strcpy(info.tag, "COMM");
                strcpy(info.change_data, argv[3]);
            }

            if (edit_info(&info) == failure)
            {
                return failure;
            }
        }
        else
        {
            printf(RED"ERROR: %s is unsupported command\n"RESET, argv[1]);
            return failure;
        }
    }
}

/* function to validate CLA arguments*/
Status validate_args(int argc, char *argv[], details *info)
{
    if (argc < 2)   // menu to user to understand the process
    {
        printf(YELLOW "\n-----------------------------------------------------\n\n" RESET);
        printf(RED "ERROR: ./a.out : INVALID ARGUMENTS\n" RESET);
        printf(CYAN "USAGE : \n" RESET);
        printf("To view please pass like: ./a.out -v <mp3_filename>\n");
        printf("To edit please pass like: ./a.out -e -t/-a/-A/-y/-m/-c changing_text <mp3_filename>\n");
        printf("To get help pass like: ./a.out --help\n");
        printf(YELLOW "-----------------------------------------------------\n\n" RESET);
        return failure;
    }

    if (strcmp(argv[1], "--help") == 0)  // --help command menu
    {
        printf(YELLOW "\n--------------------HELP MENU--------------------------\n\n" RESET);
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");
        printf("\t2.1. -t -> to edit song title\n");
        printf("\t2.2. -a -> to edit artist name\n");
        printf("\t2.3. -A -> to edit album name\n");
        printf("\t2.4. -y -> to edit year\n");
        printf("\t2.5. -m -> to edit content\n");
        printf("\t2.6. -c -> to edit comment\n");
        printf(YELLOW "-----------------------------------------------------\n\n" RESET);

        return failure;
    }

    if (strcmp(argv[1], "-v") == 0)   
    {
        if (argc < 3)
        {
            // printf(RED "ERROR: ./a.out -v <mpeg file>\n" RESET);
            printf(RED "ERROR: ./a.out -v <mp3 file>\n" RESET);
            return failure;
        }

        // info->source_fname = strstr(argv[2], ".mpeg");      // checking the file is .mpeg file or not
        info->source_fname = strstr(argv[2], ".mp3");      // checking the file is .mpeg file or not

        if (info->source_fname == NULL)
        {
            // printf(RED "ERROR: %s is not <mpeg file>\n" RESET, argv[2]);
            printf(RED "ERROR: %s is not <mp3 file>\n" RESET, argv[2]);
            return failure;
        }

        // if (strcmp(info->source_fname, ".mpeg") == 0)
        if (strcmp(info->source_fname, ".mp3") == 0)
        {
            info->source_fname = argv[2];
        }
        else
        {
            // printf(RED "ERROR: %s is not <mpeg file>\n" RESET,argv[2]);
            printf(RED "ERROR: %s is not <mp3 file>\n" RESET,argv[2]);
            return failure;
        }

        return success;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc < 5)
        {
            // printf(RED"EDIT: ./a.out -e -t/-a/-A/-y/-m/-c <changing_text> <mpeg file_name>\n"RESET);
            printf(RED"EDIT: ./a.out -e -t/-a/-A/-y/-m/-c <changing_text> <mp3 file_name>\n"RESET);
            return failure;
        }
        else
        {
            // info->source_fname = strstr(argv[4], ".mpeg");      // checking the file is .mpeg or not
            info->source_fname = strstr(argv[4], ".mp3");      // checking the file is .mpeg or not
            if (info->source_fname == NULL)
            {
                // printf(RED "ERROR: %s is not <mpeg file>\n" RESET, argv[4]);
                printf(RED "ERROR: %s is not <mp3 file>\n" RESET, argv[4]);
                return failure;
            }

            // if (strcmp(info->source_fname, ".mpeg") == 0)
            if (strcmp(info->source_fname, ".mp3") == 0)
            {
                info->source_fname = argv[4];
            }
            else
            {
                // printf(RED "ERROR: %s is not <.mpeg file>\n" RESET, argv[4]);
                printf(RED "ERROR: %s is not <.mp3 file>\n" RESET, argv[4]);
                return failure;
            }
        }

        if (!((strcmp(argv[2], "-t") == 0) || (strcmp(argv[2], "-a") == 0) || (strcmp(argv[2], "-A") == 0) || (strcmp(argv[2], "-y") == 0) || (strcmp(argv[2], "-m") == 0) || (strcmp(argv[2], "-c") == 0)))
        {
            printf(RED"ERROR: %s is unsupported command\n"RESET,argv[2]);
            return failure;
        }

    }

    return success;
}