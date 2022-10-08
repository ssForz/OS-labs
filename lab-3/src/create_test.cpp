#include <iostream>
#include <ctime>
#include<fstream>                    
using namespace std;

int main(int argc, char* argv[])
{
    ofstream out;
    out.open("big_massives.txt");
    srand(time(0));
    const int array_size = 1000; // размер одномерного массива
    int array1[array_size]; // объявление одномерного массива
    for (int counter = 0; counter < array_size; counter++) {
        array1[counter] = rand() % 50 - rand() % 50; // заполняем массив случайными значениями в диапазоне от -49 до 49 включительно
        cout << array1[counter] << " "; // печать элементов одномерного массива array1
    }
    
    for (int counter = 0; counter < array_size; counter++) {
        out<<array1[counter]<<" ";
    }
    cout << "runtime = " << clock()/1000.0 << endl; // время работы программы                  
    return 0;
}