/*  pwd - print current working directory version 0.9

    Copyright (C) Free Software "Protected under copyright": Apache License 2.0


    This program is free software: you are free to download, use and redistribute this software under the terms of the project license.

    Differences with others unix-like pwd:
       This is a different code what it means that both programs may do the samething but in different ways
       This is a not GNU Core utilities program, it means it was created this program from scratch by myself

       By Yassir Daniel known as Sam Jamsh

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef char bool;
bool true = 1;
bool false = 0;
bool SUCCESS = 0;
bool ERROR = 1;


bool cmpstr(char *string1, char *string2)
{
    unsigned char s1_char = 1;
    unsigned char s2_char = 1;

    for (unsigned short i = 0; i < 65535 && s1_char != 0 && s2_char != 0; i++)
    {
        s1_char = string1[i];
        s2_char = string2[i];

        if (s1_char != s2_char)
        {
            return 1; 
        }
    }
    return 0; 
}


unsigned short str_len(unsigned char *string)
{
    unsigned short counter = 0;

    while (counter < 65535)
    {
        if (string[counter] == 0) {
            break;
        }
        counter++;
    }
    return counter;
}


unsigned short get_pid_len(long int number)
{
    double result = 1;
    long int n = 10;
    unsigned short counter = 0;

    while (result > (double) 0)
    {
        result =  number / n;
        n *= 10;
        counter++;
    }
    return counter;
}


char* get_path(pid_t pid)
{
    unsigned short pid_len = get_pid_len((long int) pid);

    size_t buflen = sizeof(char) * (size_t) pid_len + 1;
    char *strpid = (char*) malloc(buflen);
    snprintf(strpid, buflen, "%d", pid);

    int pidlen = (int) str_len(strpid);

    size_t buffer_length = 6 + pidlen + 8 + 1;
    char *path = (char*) malloc(sizeof(char) * buffer_length);  // from 18 to 24

    char part1[6] = {'/', 'p', 'r', 'o', 'c', '/'};
    char part2[8] = {'/', 'e', 'n', 'v', 'i', 'r', 'o', 'n'};


    int i, j, lim;
    for (i = 0; i < 6; i++)
    {
        *(path + i) = part1[i];
    }


    j = 0;
    lim = i + pidlen;
    for (i; i < lim; i++)
    {
        *(path + i) = strpid[j];
        j++;
    }


    j = 0;
    lim = i + 8;
    for (i = i; i < lim; i++)
    {
        *(path + i) = part2[j];
        j++;

    }

  
    *(path + i) = 0;
    return path;
}


char *get_cwd(char *path)
{
    FILE *filedata;
    filedata = fopen(path, "r");

    if (filedata == NULL)
    {
        perror("Error opening file for reading");
    }

    unsigned char *curdir = (unsigned char*) malloc(sizeof(unsigned char) * 128);
    if (curdir == NULL)
    {
        perror("Error allocating memory for curdir");
    }

    bool found = false;
    unsigned char pwd[3] = {'P', 'W', 'D'};
    unsigned char buf[3];
    unsigned char ch = 1;
    unsigned short i = 0;
    unsigned short j = 0;

    while ((ch = fgetc(filedata)) != EOF)
    {

        if (found == true & ch != 0)
        {
            if (j < 128)
            {
                *(curdir + j) = ch; // while < 128
                j++;
            }
            else {
                curdir[127] = 0; // or return NULL
            }
        }
        else if (found == true & ch == 0)
        {
            *(curdir + j) = 0; // null byte or \0
            fclose(filedata);
            return curdir;
        }


        if (ch == pwd[i])
        {
            i++;

            if (i == 3)
            {
                found = true;
            }

        } else {
            i = 0;
        }

    }
    fclose(filedata);
    return NULL; // null or eof
}


int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (cmpstr(argv[1], "--version") == SUCCESS)
        {
            printf("pwd (NGCU coreutils) 0.9\nCopyright (C) Apache License 2.0\nApache 2 License: license<https>\nThis is a free software but protected under copyright.\n\nWritten by Yassir Daniel aka Sam Jamsh.\n");
            return 0;
        } else {
            printf("%s: invalid option", argv[0]);
            return 1;
        }
    }

    pid_t pid;
    pid = getpid();

    char *environ_path = get_path(pid);

    unsigned char *cwd = get_cwd(environ_path);

    if (cwd != NULL)
    {
        printf("%s\n", ++cwd);

    } else {
        puts("error getting the current working directory");
    }
    free(environ_path);
    free(--cwd);
    return 0;
}
