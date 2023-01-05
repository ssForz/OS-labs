#include "FBalloc.h"
#include <iostream>
#include <cinttypes>

using namespace std;

FBalloc::FBalloc()
{
	//initialize method
	mem_page_ptr = (uint8_t*)malloc(mem_page_size);

	if (mem_page_ptr == nullptr) {
		throw runtime_error("Error: Cannot allocate a memory page");
	}

	first_block_ptr = (Block*)mem_page_ptr;
	first_block_ptr->next_block_ptr = nullptr;
	first_block_ptr->availible_block_size = mem_page_size;

}
FBalloc::~FBalloc()
{
	if (mem_page_ptr != nullptr) {
		::free(mem_page_ptr);
	}
}

void* FBalloc::fbmalloc(const size_t size)
{
	if (size == 0) {
		return nullptr;
	}
	//user cant request allocation of size less than size of Block struct
	const size_t req_size = max(size, sizeof(Block));
	size_t fit_min = 1e8;

	Block* block_ptr = first_block_ptr;

	while(block_ptr != nullptr) {
		//find more perfect fit of availible memory
		if (block_ptr->availible_block_size >= req_size + sizeof(Block)) {
			if (block_ptr->availible_block_size - (req_size + sizeof(Block)) <= fit_min) { 
				fit_min = block_ptr->availible_block_size - (req_size + sizeof(Block));

			}
		}

		block_ptr = block_ptr->next_block_ptr;

	}


	//error if there are no fits
	if (fit_min == 1e8) {
		throw runtime_error("Error: Can't allocate requested memory");
	}
	block_ptr = first_block_ptr;
	while(block_ptr != nullptr) {

		//take this space in block
		if (block_ptr->availible_block_size - (req_size + sizeof(Block)) == fit_min) {
			uint8_t* take_block_space_ptr = (uint8_t*)block_ptr + block_ptr->availible_block_size - req_size; 
			block_ptr->availible_block_size = block_ptr->availible_block_size - req_size;
			return take_block_space_ptr;
		}
		block_ptr = block_ptr->next_block_ptr;
	}
	throw runtime_error("Error: Can't allocate requested memory");
}


void FBalloc::fbfree(const void* ptr, const size_t size)
{
	if (ptr == nullptr) {
		return;
	}

	const size_t freed_space = max(size, sizeof(Block));

	Block* freed_block_ptr = (Block*)ptr;
	freed_block_ptr->next_block_ptr = nullptr;
	freed_block_ptr->availible_block_size = freed_space;

	Block* block_ptr = first_block_ptr;
	Block** previous_ptr_ptr = &block_ptr;

	while(block_ptr != nullptr) {

		if (ptr < block_ptr) {

			freed_block_ptr->next_block_ptr = block_ptr;

			break;
		}
		previous_ptr_ptr = &block_ptr->next_block_ptr;
		block_ptr = block_ptr->next_block_ptr;

	}
	*previous_ptr_ptr = freed_block_ptr;


}

void FBalloc::fbdefragment()
{
	Block* block_ptr = first_block_ptr;
	while(block_ptr != nullptr) {

		while (block_ptr != nullptr) {
			uint8_t* end_ptr_of_block = (uint8_t*)block_ptr + block_ptr->availible_block_size;

			if (end_ptr_of_block != (uint8_t*)block_ptr->next_block_ptr) {
				break;
			}

			block_ptr->availible_block_size += block_ptr->next_block_ptr->availible_block_size;
			block_ptr->next_block_ptr = block_ptr->next_block_ptr->next_block_ptr;

		}


		block_ptr = block_ptr->next_block_ptr;
	}
}

void FBalloc::fb_print_info()
{
	int block_counter = 0;
	Block* block_ptr = first_block_ptr;
	cout<<"Free blocks allocator info:"<<endl;
	cout<<"Size of memory page: "<<mem_page_size<<endl;
	cout<<"Pointer of memory page: "<<(void*)mem_page_ptr<<endl;
	cout<<"Availible_blocks:"<<endl;
	cout<<"|№ | block pointer | block size |"<<endl;
	while(block_ptr != nullptr) {
		cout<<"|"<<block_counter<<" | "<< (void*)block_ptr<<"|    "<<block_ptr->availible_block_size<<"     |"<<endl;
		block_counter++;
		block_ptr = block_ptr->next_block_ptr;
	}
	cout<<endl;
	
}