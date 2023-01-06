#include <iostream>
#include "FBalloc.h"
#include "MKKalloc.h"
#include<chrono>
using namespace std;

int main()
{
    cout << "Memory allocators:" << endl;
    cout << "- Free blocks allocator (most fit policy)" << endl;
    cout << "- McKusick-Karels allocator" << endl;
    cout << endl<<endl;

    FBalloc a1;
    MKalloc a2;

    cout<<"Free blocks allocator (most fit policy):"<<endl<<endl;
    auto begin = chrono::steady_clock::now();
    a1.fb_print_info();

    void* p1 = a1.fbmalloc(100);
    cout << "p1 = " << p1 << endl << endl;
    a1.fb_print_info();

    void* p2 = a1.fbmalloc(100);
    cout << "p2 = " << p2 << endl << endl;
    a1.fb_print_info();

    void* p3 = a1.fbmalloc(100);
    cout << "p3 = " << p3 << endl << endl;
    a1.fb_print_info();

    a1.fbfree(p2, 100);
    cout << "p2 freed" << endl << endl;
    a1.fb_print_info();

    a1.fbfree(p1, 100);
    cout << "p1 freed" << endl << endl;
    a1.fb_print_info();

    a1.fbfree(p3, 100);
    cout << "p3 freed" << endl << endl;
    a1.fb_print_info();

    a1.fbdefragment();
    cout << "defragmented" << endl << endl;
    a1.fb_print_info();
    auto end = chrono::steady_clock::now();
  
    auto elapsed_ms = std::chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "The time: " << elapsed_ms.count() << " ms\n";
    cout<<"- McKusick-Karels allocator"<<endl<<endl;

    begin = chrono::steady_clock::now();
    a2.mk_print_info();

    void* p4 = a2.mkmalloc(256);
    cout << "p4 = " << p4 << endl << endl;
    a2.mk_print_info();

    void* p5 = a2.mkmalloc(256);
    cout << "p5 = " << p5 << endl << endl;
    a2.mk_print_info();

    void* p6 = a2.mkmalloc(250);
    cout << "p6 = " << p6 << endl << endl;
    a2.mk_print_info();

    void* p7 = a2.mkmalloc(256);
    cout << "p7 = " << p7 << endl << endl;
    a2.mk_print_info();

    a2.mkfree(p4);
    cout << "p4 freed" << endl << endl;
    a2.mk_print_info();

    a2.mkfree(p7);
    cout << "p7 freed" << endl << endl;
    a2.mk_print_info();

    a2.mkfree(p5);
    cout << "p5 freed" << endl << endl;
    a2.mk_print_info();

    a2.mkfree(p6);
    cout << "p6 freed" << endl << endl;
    a2.mk_print_info();

    a2.mkdefragment();
    cout << "defragmented" << endl << endl;
    a2.mk_print_info();
    end = std::chrono::steady_clock::now();
  
    elapsed_ms = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << "The time: " << elapsed_ms.count() << " ms\n";
}