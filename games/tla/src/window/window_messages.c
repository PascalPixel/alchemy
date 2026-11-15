/// @file
/// Message strings and arguments, symbol decoding and portrait windows.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowWork)

/// Expands message into buffer without an upper length bound.
void Message_DecodeRawUnbounded(s32 message, u16 *buffer) {
  Message_DecodeRaw(message, buffer, 0x7fffffff);
}

/// Builds message key's text entries and copies them to destination, at most
/// capacity - 1 characters and a terminator; returns the characters copied.
s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity) {
  struct WindowWork *work;
  u32 count;

  work = UI_WORK;
  work->textBufferEnd = 0;
  Message_Decode(key, 1);
  for (count = 0; count < capacity - 1 &&
                  (destination[count] = work->textBuffer[count]) != 0;
       count++) {
  }
  destination[count] = 0;
  return count;
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DECODE_EUROPEAN
#endif

extern const u8 Text_DecodeSymbolCode[];
extern u8 Text_DecodeSymbolCodeSize[];

/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands.
/// capacity includes the terminating halfword.
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity) {
  struct TextReader reader;
  void *original;
  s32 (*decode)(struct TextReader *);
  u32 value;
#if !defined(DECODE_EUROPEAN)
  u16 delta;
#endif

  original = Ram_HeapSlots->messageDecoder;
  if (original == NULL)
    RamCode_LoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbolCode,
                     Text_DecodeSymbolCodeSize);
  decode = (s32 (*)(struct TextReader *))Ram_HeapSlots->messageDecoder;
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
    case MESSAGE_TEXT_COLOR:
    case MESSAGE_TEXT_EFFECT:
    case MESSAGE_LINE_SPACING:
    case 11:
    case 12:
    case MESSAGE_WINDOW_EFFECT:
#if defined(DECODE_EUROPEAN)
    case MESSAGE_ARTICLE_CLASS:
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
    HeapSlot_Release(HEAP_SLOT_MESSAGE_DECODER);
  *text = 0;
}

/// Clears *p if nonzero and returns p.
s32 *Memory_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}

/// Closes every window: first the messages' windows, waiting until they have
/// closed, then the windows opened directly, which it does not wait for.
void Window_CloseAll(void) {
  struct WindowWork *state;
  struct UiChannelSlot *slot;
  struct Window *direct;
  u32 done;
  struct Window *work;
  struct Window *pollWork;
  s32 index;

  state = (struct WindowWork *)Ram_HeapSlots->windowWork;
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
        if ((*(s32 *)&pollWork->zoomStage) == 0) {
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

/// The flash border's frames: eight tiles each, shown in tiles 8 to 15.
#define FLASH_FRAME_BYTES (8 * TILE_SIZE_4BPP)
#define FLASH_FRAME_TILES (BG_CHAR_BLOCK(0) + 8 * TILE_SIZE_4BPP)

/// Loads the flash border frame `offset` bytes into its picture
/// (ResourceId_WindowBorderFlash) into tiles 8 to 15 and draws the window's
/// rectangle with it.
void Window_ShowFlashFrame(struct Window **window, s32 offset) {
  u8 *source =
      (u8 *)Resource_GetTableEntry((s32)&ResourceId_WindowBorderFlash) + offset;
  struct Window *rect = *window;
  s32 x = (s16)rect->x;
  s32 y = (s16)rect->y;
  s32 width = rect->width;
  s32 height = rect->height;

  Dma_Set(source, (void *)FLASH_FRAME_TILES,
          DMA_COPY_WORDS(FLASH_FRAME_BYTES / 4), REG_DMA3);
  Window_DrawFlashBorder(x, y, width, height);
}

/// Steps a message's window animation (WINDOW_ANIMATION_SWELL): the frame
/// flashes through the flash border's pictures as the countdown runs, the plain
/// border returns two frames before its end, and the flash tiles are restored
/// at the end.
void Window_StepSwellEffect(struct UiChannelSlot *window) {
  if (window->work->animation == WINDOW_ANIMATION_SWELL) {
    if (window->countdown == 10)
      Window_ShowFlashFrame(&window->work, 3 * FLASH_FRAME_BYTES);
    else if (window->countdown == 9)
      Window_ShowFlashFrame(&window->work, 2 * FLASH_FRAME_BYTES);
    else if (window->countdown == 8)
      Window_ShowFlashFrame(&window->work, 1 * FLASH_FRAME_BYTES);
    else if (window->countdown == 6)
      Window_ShowFlashFrame(&window->work, 2 * FLASH_FRAME_BYTES);
    else if (window->countdown == 4)
      Window_ShowFlashFrame(&window->work, 3 * FLASH_FRAME_BYTES);
    if (window->countdown == 2) {
      struct Window *rect = window->work;

      Window_DrawBorder((s16)rect->x, (s16)rect->y, rect->width, rect->height);
    }
    window->countdown--;
    if (window->countdown == 0) {
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowBorderFlash),
              (void *)FLASH_FRAME_TILES, DMA_COPY_WORDS(FLASH_FRAME_BYTES / 4),
              REG_DMA3);
      window->work->animation = 0;
    }
  }
}

