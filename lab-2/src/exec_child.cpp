#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <string.h>

using namespace std;

int main(int argc, char const *argv[])
{	
	string filename = argv[3];
	int fd[2];
	fd[0] = stoi(argv[0]);
	fd[1] = stoi(argv[1]);
	fstream cur_file;
	cur_file.open(filename, fstream::in | fstream::out | fstream::app);
	while (true) {
		int size_of_str;
		read(fd[0], &size_of_str, sizeof(int));
		char str_array[size_of_str];
		read(fd[0], &str_array, sizeof(char)*size_of_str);
		string result_str;
		for (int i = size_of_str - 1; i >= 0; i--) {
			result_str.push_back(str_array[i]);
		}
		cur_file << result_str << endl;
		cout<<"Added result stroke ("<<result_str<<") to child-"<<argv[2]<<" file!"<<endl;
	}


	return 0;

}