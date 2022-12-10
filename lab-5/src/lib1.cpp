#include<iostream>

using namespace std;


extern "C" int PrimeCount(int a, int b)
{	
	if (a > b) {
		int c = b;
		b = a;
		a = c;
	}
	int counter = 0;
	for (int i = a; i <= b; i++) {
		int flag = 0;
		for (int k = 2; k < i; k++) {
			if (i%k == 0) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			counter++;
		}
	}
	return counter;
}


extern "C" char* translation(long x)
{	
	if (x == -1) { 
		cout<<"binary";
	}
	int cnt = 0;
	int sizelong = 31;
	char* binary = new char[sizelong];
	for (int i = 0; i < sizelong; i++) {
		binary[i] = '9';
	}
	while(x > 0) {
		if (x%2 == 1) {
			binary[sizelong - cnt - 1] = '1';
		} else {
			binary[sizelong - cnt - 1] = '0';
		}
		x = x/2;
		cnt++;
	}
	return binary;
}

/*int main()
{
	int a, b;
	long p;
	char* answ;
	cin>>a>>b;
	cin>>p;
	cout<<PrimeCount(a,b)<<endl;
	answ = translation(p);
	for (int i = 0; i < 32; i++) {
		if (answ[i] == '1' || answ[i] == '0') {
			cout << answ[i];
		}
	}
	cout<<endl;
	delete answ;
	return 0;
}*/