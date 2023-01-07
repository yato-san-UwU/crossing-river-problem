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
#include "bibi.h"    
                    // programme window similaire au hacker 
                    // difference au niveau des semaphore P() V()

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
    //------------------------------------------------------------------------------------------------------------
    // partie memoire partager 
    //------------------------------------------------------------------------------------------------------------
    key_t key=ftok("chemin ou ce trouve votre fichier",'c');
    key_t key2=ftok("chemin ou ce trouve votre fichier",3); 
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
    // partie memoire partager 
    //------------------------------------------------------------------------------------------------------------
    int y;
    bool isCapitain=false;
    printf("nombre de processus windows qui traverse :");
    scanf("%d",&y);

    for(int R=0;R<y;R++){  // y nbr de windows
        int pid=fork();
        if(pid==-1){
            perror("erreur de creation");
            exit(0);
        }
        if(pid==0){
            p(semid,0); // mutex
            (sd->windows)=(sd->windows+1);
            if ((sd->windows)==4){
                for (int i=0;i<3;i++){
                    v(semid,2); //windowQueue
                }
                sd->windows=0;
                isCapitain=true;
                printf("je suis le capitaine 'CONDITON 1' mon pid est %d et je suis un window\n",getpid());
            }
            else if((sd->windows==2) && (sd->hackers >= 2)){
                v(semid,2);     // windowsQueue
                v(semid,1);     // hackersQueue
                v(semid,1);     // hackersQueue
                (sd->hackers)=(sd->hackers-2);
                sd->windows=0;
                isCapitain=true;
                printf("je suis le capitaine 'CONDIION 2' mon pid est %d et je suis un window\n",getpid());
            }
            else{
                v(semid,0);     // mutex
                p(semid,2); // windowsQueue
            }
            
            printf("je suis le window N%d de pid '%d' j'embarque\n",R+1,getpid()); // fct board
            printf("en att des autres processus N%d (W)\n",R+1);
            barrier(sd,semid,y);            // y nbr de processus
            printf("fin de l'attente N%d (W)\n",R+1);

            if (isCapitain==true){
                printf("je suis le capitaine de pid '%d' le bateau a demarré\n",getpid());   //fct rowboat
                v(semid,0); // mutex
            }
        }
    }
}
