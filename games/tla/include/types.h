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

#ifdef MODERN
// the modern build places code and data where its own compiler puts them:
// a fixed address is the linked name, called through a register as at a
// fixed address, so that code copied into RAM still reaches it
#define FIXED_LINKED(linked)                            \
    ({                                                  \
        extern char modern_##linked[] __asm__(#linked); \
        void       *modern_address = modern_##linked;   \
        __asm__("" : "+r"(modern_address));             \
        modern_address;                                 \
    })
#define FIXED_ADDRESS(type, address, linked) (type FIXED_LINKED(linked))
#define FIXED_OFFSET(value, linked, from) ((u32)FIXED_LINKED(linked) - (u32)FIXED_LINKED(from))
#else
#define FIXED_ADDRESS(type, address, linked) (type address)
#define FIXED_OFFSET(value, linked, from) value
#endif
#define CAMELOT_ASM(proof)

#endif
