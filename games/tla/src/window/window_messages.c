/// @file
/// Message strings and arguments, symbol decoding and portrait windows.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"

/// A capacity no message reaches: the largest s32.
#define MESSAGE_CAPACITY_UNLIMITED ((u32)-1 >> 1)

/// The previous code's low byte, which picks a symbol's tree in its bank.
#define SYMBOL_TREE_MASK 0xff
/// A byte of the bit stream as it is read from: the first bit taken, the seven
/// left, and a stop bit above them that reads as the last 1 with nothing left,
/// when the next byte is loaded.
#define BIT_STREAM_STOP (1 << 7)

/// Decodes message into text (a u16 string) with no length limit: text must
/// have room for the whole message and its terminator.
void Message_DecodeRawUnbounded(s32 message, u16 *text) {
  Message_DecodeRaw(message, text, MESSAGE_CAPACITY_UNLIMITED);
}

/// Decodes message into destination as a zero-terminated string of at most
/// capacity entries, and returns its length. capacity counts halfwords
/// including the terminator and must be at least 1.
s32 Message_CopyExpanded(s32 message, u16 *destination, u32 capacity) {
  struct WindowWork *work;
  u32 count;

  work = WINDOW_WORK;
  work->textBufferEnd = 0;
  Message_Decode(message, 1);
  for (count = 0; count < capacity - 1 &&
                  (destination[count] = work->textBuffer[count]) != 0;
       count++) {
  }
  destination[count] = 0;
  return count;
}

/// The German, Spanish, French and Italian editions' text: its operands are
/// stored biased by OPERAND_UNBIAS, and it marks article classes.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DECODE_EUROPEAN
#endif

extern const u8 Text_DecodeSymbolCode[];
extern u8 Text_DecodeSymbolCodeSize[];

/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands.
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
  delta = -1;
