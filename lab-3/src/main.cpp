#include<pthread.h>
#include<iostream>
#include <ctime> 
#include<vector>
#include<fstream>
#include<chrono>

using namespace std;

static int minimal = 1e9;
pthread_mutex_t mutex;
int flag = 0;
vector<int> result(100);

struct arg_to_thread {
	int* big_array;
	int partition;
	int num_of_thread;
	int count_threads;
	int size_array;
};

void* thread_func(void *args)
{	
    int minimum = 1e9;
	arg_to_thread* arguments = (arg_to_thread*) args;
	int num_of_thread = arguments->num_of_thread;
	int partition = arguments->partition;
	flag = 1;
	int count_threads = arguments->count_threads;
	int size_array = arguments->size_array;
	int* big_array = arguments->big_array;

		if (num_of_thread != count_threads - 1) {
			for (int j = num_of_thread * partition; j < num_of_thread * partition + partition; ++j) {
					if (big_array[j] < minimum) {
						minimum = big_array[j];
                        result[num_of_thread] = big_array[j];
					}
			}	
		} else {
			for (int j = size_array - 1; j > size_array - partition - 1; --j) {	
					if (big_array[j] < minimum) {
                        minimum = big_array[j];
                        result[num_of_thread] = big_array[j];
                    }
			}	

		}


	
	return 0;
}

int main(int argc, char const *argv[])
{
	
    ifstream file("./src/big_massives.txt");
	int count_threads;
	cout<<"Введите количество потоков: ";
	cin>>count_threads;
	cout<<endl;
	int size_array;
	cout<<"Введите размер массива: ";
	cin>>size_array;
	cout<<endl;
	vector<int>big_array(size_array);
	cout<<"Введите массив: "<<endl;
	for(int i = 0; i < size_array; ++i) {
        string s;
        getline(file, s);

	   big_array[i] = atoi(s.c_str());
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
	arg.big_array = big_array.data();
	arg.count_threads = count_threads;
	arg.size_array = size_array;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
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
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

	pthread_mutex_destroy(&mutex);
    int minimum2 = 1e9;
    for (int i = 0; i < count_threads; ++i) {
        if (result[i] < 1e9) {
            minimum2 = result[i];
        }
    }
	cout<<"Минимальное значение: "<<minimum2<<endl;
    cout<<chrono::duration_cast<chrono::microseconds>(end-begin).count();
	return 0;

}
//find_minimal(big_array, i, partition)



