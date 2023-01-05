#pragma once
#include<iostream>
#include <cinttypes>
#include <vector>
using namespace std;

class MKalloc
{
private:

	size_t mem_page_size = 1024;
	uint8_t* page_ptr;

	size_t unit_size;
	size_t unit_count;
	size_t free_units_count;
	vector<int> frees;

public:
	MKalloc();
	virtual ~MKalloc();

	void* mkmalloc(const size_t size);
	void mkfree(const void* ptr);
	void mk_print_info();
	void mkdefragment();

};