/// Clears the eight-entry table of values substituted into messages.
void MessageArgument_ClearAll(void) {
  s32 no;
  struct WindowWork *work;

  work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  no = 0;

  do {
    work->argumentValues[no] = 0;
    work->argumentKinds[no] = MESSAGE_ARGUMENT_NONE;
    no++;
  } while (no != MESSAGE_ARGUMENT_COUNT);
}

/// Inserts value of type kind in a free entry; does nothing if the table is
/// full.
void MessageArgument_Push(u32 value, u32 kind) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u32 no = 0;
  u32 limit = MESSAGE_ARGUMENT_COUNT;

  do {
    if (work->argumentKinds[no] == MESSAGE_ARGUMENT_NONE) {
      work->argumentValues[no] = value;
      work->argumentKinds[no] = kind;
      break;
    }
    no++;
  } while (no != limit);
}

/// Returns the value of the first message argument of kind (MESSAGE_ARGUMENT_),
/// or 0 when there is none.
u32 MessageArgument_Lookup(u32 kind, u32 clear) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u32 index;
  u32 value = 0;

  for (index = 0; index < MESSAGE_ARGUMENT_COUNT; index++) {
    if (work->argumentKinds[index] == kind) {
      value = work->argumentValues[index];
      if (clear != 0) {
        work->argumentValues[index] = 0;
        work->argumentKinds[index] = MESSAGE_ARGUMENT_NONE;
      }
      break;
    }
  }
  return value;
}

