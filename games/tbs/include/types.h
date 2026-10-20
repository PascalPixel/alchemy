#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

/// @file
/// The integer types, TRUE, UNUSED and the fixed-address macros.

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
/// Reaches a fixed address through its linked name, in the modern build.
// The modern build places code and data where its own compiler puts them: a
// fixed address is the linked name, called through a register as at a fixed
// address, so that code copied into RAM still reaches it. The linked name is
// pasted before it is macro-expanded: a name that is itself a macro would
// otherwise expand first and cannot be pasted.
#define FIXED_LINKED_(symbol, name)                                            \
  ({                                                                           \
    extern char symbol[] __asm__(name);                                        \
    void *volatile modern_address = symbol;                                    \
    modern_address;                                                            \
  })
#define FIXED_ADDRESS(type, address, linked)                                   \
  (type FIXED_LINKED_(modern_##linked, #linked))
#define FIXED_OFFSET(value, linked, from)                                      \
  ((u32)FIXED_LINKED_(modern_##linked, #linked) -                              \
   (u32)FIXED_LINKED_(modern_##from, #from))
#else
/// One of Camelot's fixed addresses, checked against the link, as a constant
/// of `type`; `linked` names the symbol the modern build reaches it by.
#define FIXED_ADDRESS(type, address, linked) (type address)
/// The distance between two fixed places, `value`, which the modern build
/// computes from their linked names.
#define FIXED_OFFSET(value, linked, from) value
#endif
/// Marks one of the few proven handwritten helpers inside C; proof says what
/// no C compiles to. It expands to nothing.
#define CAMELOT_ASM(proof)

#endif
