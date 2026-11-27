/* @file */
/* LZ decompression. */
/* Part of field.c. */

#include "field_internal.h"

/* An LZ stream's commands: a byte of eight flags, read from its low bit, each
 */
/* taking one literal byte (1) or one run (0). A run's token is two bytes, high
 */
/* first: the run's length in its top four bits, how far back it copies from in
 */
/* its low twelve. */
enum {
	LZ_FLAGS_MARKER = 1 << 8, /* set above a flag byte's eight flags */
	LZ_LENGTH_SHIFT = 12,
	LZ_DISTANCE_MASK = (1 << LZ_LENGTH_SHIFT) - 1,
	/* A run whose length nibble is 0 takes its length from the next byte, plus
	 */
	/* the lengths the nibble holds. */
	LZ_LONG_LENGTH_BASE = 16
};

/* Decompresses an LZ stream from source into destination and returns */
/* destination. */
u8 *fdDecompressLz(const u8 *source, u8 *destination) {
	s32 token;
	u32 length;
	u32 control;
	u8 *start;
	const u8 *base;
	const u8 *command;
	const u8 *copy;
	const u8 *literal;

	/* Uncompressed blocks borrow their payload; compressed blocks use the
	 * output. */
	token = source[0] | (source[1] << 8);
	literal = source + 2;
	if(token == 0)
		return (u8 *)literal;
	command = source + token;
	control = *command;
	base = literal;
	start = destination;
	command++;
	/* A marker bit, shifted down as the flags are read. */
	control |= LZ_FLAGS_MARKER;
	/* Consume literals or copy runs until the zero token ends the stream. */
	for(;;) {
		if((control & 1) != 0) {
			/* Only the marker left means: read the next command byte. */
			if(control != 1)
				*destination++ = *literal++;
		}
		else {
			token = *command++ << 8;
			token |= *command++;
			if(token == 0)
				break;
			length = token >> LZ_LENGTH_SHIFT;
			if(length == 0) {
				length = *command + LZ_LONG_LENGTH_BASE;
				command++;
			}
			length += 2;
			/* A run copies from before the literals' start, not from the
			 * output: */
			/* a sprite's codec 3 pictures are stored one after another, and
			 * each is */
			/* packed against the pictures stored before it, as IwramDecompress
			 * also */
			/* reads them. 253 sprites, Isaac's among them, use codec 3. */
			copy = base - (token & LZ_DISTANCE_MASK);
			while(length != 0) {
				*destination++ = *copy++;
				length--;
			}
		}
		control >>= 1;
		if(control == 0) {
			control = *command;
			command++;
			control |= LZ_FLAGS_MARKER;
		}
	}
	return start;
}
