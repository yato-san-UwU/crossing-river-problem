#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdbool.h>
#include "bib.h"


int main(){
    key_t key=ftok("chemin ou ce trouve votre fichier",'c'); // mettre le chemin de linux


    int sem=semcreate(key,5); // possible que 4 je sais pas aa voir

    sem_init(sem,0,1); // mutex = 1
    sem_init(sem,1,0); // hackerQ = 0
    sem_init(sem,2,0); // windowsQ = 0
    sem_init(sem,3,0); // compteur
    // initialisation des semaphore
    printf("l'id du groupe de semaphores = '%d'\n",sem);

    key_t key2=ftok("chemin ou ce trouve votre fichier",3); // mettre le chemin de linux

    int shmid=shmget(key,sizeof(sdata),IPC_CREAT|0666); //key 2
    printf("l'id du segment memoire shmid = '%d'\n",shmid);
    if(shmid==-1){
        perror("shmget");
    }
    //initialisation des variable partager
    sdata *sd=NULL;
    sd=shmat(shmid,sd,0);
    sd->hackers=0;
    sd->windows=0;
    sd->compteur=0;

    printf("hacker = %d\n",sd->hackers);
    printf("windows = %d\n",sd->windows);

    return 0;
}

