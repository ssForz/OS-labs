#pragma once
#include<iostream>
#include <cinttypes>

using namespace std;

class FBalloc
{
private:
	struct Block
	{
		Block* next_block_ptr;
		size_t availible_block_size;
	};

	const size_t mem_page_size = 1024;
	uint8_t* mem_page_ptr;
	Block* first_block_ptr;

public:
	FBalloc();
	virtual ~FBalloc();
	void* fbmalloc(const size_t size);
	void fbfree(const void* ptr, const size_t size);
	void fb_print_info();
	void fbdefragment();

};