#ifndef ALCHEMY_TEXT_READER_H
#define ALCHEMY_TEXT_READER_H

/// @file
/// Finding a message in the game's compressed text.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A place in a Huffman-coded message.
struct TextReader {
  u32 previousCode; ///< the symbol decoded last, which picks the next's tree
  /// The message's next byte of bits, though typed as words: the decoder reads
  /// a byte at a time.
  const u32 *bytes;
  /// The bits of the byte read last not yet used, lowest first, ended by a
  /// stop bit (BIT_STREAM_STOP).
  u32 bits;
};

STATIC_ASSERT(sizeof(struct TextReader) == 12);
STATIC_ASSERT(offsetof(struct TextReader, bytes) == 4);
STATIC_ASSERT(offsetof(struct TextReader, bits) == 8);

/// Points reader at the start of message.
void Message_Find(struct TextReader *reader, s32 message);

#endif
