#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

#include "VERSION.H"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef int bool;

#define NULL ((void *)0)
#define FALSE 0
#define TRUE 1

#define FIXED_ADDRESS(type, address, linked) (type address)
#define FIXED_OFFSET(value, linked, from) value
#define CAMELOT_ASM(proof)

#endif
