#include "mytypes.h"

int producers_proc(void) {
    int  pid, pid1,pid2,pid4,pid3, er,nu_line, sem_pid, count=0 ;
    int  pid_match=0;
    char line[200];
    char* message;
    struct shared_mem * mem ;
    static const char filename[] = "example.txt";
    struct sembuf down[1]={0,-1,0}, up[1]={0,1,0};
    srand(time(NULL));
    nu_line = rand()% LINES ;
    printf("Start of producer's function with pid : %d\n", getpid());
    
    sem_pid = shmget(MEM_KEY, 0, 0);
    pid1 = semget(SEM_KEY1, 1, IPC_CREAT);
    pid2 = semget(SEM_KEY2, 1, IPC_CREAT);
    pid4 = semget(SEM_KEY4, 1, IPC_CREAT);
    pid3 = semget(SEM_KEY3, 1, IPC_CREAT);
    mem = shmat(sem_pid, (void*)0 , 0);
    
    while(1) {
       FILE* file= fopen(filename, "r");
       while (count!= nu_line) {
           message = fgets(line, sizeof(line), file);
           count++;
       }
       count=0;
       while(count<200 && strcmp(&line[count],"\n")!=0){
           count++;
       }
       line[count]= '\0'; 
       count=0;
       fclose(file);
       
       er =semop(pid1, down, 1);
       if (er>=0) printf("I am  prod : %d and i did a down op at semaphore with key: KEY1  \n",getpid()); 
       
       (mem ->in_ds).pid = getpid();
       (mem ->in_ds).line = line;
      
       er =semop(pid2, up, 1);
       if (er>=0) printf("I am prod : %d i did an up op in semaprhore with key: KEY2\n", getpid());
    
       er =semop(pid4, down, 1);
       if (er>=0) printf("I am  prod : %d and i did a down op at semaphore with key: KEY4\n",getpid());
       
       printf("i am producer : %d and this is the capitalized line:\n", getpid());
       printf("%s\n", (mem->out_ds).line);
       if (mem ->out_ds.pid == getpid()) {
           pid_match++;
       }
       else if (mem->out_ds.pid != 0)
           printf("This is the pid of process which manage to enter shared mem  %d\n",(mem ->out_ds).pid);
       else if (mem ->out_ds.pid == 0) {
           printf("I'm  producer : %d  and i'm about to exit\n",getpid()); 
           er=shmdt(mem);
           if(er>=0) printf("Producer: %d was detached successfully from  shared memory\n",getpid());
           exit(pid_match);
       }

       er =semop(pid3, up, 1);
       if (er>=0) printf("I am  prod : %d and i did a up op at semaphore with key: KEY3\n",getpid());
   }   

}

///////////////////////////////////////////   CONSUMER    //////////////////////////////////////////////////////////////

int consumer_proc(int K, int N) {
    int pid1, pid2, pid3, pid4, er, sem_pid, p_pid, i, status, count=0 ;
    char* message;
    char* mes1;
    int* ch;
    struct shared_mem * mem ;
    struct sembuf down[1]={0,-1,0}, up[1]={0,1,0};
    
    sem_pid = shmget(MEM_KEY, 0, 0);
    mem = shmat(sem_pid, (void*)0 , 0);
    pid2 = semget(SEM_KEY2, 1, IPC_CREAT);    
    pid1 = semget(SEM_KEY1, 1, IPC_CREAT);
    pid3 = semget(SEM_KEY3, 1, IPC_CREAT);
    pid4 = semget(SEM_KEY4, 1, IPC_CREAT); 

    printf("Hi i am consumer with pid :%d\n", getpid());    
    while(K>0) {
        er =semop(pid2, down, 1);
        if (er>=0) printf("I am  the consumer: %d and i am readind from in_ds struct\n",getpid());
    
        p_pid = mem ->in_ds.pid;
        message = mem ->in_ds.line;
    
        er =semop(pid1, up, 1);
        if (er>=0) printf("I am  consumer : %d and i did an up op at semaphore with key: KEY1\n",getpid());
        mes1 = message;
        while(strcmp((message+count),"\0")!=0) {
            if(strcmp((message+count)," ")!=0 && *(message+count)>='a' && *(message+count)<='z')
                *(message+count)-=('a'-'A');
            count++;
        }
        count=0;
     
        er =semop(pid3, down, 1);
        if (er>=0) printf("I am  consumer : %d and i did a down op at semaphore with key: KEY3\n",getpid());
        mem ->out_ds.pid = p_pid;
        mem ->out_ds.line = mes1;
    
        er =semop(pid4, up, 1);
        if (er>=0) printf("I am  consumer : %d and i did an up op at semaphore with key: KEY4\n",getpid());
    
        K--;
    }
    
    for(i=0; i<N; i++) {
        er =semop(pid3, down, 1);
        mem->out_ds.pid = 0;
        er =semop(pid4, up, 1);
        wait(&status);
        count+=WEXITSTATUS(status);
        er =semop(pid1, up, 1);
        er =semop(pid3, up, 1);
        printf("I am consumer and i took the exit call number: %d\n",i+1);        
    }
    printf("Number of proccesses : %d\n", N);
    printf("Number of steps : %d\n", K);
    printf("Number of matches : %d\n", count); 
    printf("I am consumer and i am about to exit!\n");
    er=shmdt(mem);
    if (er>=0) printf("I am consumer: %d and i was detached successfully from shared memory!\n", getpid());
    return 0;

}

