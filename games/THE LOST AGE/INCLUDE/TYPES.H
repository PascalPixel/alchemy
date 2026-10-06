#ifndef TLA_TYPES_H
#define TLA_TYPES_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

#define NULL ((void *)0)

#define FIXED_ADDRESS(type, address, linked) (type address)
#define FIXED_OFFSET(value, linked, from) value
#define CAMELOT_ASM(proof)

#endif
