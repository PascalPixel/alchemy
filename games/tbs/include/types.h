#ifndef ALCHEMY_TYPES_H
#define ALCHEMY_TYPES_H

/* SGL's integer types, Bool and logic constants, UNUSED, unread results and the
   fixed-address macros. */

#include "config.h"
#include "stddef.h"
#include "system/asm.h"
#include "edition.h"

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

/* The result of a function whose callers never read it: an event, an update
   function, or a call whose result is dropped. The original returns a value, as
   a function with an int result does, while the event runner and the field
   engine call it as void. That call is undefined behaviour, so under UBFIX the
   function returns nothing; RETURN_UNREAD(value) returns its result. */
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

#ifdef MODERN
/* Reaches a fixed address through its linked name, in the modern build. The
   modern build places code and data where its own compiler puts them: a fixed
   address is the linked name, called through a register as at a fixed address,
   so that code copied into RAM still reaches it. The linked name is pasted
   before it is macro-expanded: a name that is itself a macro would otherwise
   expand first and cannot be pasted. */
#define FIXED_LINKED_(symbol, name) \
	({ \
		ASM_LINKED_NAME(symbol, name); \
		void *volatile modern_address = symbol; \
		modern_address; \
	})
#define FIXED_ADDRESS(type, address, linked) \
	(type FIXED_LINKED_(modern_##linked, #linked))
#define FIXED_OFFSET(value, linked, from) \
	((Uint32)FIXED_LINKED_(modern_##linked, #linked) - \
		(Uint32)FIXED_LINKED_(modern_##from, #from))
#else
/* One of Camelot's fixed addresses, as a constant of `type`; `linked` names the
   symbol the modern build reaches it by, where `make modern` resolves it by
   name. */
#define FIXED_ADDRESS(type, address, linked) (type address)
/* The distance between two fixed places, `value`, which the modern build
   computes from their linked names. */
#define FIXED_OFFSET(value, linked, from)	 value
#endif
/* Marks one of the few proven handwritten helpers inside C; proof says what no
   C compiles to. It expands to nothing. */

#endif
