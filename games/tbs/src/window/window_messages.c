/// @file
/// Message strings and arguments, symbol decoding and portrait windows.
// Part of window.c.

#include "window_internal.h"

/// A capacity no message reaches: the largest s32.
#define MESSAGE_CAPACITY_UNLIMITED 0x7fffffff

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
  struct WindowState *state;
  u32 count;

  state = WINDOW_STATE;
  state->textBufferEnd = 0;
  Message_Decode(message, 1);
  for (count = 0; count < capacity - 1 &&
                  (destination[count] = state->textBuffer[count]) != 0;
       count++) {
  }
  destination[count] = 0;
  return count;
}

/// Defined in the German, Spanish, French and Italian editions, whose raw
/// decoding takes no delta off the operands.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define DECODE_EUROPEAN
#endif

/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands.
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity) {
  struct MessageReader reader;
  void *original;
  s32 (*decode)(struct MessageReader *);
  u32 value;
#if !defined(DECODE_EUROPEAN)
  u16 delta;
#endif

  // The decoder may already be in HEAP_SLOT_MESSAGE_DECODER, as it is while
  // Message_Decode decodes the names this is called for; then it is
  // borrowed, and freed only if loaded here.
  original = ((union HeapState *)&gHeapSlots)->slots[HEAP_SLOT_MESSAGE_DECODER];
  if (original == NULL)
    RamCode_LoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbol,
                     Text_DecodeSymbolCodeSize);
  decode = (s32 (*)(struct MessageReader *))((union HeapState *)&gHeapSlots)
               ->slots[HEAP_SLOT_MESSAGE_DECODER];
  Message_Find(&reader, message);
#if !defined(DECODE_EUROPEAN)
  delta = (u16)-1;
