# OperatingSystems-EX1

First exercise on Operating Systems course - Departement of Informatics and Telecommunications, National & Kapodistrian Uneversity of Athens.

# Run
**./ask1 ProdNum ConsNum**
- ProdNum: number of producer processes forked by the initial process
- ConsNum: number of consumer processes forked by the initial process

# Implementation

This project is a simulation of the coordination between different producers and consumers on a shared memory segment using semaphores. To create and access the shared memory, different functions were used: shmat, shmget, shmctl and shmdt. To coordinate the access of the various processes to shared memory four semapfores were designed.

The report of the project, where the implementation is fully explained, is also uploaded on the repository in Greek.
