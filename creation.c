#include "mytypes.h"

int* creation (int N) {
    int i, k;
    int *childs = malloc(N*sizeof(int));
    for (i=0; i<N; i++) {
        int pid=fork();
        if (pid < 0) {
            perror("Fork Failed!");
            exit(1);
        } 
        else if (pid == 0) {
            producers_proc(); 
            exit(0);
        } 
        else {
            childs[i] = pid;
            printf("%dst Producer's pid : %d\n",i+1, pid);
        }
     }

     return childs;
}


