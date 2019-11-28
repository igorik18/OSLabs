#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void getmode(mode_t mode, char str[])
{
    mode_t type = mode & S_IFMT;

    switch (type)
    {
        case S_IFREG:
            str[0] = '-';
            break;
        case S_IFSOCK:
            str[0] = 's';
            break;
        case S_IFLNK:
            str[0] = 'l';
            break;
        case S_IFBLK:
            str[0] = 'b';
            break;
        case S_IFDIR:
            str[0] = 'd';
            break;
        case S_IFCHR:
            str[0] = 'c';
            break;
        case S_IFIFO:
            str[0] = 'p';
    }
    if ((mode & S_IRUSR) > 0) str[1] = 'r'; else str[1] = '-';
    if ((mode & S_IWUSR) > 0) str[2] = 'w'; else str[2] = '-';
    if ((mode & S_IXUSR) > 0) str[3] = 'x'; else str[3] = '-';
    if ((mode & S_IRGRP) > 0) str[4] = 'r'; else str[4] = '-';
    if ((mode & S_IWGRP) > 0) str[5] = 'w'; else str[5] = '-';
    if ((mode & S_IXGRP) > 0) str[6] = 'x'; else str[6] = '-';
    if ((mode & S_IROTH) > 0) str[7] = 'r'; else str[7] = '-';
    if ((mode & S_IWOTH) > 0) str[8] = 'w'; else str[8] = '-';
    if ((mode & S_IXOTH) > 0) str[9] = 'x'; else str[9] = '-';
    str[10] = '\0';
}

int main(int argc, char* argv[])
{
    char  buf[400];
    getwd(buf);
    char cur[400];
    struct stat st;
    char str[11];
    struct tm *time;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (buf)) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 && ent->d_name[0] != '.')
            {
                strcpy(cur, "\0");
                strcat(cur, buf);
                strcat(cur, "/");
                strcat(cur, ent->d_name);

                stat(cur, &st);
                getmode(st.st_mode, str);
                time = localtime(&st.st_mtime);

                printf("%s %i.%i.%i %i:%i %s\n", str, time->tm_mday, time->tm_mon+1, time->tm_year+1900, time->tm_hour, time->tm_min, ent->d_name);
            }
        }
        closedir(dir);
    }
    return 0;
}