#endif
  while ((value = decode(&reader)) != 0) {
    switch (value) {
    case 14:
      capacity -= 3;
      if (capacity <= 0)
        goto done;
      *text++ = value;
#if defined(DECODE_EUROPEAN)
      value = decode(&reader) + OPERAND_UNBIAS;
      *text++ = value;
      value = decode(&reader) + OPERAND_UNBIAS;
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
      value = decode(&reader) + OPERAND_UNBIAS;
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
  struct WindowWork *work;
  struct Message *message;
  struct Window *win;
  u32 done;
  struct Window *window;
  struct Window *pollWindow;
  s32 i;

  work = WINDOW_WORK;
  message = work->messages;
  win = work->windows;
  i = 0;
  do {
    window = message->window;
    if (window != 0 && window->flags != 0)
      Window_Close(window, 0);
    i++;
    message++;
  } while (i != ARRAY_COUNT(work->messages));

  // Free each message whose window has closed; while one is still closing,
  // wait a frame. The zoom's stage and duration are read as one word.
  for (;;) {
    done = 1;
    message = work->messages;
    i = 0;
    do {
      pollWindow = message->window;
      if (pollWindow != 0) {
        if ((*(s32 *)&pollWindow->zoomStage) == 0) {
          if (pollWindow->flags == 0)
            message->window = NULL;
          else
            done = 0;
        } else {
          done = 0;
        }
      }
      i++;
      message++;
    } while (i != ARRAY_COUNT(work->messages));
    if (done)
      break;
    wait(1);
  }

  // Then the windows opened directly.
  for (i = 0; i != WINDOW_COUNT; i++) {
    if (win->flags != 0)
      Window_Close(win, 0);
    win++;
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
      (u8 *)Resource_GetTableEntry(RESOURCE_WINDOW_BORDER_FLASH) + offset;
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
void Window_StepSwellEffect(struct Message *message) {
  if (message->window->animation == WINDOW_ANIMATION_SWELL) {
    if (message->countdown == 10)
      Window_ShowFlashFrame(&message->window, 3 * FLASH_FRAME_BYTES);
    else if (message->countdown == 9)
      Window_ShowFlashFrame(&message->window, 2 * FLASH_FRAME_BYTES);
    else if (message->countdown == 8)
      Window_ShowFlashFrame(&message->window, 1 * FLASH_FRAME_BYTES);
    else if (message->countdown == 6)
      Window_ShowFlashFrame(&message->window, 2 * FLASH_FRAME_BYTES);
    else if (message->countdown == 4)
      Window_ShowFlashFrame(&message->window, 3 * FLASH_FRAME_BYTES);
    if (message->countdown == 2) {
      struct Window *window = message->window;

      Window_DrawBorder((s16)window->x, (s16)window->y, window->width,
                        window->height);
    }
    message->countdown--;
    if (message->countdown == 0) {
      Dma_Set(Resource_GetTableEntry(RESOURCE_WINDOW_BORDER_FLASH),
              (void *)FLASH_FRAME_TILES, DMA_COPY_WORDS(FLASH_FRAME_BYTES / 4),
              REG_DMA3);
      message->window->animation = 0;
    }
  }
}

/// Empties the table of message arguments.
void MessageArgument_ClearAll(void) {
  s32 no;
  struct WindowWork *work;

  work = WINDOW_WORK;

  for (no = 0; no != MESSAGE_ARGUMENT_COUNT; no++) {
    work->argumentValues[no] = 0;
    work->argumentKinds[no] = MESSAGE_ARGUMENT_NONE;
  }
}

/// Adds a message argument of kind (MESSAGE_ARGUMENT_) with value to the first
/// free place of the argument table; when all eight are in use it is dropped.
void MessageArgument_Push(u32 value, u32 kind) {
  struct WindowWork *work = WINDOW_WORK;
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
  struct WindowWork *work = WINDOW_WORK;
  u32 i;
  u32 value = 0;

  for (i = 0; i < MESSAGE_ARGUMENT_COUNT; i++) {
    if (work->argumentKinds[i] == kind) {
      value = work->argumentValues[i];
      if (clear != 0) {
        work->argumentValues[i] = 0;
        work->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
      }
      break;
    }
  }
  return value;
}

/// Ends a message's pause early: when the message waits on a sound that has
/// finished, or while A, B, L or R is held.
s32 Text_CheckPauseSkipped(struct Message *message) {
  struct WindowWork *work = WINDOW_WORK;
  s32 cancelKeys = 0;

  if (work->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancelKeys = 1;
  if (gInput.held & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (gInput.held & KEY_B)
    cancelKeys = 1;
  if (cancelKeys) {
    message->countdown = 0;
    return 1;
  }
  return 0;
}

/// As Text_CheckPauseSkipped, but on a newly pressed button (the latched
/// presses while WindowWork.mindRead is set). While B is held the text also
/// goes on by itself once the wait's countdown runs down
/// (gMessageSpeed_HeldPageDelays); while it is up the countdown stays at 2.
s32 Text_CheckTextDismissed(struct Message *message) {
  struct WindowWork *work = WINDOW_WORK;
  s32 cancelKeys = 0;
  u32 pressedKeys;

  if (work->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancelKeys = 1;
  pressedKeys = gInput.pressed;
  if (work->mindRead != 0)
    pressedKeys = gInput.pendingPressed;
  if (pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (gInput.held & KEY_B) {
    if (message->countdown == 1)
      cancelKeys = 1;
  } else {
    message->countdown = 2;
  }
  if (cancelKeys) {
    message->countdown = 0;
    return 1;
  }
  return 0;
}

/// Closes each message's window whose text has ended, then waits ten frames.
void Message_CloseFinishedWindows(void) {
  s32 i;
  struct Message *message;
  struct Window *window;

  message = WINDOW_WORK->messages;
  i = 0;
  do {
    window = message->window;
    // The zoom's stage and duration are read as one word: both zero.
    if (window != NULL && *(s32 *)&window->zoomStage == 0 &&
        window->flags != 0 && window->state != 0) {
      Window_Close(window, WINDOW_FLAG_INSTANT & window->flags);
    }
    i++;
    message++;
  } while (i != ARRAY_COUNT(WINDOW_WORK->messages));
  wait(10);
}

/// Shows a message centred across the screen and in its top 15 rows (yOffset
/// tiles lower) and waits until it is typed out, then closes it, in the window
/// mode (CENTERED_MESSAGE_) gives.
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset) {
  struct WindowWork *work;
  struct Window *window;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entryIndex;

  work = WINDOW_WORK;
  x = 8;
  y = 8;
  window = NULL;
  entryIndex = Message_Decode(message, 1);
  if (work->textBuffer[entryIndex] != 0) {
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
    if (Message_Start(window, entryIndex, 0, 0, 0, 0) == 0)
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

/// Decodes message and empties the argument table; returns where its
/// decoded text starts. The Djinn and party join messages
/// (screens_joined.c) use it.
s32 Message_DecodeAndClearArguments(s32 message) {
  return Message_Decode(message, 1);
}

/// Decodes message after the text decoded last, over its terminator.
void Message_DecodeAppend(s32 message) {
  struct WindowWork *work = WINDOW_WORK;
  u32 count = work->textBufferEnd;

  count = (u16)(count - 1) & TEXT_BUFFER_MASK;
  work->textBufferEnd = (u16)(count - 1) & TEXT_BUFFER_MASK;
  Message_Decode(message, 1);
}

/// The story crawl's canvas, one byte a pixel: a line of 24 tiles of the
/// credits' tile ring.
#define CRAWL_CANVAS_WIDTH 192

/// The rows of a glyph drawn: all twelve of the Japanese font's, and fourteen
/// of the European fonts' fifteen.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PLOT_GLYPH_ROWS 12
#else
#define PLOT_GLYPH_ROWS 14
#endif

/// Plots a glyph's rows of bits onto the story crawl's canvas at dst, one byte
/// a pixel in color, leftmost pixel from the high bit; clear bits are left.
void Text_PlotGlyphRows(u16 *rows, u8 *dst, s32 color) {
  s32 y;
  u8 *p;
  s32 bits;

  for (y = 0; y != PLOT_GLYPH_ROWS; y++) {
    bits = *rows++ & ~3;
    p = dst;
    bits <<= GLYPH_ROW_BITS;
    while (bits != 0) {
      if (bits < 0)
        *p = color;
      bits <<= 1;
      p++;
    }
    dst += CRAWL_CANVAS_WIDTH;
  }
}

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
    markCode -= TEXT_SPACE;
    mark = gFont_Glyphs[markCode].rows;
  }
  ch &= (1 << TEXT_MARK_SHIFT) - 1;
  if (ch < KANJI_FIRST_CODE) {
    s32 index = ch - TEXT_SPACE;

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
  u16 *glyph = (u16 *)((u8 *)gFont_Glyphs +
                       (((ch & TEXT_LAST_FONT_CODE) - TEXT_SPACE) << 5));
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

/// Draws message centred in a 192-pixel canvas of one byte a pixel, as the
/// story crawl draws its lines. A line of dashes draws nothing and returns how
/// many dashes past the first it has (the crawl waits on it); any other line
/// returns 0.
s32 Text_DrawMessageToCanvas(s32 message, u8 *canvas) {
  struct WindowWork *work = WINDOW_WORK;
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
    if (code < TEXT_COMMANDS) {
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
      case TEXT_ACTOR_NAME_START:
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

/// Each decoding context's Huffman tree and its symbols, two pointers a row: a
/// message's previous symbol picks the row.
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
  code &= SYMBOL_TREE_MASK;
  base = Text_MessageContexts[contextBank][0];
  base += ((const u16 *)Text_MessageContexts[contextBank][1])[code];
  tree = base;
  leaves = base - 1;
  rank = 0;
  // The tree is its shape in preorder, a bit a node: 0 a branch, 1 a leaf.
  // Bits are read from each byte's lowest up; a buffer that has shifted down
  // to its stop bit (BIT_STREAM_STOP) is empty.
  // A text bit of 0 takes the left branch; 1 skips the left subtree,
  // counting its leaves in rank, and takes the right.
  for (;;) {
    bit = treeBits & 1;
    treeBits >>= 1;
    if (bit != 0 && treeBits == 0) {
      treeBits = *tree++;
      bit = treeBits & 1;
      treeBits = (treeBits >> 1) | BIT_STREAM_STOP;
    }
    if (bit != 0)
      break;
    bit = textBits & 1;
    textBits >>= 1;
    if (bit != 0 && textBits == 0) {
      textBits = *text++;
      bit = textBits & 1;
      textBits = (textBits >> 1) | BIT_STREAM_STOP;
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
        treeBits = (treeBits >> 1) | BIT_STREAM_STOP;
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

/// Marks both portrait slots free (resource 999). Only the window system's
/// setup calls it.
void Portrait_ClearSlots(void) {
  struct WindowWork *work = WINDOW_WORK;
  s32 index;

  for (index = 0; index != ARRAY_COUNT(work->portraitPictures); index++) {
    work->portraitPictures[index] = PORTRAIT_SLOT_EMPTY;
  }
}

/// A speaker below this is a character, found in gPortrait_CharacterMap; one
/// from it on is a sprite, found in gPortrait_SpriteMap.
#define PORTRAIT_FIRST_SPRITE_SPEAKER 16

/// Returns the portrait of a character (ids below
/// PORTRAIT_FIRST_SPRITE_SPEAKER) or another speaker's sprite (from
/// PORTRAIT_FIRST_SPRITE_PORTRAIT for those), or -1 when it has none.
s32 Portrait_GetForSpeaker(u32 value) {
  s32 result = -1;
  s32 portraitIndex = 0;

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
  // Both maps are {speaker, portrait} pairs ending in -1.
  if (value < PORTRAIT_FIRST_SPRITE_SPEAKER) {
    for (;;) {
      s32 speakerId = gPortrait_CharacterMap[portraitIndex];

      if (speakerId == -1)
        break;
      if (speakerId == value) {
        portraitIndex++;
        result = gPortrait_CharacterMap[portraitIndex];
        break;
      }
      portraitIndex += 2;
    }
  } else {
    for (;;) {
      s32 speakerId = gPortrait_SpriteMap[portraitIndex];

      if (speakerId == -1)
        break;
      if (speakerId == value) {
        portraitIndex++;
        result = gPortrait_SpriteMap[portraitIndex];
        result += PORTRAIT_FIRST_SPRITE_PORTRAIT;
        break;
      }
      portraitIndex += 2;
    }
  }
  return result;
}

/// Opens a portrait window at tile (x, y) showing the speaker's portrait.
/// portraitOption is passed on through Portrait_Create to Icon_LoadWithPalette,
/// which ignores it.
struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x,
                                   s32 y) {
  struct Window *window;
  s32 portraitY;
  s32 portraitX;
  struct WindowWork *work;

  work = WINDOW_WORK;
  if (Portrait_GetForSpeaker(speaker) == -1) {
    return 0;
  }
  portraitY = -4;
  portraitX = portraitY;
  if (work->mindRead != 0) {
    window = Window_Create(x, y, PORTRAIT_WINDOW_TILES + 1,
                           PORTRAIT_WINDOW_TILES, WINDOW_FLAG_INSTANT);
    portraitX = 0;
  } else {
    window = Window_Create(x, y, PORTRAIT_WINDOW_TILES, PORTRAIT_WINDOW_TILES,
                           WINDOW_FLAG_INSTANT);
  }
  if (window != 0) {
    Portrait_Create(speaker, portraitOption, -1, window, portraitX, portraitY);
  }
  return window;
}

/// Closes the portrait window showing speaker entry, if one is open, at once.
/// portraitPictures selects the portrait slot, then portraitSprites identifies
/// its window sprite, found as the first sprite of some window.
void Portrait_CloseWindow(s32 speaker) {
  struct WindowWork *state;
  struct Window *window;
  s32 resourceId;
  s32 portraitSlot;
  s32 windowIndex;
  struct WindowSprite *portraitSprite;

  state = WINDOW_WORK;
  window = state->windows;
  resourceId = Portrait_GetForSpeaker(speaker);
  if (resourceId == -1)
    return;

  if (state->portraitPictures[1] == resourceId) {
    portraitSlot = 1;
  } else if (state->portraitPictures[0] == resourceId) {
    portraitSlot = 0;
  } else {
    return;
  }

  resourceId = state->portraitSprites[portraitSlot];

  for (windowIndex = 0; windowIndex != WINDOW_COUNT; windowIndex++, window++) {
    portraitSprite = window->sprites.head;
#ifdef UBFIX
    // Window_Create and the closing update leave empty heads NULL. Skip them
    // before reading kind: dereferencing that NULL pointer is undefined.
    if (portraitSprite == NULL)
      continue;
#endif
    if ((u8)portraitSprite->kind == WINDOW_SPRITE_PORTRAIT &&
        (u8)portraitSprite->index == resourceId) {
      Window_Close(window, WINDOW_FLAG_INSTANT);
      return;
    }
  }
}

#undef DECODE_EUROPEAN
#undef PLOT_GLYPH_ROWS
#undef SYMBOL_TREE_MASK
#undef BIT_STREAM_STOP
#undef MESSAGE_CAPACITY_UNLIMITED
