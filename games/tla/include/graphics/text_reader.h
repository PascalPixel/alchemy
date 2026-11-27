#ifndef ALCHEMY_TEXT_READER_H
#define ALCHEMY_TEXT_READER_H

// Finding a message in the game's compressed text.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

// A place in a Huffman-coded message.
typedef struct TEXT_READER
{
	Uint32       prevCode; // the symbol decoded last, which picks the next's tree
	const Uint8 *bytes;    // the message's next byte of bits
	// The bits of the byte read last not yet used, lowest first, ended by a stop bit
	// (BIT_STREAM_STOP).
	Uint32       bits;
} TEXT_READER;

STATIC_ASSERT(sizeof(TEXT_READER) == 12);
STATIC_ASSERT(offsetof(TEXT_READER, bytes) == 4);
STATIC_ASSERT(offsetof(TEXT_READER, bits) == 8);

// Points reader at the start of message.
void mnMessageFind(TEXT_READER *reader, Sint32 msg);

#endif
