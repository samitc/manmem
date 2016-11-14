//
// Created by amit on 14/11/2016.
//

#ifndef _MANMEMGENERALTYPES_H
#define _MANMEMGENERALTYPES_H

#if defined(__GNUC__)
	#define GNU_COMPILER
	#if defined(__x86_64__)
		#define BIT_64
	#elif defined(__i386__)
		#define BIT_32
	#endif
#elif defined(_MSC_VER)
	#define VS_COMPILER
#endif
#if defined(__linux__)
	#define LINUX_ENV
#elif defined(_WIN32)
	#define WINDOWS_ENV
	#if defined(_WIN64)
		#define BIT_64
	#else
		#define BIT_32
	#endif
#endif

#define createType(newName,oldName,size)\
	using newName = oldName;\
	static_assert(sizeof(newName)==size,"Error in types.The new type is: "#newName ". and old type is: "#oldName);
createType(int32, signed int,4);
createType(int16, signed short,2);
createType(int64, signed long long,8);
createType(int8, signed char,1);
createType(uint32, unsigned int,4);
createType(uint16, unsigned short,2);
createType(uint64, unsigned long long,8);
createType(uint8, unsigned char,1);
using intp = uint32;
#endif