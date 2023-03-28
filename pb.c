#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
            if(S_ISREG(infos.st_mode) == 1)
            {
                printf("%d is a regular file\n Choose:\n-n(file name)\n-d(size)\n-h(no. of hard links)\n-m(time of last modif)\n-a(access rights)\n-l(create symlink)\n", i);
            }
            else
                printf("%d is not a regular file\n", i);
        }
    }

    return 0;
}