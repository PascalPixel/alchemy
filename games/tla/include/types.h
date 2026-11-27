#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

/* @file */
/* The integer types, TRUE, UNUSED, unread results and the fixed-address */
/* macros. */

#include "stddef.h"
#include "system/asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

#define UNUSED __attribute__((unused))
#define FALSE  0
#define TRUE   1

#ifdef MODERN
/* The address of one of Camelot's fixed places in the modern build, which */
/* puts code and data where its own compiler does: the linked name, read */
/* through a register as a fixed address would be, so that code copied into */
/* RAM still reaches it. */
#define FIXED_LINKED(linked)                                                   \
	({                                                                         \
		ASM_LINKED_NAME(modern_##linked, #linked);                             \
		void *modern_address = modern_##linked;                                \
		ASM_OPAQUE(modern_address);                                            \
		modern_address;                                                        \
	})
/* One of Camelot's fixed addresses, as a constant of `type`; `linked` names */
/* the symbol the modern build reaches it by, where `make modern` resolves it */
/* by name. */
#define FIXED_ADDRESS(type, address, linked) (type FIXED_LINKED(linked))
/* The distance between two fixed places, `value`, which the modern build */
/* computes from their linked names. */
#define FIXED_OFFSET(value, linked, from)                                      \
	((u32)FIXED_LINKED(linked) - (u32)FIXED_LINKED(from))
#else
#define FIXED_ADDRESS(type, address, linked) (type address)
#define FIXED_OFFSET(value, linked, from)	 value
#endif
/* Marks one of the few proven handwritten helpers inside C; proof says what */
/* no C compiles to. It expands to nothing. */

/* The old compilers' soft frame pointer as an asm clobber, which a current GCC
 */
/* refuses; there the barrier clobbers memory instead. */
#ifdef MODERN
#define SFP_CLOBBER "memory"
#else
#define SFP_CLOBBER "sfp"
#endif

/* The result of a function whose callers never read it: a task, an update */
/* function, or a call whose result is dropped. The ROM's function sets r0 */
/* before it returns, as one with an int result does, while its callers call */
/* it as void. That call is undefined behaviour, so under UBFIX the function */
/* returns nothing; RETURN_UNREAD(value) returns its result. As The Broken */
/* Seal's types.h. */
#ifdef UBFIX
#define UNREAD_RESULT void
#define RETURN_UNREAD(value)                                                   \
	do {                                                                       \
		(void)(value);                                                         \
		return;                                                                \
	} while(0)
#else
#define UNREAD_RESULT		 s32
#define RETURN_UNREAD(value) return value
#endif

#endif
