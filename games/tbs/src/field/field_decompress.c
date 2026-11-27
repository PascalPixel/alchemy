/* LZ decompression. Part of field.c. */

#include "field_internal.h"

/* An LZ stream's commands: a byte of eight flags, read from its low bit, each
   taking one literal byte (1) or one run (0). A run's token is two bytes, high
   first: the run's length in its top four bits, how far back it copies from in
   its low twelve. */
enum {
	LZ_FLAGS_MARKER = 1 << 8, /* set above a flag byte's eight flags */
	LZ_LENGTH_SHIFT = 12,
	LZ_DISTANCE_MASK = (1 << LZ_LENGTH_SHIFT) - 1,
	/* A run whose length nibble is 0 takes its length from the next byte, plus
	   the lengths the nibble holds. */
	LZ_LONG_LENGTH_BASE = 16
};

#include "field/decompress_lz.c"
