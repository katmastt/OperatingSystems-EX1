#include "mytypes.h"



int main( int argc, char* argv[]) {
    int er, N, K,count, MEM_SIZE, i ;
    int * ch;
    pid_t shm_pid, w_in_sem, r_in_sem, w_out_sem, r_out_sem ;
    union semun arg;
    struct shared_mem* mem;
  
    arg.val=0;
    MEM_SIZE= sizeof(struct shared_mem);
    N=atoi(argv[1]);
    K=atoi(argv[2]);

     
    shm_pid=shmget(MEM_KEY, MEM_SIZE, IPC_CREAT);
    mem =  (struct shared_mem*)shmat(shm_pid, (void*)0 , 0);
    mem = (struct shared_mem*)malloc(MEM_SIZE);    

    w_in_sem =semget(SEM_KEY1, 1, IPC_CREAT|0660);
    r_in_sem =semget(SEM_KEY2, 1, IPC_CREAT|0660);  
    w_out_sem =semget(SEM_KEY3, 1, IPC_CREAT|0660);
    r_out_sem =semget(SEM_KEY4, 1, IPC_CREAT|0660);    

    semctl(r_in_sem, 0, SETVAL, arg.val);
    semctl(r_out_sem, 0, SETVAL, arg.val);
    arg.val=1;
    semctl(w_in_sem, 0, SETVAL, arg.val);
    semctl(w_out_sem, 0, SETVAL, arg.val);
//////////////////////////////////////////////////////////////////////////

    printf("Father's PID : %d\n", getpid()); 
    ch=creation(N);
    count = consumer_proc(K,N);
     
    free(mem); 
    er=shmctl(shm_pid,IPC_RMID,0);
    if(er>=0) printf("Shared memory was removed successfully\n");
    er=semctl(w_in_sem,0,IPC_RMID,0);
    if(er>=0) printf("Semaphore with key: KEY1, was removed successfully\n");
    er=semctl(r_in_sem,0,IPC_RMID,0);
    if(er>=0) printf("Semaphore with key: KEY2, was removed successfully\n");
    er=semctl(w_out_sem,0,IPC_RMID,0);
    if(er>=0) printf("Semaphore with key: KEY3, was removed successfully\n");
    er=semctl(r_out_sem,0,IPC_RMID,0);
    if(er>=0) printf("Semaphore with key: KEY4, was removed successfully\n");
    printf("Exit\n");
    free(ch);
    return 0;
}
  


