#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void FuncForAtExit()
{
    printf("Procces %d - happy ended (atexit())\n", getpid());
}

void FuncForOnExit()
{
    printf("Procces %d - happy ended (on_exit())\n", getpid());
}
int zero = 0;
int main(int argc, char **argv)
{
    //for (int i = 0; i < argc; i++) printf("%s - huy\n", argv[i]);
    atexit(FuncForAtExit);
    on_exit(FuncForOnExit, NULL);
    pid_t  fpid;
    pid_t spid;
    int status;
    int rv;
    int j = 3;
    fpid = fork();
  //  spid = fork();

    printf("fpid from fork: %d\n", fpid);
    switch (fpid)
    {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            printf("CHILD(%d): PID моего родителя -- %d\n", getpid(), getppid());
            printf("CHILD(%d): Введите мой код возврата (как можно меньше):", getpid());
            scanf("%d", &rv);
            printf("CHILD(%d): Выход!\n", getpid());
            exit(rv);
        default:
            printf("PARENT(%d): PID моего потомка -- %d\n", getpid(), fpid);
            printf("PARENT(%d): Я жду, пока потомок не вызовет exit()...\n", getpid());
            wait(&status);
            printf("PARENT(%d): Код возврата потомка: %d\n", getpid(), WEXITSTATUS(status));
            printf("PARENT(%d): Статус выхода потомка -- %d\n", getpid(), status);
            printf("PARENT(%d): Выход!\n", getpid());
    }

    spid = fork();

    printf("spid from fork: %d\n", spid);
    switch (spid)
    {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            j = 7/zero;
            printf("CHILD(%d): PID моего родителя -- %d\n", getpid(), getppid());
            printf("CHILD(%d): Введите мой код возврата (как можно меньше):", getpid());
            scanf("%d", &rv);
            printf("CHILD(%d): Выход!\n", getpid());
            exit(rv);
        default:
            printf("PARENT(%d): PID моего потомка -- %d\n", getpid(), spid);
            printf("PARENT(%d): Я жду, пока потомок не вызовет exit()...\n", getpid());
            wait(&status);
            printf("PARENT(%d): Код возврата потомка: %d\n", getpid(), WEXITSTATUS(status));
            printf("PARENT(%d): Статус выхода потомка -- %d\n", getpid(), status);
            printf("PARENT(%d): Выход!\n", getpid());
    }



    return 0;
}