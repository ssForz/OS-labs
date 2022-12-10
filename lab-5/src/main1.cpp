#include<iostream>
#include<stdio.h>
#include<string>
#include"lib.h"
#include"config.h"
using namespace std;

int main()
{
	cout<<"Compiler version: "<<COMP_VER<<endl;
	cout<<"Compiler ID: "<<COMP_ID<<endl;
	cout<<"Compilation date: "<<TIME_NOW<<endl;
	cout<<"You should write commands: <command> <arg1> <arg2> ... <argn>"<<endl;
	cout<<"If you want to count prime numbers in range [a,b], write: 1 <a> <b>"<<endl;
	cout<<"If you want to translate number from decimal to ";
	translation(-1);
	cout<<", write: 2 <number>"<<endl;
	int command = 0;
	while(cin>>command) {
		if (command == 1) {
			int a, b;
			int cnt;
			cin>>a>>b;
			cnt = PrimeCount(a, b);
			cout<<"Count of rime numbers in range ["<<a<<","<<b<<"]: "<<cnt<<endl;
			continue;
		}
		if (command == 2) {
			long a;
			char* answ;
			cin>>a;
			answ = translation(a);
			cout<<"Result of translation:"<<endl;
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