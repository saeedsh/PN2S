#pragma once

//#define ARCH_ 	ARCH_SM35
#define ARCH_ 	ARCH_SM30
//#define TYPE_	float
#define TYPE_	double

#if TYPE_ == double
	#define TYPE2_	double2
#else
	#define TYPE2_	float2
#endif

namespace pn2s
{
class Parameters {
public:
	static int MAX_DEVICE_NUMBER;
	static int MAX_STREAM_NUMBER;
	static long int MP_SIZE;

private:

};
}

