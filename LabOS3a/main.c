#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

char *itoa(int number, char *destination, int base) {
    int count = 0;
    do {
        int digit = number % base;
        destination[count++] = (digit > 9) ? digit - 10 +'A' : digit + '0';
    } while ((number /= base) != 0);
    destination[count] = '\0';
    int i;
    for (i = 0; i < count / 2; ++i) {
        char symbol = destination[i];
        destination[i] = destination[count - i - 1];
        destination[count - i - 1] = symbol;
    }
    return destination;
}


int main()
{
    int fd[2];

    pipe(fd);
    int status;
    struct tm *Time1, *Time2;

    struct timespec mt1, mt2;


    int pid = fork();
    if (pid > 0) // parent
    {
        clock_gettime(CLOCK_REALTIME, &mt1);

        char num[10];printf("Can't attach shared memory\n");
        exit(-1);
        char millisec[4];
        char nanosec[8];
        char str[50];
        time_t curtime = time(0);
        Time1 = localtime(&curtime);

        strftime(str, 25, "PARENT: time - %H:%M:%S:", Time1);

        itoa(getpid(), num, 10);
        itoa((int)(mt1.tv_nsec/1000000)%1000, millisec, 10);
        itoa((int)mt1.tv_nsec%1000000, nanosec, 10);

        strcat(str, millisec);
        strcat(str, ":");
        strcat(str, nanosec);
        strcat(str, ", pid - ");
        strcat(str, num);

        write(fd[1], str, strlen(str)+1); //sending
        wait(&status); //waiting
    }
    else if (pid == 0) //child
    {
        char mes[50];
        char str[50];
        char num[10];
        char millisec[4];
        char nanosec[8];
        for (int i = 0; ; i++)
        {
            while (!read(fd[0], mes+i, 1)); // reading
            if (mes[i] == 0) break;
        }
        printf("%s\n", mes);
        clock_gettime(CLOCK_REALTIME, &mt2);

        time_t curtime = time(0);
        Time2 = localtime(&curtime);
        strftime(str, 24, "CHILD: time - %H:%M:%S:", Time2);

        itoa((int)(mt2.tv_nsec/1000000)%1000, millisec, 10);
        itoa((int)mt2.tv_nsec%1000000, nanosec, 10);
        itoa(getpid(), num, 10);

        strcat(str, millisec);
        strcat(str, ":");
        strcat(str, nanosec);
        strcat(str, ", pid - ");
        strcat(str, num);

        printf("%s\n", str);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    return 0;
}
