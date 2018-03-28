#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

typedef struct ct_sum { 
	long long sum;
  	pthread_mutex_t lock;
}ct_sum;

/***
 
1. 如果不加锁的话，会出现sum 被覆盖的情形
2. https://blog.csdn.net/scucj/article/details/3479829
3. 
 	* a = a + 1 // 汇编执行步骤如下, 非原子操作，所以也就是非线程安全
	*把a读取到一个寄存器
	*将寄存器中的值加1
	*寄存器中的新值写入a中


 */

//thread1
void * add1(void * cnt)
{     

    pthread_mutex_lock(&(((ct_sum*)cnt)->lock));
	//看进程是否处于休眠状态
	sleep(20);

    int i;
    for(i = 0; i < 50000; i++)
    {
		(*(ct_sum*)cnt).sum+=i;
    }
 	//printf("sum %lld\n",((ct_sum*)cnt)->sum);
    pthread_mutex_unlock(&(((ct_sum*)cnt)->lock));

    pthread_exit(NULL);

    return 0;
}

//thread2
void * add2(void *cnt)
{     
    int j;
    //cnt = (ct_sum*)cnt;

    pthread_mutex_lock(&(((ct_sum*)cnt)->lock));

    for( j = 50000;j < 100001; j++)
    {    
		(*(ct_sum*)cnt).sum += j;
    }
 	//printf("sum %lld\n",((ct_sum*)cnt)->sum);
    pthread_mutex_unlock(&(((ct_sum*)cnt)->lock));
    pthread_exit(NULL);
    return 0;
}

int main(void)
{ 
  	pthread_t ptid1,ptid2;

  	ct_sum cnt;

  	pthread_mutex_init(&(cnt.lock),NULL);

  	cnt.sum = 0;

 	//printf("sum %d\n",cnt.sum);

  	pthread_create(&ptid1, NULL, add1, &cnt);
	pthread_create(&ptid2, NULL, add2, &cnt);




 	pthread_join(ptid1,NULL);
 	pthread_join(ptid2,NULL);

  	pthread_mutex_destroy(&(cnt.lock));

	// 这里可以保证线程都执行完再打印
 	printf("sum %lld\n",cnt.sum);

  	return 0;
}	 
