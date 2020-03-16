#include <iostream>
#include <iomanip>
#include <pthread.h>
using namespace std;


const int ARRAY_SIZE = 400000;
const int NUM_OF_THREADS = 4;


class Numbers {

private:

          int size;

          double *myarray;

          double sum;



public:

          Numbers () {
            myarray = new double [ARRAY_SIZE];

            cout << "Populating array:" << endl;
            for(int i = 0; i < ARRAY_SIZE; i++){
              myarray[i] = double((rand() % 400 + 100))/100;

              cout << "\r" << setprecision(2) << fixed << double(i + 1 )/ (ARRAY_SIZE/100) << "%  " << flush;
            }
            cout << endl;
          }

          ~Numbers(){
            delete []myarray;
          }

          double get_sum(){
            return sum;
          }

          void add_sum(double addedNum){
            sum += addedNum;
          }

          double* get_array_ptr(int index){
            return &myarray[index];
          }

};

struct thread_data{
  int thread_id;
  Numbers * numbers;
  pthread_mutex_t * lock;
};



void *do_work(void *arg){

  struct thread_data * data = (struct thread_data *)arg;
  int thread_id = data -> thread_id;
  Numbers * numbers = data -> numbers;
  double * arrayptr = numbers -> get_array_ptr(thread_id * ARRAY_SIZE/NUM_OF_THREADS);
  pthread_mutex_t * lock = data -> lock;

  double tempSum = 0;
  double * tempptr = (double * )arg;
  for(int i = 0; i < (ARRAY_SIZE/NUM_OF_THREADS); i++){
    tempSum += arrayptr[i];
    }
  cout << endl;

  pthread_mutex_lock(lock);
  cout << "Locked for thread[" << thread_id << "] and adding " << tempSum << " to existing sum:" << numbers-> get_sum() << endl;
  numbers-> add_sum(tempSum);
  cout << "New sum is: " << numbers -> get_sum() << endl;
  cout << "Releasing lock from thread[" << thread_id <<"]..." << endl;
  pthread_mutex_unlock(lock);
}


int main(){

  Numbers numbers = Numbers();

  pthread_t threadList[NUM_OF_THREADS];
  struct thread_data data[NUM_OF_THREADS];
  pthread_mutex_t lock;

  pthread_mutex_init(&lock, NULL);

  for(int i = 0; i <NUM_OF_THREADS; i++){
    data[i].thread_id = i;
    data[i].numbers = &numbers;
    data[i].lock = &lock;
    pthread_create(&threadList[i], NULL, do_work, &data[i]);
  }

  for(int i = 0; i < NUM_OF_THREADS; i++){
    pthread_join(threadList[i], NULL);
  }

  cout << "\n---\nFINAL RESULT: " << numbers.get_sum()<<endl;

  return 0;
}
