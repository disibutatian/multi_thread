#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

typedef struct ct_sum { 
	long long sum;
}ct_sum;


pthread_spinlock_t lock;

//thread1
void * add1(void * cnt)
{     

    pthread_spin_lock(&lock);
	//看进程是否处于休眠状态
	sleep(20);

    int i;
    for(i = 0; i < 50000; i++)
    {
		(*(ct_sum*)cnt).sum+=i;
    }
    pthread_spin_unlock(&lock);

    pthread_exit(NULL);

    return 0;
}

//thread2
void * add2(void *cnt)
{     
    int j;
    //cnt = (ct_sum*)cnt;

    pthread_spin_lock(&lock);

    for( j = 50000;j < 100001; j++)
    {    
		(*(ct_sum*)cnt).sum += j;
    }
    pthread_spin_unlock(&lock);
    pthread_exit(NULL);
    return 0;
}

int main(void)
{ 
  	pthread_t ptid1,ptid2;

  	ct_sum cnt;

    pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);//maybe PHREAD_PROCESS_PRIVATE or PTHREAD_PROCESS_SHARED

  	cnt.sum = 0;

 	//printf("sum %d\n",cnt.sum);

  	pthread_create(&ptid1, NULL, add1, &cnt);
	pthread_create(&ptid2, NULL, add2, &cnt);




 	pthread_join(ptid1,NULL);
 	pthread_join(ptid2,NULL);

  	pthread_spin_destroy(&lock);

	// 这里可以保证线程都执行完再打印
 	printf("sum %lld\n",cnt.sum);

  	return 0;
}	 
