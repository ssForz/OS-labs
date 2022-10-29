#include <iostream>
#include <ctime>
#include<fstream> 
#include<vector>                   
using namespace std;

int main(int argc, char* argv[])
{
    ofstream out;
    out.open("big_massives.txt");
    srand(time(0));
    const int array_size = 1e8; // размер одномерного массива
    for (int counter = 0; counter < array_size; counter++) {
        out<<rand() % 50 - rand() % 50<<endl;
    }
    
    cout << "runtime = " << clock()/1000.0 << endl; // время работы программы                  
    return 0;
}
