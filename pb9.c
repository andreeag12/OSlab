#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int args, char* argv[])
{
    if(args < 2)
    {
        printf("Not enough arguments.\n");
        return -1;
    }

    struct stat infos;
    for(int i = 1; i <= args; i++)
    {
        if(lstat(argv[i],&infos) == 0)
        {
            pid_t pid1 = fork();
            if(pid1 < 0)
            {
                perror("Fork failed.\n");
                return -1;
            }
            if(pid1 == 0)
            {
                if(S_ISREG(infos.st_mode) == 1)
                {
                    printf("%d is a regular file.\n File menu :\n-n (file name)\n-d (size)\n-h (no. of hard links)\n-m (time of last modification)\n-a (access rights)\n-l (create symlink)\n", i);
                    printf("Choose: \n");
                    char c; 
                    while((c = getchar()) != '\n')
                    {
                        if(c == 'n')
                        {
                            printf("File name is: %s\n", argv[1]);
                        }
                        if(c == 'd')
                        {
                            printf("Size of the file is: %ld bytes.\n", infos.st_size);
                        }
                        if(c == 'h')
                        {
                            printf("Number of hard links is: %ld\n", infos.st_nlink);
                        }
                        if(c == 'm')
                        {
                            printf("Time of last modification is: %ld\n", infos.st_mtime);
                        }
                        if(c == 'a')
                        {
                            printf("Acces rights are: %d\n", infos.st_mode);
                        }
                        if(c == 'l')
                        {
                            char link_name[25];
                            scanf("%s", link_name);
                            printf("Name for the link is: %s\n", link_name);
                        }
                    }
                }
            }
            else 
            {
                pid_t pid = fork();
                if(pid < 0)
                {
                    perror("Fork failed.\n");
                    return -1;
                }
                else if(pid == 0)
                {
                    sleep(5);
                    execlp("bash", "bash", "compile.sh", argv[i], NULL);
                    perror("\n Exec failed.\n");
                    return -1;
                }
                else
                {
                    int status;
                    wait(&status);
                }
            }
        }
        /*if(S_ISLNK(infos.st_mode) == 1)
            {
                printf("%d is a sym link.\n SymLink menu :\n-n (link name)\n-l (delete link)\n-d (size of link)\n-t (size of target)\n-a (access rights for sym link)\n", i);
            } 
        if(S_ISDIR(infos.st_mode) == 1)
            {
                printf("%d is a directory.\n Directory menu :\n-n (dir name)\n-d (size of dir)\n-a (access rights)\n-c (total number of .c files)\n", i);
            }
        }*/
        else
        {
            int status;
            wait(&status);
        }
    }

    return 0;
}