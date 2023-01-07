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

void barrier(sdata *sd,int semid, int n){
    sd->compteur=sd->compteur+1;
    if((sd->compteur)!=n){
        p(semid,3);
    }
    else{
        while(sd->compteur!=0){
            sd->compteur=sd->compteur-1;
            v(semid,3);
        }
    }
}

int main(){

    key_t key=ftok("/mnt/c/Users/yato/Documents/TP M1/systeme/projet",'c'); // mettre le chemin de linux
    key_t key2=ftok("/mnt/c/Users/yato/Documents/TP M1/systeme/projet",3); // mettre le chemin de linux

    int shmid=shmget(key,sizeof(sdata),0); // key2
    if(shmid==-1){
        perror("shmget");
    }
    sdata *sd=NULL;
    sd=shmat(shmid,sd,0);
    int semid=semget(key,5,0); // possible changer 4 "nbr de semaphore"
    if(semid==-1){
        perror("semget");
    }

    //------------------------------------------------------------------------------------------------------------
    int y;
    bool isCapitain=false;
    printf("nombre de processus hackers qui traverse :");
    scanf("%d",&y);

    for(int R=0;R<y;R++){ // nbr de processus hacker
        int pid=fork();
        if(pid==-1){
            perror("erreur de creation");
            exit(0);
        }
        if(pid==0){
            p(semid,0); // mutex
            (sd->hackers)=(sd->hackers+1);
            if ((sd->hackers)==4){
                for (int i=0;i<3;i++){
                    v(semid,1); //hackerQueue 
                }
                sd->hackers=0;
                isCapitain=true;
                printf("je suis le capitaine 'CONDITON 1' mon pid est %d et je suis un hacker\n",getpid());
            }
            else if((sd->hackers==2) && (sd->windows >= 2)){
                v(semid,1);     // hackerQueue
                v(semid,2);     // windowsQueue
                v(semid,2);     // windowsQueue
                (sd->windows)=(sd->windows-2);
                sd->hackers=0;
                isCapitain=true;
                printf("je suis le capitaine'CONDITION 2' mon pid est %d et je suis un hacker\n",getpid());
            }
            else{
                v(semid,0);     // mutex
                p(semid,1); // hackerQueue
            }
            
            printf("je suis le hacker N%d pid =%d et j'embarque\n",R+1,getpid()); // fct board
            //sleep(15); //pour les processus
            printf("en att des autres processus N%d (H)\n",R+1);
            barrier(sd,semid,y);
            printf("fin de l'attente N%d (H)\n",R+1);

            //sleep(15); //pour stop le capitaine
            if (isCapitain==true){
                printf("je suis le capitaine de pid =%d le bateau a demarré\n",getpid());   //fct rowboat
                v(semid,0); // mutex
            }
        }
    }
}