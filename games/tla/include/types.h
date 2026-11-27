#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

/* SGL's integer types, Bool and logic constants, UNUSED, unread results and the
   fixed-address macros. */

#include "stddef.h"
#include "system/asm.h"
#include "fixes.h"

/* SGL's integer types, as its sl_def.h names them. SGL makes the 32-bit pair
   long; here they are int, the type the original was compiled with: long is
   another type of the same size, and a pointer to one is not a pointer to the
   other. The 64-bit pair has no SGL name and follows the same pattern. */
typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned short Uint16;
typedef signed short Sint16;
typedef unsigned int Uint32;
typedef signed int Sint32;
typedef unsigned long long Uint64;
typedef signed long long Sint64;
typedef int Bool;

/* The development kit's code keeps its own names for the same types. */
typedef Uint8 u8;
typedef Sint8 s8;
typedef Uint16 u16;
typedef Sint16 s16;
typedef Uint32 u32;
typedef Sint32 s32;

/* SGL's truth values, switches and results, as sl_def.h defines them. */
enum BooleanLogic { FALSE = 0, TRUE = 1 };

enum BooleanSwitch { OFF = 0, ON = 1 };

enum Judgement { OK = 0, NG = -1 };

#define UNUSED __attribute__((unused))

#ifdef MODERN
/* The address of one of Camelot's fixed places in the modern build, which puts
   code and data where its own compiler does: the linked name, read through a
   register as a fixed address would be, so that code copied into RAM still
   reaches it. */
#define FIXED_LINKED(linked) \
	({ \
		ASM_LINKED_NAME(modern_##linked, #linked); \
		void *modern_address = modern_##linked; \
		ASM_OPAQUE(modern_address); \
		modern_address; \
	})
/* One of Camelot's fixed addresses, as a constant of `type`; `linked` names the
   symbol the modern build reaches it by, where `make modern` resolves it by
   name. */
#define FIXED_ADDRESS(type, address, linked) (type FIXED_LINKED(linked))
/* The distance between two fixed places, `value`, which the modern build
   computes from their linked names. */
#define FIXED_OFFSET(value, linked, from) \
	((Uint32)FIXED_LINKED(linked) - (Uint32)FIXED_LINKED(from))
#else
#define FIXED_ADDRESS(type, address, linked) (type address)
#define FIXED_OFFSET(value, linked, from)	 value
#endif
/* Marks one of the few proven handwritten helpers inside C; proof says what no
   C compiles to. It expands to nothing. */

/* The old compilers' soft frame pointer as an asm clobber, which a current GCC
   refuses; there the barrier clobbers memory instead. */
#ifdef MODERN
#define SFP_CLOBBER "memory"
#else
#define SFP_CLOBBER "sfp"
#endif

/* The result of a function whose callers never read it: an event, an update
   function, or a call whose result is dropped. The ROM's function sets r0
   before it returns, as one with an int result does, while its callers call it
   as void. That call is undefined behaviour, so under UBFIX the function
   returns nothing; RETURN_UNREAD(value) returns its result. As The Broken
   Seal's types.h. */
#if FIX_FUNCTION_SIGNATURES
#define UNREAD_RESULT void
#define RETURN_UNREAD(value) \
	do { \
		(void)(value); \
		return; \
	} while(0)
#else
#define UNREAD_RESULT		 Sint32
#define RETURN_UNREAD(value) return value
#endif

#endif
