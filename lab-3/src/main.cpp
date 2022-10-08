#include<pthread.h>
#include<iostream>
#include <ctime> 

using namespace std;

static int minimal = 1e9;
pthread_mutex_t mutex;
int flag = 0;


struct arg_to_thread {
	int* big_array;
	int partition;
	int num_of_thread;
	int count_threads;
	int size_array;
};

void* thread_func(void *args)
{	
	arg_to_thread* arguments = (arg_to_thread*) args;
	int num_of_thread = arguments->num_of_thread;
	int partition = arguments->partition;
	flag = 1;
	int count_threads = arguments->count_threads;
	int size_array = arguments->size_array;
	int* big_array = arguments->big_array;
		cout<<"You in thread: "<<num_of_thread<<endl;

		if (num_of_thread != count_threads - 1) {
			for (int j = num_of_thread * partition; j < num_of_thread * partition + partition; ++j) {
				cout<<"Выполнение в потоке "<<num_of_thread<<endl;
				pthread_mutex_lock(&mutex);
					if (big_array[j] < minimal) {
						minimal = big_array[j];
					}
				pthread_mutex_unlock(&mutex);
			}	
		} else {
			for (int j = size_array - 1; j > size_array - partition - 1; --j) {	
				cout<<"Выполнение в потоке "<<num_of_thread<<endl;
				pthread_mutex_lock(&mutex);
					if (big_array[j] < minimal) {
						minimal = big_array[j];
					}
				pthread_mutex_unlock(&mutex);
			}	

		}


	
	return 0;
}

int main(int argc, char const *argv[])
{
	srand(time(0));
	int count_threads;
	cout<<"Введите количество потоков: ";
	cin>>count_threads;
	cout<<endl;
	int size_array;
	cout<<"Введите размер массива: ";
	cin>>size_array;
	cout<<endl;
	int big_array[size_array];
	cout<<"Введите массив: "<<endl;
	for(int i = 0; i < size_array; ++i) {
		cin>>big_array[i];
	}
	if (count_threads > size_array) {
		cout<<"Количество потоков больше максимального"<<endl;
		count_threads = size_array;
	}
	pthread_t threads[count_threads];

	pthread_mutex_init(&mutex, NULL);
	int partition = size_array / count_threads;
	struct arg_to_thread arg;
	arg.partition = partition;
	arg.big_array = big_array;
	arg.count_threads = count_threads;
	arg.size_array = size_array;

	for (int i = 0; i < count_threads; i++) {
		arg.num_of_thread = i;
		if (i == count_threads - 1) {
			partition += size_array % count_threads;
			arg.partition = partition;
		}
		int status = pthread_create(&threads[i], NULL, thread_func, (void*)&arg);
		while(flag != 1) {
			//ожидание пока функции передадутся аргументы
		}
		flag = 0;
		if (status != 0) {
			cout<<"Create thread error"<<endl;
		}
	}

	for (int i = 0; i < count_threads; ++i) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);

	cout<<"Минимальное значение: "<<minimal<<endl;
	cout << "runtime = " << clock()/1000.0 << endl;
	return 0;

}
//find_minimal(big_array, i, partition)



