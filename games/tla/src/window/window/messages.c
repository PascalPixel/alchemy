/// @file
/// Message strings and arguments, symbol decoding and portrait windows.
// Part of window.c.

#include "../window_internal.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

/// Expands message into buf without an upper length bound.
void Message_DecodeRawUnbounded(s32 message, s32 buf) {
  Message_DecodeRaw(message, buf, 0x7fffffff);
}

/// Builds message key's text entries and copies them to destination, at most
/// capacity - 1 characters and a terminator; returns the characters copied.
s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity) {
  struct WindowWork *work;
  u32 count;

  work = UI_WORK;
  work->count = 0;
  Message_Decode(key, 1);
  for (count = 0;
       count < capacity - 1 && (destination[count] = work->entries[count]) != 0;
       count++) {
  }
  destination[count] = 0;
  return count;
}

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

/// Clears *p if nonzero and returns p.
s32 *Memory_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}

void Window_CloseAll(void) {
  struct WindowWork *state;
  struct UiChannelSlot *slot;
  struct Window *direct;
  u32 done;
  struct Window *work;
  struct Window *pollWork;
  s32 index;

  state = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  slot = state->channels;
  direct = state->windows;
  index = 0;
  do {
    work = slot->work;
    if (work != 0 && work->flags != 0)
      Window_Close(work, 0);
    index++;
    slot++;
  } while (index != 3);

  for (;;) {
    done = 1;
    slot = state->channels;
    index = 0;
    do {
      pollWork = slot->work;
      if (pollWork != 0) {
        if ((*(s32 *)&pollWork->frame) == 0) {
          if (pollWork->flags == 0)
            slot->work = NULL;
          else
            done = 0;
        } else {
          done = 0;
        }
      }
      index++;
      slot++;
    } while (index != 3);
    index = 0;
    if (done)
      break;
    wait(1);
  }
  while (index != UI_WINDOW_COUNT) {
    if (direct->flags != 0)
      Window_Close(direct, 0);
    direct++;
    index++;
  }
}

struct Rect_0803cba8 {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
  u8 unk10[2];
  u16 state;
};

extern char ResourceId_WindowBorderFlash;

void Func_0803cba8(struct Rect_0803cba8 **window, s32 offset) {
  u8 *source =
      (u8 *)Resource_GetTableEntry((s32)&ResourceId_WindowBorderFlash) + offset;
  struct Rect_0803cba8 *rect = *window;
  s32 x = rect->x;
  s32 y = rect->y;
  s32 width = rect->width;
  s32 height = rect->height;

  Dma_Set(source, (void *)0x06000100, DMA_COPY_WORDS(64), REG_DMA3);
  Func_0803a1c0(x, y, width, height);
}

struct Window_0803cbf8 {
  struct Rect_0803cba8 *rect;
  u8 unk04[16];
  u16 timer;
};

void Func_0803cbf8(struct Window_0803cbf8 *window) {
  if (window->rect->state == 4) {
    if (window->timer == 10)
      Func_0803cba8(&window->rect, 0x300);
    else if (window->timer == 9)
      Func_0803cba8(&window->rect, 0x200);
    else if (window->timer == 8)
      Func_0803cba8(&window->rect, 0x100);
    else if (window->timer == 6)
      Func_0803cba8(&window->rect, 0x200);
    else if (window->timer == 4)
      Func_0803cba8(&window->rect, 0x300);
    if (window->timer == 2) {
      struct Rect_0803cba8 *rect = window->rect;

      Window_DrawBorder(rect->x, rect->y, rect->width, rect->height);
    }
    window->timer--;
    if (window->timer == 0) {
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowBorderFlash),
              (void *)0x06000100, DMA_COPY_WORDS(64), REG_DMA3);
      window->rect->state = 0;
    }
  }
}

/// Clears the eight-entry table of values substituted into messages.
void MessageArgument_ClearAll(void) {
  s32 no;
  struct WindowWork *work;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  no = 0;

  do {
    work->values[no] = 0;
    work->names[no] = 0;
    no++;
  } while (no != 8);
}

