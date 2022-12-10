#include<iostream>
#include<stdio.h>
#include<dlfcn.h>
#include"lib.h"
#include<string>
#include"config.h"

using namespace std;

int main()
{
	cout<<"Compiler version: "<<COMP_VER<<endl;
	cout<<"Compiler ID: "<<COMP_ID<<endl;
	cout<<"Compilation date: "<<TIME_NOW<<endl;
	string lib1 = "./liblib1.so";
	string lib2 = "./liblib2.so";
	int command;
	cout<<"You are in program-1 now"<<endl;
	cout<<"You should write commands: <command> <arg1> <arg2> ... <argn>"<<endl;
	cout<<"If you want to change realization of calculation, write 0"<<endl;
	cout<<"If you want to count prime numbers in range [a,b], write: 1 <a> <b>"<<endl;
	cout<<"If you want to translate number from decimal to other, write: 2 <number>"<<endl;
	void* curlib = dlopen(lib1.c_str(), RTLD_LAZY);
	cout<<"Using lib1 at start"<<endl;
	int (*PrimeCount)(int a, int b);
	char* (*translation)(long x);
	PrimeCount = (int(*)(int, int))dlsym(curlib, "PrimeCount");
    translation = (char*(*)(long))dlsym(curlib, "translation");
	//CountPrime = dlsym(curlib, "CountPrime");
	//translation = dlsym(curlib, "translation");
	
	int lib_id = 1;

	while(cin>>command) {
		if (command == 0) {
			dlclose(curlib);
			if (lib_id == 1) {
				curlib = dlopen(lib2.c_str(), RTLD_LAZY);
				lib_id = 2;
			} else {
				curlib = dlopen(lib1.c_str(), RTLD_LAZY);
				lib_id= 1;
			}
			PrimeCount = (int(*)(int, int))dlsym(curlib, "PrimeCount");
    		translation = (char*(*)(long))dlsym(curlib, "translation");
			continue;
		}
		if (command == 1) {
			int a, b;
			int cnt;
			cin>>a>>b;
			cnt = PrimeCount(a, b);
			cout<<"Count of prime numbers in range ["<<a<<","<<b<<"]: "<<cnt<<endl;
			continue;
		}
		if (command == 2) {
			long a;
			char* answ;
			cin>>a;
			answ = translation(a);
			cout<<"Result of translation to "; 
			translation(-1);
			cout<<":"<<endl;
			for (int i = 0; i < 32; i++) {
				if (answ[i] == '1' || answ[i] == '0' || answ[i] == '2') {
					cout << answ[i];
				}
			}
			cout<<endl;
			delete answ;
			continue;
		}
		cout<<"Wrong command input"<<endl;
	}

}