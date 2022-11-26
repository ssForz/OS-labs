#include <iostream>
#include <string>
#include <sys/types.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <sys/mman.h> 
#include <cstdio>


using namespace std;


int flaccess = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

int child_execute(string path, char *mapped, string sem_file) {

	int counter = 0;
	string filename = path;
	fstream cur_file;
	cur_file.open(filename, fstream::in | fstream::out | fstream::app);
	sem_t *semaphore = sem_open(sem_file.c_str(), 1);
	
	

	while (true) {
		if (sem_wait(semaphore) == -1) {
			perror("Semaphore error");
			exit(EXIT_FAILURE);
		}
		if (mapped[counter] == '!') {
			cout<<"ITS OVER"<<endl;
			break;
		}
		int cur_size = (int)mapped[counter];
		
		int start = counter;

		char str_array[cur_size];
		int i = 0;
		for (; counter < start + cur_size ; counter++) {
			str_array[i] = mapped[counter+1];
			i++;
		}
		string result_str;
		for (int j = cur_size - 1; j >= 0; j--) {
			cur_file << str_array[j];
		}
		cur_file<<endl;
		cout<<"Added result stroke to "<<filename<<endl;
		counter++;
	}


	return 0;

}


int main()
{
	string current_str;
	string sem_file1 = "a.semaphore";
	string sem_file2 = "b.semaphore";
	string filename1, filename2;

	cout<<"Enter the name for first child file: ";
	cin>>filename1;
	cout<<endl;

	cout<<"Enter the name for second child file: ";
	cin>>filename2;
	cout<<endl;
	
	cout<<"Enter amount of strings: ";
	int cnt;
	cin>>cnt;
	const int mapsize = cnt*256;

	sem_t *semaphore1 = sem_open(sem_file1.c_str(), O_CREAT, flaccess, 0);
	if (semaphore1 == SEM_FAILED) {
		perror("Semaphore error");
		exit(EXIT_FAILURE);
	}
	sem_t *semaphore2 = sem_open(sem_file2.c_str(), O_CREAT, flaccess, 0);
	if (semaphore2 == SEM_FAILED) {
		perror("Semaphore error");
		exit(EXIT_FAILURE);
	}

	char *mapped1 = (char *)mmap(0, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	char *mapped2 = (char *)mmap(0, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	pid_t f_id1 = fork();
	if (f_id1 == -1) {
		cout<<"Fork error with code -1 returned in the parent, no child_1 process is created"<<endl;
		exit(EXIT_FAILURE);
	} else if (f_id1 == 0) { //child1
		string childfile = filename1;
		child_execute(childfile, mapped1, sem_file1);
		return 0;
		
		
	}

	pid_t f_id2 = fork();
	if (f_id2 == -1) {
		cout<<"Fork error with code -1 returned in the parent, no child_2 process is created"<<endl;
		exit(EXIT_FAILURE);
	} else if (f_id2 == 0) { //child2
		string childfile = filename2;
		child_execute(childfile, mapped2, sem_file2);
		return 0;
	}

	if (f_id1 != 0 && f_id2 != 0){ //parent
		cout<<endl;
		int stat_counter1 = 0;
		int stat_counter2 = 0;
		for (int i = 0; i < cnt+1; i++) {
			if (i == cnt) {
				mapped1[stat_counter1] = '!';
				mapped2[stat_counter2] = '!';
				sem_post(semaphore1);
				sem_post(semaphore2);
				break;
			}

			cin>>current_str;
			while (current_str.size() >= 20) {
				cout<<"Wrong size of string, try again (<20): "<<endl;
				cin>>current_str;
			}
			if (rand() % 6 < 5) {
				for (int j = 0; j < current_str.size()+1; j++) {
					if (j == 0) {
						mapped1[stat_counter1] = (char)current_str.size();
						continue;
					}
					mapped1[stat_counter1 + j] = current_str[j - 1];
				}
				sem_post(semaphore1);
				stat_counter1 += current_str.size() + 1;

			} else {
				
				for (int j = 0; j < current_str.size()+1; j++) {
					if (j == 0) {
						mapped2[stat_counter2] = (char)current_str.size();
						continue;
					}
					mapped2[stat_counter2 + j] = current_str[j - 1];
				}
				sem_post(semaphore2);
				stat_counter2 += current_str.size() + 1;
			}
		}
	}

	munmap(mapped1, mapsize);
	munmap(mapped2, mapsize);
	sem_close(semaphore1);
	sem_close(semaphore2);
	sem_unlink(sem_file1.c_str());
	sem_unlink(sem_file2.c_str());
	return 0;
}