/// Ends a message wait (clearing the channel's countdown and returning 1) when
/// the window's voice has finished or A, B or a shoulder button is held.
s32 Text_CheckPauseSkipped(struct UiChannelSlot *ch) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 cancel = 0;

  if (work->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancel = 1;
  if (gInput.held & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (gInput.held & KEY_B)
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
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 cancel = 0;
  u32 keys;

  if (work->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancel = 1;
  keys = gInput.pressed;
  if (work->mindRead != 0)
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

/// A window as Message_CloseFinishedWindows reads it: its state and flags, and
/// its zoom stage and duration read as one word, nonzero while a zoom runs.
struct UiWindowState {
  u8 unk00[0x14];
  u16 state;
  u16 flags;
  s32 pendingValue;
};

/// Closes each message's window whose text has ended, then waits ten frames.
void Message_CloseFinishedWindows(void) {
  s32 slotIndex;
  struct UiChannelSlot *slot;
  struct UiWindowState *work;

  slot = ((struct WindowWork *)Ram_HeapSlots->windowWork)->channels;
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

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowWork)

s32 Message_Decode(s32 key, s32 mode);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
s32 Message_AllTyped(void);
s32 Window_IsClosed(struct Window *window);

/// Shows a message centred across the screen and in its top 15 rows (yOffset
/// tiles lower) and waits until it is typed out, then closes it, in the window
/// mode `mode` gives.
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset) {
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
  if (work->textBuffer[entry] != 0) {
    Text_GetMessageSize(message, &x, &y, (u32 *)&width, (u32 *)&height);
    x = (DISPLAY_TILE_WIDTH - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != CENTERED_MESSAGE_BARE)
      window = Window_Create(x, y, width, height, 0);
    else {
      window = Window_Create(x, y, 0, 0, WINDOW_FLAG_INSTANT);
      window->width = mode;
      window->height = mode;
    }
    if (Message_Start(window, entry, 0, 0, 0, 0) == 0)
      Window_Close(window, 1);
    else {
      while (Message_AllTyped() == 0)
        wait(1);
      if (mode != CENTERED_MESSAGE_BARE) {
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

s32 Message_DecodeMode1(s32 message) { return Message_Decode(message, 1); }

s32 Message_Decode(s32 message, s32 mode);

/// Decodes message after the text decoded last, over its terminator.
void Message_DecodeAppend(s32 message) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u32 count = work->textBufferEnd;

  count = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  work->textBufferEnd = (u16)(count - 1) & (UI_TEXT_ENTRY_COUNT - 1);
  Message_Decode(message, 1);
}

/// The story crawl's canvas, one byte a pixel: a line of 24 tiles of the
/// credits' tile ring.
#define CRAWL_CANVAS_WIDTH 192

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
    dst += CRAWL_CANVAS_WIDTH;
  }
}

void Text_PlotGlyphRows(u16 *rows, u8 *dst, s32 color);

#if (LANGUAGE == LANGUAGE_JAPANESE)

/// Draws a character on the canvas at (x, row) with its shadow and returns its
/// width: a font glyph, or a kanji from KANJI_FIRST_CODE on, with the mark its
/// code carries above TEXT_MARK_SHIFT drawn over it one pixel wider.
s32 Text_DrawGlyphToCanvas(u8 *canvas, u32 ch, s32 x, s32 row) {
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
  canvas += x + row * CRAWL_CANVAS_WIDTH;
  Text_PlotGlyphRows((u16 *)rows, canvas + 3 * CRAWL_CANVAS_WIDTH + 1, 1);
  if (mark != NULL)
    Text_PlotGlyphRows((u16 *)mark, canvas + CRAWL_CANVAS_WIDTH + 8, 1);
  Text_PlotGlyphRows((u16 *)rows, canvas + 2 * CRAWL_CANVAS_WIDTH,
                     TEXT_COLOR_WHITE);
  if (mark != NULL) {
    Text_PlotGlyphRows((u16 *)mark, canvas + 7, TEXT_COLOR_WHITE);
    width++;
  }
  return width;
}
#else
s32 Text_DrawGlyphToCanvas(u8 *canvas, s32 ch, s32 x, s32 row) {
  u16 *glyph = (u16 *)((u8 *)gFont_Glyphs + (((ch & 0xff) - 32) << 5));
  s32 width = *glyph;
  u8 *dst = canvas + (x + row * CRAWL_CANVAS_WIDTH);
  u8 *rows;

  glyph++;
  rows = dst + 3 * CRAWL_CANVAS_WIDTH + 1;
  Text_PlotGlyphRows(glyph, rows, 1);
  Text_PlotGlyphRows(glyph, rows = dst + 2 * CRAWL_CANVAS_WIDTH,
                     TEXT_COLOR_WHITE);
  return width;
}
#endif

s32 Message_Decode(s32 message, s32 mode);
void Text_ResetStyle(void);

/// Draws message centred in a 192-pixel canvas of one byte a pixel, as the
/// story crawl draws its lines. A line of dashes draws nothing and returns how
/// many dashes past the first it has (the crawl waits on it); any other line
/// returns 0.
s32 Text_DrawMessageToCanvas(s32 message, u8 *canvas) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 x = 0;
  s32 y = 0;
  s32 entry;
  s32 width;
  s32 height;
  u16 *text;
  u32 code;

  work->textBufferEnd = x;
  entry = Message_Decode(message, 0);
  Text_MeasureDecoded(entry, (u32 *)&width, (u32 *)&height, 0);
  text = &work->textBuffer[entry];
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
  x = (CRAWL_CANVAS_WIDTH - width) / 2;
  code = *text++;
  while (code != 0) {
    if (code <= 30) {
      switch (code) {
      case TEXT_COLOR:
        work->colour = *text;
        text++;
        break;
      case TEXT_OUTLINE:
        work->outline = *text;
        text++;
        break;
      case TEXT_LINE_SPACING:
        work->lineSpacing = *text;
        text++;
        break;
      case 11:
      case 12:
      case TEXT_ACTOR_NAME:
      skip:
        text++;
        break;
      case TEXT_COLORS_RESET:
        Text_ResetStyle();
        break;
      case TEXT_NEWLINE:
        x = 0;
        y += TEXT_LINE_HEIGHT;
        break;
      case 14:
      case TEXT_WINDOW_STATE:
      case 28:
        text++;
        goto skip;
      }
    } else {
      // Japanese text folds a mark after a kana into the kana's code, which
      // Text_DrawGlyphToCanvas draws.
#if (LANGUAGE == LANGUAGE_JAPANESE)
      if (*text == TEXT_MARK_VOICED) {
        code |= 1 << TEXT_MARK_SHIFT;
        text++;
      } else if (*text == TEXT_MARK_SEMIVOICED) {
        code |= 2 << TEXT_MARK_SHIFT;
        text++;
      }
#endif
      x += Text_DrawGlyphToCanvas(canvas, code, x, y);
    }
    code = *text++;
  }
  return 0;
}

/// Points reader at the start of message (its compressed bits and first
/// context), running the ARM lookup routine from a RAM copy, which it frees
/// before returning.
void Message_Find(struct TextReader *reader, s32 message) {
  void (*routine)(struct TextReader *, s32);

  RamCode_Load(routine, Text_LookupSymbolCode, Text_LookupSymbolCodeSize);
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

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
/* VIEW(window): routine Portrait_Create as s32 Portrait_Create(s32 id, s32
 * kind, s32 slot, struct Window *window, s32 x, s32 y); window/screens has
 * struct WindowSprite *Portrait_Create(s32 objectKind, s32 position, s32 side,
 * s32 arg3, s32 arg4, s32 arg5) {; tried as the definition's struct
 * WindowSprite *Portrait_Create(s32 objectKind, s32 position, s32 side, s32
 * arg3, s32 arg4, s32 arg5), it does not compile in tla-ja (make modern):
 * games/tla/include/system/task.h:53:23: note: expected 'TaskFunc' {aka 'void
 * (*)(void)'} but argument is of type 'int'. */
s32 Portrait_Create__window(s32 id, s32 kind, s32 slot, struct Window *window,
                            s32 x, s32 y) __asm__("Portrait_Create");
#define Portrait_Create Portrait_Create__window

/// Marks both portrait slots free (resource 999). Only the window system's
/// setup calls it.
void Portrait_ClearSlots(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 index = 0;

  do {
    work->resourceIds[index] = PORTRAIT_SLOT_EMPTY;
    index++;
  } while (index != 2);
}

extern const s16 gPortrait_CharacterMap[];
extern const s16 gPortrait_SpriteMap[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Portrait_Create__window(s32 id, s32 kind, s32 slot, struct Window *window,
                            s32 x, s32 y) __asm__("Portrait_Create");

/// Returns the portrait of a character (ids below 20) or another speaker's
/// sprite (128 and up for those), or -1 when it has none.
s32 Portrait_GetForSpeaker(u32 value) {
  s32 result = -1;
  s32 i = 0;

  // The party members' small sprites speak as the party members.
  switch (value) {
  case SPRITE_JENNA_SMALL - 1: // Felix's small sprite
    value = FELIX;
    break;
  case SPRITE_SHEBA_SMALL:
    value = SHEBA;
    break;
  case SPRITE_JENNA_SMALL:
    value = JENNA;
    break;
  case SPRITE_PIERS_SMALL:
    value = PIERS;
    break;
  case SPRITE_ISAAC_SMALL:
    value = ISAAC;
    break;
  case SPRITE_GARET_SMALL:
    value = GARET;
    break;
  case SPRITE_IVAN_SMALL:
    value = IVAN;
    break;
  case SPRITE_MIA_SMALL:
    value = MIA;
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

/// Opens a portrait window at tile (x, y) showing the speaker's portrait.
/// portraitOption is passed on through Portrait_Create to Icon_LoadWithPalette,
/// which ignores it.
struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x,
                                   s32 y) {
  struct Window *win;
  s32 minusFour;
  s32 offset;
  struct WindowWork *work;

  work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  if (Portrait_GetForSpeaker(speaker) == -1) {
    return 0;
  }
  minusFour = -4;
  offset = minusFour;
  if (work->mindRead != 0) {
    win = Window_Create(x, y, 6, 5, WINDOW_FLAG_INSTANT);
    offset = 0;
  } else {
    win = Window_Create(x, y, 5, 5, WINDOW_FLAG_INSTANT);
  }
  if (win != 0) {
    Portrait_Create(speaker, portraitOption, -1, win, offset, minusFour);
  }
  return win;
}

/// Closes the portrait window showing speaker entry, if one is open, at once.
/// portraitPictures selects the portrait slot, then portraitSprites identifies
/// its window sprite, found as the first sprite of some window.
void Portrait_CloseWindow(s32 id) {
  struct WindowWork *state;
  struct Window *work;
  s32 index;
  s32 i;
  struct WindowSprite *entity;

  state = (struct WindowWork *)Ram_HeapSlots->windowWork;
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
    entity = work->sprites.head;
    if ((u8)entity->kind == WINDOW_SPRITE_PORTRAIT && (u8)entity->index == id) {
      Window_Close(work, 2);
      return;
    }
  }
}

#undef UI_WORK
#undef DECODE_EUROPEAN
#undef PLOT_GLYPH_ROWS
#undef KANJI_FIRST_CODE
#undef KANJI_WIDTH
