#ifndef __PROXY_H__
#define __PROXY_H__

#include "stdint.h"

template<typename TElement, typename T>
class Proxy
{
public:
	T* vector;
	uint16_t index;

	Proxy(T* vector, uint16_t index)
	{
		this->vector = vector;
		this->index = index;
	}

	void operator= (const TElement val)
	{
		this->vector->SetValue(this->index, val);
	}

	operator TElement() const
	{
		return this->vector->array[this->index];
	}

	//&operator TElement*() const
//	{
		//return &(this->vector->array[this->index]);
	//}
};

#endif
