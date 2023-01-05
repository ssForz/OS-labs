#include "MKKalloc.h"
#include <iostream>
#include <cinttypes>
#include<vector>

using namespace std;

MKalloc::MKalloc()
{
	//initialize method
	page_ptr = (uint8_t*)malloc(mem_page_size);
	if (page_ptr == nullptr) {
		throw runtime_error("Error: Cannot allocate a memory page");
	}
	unit_size = mem_page_size;
	unit_count = 1;
	free_units_count = 1;
	frees.push_back(1);

}
MKalloc::~MKalloc()
{
	if (page_ptr != nullptr) {
		::free(page_ptr);
	}
}

void* MKalloc::mkmalloc(const size_t size)
{
	if (size == 0) {
		return nullptr;
	}
	if (size > mem_page_size) {
		throw runtime_error("Error: Cannot allocate a memory of size more than requested at start");
	}
	if (free_units_count == 0) {
		throw runtime_error("Error: Out of memory");
	}
	//convert request size to 2^n
	size_t ssize = size;
	for (ssize; ssize <= mem_page_size; ssize++) {
		if (ssize == 2 || ssize == 4 || ssize == 8 || ssize == 16 || ssize == 32 || ssize == 64 || ssize == 128 || ssize == 256 || ssize == 512 || ssize == 1024) {
			break;
		}
	}
	if (unit_count != 1) {
		if (unit_size != ssize) {
			throw runtime_error("Error: Cannot allocate a block of size different to one defined previously");
		}
	}
	if (unit_count == 1) {
		unit_size = ssize;
		unit_count = mem_page_size/ssize;
		free_units_count = unit_count;
		frees.resize(unit_count);
		for (int i = 0; i < unit_count; i++) {
			frees[i] = 1;
		}
	}
	uint8_t* unit_ptr;
	for (int i = 0; i < unit_count; i++) {
		if (frees[i] == 1) {
			unit_ptr = (uint8_t*)(page_ptr + mem_page_size - (unit_count-i)*unit_size);
			free_units_count--;
			frees[i] = 0;
			mem_page_size -= unit_size;
			return unit_ptr;
		}
	}
	throw runtime_error("Error: Cannot allocate");
	
}


void MKalloc::mkfree(const void* ptr)
{
	if (ptr == nullptr) {
		return;
	}
	for (int i = 0; i <= unit_count; i++) {
		if (ptr == page_ptr + i*unit_size) {
			mem_page_size += unit_size;
			free_units_count++;
			frees[i] = 1;
			return;
		}
	}



}

void MKalloc::mkdefragment()
{
	if (unit_count = free_units_count) {
		unit_count = 1;
		mem_page_size = 1024;
		unit_size = mem_page_size;
		free_units_count = unit_count;
		frees.resize(1);
		frees[0] = 1;
	}
}

void MKalloc::mk_print_info()
{
	int block_counter = unit_count;
	uint8_t* show_page_ptr = page_ptr;
	cout<<"MKK allocator info:"<<endl;
	cout<<"Availible size of memory page: "<<mem_page_size<<endl;
	cout<<"Pointer of memory page: "<<(void*)page_ptr<<endl;
	cout<<"Availible_blocks:"<<endl;
	cout<<"|№ | block pointer | block size |"<<endl;
	for (int i = 0; i < free_units_count; i++) {
		cout<<"|"<<i<<" | "<< (void*)(page_ptr + i*unit_size)<<"|    "<<unit_size<<"     |"<<endl;
	}
	cout<<endl;
	
}