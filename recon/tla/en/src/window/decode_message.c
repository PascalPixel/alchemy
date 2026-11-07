#include "types.h"
#include "edition.h"
#include "heap_release.h"
#include "dma.h"
#include "ram_code.h"
#include "ram_buffer.h"
#include "text_reader.h"

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DECODE_EUROPEAN
#endif

/// The heap slot holding the message decoder while a message is decoded (slot
/// 50, addressed by its byte offset).
#define TEXT_DECODER_SLOT 200

extern const u8 Text_DecodeSymbolCode[];
extern u8 Text_DecodeSymbolCodeSize[];

void Message_Find(struct TextReader *reader, s32 message);

/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands.
/// capacity includes the terminating halfword.
// As ☀️'s function of the same name.
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity) {
  struct TextReader reader;
  void *original;
  s32 (*decode)(struct TextReader *);
  u32 value;
#if !defined(DECODE_EUROPEAN)
  u16 delta;
#endif

  original = ((void **)Ram_HeapSlots)[TEXT_DECODER_SLOT / 4];
  if (original == NULL)
    RamCode_LoadSlot(TEXT_DECODER_SLOT, Text_DecodeSymbolCode,
                     Text_DecodeSymbolCodeSize);
  decode = (s32 (*)(struct TextReader *))(
      (void **)Ram_HeapSlots)[TEXT_DECODER_SLOT / 4];
  Message_Find(&reader, message);
#if !defined(DECODE_EUROPEAN)
  delta = 0xffff;
#endif
  while ((value = decode(&reader)) != 0) {
    switch (value) {
    case 14:
      capacity -= 3;
      if (capacity <= 0)
        goto done;
      *text++ = value;
#if defined(DECODE_EUROPEAN)
      value = decode(&reader) + 0xffff;
      *text++ = value;
      value = decode(&reader) + 0xffff;
#else
      *text++ = decode(&reader) + delta;
      value = decode(&reader) + delta;
#endif
      break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 15:
#if defined(DECODE_EUROPEAN)
    case 29:
#endif
      capacity--;
      if (capacity <= 0)
        goto done;
      *text++ = value;
      value = decode(&reader) + 0xffff;
      break;
    default:
      capacity--;
      if (capacity <= 0)
        goto done;
      break;
    }
    *text++ = value;
  }
done:
  if (original == NULL)
    HeapSlot_Release(TEXT_DECODER_SLOT);
  *text = 0;
}
