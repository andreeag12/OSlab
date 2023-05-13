#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

void access_rights(char* arg)
{
    struct stat infos;
    lstat(arg, &infos);
    printf("User:\n");
    printf("Read - %s\n", (infos.st_mode & S_IRUSR) ? "Yes" : "No");
    printf("Write - %s\n", (infos.st_mode & S_IWUSR) ? "Yes" : "No");
    printf("Exec - %s\n", (infos.st_mode & S_IXUSR) ? "Yes" : "No");
    printf("\nGroup:\n");
    printf("Read - %s\n", (infos.st_mode & S_IRGRP) ? "Yes" : "No");
    printf("Write - %s\n", (infos.st_mode & S_IWGRP) ? "Yes" : "No");
    printf("Exec - %s\n", (infos.st_mode & S_IXGRP) ? "Yes" : "No");
    printf("\nOther:\n");
    printf("Read - %s\n", (infos.st_mode & S_IROTH) ? "Yes" : "No");
    printf("Write - %s\n", (infos.st_mode & S_IWOTH) ? "Yes" : "No");
    printf("Exec - %s\n", (infos.st_mode & S_IXOTH) ? "Yes" : "No");
}

void countCFiles(char* path) 
{
    DIR* d;
    struct dirent* entry;
    int k = 0;
    d = opendir(path);
    if(d == NULL) 
    {
        perror("Couldn't open directory\n");
    } 
    
    while((entry = readdir(d)) != NULL) 
    {
        //check if the file has .c extension
        if(strstr(entry->d_name, ".c") != NULL)
            k++;
    }
    closedir(d);
    printf("Total number of C files is: %d\n", k);
}

int countLines(char* name)
{
    FILE *file = fopen(name, "r");
    if (file == NULL) 
    {
        perror("Failed to open file");
    }
    int lineCount = 0;
    char buffer[1024];  // Buffer to store each line
    while (fgets(buffer, sizeof(buffer), file) != NULL) 
    {
        lineCount++;
    }
    fclose(file);
    return lineCount;
}

void Score(int errors, int warnings, char* arg) 
{
    float score = 0;
    if(errors != 0)
        score = 1;
    else if (warnings == 0)
        score = 10;
    else if (warnings > 10)
        score = 2;
    else score = 2 + 8 * (10 - warnings) / 10.0;

    FILE *file = fopen("grades.txt", "a");  // Open the file in append mode
    if (file == NULL) 
    {
        perror("Failed to open file\n");
    }
    fprintf(file, "%s : %.2f\n", arg, score);
    fclose(file);
}

