/*
 * compress.h
 *
 * Native compression interfaces. Encoders borrow input buffers and return
 * an owned result; selection is by format and never by expected output.
 */
#ifndef AGS_COMPRESS_H
#define AGS_COMPRESS_H
#include "common.h"
typedef struct
{
	size_t window, ahead, distance, paletteAhead;
} LzMachine;
extern const LzMachine packer;
/* Shared move-to-front alphabet used by the LZ and icon codecs. */
unsigned moveToFront(unsigned char table[16], unsigned value);
/* shipped and packed differ only at documented post-packing edits. */
Buffer compressLz(const Buffer *shipped, const Buffer *packed, const char *kind,
				  LzMachine machine);
Buffer compressDelta7(const Buffer *data);
Buffer compressHalfwords(const Buffer *data);
Buffer compressIcon(const Buffer *pixels);
Buffer compressZeros(const Buffer *pixels);
Buffer deltaMap(const Buffer *data, unsigned mode);
Buffer compressArena(const Buffer *data, const Buffer *arena);
#endif
