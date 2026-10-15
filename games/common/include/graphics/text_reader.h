#ifndef ALCHEMY_TEXT_READER_H
#define ALCHEMY_TEXT_READER_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

struct TextReader {
  u32 previousCode;
  const u32 *words;
  u32 bits;
};

LAYOUT_SIZE_GUARD(TextReader_Size, struct TextReader, 0x0c);
LAYOUT_OFFSET_GUARD(TextReader_Words, struct TextReader, words, 4);
LAYOUT_OFFSET_GUARD(TextReader_Bits, struct TextReader, bits, 8);

#if GAME_TLA
void UiText_LookupMessage(struct TextReader *reader, s32 message);
#endif

#endif
