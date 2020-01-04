#ifndef _FIELD_LIST_H_
#define _FIELD_LIST_H_
#include <vector>

template <class T>
std::vector<T> CreateFieldList(T seed)
{
	std::vector<T> retvec;

	uint64_t base = static_cast<uint64_t>(seed);
	uint32_t len = sizeof(T) * 8;
	uint64_t mask_base = 1UL, mask=0UL;
	T temp;

	retvec.push_back(seed);

	for(int th = int(len - 1); th >= 0; --th){
		mask = ( mask_base << th);
		temp = static_cast<T>(base ^ mask);
		retvec.push_back(temp);
	}
	
	return retvec;	
}

#endif
