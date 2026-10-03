#ifndef ALCHEMY_TEXT_READER_H
#define ALCHEMY_TEXT_READER_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* LOOKUP_SYMBOL initializes the three words DECODE_SYMBOL advances. */
struct TextReader {
    u32 previous_code;
    const u32 *words;
    u32 bits;
};

LAYOUT_SIZE_GUARD(TextReader_Size, struct TextReader, 0x0c);
LAYOUT_OFFSET_GUARD(TextReader_Words, struct TextReader, words, 4);
LAYOUT_OFFSET_GUARD(TextReader_Bits, struct TextReader, bits, 8);

void UiText_LookupMessage(struct TextReader *reader, s32 message);

#endif