/// Inserts value of type kind in a free entry; does nothing if the table is
/// full.
void MessageArgument_Push(u32 value, u32 kind) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 no = 0;
  u32 limit = 8;

  do {
    if (work->names[no] == 0) {
      work->values[no] = value;
      work->names[no] = kind;
      break;
    }
    no++;
  } while (no != limit);
}

u32 MessageArgument_Lookup(u32 kind, u32 clear) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 index;
  u32 value = 0;

  for (index = 0; index < 8; index++) {
    if (work->names[index] == kind) {
      value = work->values[index];
      if (clear != 0) {
        work->values[index] = 0;
        work->names[index] = 0;
      }
      break;
    }
  }
  return value;
}

s32 Sound_GetBgmRestoreDelayFar(void);

/// Ends a message wait (clearing the channel's countdown and returning 1) when
/// the window's voice has finished or A, B or a shoulder button is held.
s32 Text_CheckPauseSkipped(struct UiChannelSlot *ch) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 cancel = 0;
  // The held keys read through a volatile pointer, so each test loads them
  // again; as a plain pointer, 13 of 40 instructions differ in tla-en.
  volatile u32 *held;

  if (work->busy != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancel = 1;
  held = &gInput.held;
  if (*held & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (*held & KEY_B)
    cancel = 1;
  if (cancel) {
    ch->countdown = 0;
    return 1;
  }
  return 0;
}

/// As Text_CheckPauseSkipped, but on newly pressed A, B or shoulder buttons
/// (the pending presses in a window mode); while B is held the wait ends only
/// at countdown 1, and otherwise the countdown is set to 2.
s32 Text_CheckTextDismissed(struct UiChannelSlot *ch) {
  struct WindowWork *work =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 cancel = 0;
  u32 keys;

  if (work->busy != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancel = 1;
  keys = gInput.pressed;
  if (work->mode != 0)
    keys = gInput.pendingPressed;
  if (keys & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (gInput.held & KEY_B) {
    if (ch->countdown == 1)
      cancel = 1;
  } else {
    ch->countdown = 2;
  }
  if (cancel) {
    ch->countdown = 0;
    return 1;
  }
  return 0;
}

struct UiWindowState {
  u8 unk00[0x14];
  u16 state;
  u16 flags;
  s32 pendingValue;
};

void Message_CloseFinishedWindows(void) {
  s32 slotIndex;
  struct UiChannelSlot *slot;
  struct UiWindowState *work;

  slot = ((struct WindowWork *)Ram_HeapSlots->windowTiles)->channels;
  slotIndex = 0;
  do {
    work = (struct UiWindowState *)slot->work;
    if (work != NULL && work->pendingValue == 0 && work->flags != 0 &&
        work->state != 0) {
      Window_Close((struct Window *)work, work->flags & 2);
    }
    slotIndex++;
    slot++;
  } while (slotIndex != 3);
  wait(10);
}

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

s32 Message_Decode(s32 key, s32 mode);
s32 Text_GetMessageSize(s32 message, s32 *x, s32 *y, u32 *width,
                                 u32 *height);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs);
s32 Message_AllTyped(void);
s32 Window_IsClosed(struct Window *window);
void wait(s32 frames);

void Func_0803ce64(s32 message, s32 mode, s32 yOffset) {
  struct WindowWork *work;
  struct Window *window;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entry;

  work = UI_WORK;
  x = 8;
  y = 8;
  window = NULL;
  entry = Message_Decode(message, 1);
  if (work->entries[entry] != 0) {
    Text_GetMessageSize(message, &x, &y, (u32 *)&width,
                                 (u32 *)&height);
    x = (30 - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != 0)
      window = Window_Create(x, y, width, height, 0);
    else {
      window = Window_Create(x, y, 0, 0, 2);
      window->width = mode;
      window->height = mode;
    }
    if (Message_Start(window, entry, 0, 0, 0, 0) == 0)
      Window_Close(window, 1);
    else {
      while (Message_AllTyped() == 0)
        wait(1);
      if (mode != 0) {
        Window_Close(window, 0);
        while (Window_IsClosed(window) == 0)
          wait(1);
      } else
        Window_Close(window, 1);
      work->speakerVoiceSound = 0;
      work->speakerVoiceCountdown = 0;
    }
  }
}

s32 Message_Decode(s32 message, s32 mode);

s32 Message_DecodeMode1(s32 message) {
  return Message_Decode(message, 1);
}

s32 Message_Decode(s32 message, s32 mode);

void Func_0803cf6c(s32 message) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 count = work->count;

  count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  work->count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  Message_Decode(message, 1);
}

// The rows of a glyph drawn: all twelve of the Japanese font's, and fourteen of
// the European fonts' fifteen.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PLOT_GLYPH_ROWS 12
#else
#define PLOT_GLYPH_ROWS 14
#endif

void Text_PlotGlyphRows(u16 *rows, u8 *dst, s32 color) {
  s32 y;
  u8 *p;
  s32 bits;

  for (y = 0; y != PLOT_GLYPH_ROWS; y++) {
    bits = *rows++ & ~3;
    p = dst;
    bits <<= 16;
    while (bits != 0) {
      if (bits < 0)
        *p = color;
      bits <<= 1;
      p++;
    }
    dst += 192;
  }
}

void Text_PlotGlyphRows(u16 *rows, u8 *dst, s32 color);

#if (LANGUAGE == LANGUAGE_JAPANESE)

/// The first kanji's code; the codes below it are the font's glyphs.
#define KANJI_FIRST_CODE 256
/// Every kanji is drawn 10 pixels wide.
#define KANJI_WIDTH 10

/// Draws a character on the canvas at (x, row) with its shadow and returns its
/// width: a font glyph, or a kanji from KANJI_FIRST_CODE on, with the mark its
/// code carries above TEXT_MARK_SHIFT drawn over it one pixel wider.
s32 Func_0803cfd0(u8 *canvas, u32 ch, s32 x, s32 row) {
  const u16 *mark = NULL;
  const u16 *rows;
  s32 width;
  s32 markCode = gFont_MarkGlyphCodes[ch >> TEXT_MARK_SHIFT];

  if (markCode != 0) {
    markCode -= 32;
    mark = gFont_Glyphs[markCode].rows;
  }
  ch &= (1 << TEXT_MARK_SHIFT) - 1;
  if (ch < KANJI_FIRST_CODE) {
    s32 index = ch - 32;

    // The width, then the rows after it.
    rows = &gFont_Glyphs[index].width;
    width = *rows++;
  } else {
    s32 index = ch - KANJI_FIRST_CODE;

    width = KANJI_WIDTH;
    rows = (const u16 *)(gFont_KanjiGlyphs + index * (FONT_GLYPH_ROWS * 2));
  }
  canvas += x + row * 192;
  Text_PlotGlyphRows((u16 *)rows, canvas + 3 * 192 + 1, 1);
  if (mark != NULL)
    Text_PlotGlyphRows((u16 *)mark, canvas + 192 + 8, 1);
  Text_PlotGlyphRows((u16 *)rows, canvas + 2 * 192, 15);
  if (mark != NULL) {
    Text_PlotGlyphRows((u16 *)mark, canvas + 7, 15);
    width++;
  }
  return width;
}
#else
s32 Func_0803cfd0(u8 *canvas, s32 ch, s32 x, s32 row) {
  u16 *glyph = (u16 *)((u8 *)gFont_Glyphs + (((ch & 0xff) - 32) << 5));
  s32 width = *glyph;
  u8 *dst = canvas + (x + row * 192);
  u8 *rows;

  glyph++;
  rows = dst + 3 * 192 + 1;
  Text_PlotGlyphRows(glyph, rows, 1);
  Text_PlotGlyphRows(glyph, rows = dst + 2 * 192, 15);
  return width;
}
#endif

s32 Message_Decode(s32 message, s32 mode);
void Text_ResetStyle(void);

s32 Func_0803d020(s32 message, u8 *canvas) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 x = 0;
  s32 y = 0;
  s32 entry;
  s32 width;
  s32 height;
  u16 *text;
  u32 code;

  work->count = x;
  entry = Message_Decode(message, 0);
  Text_MeasureDecoded(entry, (u32 *)&width, (u32 *)&height, 0);
  text = &work->entries[entry];
  if (text[0] == '-') {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (text[1] == '-')
      return text[2] == '-' ? 2 : 1;
    return 0;
#else
    if (text[1] != '-')
      return 0;
    if (text[2] == '-')
      return text[3] == '-' ? 3 : 2;
    return 1;
#endif
  }
  x = (192 - width) / 2;
  code = *text++;
  while (code != 0) {
    if (code <= 30) {
      switch (code) {
      case 8:
        work->colour = *text;
        text++;
        break;
      case 9:
        work->outline = *text;
        text++;
        break;
      case 10:
        work->lineSpacing = *text;
        text++;
        break;
      case 11:
      case 12:
      case 17:
      skip:
        text++;
        break;
      case 7:
        Text_ResetStyle();
        break;
      case 3:
        x = 0;
        y += 15;
        break;
      case 14:
      case 15:
      case 28:
        text++;
        goto skip;
      }
    } else {
      // Japanese text folds a mark after a kana into the kana's code, which
      // Func_0803cfd0 draws.
#if (LANGUAGE == LANGUAGE_JAPANESE)
      if (*text == TEXT_MARK_VOICED) {
        code |= 1 << TEXT_MARK_SHIFT;
        text++;
      } else if (*text == TEXT_MARK_SEMIVOICED) {
        code |= 2 << TEXT_MARK_SHIFT;
        text++;
      }
#endif
      x += Func_0803cfd0(canvas, code, x, y);
    }
    code = *text++;
  }
  return 0;
}

