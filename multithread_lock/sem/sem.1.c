#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void func1(void* arg)
{
	while(1) {
		// <= 0, 阻塞
    	sem_wait(&sem);

    	int *running = arg;

    	printf("pthread1 running\n");
    	printf("%d\n",*running);
	}
}

void func2(void* arg)
{
	while(1) {

    	printf("pthread2 running\n");
		//信号量 +1
    	sem_post(&sem);
		sleep(1);
	}
}

int main()
{
    sem_init(&sem,0,0);
    pthread_t thread[2];

    int a = 5;

    pthread_create(&(thread[0]),NULL,(void*)func1,(void*)&a);
    printf("main thread running\n");
    sleep(5);

    pthread_create(&(thread[1]),NULL,(void*)func2,(void*)&a);
    printf("main thread running2\n");

    pthread_join(thread[0],NULL);
    pthread_join(thread[1],NULL);
    sem_destroy(&sem);

    return 0;
}
