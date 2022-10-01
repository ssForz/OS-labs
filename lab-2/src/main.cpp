#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>


using namespace std;


int main()
{
	string current_str;
	int child_tag;
	int fd[2];
	char *const child_args[] = { "./exec_child", NULL };
	fstream res_file;
	string child1, child2;
	cout<<"Enter the name for first child file: ";
	cin>>child1;
	cout<<endl;
	cout<<"Enter the name for second child file: ";
	cin>>child2;
	cout<<endl;
	int fd1[2];
	int fd2[2];
	
	if (pipe(fd1) == -1) {
		cout<<"Pipe error occured"<<endl;
		exit(EXIT_FAILURE);
	}
	if (pipe(fd2) == -1) {
		cout<<"Pipe error occured"<<endl;
		exit(EXIT_FAILURE);
	}

	pid_t f_id1 = fork();
	if (f_id1 == -1) {
		cout<<"Fork error with code -1 returned in the parent, no child_1 process is created"<<endl;
		exit(EXIT_FAILURE);
	} else if (f_id1 == 0) { //child1
		child_tag = 1;
		fd[1] = fd1[1];
		fd[0] = fd1 [0];
		string child = child1;
		execlp(child_args[0], to_string(fd[0]).c_str(), to_string(fd[1]).c_str(), "1", child.c_str(), NULL);
		perror("Execlp error");
		
	}


	pid_t f_id2 = fork();
	if (f_id2 == -1) {
		cout<<"Fork error with code -1 returned in the parent, no child_2 process is created"<<endl;
		exit(EXIT_FAILURE);
	} else if (f_id2 == 0) { //child2
		child_tag = 2;
		fd[1] = fd2[1];
		fd[0] = fd2[0];
		string child = child2;
		execlp(child_args[0], to_string(fd[0]).c_str(), to_string(fd[1]).c_str(), "2", child.c_str(), NULL);
		perror("Execlp error");
		
	}

	else { //parent
		cout<<"Enter amount of strings: ";
		int cnt;
		cin>>cnt;
		cout<<endl;
		for (int i = 0; i < cnt; ++i) {
			cin>>current_str;
			int s_size = current_str.size();
			char str_array[s_size];
			for (int k = 0; k < s_size; ++k) {
				str_array[k] = current_str[k];
			}
			if (rand() % 6 < 5) {
				write(fd1[1], &s_size, sizeof(int));
				write(fd1[1], str_array, sizeof(char)*s_size);
			
			} else {
				write(fd2[1], &s_size, sizeof(int));
				write(fd2[1], str_array, sizeof(char)*s_size);
				
			}
		}
	}
	close(fd2[1]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd1[0]);
	/*kill(f_id1, SIGTERM);
	kill(f_id2, SIGTERM);*/
	return 0;
}