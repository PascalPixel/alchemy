#ifndef ALCHEMY_TEXT_READER_H
#define ALCHEMY_TEXT_READER_H

/// @file
/// TextReader, a cursor through message text.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A position in a Huffman-coded message: the previous code, which picks the
/// next symbol's tree, and the message's remaining bits.
struct TextReader {
  /// The last symbol: its high bits pick the context bank and its low byte
  /// the tree the next symbol is read with.
  u32 previousCode;
  /// The message's next byte of bits, a byte pointer despite its type:
  /// Text_DecodeSymbol, the ARM routine UiText_BuildRenderEntries runs, reads
  /// it a byte at a time, as does the unused C form, UiText_DecodeSymbol.
  const u32 *words;
  /// The current byte's unread bits, lowest first, under a marker bit; empty
  /// once only the marker is left.
  u32 bits;
};

STATIC_ASSERT(sizeof(struct TextReader) == 0x0c);
STATIC_ASSERT(offsetof(struct TextReader, words) == 4);
STATIC_ASSERT(offsetof(struct TextReader, bits) == 8);

#endif
