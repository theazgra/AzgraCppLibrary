#pragma once
#include <math.h>
namespace azgra
{
	inline double floor(const double _value)
	{
#ifdef _WIN32
		return floor(_value);
#else
		return std::floor(_value);
#endif
	}
}