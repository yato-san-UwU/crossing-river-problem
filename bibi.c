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

#define mutex           0 // semaphore pour tout le programme 
#define hackerQueue     1 // semaphore pour hacker
#define windowsQueue    2 // semaphore pour windows
#define compteur        3 // semaphore barriere pour le nombre de processus en entier



int sem_init(int semid, int numsem, int initval){ // initialisation semaphore 
    if(semctl(semid,numsem,SETVAL,initval)==-1){
        printf("erreur '%d' dans l'insertion du semaphore '%d'\n",semid,numsem);
    }
    else{
        printf("id sem '%d' le semaphore '%d' a ete initialiser a '%d'\n",semid,numsem,initval);
    }
}
int semcreate(key_t key, int n){  // creation semaphore
        int semid=semget(key, n, IPC_CREAT|IPC_EXCL|0666);
        printf("semid=%d\n", semid);
        if(semid==-1){
            perror("semget");
            semid=semget(key, n, 0); //option recuperation du semid
            printf("un groupe de séma existe déja pour cette clé et son identifiant est semid=%d\n", semid) ;
    }
    else{printf("un nouveau groupe de séma a été cré avec le semid= %d\n\n", semid) ;
    }
    return semid;
} 
int semtimedop(int semid, struct sembuf *sops, unsigned nsops,struct timespec *timeout);
int Ptime(int semid ,int x){
    struct timespec t;
    t.tv_sec = 5;
    t.tv_nsec = 5;
    struct sembuf op ;
    op.sem_num = x;        
    op.sem_op = -1;        
    op.sem_flg = 0;
    if (semtimedop(semid, &op, 1,&t) == -1) {  
        printf(" voyage annulé\n");
        exit(1);
    }
           
}
int p(int semid, int sem){
    struct sembuf s;
    s.sem_num = sem; 
    s.sem_op =-1;
    s.sem_flg=0;
    int r =semop(semid, &s, 1);
    if ( r == -1 ) perror("semop");
    return r;
}
int v(int semid, int sem){
    struct sembuf s;
    s.sem_num = sem;
    s.sem_op =1;
    s.sem_flg=0;
    int r =semop(semid, &s, 1);
    if ( r == -1 ) perror("semop");
    return r;
}
