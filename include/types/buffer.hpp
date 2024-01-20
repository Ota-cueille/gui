#pragma once

#include <stack>
#include <vector>
#include <cstring>
#include <cassert>

template<typename Element, unsigned long long ElementCount>
struct instance_allocator {
	Element* base;
	Element* hash_table[ElementCount];

	Element** create();
	void release(Element**);
};


void example() {

}