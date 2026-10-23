#ifndef ALCHEMY_MESSAGE_READER_H
#define ALCHEMY_MESSAGE_READER_H

/// @file
/// MessageReader, a cursor through message text.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A position in a Huffman-coded message: the previous code, which picks the
/// next symbol's tree, and the message's remaining bits.
struct MessageReader {
  /// The last symbol: its high bits pick the context bank and its low byte
  /// the tree the next symbol is read with.
  u32 previousCode;
  /// The message's next byte of bits, a byte pointer despite its type:
  /// Text_DecodeSymbol, the ARM routine Message_Decode runs, reads
  /// it a byte at a time, as does the unused C form, Text_DecodeSymbolInC.
  const u32 *words;
  /// The current byte's unread bits, lowest first, under a marker bit; empty
  /// once only the marker is left.
  u32 bits;
};

STATIC_ASSERT(sizeof(struct MessageReader) == 0x0c);
STATIC_ASSERT(offsetof(struct MessageReader, words) == 4);
STATIC_ASSERT(offsetof(struct MessageReader, bits) == 8);

#endif
