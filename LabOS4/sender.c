#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
   // key_t shkey = ftok(".", 'b');
    int flags = IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    struct tm *Time;
    struct timespec mt;
    char * mem_str;
    char str[40];


    int idmem = shmget(2020, 100, flags);
    if (idmem < 0)
    {
        if (errno != EEXIST)
        {
            printf("Can't create shared memory\n");
            exit(-1);
        }
        else
        {
            idmem = shmget(2020, 100, 0);
            if(idmem < 0)
            {
                printf("Can't find shared memory\n");
                exit(-1);
            }
        }
    }

    while (1)
    {
        mem_str = (char *)shmat(idmem, NULL, 0);
        if (mem_str == (char *)(-1))
        {
            printf("Can't attach shared memory\n");
            exit(-1);
        }
        clock_gettime(CLOCK_REALTIME, &mt);
        time_t curtime = time(0);
        Time = localtime(&curtime);

        strftime(str, sizeof(str), "SENDER: time - %H:%M:%S", Time);
        sprintf(mem_str, "%s:%i:%i, pid - %i", str, (int)(mt.tv_nsec/1000000)%1000, (int)mt.tv_nsec%1000000, getpid());
      //  printf("%s\n", mem_str);
        if(shmdt(mem_str) < 0)
        {
            printf("Can't detach shared memory\n");
            exit(-1);
        }
    }
}