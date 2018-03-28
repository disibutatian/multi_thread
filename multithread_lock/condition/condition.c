#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include<unistd.h>

int finished = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//条件变量
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* do_homework(void* arg) {
  // doing homework
  sleep(5);
  pthread_mutex_lock(&lock);
  finished = 1;
  pthread_mutex_unlock(&lock);
  // 唤醒队列中的一个线程（如果有线程的话）
  pthread_cond_signal(&cond);
  printf("发送条件信号--------------\n");
}

void* check_homework(void* arg) {
  // 打电话
  sleep(1);
  // 电话接通
  pthread_mutex_lock(&lock);
  // 作业写完了吗？
  printf("老师：作业写完了吗？!\n");
  while(finished == 0) {
    // 没写完呐！
    printf("学生：没写完呐!\n");
    // 好的，你接着写
    printf("老师：好的，你接着写吧!\n");
    printf("-------------------------\n");
    // 因为此时加了锁，所以 finished 变量不可能产生变化。
    // 将线程加入等待队列，线程让出 cpu，同时将 mutex 解锁。
    pthread_cond_wait(&cond, &lock);
    printf("老师：作业写完了吗？!\n");
  }
  printf("学生：写完啦!\n");
  pthread_mutex_unlock(&lock);
  printf("老师开始检查---------------\n");
}


int main() {

  pthread_t tid1, tid2;

  pthread_create(&tid1, NULL, do_homework, NULL);
  pthread_create(&tid2, NULL, check_homework, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  return 0;
}
