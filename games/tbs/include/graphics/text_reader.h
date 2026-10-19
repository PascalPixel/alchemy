#ifndef ALCHEMY_TEXT_READER_H
#define ALCHEMY_TEXT_READER_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct TextReader {
  u32 previousCode;
  const u32 *words;
  u32 bits;
};

STATIC_ASSERT(sizeof(struct TextReader) == 0x0c);
STATIC_ASSERT(offsetof(struct TextReader, words) == 4);
STATIC_ASSERT(offsetof(struct TextReader, bits) == 8);

#endif
