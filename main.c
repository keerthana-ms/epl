#include "userthread.h"
#include "usersemaphore.h"


CISSem_t semaphore;

int sharedValue = 0;

void* incrementThread(void* arg) {
int threadId = *((int*)arg);

printf("Thread %d: Waiting for semaphore\n", threadId);
waitSemaphore(&semaphore);

printf("Thread %d: Semaphore acquired\n", threadId);

// Critical section
sharedValue++;
printf("Thread %d: Incremented shared value: %d\n", threadId, sharedValue);

postSemaphore(&semaphore);
printf("Thread %d: Semaphore released\n", threadId);

exitThread(NULL);
}

int main() {
CISSemErr semResult = initializeSemaphore(&semaphore, 1);
if (semResult != CIS_SEM_ERR_SUCCESS) {
printf("Semaphore initialization failed\n");
return 1;
}

ThreadId thread1, thread2;
int threadId1 = 1, threadId2 = 2;

CISThreadErr threadResult1 = createThread(&thread1, incrementThread, &threadId1);
if (threadResult1 != CIS_THREAD_ERR_SUCCESS) {
printf("Thread creation failed for Thread 1\n");
return 1;
}

CISThreadErr threadResult2 = createThread(&thread2, incrementThread, &threadId2);
if (threadResult2 != CIS_THREAD_ERR_SUCCESS) {
printf("Thread creation failed for Thread 2\n");
return 1;
}

threadResult1 = joinThread(thread1, NULL);
if (threadResult1 != CIS_THREAD_ERR_SUCCESS) {
printf("Thread join failed for Thread 1\n");
return 1;
}

threadResult2 = joinThread(thread2, NULL);
if (threadResult2 != CIS_THREAD_ERR_SUCCESS) {
printf("Thread join failed for Thread 2\n");
return 1;
}

semResult = destroySemaphore(&semaphore);
if (semResult != CIS_SEM_ERR_SUCCESS) {
printf("Semaphore destruction failed\n");
return 1;
}

return 0;
}