#endif
  // Operands are stored one above their value; adding (u16)-1 takes the one off
  // in 16 bits.
  while ((value = decode(&reader)) != 0) {
    switch (value) {
    case MESSAGE_UNUSED_14:
      capacity -= 3;
      if (capacity <= 0)
        goto done;
      *text++ = value;
#if defined(DECODE_EUROPEAN)
      value = decode(&reader) + (u16)-1;
      *text++ = value;
      value = decode(&reader) + (u16)-1;
#else
      *text++ = decode(&reader) + delta;
      value = decode(&reader) + delta;
#endif
      break;
    case MESSAGE_TEXT_COLOR:
    case MESSAGE_TEXT_EFFECT:
    case MESSAGE_LINE_SPACING:
    case MESSAGE_UNUSED_11:
    case MESSAGE_UNUSED_12:
    case MESSAGE_WINDOW_EFFECT:
#if defined(DECODE_EUROPEAN)
    case MESSAGE_ARTICLE_CLASS:
#endif
      capacity--;
      if (capacity <= 0)
        goto done;
      *text++ = value;
      value = decode(&reader) + (u16)-1;
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

/// Unused: zeroes the word at p unless it is already 0, and returns p.
UNUSED s32 *Memory_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}

// Closing the messages' windows, and the message argument table.

/// Closes every window: first the messages' windows, waiting until they have
/// closed, then the windows opened directly, which it does not wait for.
void Window_CloseAll(s32 unused) {
  struct WindowState *state;
  struct Message *message;
  struct Window *directWindow;
  u32 done;
  struct Window *window;
  struct Window *pollWindow;
  s32 i;
  u16 flag;

  state = WINDOW_STATE;
  message = state->messages;
  directWindow = state->windows;
  i = 0;
  do {
    window = message->window;
    if (window != 0 && window->flags != 0)
      Window_Close(window, 0);
    i++;
    message++;
  } while (i != ARRAY_COUNT(state->messages));

  // Free each message whose window has closed; while one is still closing,
  // wait a frame.
  for (;;) {
    done = 1;
    message = state->messages;
    i = 0;
    do {
      pollWindow = message->window;
      if (pollWindow != 0) {
        if (pollWindow->zoomStage == 0 && pollWindow->duration == 0) {
          flag = pollWindow->flags;
          // flag is 0 here: the message is set free.
          if (flag == 0)
            message->window = (struct Window *)(u32)flag;
          else
            done = 0;
        } else {
          done = 0;
        }
      }
      i++;
      message++;
    } while (i != ARRAY_COUNT(state->messages));
    i = 0;
    if (done)
      break;
    wait(1);
  }

  // Then the windows opened directly.
  for (;;) {
    if (i == WINDOW_COUNT)
      break;
    if (directWindow->flags != 0)
      Window_Close(directWindow, 0);
    directWindow++;
    i++;
  }
}

/// Steps a message's window animation (WINDOW_ANIMATION_SWELL): the frame is
/// drawn a tile bigger on every side until the countdown ends, then back to
/// normal.
void Window_StepSwellEffect(struct Message *message) {
  struct Window *window = message->window;
  s32 effect = window->animation;
  s32 x = window->x;
  s32 y = window->y;
  s32 width = window->width;
  s32 height = window->height;

  if (effect != WINDOW_ANIMATION_SWELL)
    return;

  Window_DrawBorder(x - 1, y - 1, width + 2, height + 2);
  message->countdown--;
  if (message->countdown != 0)
    return;

  message->window->animation = 0;
  Window_EraseBorderRect(x - 1, y - 1, width + 2, height + 2);
  Window_DrawBorder(x, y, width, height);
}

/// Empties the table of message arguments.
void MessageArgument_ClearAll(void) {
  s32 i;
  struct WindowState *state;

  state = WINDOW_STATE;

  for (i = 0; i != MESSAGE_ARGUMENT_COUNT; i++) {
    state->argumentValues[i] = 0;
    state->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
  }
}

/// Adds a message argument of kind (MESSAGE_ARGUMENT_) with value to the first
/// free place of the argument table; when all eight are in use it is dropped.
void MessageArgument_Push(u32 value, u32 kind) {
  struct WindowState *state = WINDOW_STATE;
  u32 i = 0;
  u32 limit = MESSAGE_ARGUMENT_COUNT;

  do {
    if (state->argumentKinds[i] == MESSAGE_ARGUMENT_NONE) {
      state->argumentValues[i] = value;
      state->argumentKinds[i] = kind;
      break;
    }
    i++;
  } while (i != limit);
}

/// Returns the value of the first message argument of kind (MESSAGE_ARGUMENT_),
/// or 0 when there is none.
u32 MessageArgument_Lookup(u32 kind, u32 clear) {
  struct WindowState *state = WINDOW_STATE;
  u32 i;
  u32 value = 0;

  for (i = 0; i < MESSAGE_ARGUMENT_COUNT; i++) {
    if (state->argumentKinds[i] == kind) {
      value = state->argumentValues[i];
      if (clear != 0) {
        state->argumentValues[i] = 0;
        state->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
      }
      break;
    }
  }
  return value;
}

/// Ends a message's pause early, returning 1: when the message waits on a
/// sound that has finished, or while A, B, L or R is held.
s32 Text_CheckPauseSkipped(struct Message *message) {
  struct WindowState *state = WINDOW_STATE;
  s32 cancelKeys = 0;

  if (state->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancelKeys = 1;
  if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (cancelKeys != 0) {
    message->countdown = 0;
    return 1;
  }
  return 0;
}

/// As Text_CheckPauseSkipped, but on a newly pressed button: the latched
/// presses while WindowState.mindRead is set.
s32 Text_CheckTextDismissed(struct Message *message) {
  struct WindowState *state = WINDOW_STATE;
  s32 pressedKeys;
  s32 cancelKeys = 0;

  if (state->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancelKeys = 1;
  pressedKeys = gKeysPressed;
  if (state->mindRead != 0)
    pressedKeys = gKeysPressedLatch;
  if (pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (cancelKeys != 0) {
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

  message = WINDOW_STATE->messages;
  i = 0;
  do {
    window = message->window;
    if (window != NULL && window->zoomStage == 0 && window->duration == 0 &&
        window->flags != 0 && window->state != WINDOW_TYPING) {
      Window_Close(window, WINDOW_FLAG_INSTANT & window->flags);
    }
    i++;
    message++;
  } while (i != ARRAY_COUNT(WINDOW_STATE->messages));
  wait(10);
}

/// Shows a message centred across the screen and in its top 15 rows (yOffset
/// tiles lower), waits until it is typed out and closes it, in the window mode
/// (enum CenteredMessageMode) asks for.
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset) {
  struct WindowState *state;
  struct Window *messageWindow;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entryIndex;

  state = WINDOW_STATE;
  x = 8;
  y = 8;
  messageWindow = NULL;
  entryIndex = Message_Decode(message, 1);
  if (state->textBuffer[entryIndex] != 0) {
    Text_GetMessageSize(message, &x, &y, (u32 *)&width, (u32 *)&height);
    x = (DISPLAY_TILE_WIDTH - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != CENTERED_MESSAGE_BARE)
      messageWindow = Window_Create(x, y, width, height, 0);
    else {
      // mode, CENTERED_MESSAGE_BARE, is 0: the window has no size.
      messageWindow = Window_Create(x, y, 0, 0, WINDOW_FLAG_INSTANT);
      messageWindow->width = mode;
      messageWindow->height = mode;
    }
    if (Message_Start(messageWindow, entryIndex, 0, 0, 0, 0) == 0)
      Window_Close(messageWindow, 1);
    else {
      while (Message_AllTyped() == 0)
        wait(1);
      if (mode != CENTERED_MESSAGE_BARE) {
        Window_Close(messageWindow, 0);
        while (Window_IsClosed(messageWindow) == 0)
          wait(1);
      } else
        Window_Close(messageWindow, 1);
      state->voiceSound = 0;
      state->voiceCountdown = 0;
    }
  }
}

/// Decodes message and empties the argument table; returns where its
/// decoded text starts. The Djinn and party join messages
/// (screens_joined.c) use it.
s32 Message_DecodeAndClearArguments(s32 message) {
  return Message_Decode(message, 1);
}

/// Points reader at the start of message (its compressed bits and first
/// context), running the ARM lookup routine from a RAM copy, which it frees
/// before returning.
void Message_Find(struct MessageReader *reader, s32 message) {
  void (*routine)(struct MessageReader *, s32);

  RamCode_Load(routine, Message_FindArm, Message_FindCodeSize);
  routine(reader, message);
  Heap_Release(routine);
}

/// Unused: decodes the next symbol of a message, as Text_DecodeSymbol, the
/// copy the game runs from RAM, does.
UNUSED s32 Text_DecodeSymbolInC(struct MessageReader *reader) {
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
  // The previous code's high bits pick the bank, its low byte the tree.
  contextBank = code >> 8;
  code &= SYMBOL_TREE_MASK;
  base = Text_MessageContexts[contextBank][0];
  base += ((const u16 *)Text_MessageContexts[contextBank][1])[code];
  tree = base;
  leaves = base - 1;
  rank = 0;
  // The tree is its shape in preorder, a bit a node: 0 a branch, 1 a leaf.
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

/// Marks both portrait slots free (PORTRAIT_SLOT_EMPTY). Only the field's
/// setup calls it.
void Portrait_ClearSlots(void) {
  struct WindowState *state = WINDOW_STATE;
  s32 index;

  for (index = 0; index != ARRAY_COUNT(state->portraitPictures); index++) {
    state->portraitPictures[index] = PORTRAIT_SLOT_EMPTY;
  }
}

/// Returns the portrait of a character or another speaker's sprite, or -1 when
/// it has none: speakers up to Garet's child sprite take theirs from
/// gPortrait_CharacterMap, the others from gPortrait_SpriteMap, numbered from
/// PORTRAIT_SPRITE_MAP_FIRST (Icon_LoadWithPalette takes it as the row
/// PORTRAIT_JUPITER_DJINNI).
s32 Portrait_GetForSpeaker(u32 value) {
  s32 result = -1;
  s32 portraitIndex = 0;

  // Both maps are {speaker, portrait} pairs ending in -1 (window/scales.c).
  if (value <= SPRITE_GARET_CHILD) {
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
        result += PORTRAIT_SPRITE_MAP_FIRST;
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
  void *state;

  state = gWindowState[0];
  if (Portrait_GetForSpeaker(speaker) == -1) {
    return 0;
  }
  {
    s32 portraitX = -4;
    s32 portraitY = -4;

    if (((struct WindowState *)state)->mindRead != 0) {
      window = Window_Create(x, y, 6, 5, WINDOW_FLAG_INSTANT);
      portraitX = 0;
    } else {
      window = Window_Create(x, y, 5, 5, WINDOW_FLAG_INSTANT);
    }
    if (window != 0) {
      Portrait_Create(speaker, portraitOption, -1, window, portraitX,
                      portraitY);
    }
  }
  return window;
}

/// Closes the window showing a speaker's portrait, if one is open, at once: the
/// portrait slot showing it names its window sprite, the first sprite of its
/// window.
void Portrait_CloseWindow(u32 speaker) {
  struct WindowState *state;
  struct Window *window;
  s32 resourceId;
  s32 portraitSlot;
  s32 windowIndex;
  struct WindowSprite *portraitSprite;

  state = WINDOW_STATE;
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

  // From here resourceId is the slot's window sprite.
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
