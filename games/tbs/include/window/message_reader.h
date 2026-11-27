#ifndef ALCHEMY_MESSAGE_READER_H
#define ALCHEMY_MESSAGE_READER_H

// MESSAGE_READER, a cursor through message text.

#include "edition.h"
#include "static_assert.h"
#include "types.h"

// A position in a Huffman-coded message: the previous code, which picks the next symbol's tree, and
// the message's remaining bits.
typedef struct MESSAGE_READER
{
    // The last symbol: its high bits pick the context bank and its low byte the tree the next
    // symbol is read with.
    Uint32        prevCode;
    // The message's next byte of bits, a byte pointer despite its type: TextDecodeSymbol, the ARM
    // function mnMessageDecode runs, reads it a byte at a time, as does the unused C form,
    // mnTextDecodeSymbolInC.
    const Uint32 *words;
    // The current byte's unread bits, lowest first, under a marker bit; empty once only the marker
    // is left.
    Uint32        bits;
} MESSAGE_READER;

STATIC_ASSERT(sizeof(MESSAGE_READER) == 12);
STATIC_ASSERT(offsetof(MESSAGE_READER, words) == 4);
STATIC_ASSERT(offsetof(MESSAGE_READER, bits) == 8);

#endif
