#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

#include "version.h"
#include "config.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

#define NULL ((void *)0)
#define FALSE 0
#define TRUE 1
#define UNUSED __attribute__((unused))

#ifdef MODERN
// the modern build places code and data where its own compiler puts them:
// a fixed address is the linked name, called through a register as at a
// fixed address, so that code copied into RAM still reaches it
// the linked name is pasted before it is macro-expanded: a name that is
// itself a macro would otherwise expand first and cannot be pasted
#define FIXED_LINKED_(symbol, name)             \
    ({                                          \
        extern char symbol[] __asm__(name);     \
        void *volatile modern_address = symbol; \
        modern_address;                         \
    })
#define FIXED_ADDRESS(type, address, linked) (type FIXED_LINKED_(modern_##linked, #linked))
#define FIXED_OFFSET(value, linked, from) \
    ((u32)FIXED_LINKED_(modern_##linked, #linked) - (u32)FIXED_LINKED_(modern_##from, #from))
#else
#define FIXED_ADDRESS(type, address, linked) (type address)
#define FIXED_OFFSET(value, linked, from) value
#endif
#define CAMELOT_ASM(proof)

#endif
