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
	int resheto[b];
	for (int i = 0; i < b + 1; i++) {
		resheto[i] = i;
	}
	for (int p = 2; p < b + 1; p++) {
		if (resheto[p] != 0) {
			for (int j = p*p; j < b + 1; j+=p) {
				resheto[j] = 0;
			}
		}
	}
	for (int i = a; i <= b; i++) {
		if (resheto[i] != 0) {
			counter++;
		}
	}
	return counter;
}


extern "C" char* translation(long x)
{	
	if (x == -1) { 
		cout<<"ternary";
	}
	int cnt = 0;
	int sizelong = 20;
	char* ternary = new char[sizelong];
	for (int i = 0; i < sizelong; i++) {
		ternary[i] = '9';
	}
	while(x > 0) {
		if (x%3 == 1) {
			ternary[sizelong - cnt - 1] = '1';
		} else if (x%3 == 2){
			ternary[sizelong - cnt - 1] = '2';
		} else {
			ternary[sizelong - cnt - 1] = '0';
		}
		x = x/3;
		cnt++;
	}
	return ternary;
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
	for (int i = 0; i < 20; i++) {
		if (answ[i] == '1' || answ[i] == '0' || answ[i] == '2') {
			cout << answ[i];
		}
	}
	cout<<endl;
	delete answ;
	return 0;
}*/