int main(int argc, char* argv[])
{
    struct stat infos;

    if(argc < 2)
    {
        printf("Not enough arguments.\n");
        return -1;
    }
    else 
    {
        for(int i = 1; i < argc; i++)
        {
            if(lstat(argv[i], &infos) == 0)
            {
                pid_t pid1 = fork();
                if(pid1 < 0)
                {
                    perror("Fork failed.\n");
                    return -1;
                }
                if(pid1 == 0)
                {   //Regular file menu
                    if(S_ISREG(infos.st_mode) == 1)
                    {
                        printf("%d is a regular file.\n File menu :\n-n (file name)\n-d (size)\n-h (no. of hard links)\n-m (time of last modification)\n-a (access rights)\n-l (create symlink)\n", i);
                        printf("Choose option: \n");
                        char c; 
                        while((c = getchar()) != '\n')
                        {
                            if (c == 'n') {
                                printf("File name is: %s\n", argv[i]);
                            } else if (c == 'd') {
                                printf("Total size of file is: %lld bytes\n", infos.st_size);
                            } else if (c == 'h') {
                                printf("Total number of hard links is: %hu\n", infos.st_nlink);
                            } else if (c == 'm') {
                                printf("\nTime of last modification is: %ld\n", infos.st_mtime);
                            } else if (c == 'a') {
                                access_rights(argv[i]);
                            } else if (c == 'l') {
                                char link_name[50];
                                printf("Enter name for the symbolic link: ");
                                scanf("%s", link_name);
                                int slink = symlink(argv[1], link_name);
                                if (slink == 0) {
                                    printf("Symbolic link created successfully.\n");
                                } else {
                                    printf("Error creating symbolic link\n");
                                }
                            } else {
                                printf("'%c' is not a valid option.\n", c);
                            }
                        }   
                    } 
                    //Symlink menu
                    if(S_ISLNK(infos.st_mode) == 1)
                    {
                        printf("%d is a sym link.\n SymLink menu :\n-n (link name)\n-l (delete link)\n-d (size of link)\n-t (size of target)\n-a (access rights for sym link)\n", i);
                        int c;
                        while((c = getchar()) != '\n')
                            {
                                if (c == 'n') {
                                    printf("Symbolic link name is: %s\n", argv[i]);
                                } else if (c == 'l') {
                                    printf("Deleting symbolic link.\n");
                                    unlink(argv[i]);
                                    exit(0);
                                } else if (c == 'd') {
                                    printf("Size of the symbolic link is: %lld\n", infos.st_size);
                                } else if (c == 't') {
                                    struct stat t;
                                    stat(argv[i], &t);
                                    printf("Size of the target file is: %lld\n", t.st_size);
                                } else if (c == 'a') {
                                    access_rights(argv[i]);
                                } else {
                                    printf("'%c' is not a valid option.\n", c);
                                }
                            }
                        } 
                        //Directory menu
                        if(S_ISDIR(infos.st_mode) == 1)
                        {
                            printf("%d is a directory.\n Directory menu :\n-n (dir name)\n-d (size of dir)\n-a (access rights)\n-c (total number of .c files)\n", i);
                            int c;
                            while((c = getchar()) != '\n')
                            {
                                switch (c)
                                {
                                case 'n':   printf("Directory name is: %s\n", argv[i]);
                                            break;
                                case 'd':   printf("Size of the directory is: %lld\n",infos.st_size);
                                            break;
                                case 'a':   access_rights(argv[i]);
                                            break;
                                case 'c':   countCFiles(argv[i]);
                                            break;
                                case '-':   break;
                                default:    printf("'%c' is not a valid option.", c);
                                            break;
                                }
                            }
                        }
                }
                else if(pid1 > 0)
                {
                    pid_t status = waitpid(pid1, &status, 0);
                    if(WIFEXITED(status))
                    {
                        int exitCode = WEXITSTATUS(status);
                        printf("The process with PID %d has ended with the exit code %d\n", status, exitCode);
                    }
                    else
                    {
                        printf("The process with PID %d has terminated abnormally\n", status);
                    }

                    pid_t child2 = fork();
                    int pid2 = 0;
                    if(child2 < 0)
                    {
                        perror("Fork process for second child failed.\n");
                        exit(1);
                    }
                    else if(child2 == 0)
                    {
                        if((strstr(argv[i], ".c") != NULL))
                        {
                            int pfd[2]; //0 read descriptor, 1 write descriptor
                            int newfd;
                            if(pipe(pfd) < 0)
                            {
                                perror("Pipe not created\n");
                                exit(-1);
                            }

                             pid2 = fork();
                            if(pid2 < 0)
                            {
                                perror("Can't create second process\n");
                                exit(-1);
                            }
                            else if(pid2 == 0)
                            {
                                //child process reads the output of the script
                                close(pfd[0]);  //close read descriptor
                                if((newfd = dup2(pfd[1], 1)) < 0)
                                {
                                    perror("Can not duplicate\n");
                                    exit(1);
                                }
                                if(S_ISREG(infos.st_mode)==1 && strstr(argv[i],".c"))
                                {
                                    if(S_ISREG(infos.st_mode)==1)
                                    {
                                        execlp("bash", "bash", "compile.sh", argv[i], NULL);
                                    } 
                                }
                                else if(S_ISREG(infos.st_mode)==1)
                                {
                                    int lineCount = countLines(argv[i]);
                                    printf("The number of lines is: %d\n", lineCount);
                                } 
                                else if(S_ISDIR(infos.st_mode)==1)
                                {
                                    char file_name[strlen(argv[i]) + 11];
                                    if(strrchr(argv[i], '/') != 0) 
                                    {
                                        char* dir_name = strrchr(argv[i], '/') + 1;
                                        sprintf(file_name, "%s/%s_file.txt", argv[i], dir_name);
                                    } 
                                    else 
                                    {
                                        strcpy(file_name, argv[i]);
                                        strcat(file_name, "/");
                                        strcat(file_name, argv[i]);
                                        strcat(file_name, "_file.txt");
                                    }
                                    int fp = open(file_name, O_WRONLY | O_CREAT, 0644);
                                    if(fp == -1) 
                                    {
                                        perror("open");
                                        exit(EXIT_FAILURE);
                                    }
                                    close(fp);
                                    exit(EXIT_SUCCESS);
                                }
                                else if(S_ISLNK(infos.st_mode)==1)
                                {
                                    printf("Changing permissions for symbolic link %s\n", argv[i]);
                                    if (execlp("chmod", "chmod", "u=rwx,g=rw,o=---", argv[i], NULL) == -1) 
                                    {
                                        perror("Failed to execute chmod command\n");
                                    }
                                }
                                close(pfd[1]);
                                exit(1);
                            }
                            //parent reads the output of the string
                            close(pfd[1]); //close the writing end of pipe

                            FILE* file = fdopen(pfd[0],"r");
                            char string[100];
                            fscanf(file, "%s", string);
                            int errors = atoi(string);
                            fscanf(file, "%s", string);
                            int warnings = atoi(string);
                            Score(errors, warnings, argv[i]);
                            close(pfd[0]);

                            pid_t status = waitpid(pid2, &status, 0);
                            if(WIFEXITED(status))
                            {
                                int exitCode = WEXITSTATUS(status);
                                printf("The process with PID %d has ended with the exit code %d\n", status, exitCode);
                            }
                            else
                            {
                                printf("The process with PID %d has terminated abnormally\n", status);
                            }
                            //close(pfd[0]);
                        }
                        /*pid_t status = waitpid(pid2, &status, 0);
                        if(WIFEXITED(status))
                        {
                            int exitCode = WEXITSTATUS(status);
                            printf("The process with PID %d has ended with the exit code %d\n", status, exitCode);

                        }
                        else
                        {
                            printf("The process with PID %d has terminated abnormally\n", status);
                        }*/
                        
                    }
                    else
                    {
                        pid_t status = waitpid(child2, &status, 0);
                        if(WIFEXITED(status))
                        {
                            int exitCode = WEXITSTATUS(status);
                            printf("The process with PID %d has ended with the exit code %d\n", status, exitCode);

                        }
                        else
                        {
                            printf("The process with PID %d has terminated abnormally\n", status);
                        }
                    }
                }
                else
                {
                    perror("Menu error.\n");
                    exit(-1);
                }
            }
        }
    }
    return 0;
}