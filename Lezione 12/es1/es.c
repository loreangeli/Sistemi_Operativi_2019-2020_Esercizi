#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

volatile sig_atomic_t contaC;
volatile sig_atomic_t contaZ;
volatile sig_atomic_t segnale;


static void gestoreC (int signum) {
    contaC++;
}

static void gestoreZ (int signum) {
    char reply[4]; 
    contaZ++;
    printf(" \n %d \n", contaC);
    if (contaZ%3==0) {
        printf("vuoi terminare il programma? [y/n] \n");
        alarm(10);
        scanf("%s",reply);
        if (strcmp("y",reply)==0)
            segnale=1;
    }

}

static void gestoreA (int signum) {
    segnale=1;
}

int main() {
    segnale=0;
    struct sigaction s, d,f;
    //inizializzo s a 0
    memset(&s,0,sizeof(s));
    s.sa_handler=gestoreC;
    //installo gestore CTRL+C
    sigaction(SIGINT,&s,NULL);
    //inizializzo d a 0
    memset(&d,0,sizeof(d));
    d.sa_handler=gestoreZ;
    //installo gestore CTRL+Z
    sigaction(SIGTSTP,&d,NULL);
    //inizializzo s a 0
    memset(&f,0,sizeof(f));
    f.sa_handler=gestoreA;
    //installo gestore SIGALRM
    sigaction(SIGALRM,&f,NULL);



    while(1) {
        if (segnale==1)
            exit(EXIT_SUCCESS);
    }
        
    



    return 0;
}