void Message_Find(struct TextReader *reader, s32 message) {
  void (*routine)(struct TextReader *, s32);
  u32 size;

  // The size's load in a once-only block keeps it after the parameters are
  // copied; as a plain assignment, 4 of 31 instructions differ in tla-en.
  do {
    size = (u32)Text_LookupSymbolCodeSize;
  } while (0);
  routine = (void (*)(struct TextReader *, s32))Heap_Alloc(size);
  Dma_Set((const void *)Text_LookupSymbolCode, routine,
          DMA_COPY_WORDS(size >> 2), REG_DMA3);
  routine(reader, message);
  Heap_Release(routine);
}

extern const void *const Text_MessageContexts[][2];

/// Decodes the next symbol of a message. Messages are Huffman coded with one
/// tree per previous symbol: the previous code picks the tree, the message's
/// bits walk it to a leaf, and the leaf's 12-bit symbol is returned and
/// becomes the next context.
/// Tree traversal consumes bytes; reader->words retains that byte position
/// between calls. Nothing calls it.
UNUSED s32 Text_DecodeSymbolInC(struct TextReader *reader) {
  u32 code;
  u32 contextBank;
  const u8 *base;
  const u8 *tree;
  const u8 *leaves;
  const u8 *text;
  s32 textBits;
  s32 treeBits;
  s32 rank;
  s32 depth;
  s32 bit;
  u32 index;
  u32 leafNibble;

  code = reader->previousCode;
  text = (const u8 *)reader->words;
  textBits = reader->bits;
  treeBits = 1;
  // The previous code's high bits pick the bank, its low byte the tree. The
  // tree runs forwards from base; its leaves' symbols run backwards from
  // base - 1.
  contextBank = code >> 8;
  code &= 0xff;
  base = Text_MessageContexts[contextBank][0];
  base += ((const u16 *)Text_MessageContexts[contextBank][1])[code];
  tree = base;
  leaves = base - 1;
  rank = 0;
  // The tree is its shape in preorder, a bit a node: 0 a branch, 1 a leaf.
  // Bits are read from each byte's lowest up; a buffer that has shifted down
  // to its marker bit (the 0x80 put above each byte's last bit) is empty.
  // A text bit of 0 takes the left branch; 1 skips the left subtree,
  // counting its leaves in rank, and takes the right.
  for (;;) {
    bit = treeBits & 1;
    treeBits >>= 1;
    if (bit != 0 && treeBits == 0) {
      treeBits = *tree++;
      bit = treeBits & 1;
      treeBits = (treeBits >> 1) | 0x80;
    }
    if (bit != 0)
      break;
    bit = textBits & 1;
    textBits >>= 1;
    if (bit != 0 && textBits == 0) {
      textBits = *text++;
      bit = textBits & 1;
      textBits = (textBits >> 1) | 0x80;
    }
    if (bit == 0)
      continue;
    depth = 0;
    do {
      bit = treeBits & 1;
      treeBits >>= 1;
      if (bit != 0 && treeBits == 0) {
        treeBits = *tree++;
        bit = treeBits & 1;
        treeBits = (treeBits >> 1) | 0x80;
      }
      if (bit == 0)
        depth++;
      else {
        rank++;
        depth--;
      }
    } while (depth >= 0);
  }
  // The leaf's symbol is the rank-th 12-bit entry, three nibbles each,
  // packed backwards from base - 1: an even rank starts on a byte, an odd one
  // in the middle of one.
  index = rank * 3;
  if (((index * 4) & 7) == 0) {
    bit = leaves[-(index >> 1)];
    code = bit << 4;
    bit = leaves[-(index >> 1) - 1];
    code |= (u32)bit >> 4;
  } else {
    leafNibble = leaves[-(index >> 1)];
    leafNibble &= 0xf;
    code = leaves[-(index >> 1) - 1] | (leafNibble << 8);
  }
  reader->bits = textBits;
  reader->previousCode = code;
  reader->words = (const u32 *)text;
  return code;
}

extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);
s32 CreateSideObject(s32 id, s32 kind, s32 slot, struct Window *window, s32 x,
                     s32 y);

void Portrait_ClearSlots(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 index = 0;

  do {
    work->resourceIds[index] = 999;
    index++;
  } while (index != 2);
}

extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);
s32 CreateSideObject(s32 id, s32 kind, s32 slot, struct Window *window, s32 x,
                     s32 y);

s32 Portrait_GetForSpeaker(u32 value) {
  s32 result = -1;
  s32 i = 0;

  switch (value) {
  case 47:
    value = 4;
    break;
  case 49:
    value = 6;
    break;
  case 48:
    value = 5;
    break;
  case 50:
    value = 7;
    break;
  case 51:
    value = 0;
    break;
  case 52:
    value = 1;
    break;
  case 53:
    value = 2;
    break;
  case 54:
    value = 3;
    break;
  }
  if (value < 16) {
    for (;;) {
      s32 key = gPortrait_CharacterMap[i];

      if (key == -1)
        break;
      if (key == value) {
        i++;
        result = gPortrait_CharacterMap[i];
        break;
      }
      i += 2;
    }
  } else {
    for (;;) {
      s32 key = gPortrait_SpriteMap[i];

      if (key == -1)
        break;
      if (key == value) {
        i++;
        result = gPortrait_SpriteMap[i];
        result += 128;
        break;
      }
      i += 2;
    }
  }
  return result;
}

struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x, s32 y) {
  struct Window *win;
  s32 minusFour;
  s32 offset;
  struct WindowWork *work;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  if (Portrait_GetForSpeaker(speaker) == -1) {
    return 0;
  }
  minusFour = -4;
  offset = minusFour;
  if (work->mode != 0) {
    win = Window_Create(x, y, 6, 5, 2);
    offset = 0;
  } else {
    win = Window_Create(x, y, 5, 5, 2);
  }
  if (win != 0) {
    CreateSideObject(speaker, portraitOption, -1, win, offset, minusFour);
  }
  return win;
}

void Portrait_CloseWindow(s32 id) {
  struct WindowWork *state;
  struct Window *work;
  s32 index;
  s32 i;
  struct WindowSprite *entity;

  state = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  work = state->windows;
  id = Portrait_GetForSpeaker(id);
  if (id == -1)
    return;

  if (state->resourceIds[1] == id) {
    index = 1;
  } else if (state->resourceIds[0] == id) {
    index = 0;
  } else {
    return;
  }

  id = state->outputIds[index];

  for (i = 0; i != UI_WINDOW_COUNT; i++, work++) {
    entity = work->output.head;
    if ((u8)entity->kind == 2 && (u8)entity->index == id) {
      Window_Close(work, 2);
      return;
    }
  }
}

#undef UI_WORK
#undef DECODE_EUROPEAN
#undef TEXT_DECODER_SLOT
#undef PLOT_GLYPH_ROWS
#undef KANJI_FIRST_CODE
#undef KANJI_WIDTH
