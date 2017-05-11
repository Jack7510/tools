//
// func: self raise process
// usage self-raise app_path app_name
// ps | grep app_name
// kill pid_app
//

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

char* g_app_name = NULL;
char* g_app_path = NULL;

void process_exit(int s)
{
    exit(0);
}


void child_fun()
{
    printf("child_fun. ppid %d\n",getppid());
    char str[64];

    printf("Input your name: ");
    scanf("%s", str);
    printf("your name is %s\n", str);
}


//
// call execl() to load app
//
void exec_fun(char* app_path, char* app_name) {
    printf("execl %s %s\n", app_path, app_name);
    execl( app_path, app_name, NULL);

    // this code line will never execute
    printf("after execl\n");
}


//
// if process terminate, fork a new one
//
void fork_child()
{
    pid_t child_process;
    int status;
    int signal_num;
    wait(&status);//等待子进程中断或终止，释放子进程资源，否则死掉的子进程会变成僵尸进程
  
    //如果子进程是由于某种信号退出的，捕获该信号
    if(WIFSIGNALED(status))
    {
        signal_num = WTERMSIG(status);
        printf("child process exit signal num is:%d\n", signal_num);
    }

    // fork new process 
    child_process = fork();
    if(child_process == 0)
    {
        printf("fork new child process.\n");
        exec_fun(g_app_path, g_app_name);
    }
}


//
// self-raise app_path, app_name
// self-raise ./hello hello
//
int main(int argc, char* argv[])
{
    pid_t child_process;

    if( argc < 3 ) {
        printf("self-raise app_path app_name\n");
        return 0;
    }

    g_app_path = argv[1];
    g_app_name = argv[2];

    while(1)
    {
        printf("fork new process.\n");
        child_process = fork();
        if(child_process > 0)
        {
            // parent process running routine
            while(1)
            {
               //捕获子进程结束信号
                signal(SIGCHLD, fork_child);
                signal(SIGTERM, process_exit);
                pause();//主进程休眠，当有信号到来时被唤醒。
            }
        }
        else if(child_process == 0)
        {
            // child process running routine
            exec_fun(g_app_path, g_app_name);
        }
    }
    return 0;
}
