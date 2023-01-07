
typedef struct data{
    int hackers;
    int windows;
    int compteur;
    int enter;
}sdata;
int sem_init(int semid, int numsem, int initval);
int semcreate(key_t key, int n);
int semtimedop(int semid, struct sembuf *sops, unsigned nsops,struct timespec *timeout);
void Ptime(int semid ,int x);
int p(int semid, int numsem);
int v(int semid, int numsem);