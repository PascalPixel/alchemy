/// @file
/// Message strings and arguments, symbol decoding and portrait windows.
// Part of window.c.

#include "../window_internal.h"

/// Decodes message into text (a u16 string) with no length limit: text must
/// have room for the whole message and its terminator.
void Message_DecodeRawUnbounded(s32 message, u16 *text) {
  Message_DecodeRaw(message, text, 0x7fffffff);
}

/// Decodes message into destination as a zero-terminated string of at most
/// capacity entries, and returns its length.
/// capacity counts halfwords including the terminator and must be at least 1.
/// Unlike Message_DecodeRaw it goes through the ring of decoded text, from
/// its start, so names and numbers are written out, and it uses up the
/// message arguments; whatever decoded text the ring held is overwritten.
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

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DECODE_EUROPEAN
#endif

/// Decodes message into text as a zero-terminated string of 16-bit codes, with
/// at most capacity entries, keeping style codes with their operands. Names
/// and numbers stay as their codes: nothing is written out. capacity includes
/// the terminating halfword; the unbounded wrapper,
/// Message_DecodeRawUnbounded, supplies 0x7fffffff.
void Message_DecodeRaw(s32 message, u16 *text, s32 capacity) {
  struct MessageReader reader;
  void *original;
  s32 (*decode)(struct MessageReader *);
  u32 value;
#if !defined(DECODE_EUROPEAN)
  u16 delta;
#endif

  // The decoder may already be in slot 50 (TEXT_WORK_BLOCK), as it is while
  // Message_Decode decodes the names this is called for; then it
  // is borrowed, and freed only if loaded here.
  original = ((union HeapState *)&gHeapSlots)->slots[50];
  if (original == NULL)
    RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
  decode = (s32 (*)(struct MessageReader *))((union HeapState *)&gHeapSlots)
               ->slots[50];
  Message_Find(&reader, message);
#if !defined(DECODE_EUROPEAN)
  delta = 0xffff;
#endif
  // Operands are stored one above their value; adding 0xffff takes the one
  // off in 16 bits. Code 14, whose meaning is unknown, has two operands and
  // the codes of the next case one each. A one-operand code counts one halfword
  // of capacity but writes two.
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
    FreeHeapSlot(TEXT_WORK_BLOCK);
  *text = 0;
}

/// Zeroes the word at p unless it is already 0, and returns p. Nothing calls
/// it.
s32 *Memory_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}

// Closing the messages' windows, and the message argument table.

/// Closes every window: first the messages' windows, waiting until
/// they have closed, then the windows opened directly, which it does not wait
/// for. A message lets go of its window only once the window has finished
/// closing. Reached from the debug message viewer (DebugBattle_ViewMessages,
/// battle/battle/debug.c).
void Window_CloseAll(s32 unused) {
  struct WindowWork *windowWork;
  struct Message *message;
  struct Window *win;
  u32 done;
  struct Window *window;
  struct Window *pollWindow;
  s32 i;
  u16 flag;

  windowWork = WINDOW_WORK;
  message = windowWork->messages;
  win = windowWork->windows;
  i = 0;
  do {
    window = message->window;
    if (window != 0 && window->flags != 0)
      Window_Close(window, 0);
    i++;
    message++;
  } while (i != 3);

  // Free each message whose window has closed; while one is still closing,
  // wait a frame.
  for (;;) {
    done = 1;
    message = windowWork->messages;
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
    } while (i != 3);
    i = 0;
    if (done)
      break;
    WaitFrames(1);
  }

  // Then the windows opened directly.
  for (;;) {
    if (i == WINDOW_COUNT)
      break;
    if (win->flags != 0)
      Window_Close(win, 0);
    win++;
    i++;
  }
}

/// Steps a message's window animation (WINDOW_ANIMATION_SWELL): the frame
/// is drawn a tile bigger on every side until the countdown ends, then back to
/// normal. Any other animation is left set and does nothing, so the message
/// types no more text. Message_UpdateAll calls it in place of typing while the
/// window has an animation.
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
  s32 no;
  struct WindowWork *work;

  work = WINDOW_WORK;
  no = 0;

  do {
    work->argumentValues[no] = 0;
    work->argumentKinds[no] = MESSAGE_ARGUMENT_NONE;
    no++;
  } while (no != MESSAGE_ARGUMENT_COUNT);
}

/// Adds a message argument of kind (MESSAGE_ARGUMENT_) with value to the first
/// free place of the argument table; when all eight are in use it is dropped.
/// MESSAGE_ARGUMENT_NONE marks an empty place. Callers push a message's
/// arguments before decoding it. The decoder reads each argument code's value
/// from the first slot of its kind (MessageArgument_Lookup); told to clear, it
/// removes each as it reads it, so arguments of one kind are taken in the order
/// they were pushed, and it empties the table at the end.
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
/// or 0 when there is none. With clear set the argument is removed from the
/// table. Repeated kinds are allowed: only the first matching slot is read or
/// consumed.
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

/// Ends a message's pause early: when the message waits on a sound that
/// has finished (busy), or while A, B, L or R is held. Clears the countdown and
/// returns 1 if it ended.
/// This path uses held keys; dismissal below uses newly pressed keys instead.
/// Message_Type asks it each frame of a short or longest pause;
/// the long pause cannot be cut short.
s32 Text_CheckPauseSkipped(struct Message *message) {
  struct WindowWork *work = WINDOW_WORK;
  s32 cancelKeys = 0;
  s32 zero;

  if (work->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancelKeys = 1;
  zero = 0;
  if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (cancelKeys != zero) {
    message->countdown = zero;
    return 1;
  }
  return zero;
}

/// As Text_CheckPauseSkipped, but on a newly pressed button: the latched
/// presses while WindowWork.mindRead is set. Returns 1 when the player has
/// dismissed the text.
/// Mode chooses latched presses rather than changing which buttons dismiss
/// text.
s32 Text_CheckTextDismissed(struct Message *message) {
  struct WindowWork *work = WINDOW_WORK;
  s32 pressedKeys;
  s32 cancelKeys = 0;
  u8 zero;

  if (work->waitingForSound != 0 && Sound_GetBgmRestoreDelayFar() == 0)
    cancelKeys = 1;
  pressedKeys = gKeysPressed;
  zero = 0;
  if (work->mindRead != zero)
    pressedKeys = gKeysPressedLatch;
  if (pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (cancelKeys != 0) {
    message->countdown = zero;
    return 1;
  }
  return 0;
}

/// Closes each message's window whose text has ended, then waits ten
/// frames. It does not wait for the zooms to finish, and leaves windows still
/// typing open.
void Message_CloseFinishedWindows(void) {
  s32 i;
  struct Message *message;
  struct Window *window;

  message = WINDOW_WORK->messages;
  i = 0;
  do {
    window = message->window;
    if (window != NULL && window->zoomStage == 0 && window->duration == 0 &&
        window->flags != 0 && window->state != 0) {
      Window_Close(window, WINDOW_FLAG_INSTANT & window->flags);
    }
    i++;
    message++;
  } while (i != 3);
  WaitFrames(10);
}

/// Shows a message centred across the screen and in its top 15 rows (yOffset
/// tiles lower) and waits until it is typed out, then closes it, in the window
/// mode (enum CenteredMessageMode) gives.
/// The speaker's voice blips are reset afterwards.
void FieldMessage_ShowCentered(s32 message, s32 mode, s32 yOffset) {
  struct WindowWork *work;
  struct Window *messageWindow;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entryIndex;

  work = WINDOW_WORK;
  x = 8;
  y = 8;
  // the window starts as NULL, though every path that reads it assigns it
  // first; the same zero is the style argument below
  messageWindow = NULL;
  entryIndex = Message_Decode(message, 1);
  if (work->textBuffer[entryIndex] != 0) {
    Text_GetMessageSize(message, &x, &y, (u32 *)&width, (u32 *)&height);
    x = (30 - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    // Flags 2 is WINDOW_FLAG_INSTANT; mode is CENTERED_MESSAGE_BARE in the
    // second branch.
    if (mode != CENTERED_MESSAGE_BARE)
      messageWindow = Window_Create(x, y, width, height, 0);
    else {
      messageWindow = Window_Create(x, y, 0, 0, WINDOW_FLAG_INSTANT);
      messageWindow->width = mode;
      messageWindow->height = mode;
    }
    if (Message_Start(messageWindow, entryIndex, 0, 0, 0, 0) == 0)
      Window_Close(messageWindow, 1);
    else {
      while (Message_AllTyped() == 0)
        WaitFrames(1);
      if (mode != CENTERED_MESSAGE_BARE) {
        Window_Close(messageWindow, 0);
        while (Window_IsClosed(messageWindow) == 0)
          WaitFrames(1);
      } else
        Window_Close(messageWindow, 1);
      work->voiceSound = 0;
      work->voiceCountdown = 0;
    }
  }
}

/// Decodes message and empties the argument table; returns where its
/// decoded text starts. The Djinn and party join messages
/// (screens/joined.c) use it.
s32 Message_DecodeMode1(s32 message) { return Message_Decode(message, 1); }

/// Points reader at the start of message (its compressed bits and first
/// context), running the ARM lookup routine from a RAM copy, which it frees
/// before returning.
/// The RAM routine initializes reader state; symbols are decoded separately.
void Message_Find(struct MessageReader *reader, s32 message) {
  void (*routine)(struct MessageReader *, s32);

  RamCode_Load(routine, Message_FindArm, Message_FindCodeSize);
  routine(reader, message);
  Free(routine);
}

/// Decodes the next symbol of a message. Messages are Huffman coded with one
/// tree per previous symbol: the previous code picks the tree, the message's
/// bits walk it to a leaf, and the leaf's 12-bit symbol is returned and
/// becomes the next context.
/// Tree traversal consumes bytes; reader->words retains that byte position
/// between calls.
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

/// Marks both portrait slots free (PORTRAIT_SLOT_EMPTY). Only the field's
/// setup calls it.
void Portrait_ClearSlots(void) {
  struct WindowWork *work = WINDOW_WORK;
  s32 index = 0;

  do {
    work->portraitPictures[index] = PORTRAIT_SLOT_EMPTY;
    index++;
  } while (index != 2);
}

/// Returns the portrait of a character (ids below 20) or another speaker's
/// sprite (128 and up for those), or -1 when it has none.
s32 Portrait_GetForSpeaker(u32 value) {
  s32 result = -1;
  s32 portraitIndex = 0;

  // Both maps are {speaker, portrait} pairs ending in -1 (window/scales.c).
  if (value < 20) {
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
        result += 128;
        break;
      }
      portraitIndex += 2;
    }
  }
  return result;
}

/// Opens a portrait window at tile (x, y) showing the speaker's portrait.
/// portraitOption is passed on through Portrait_Create to
/// Icon_LoadWithPalette, which ignores it.
/// Returns the window, or 0 when the speaker has no portrait.
/// Portrait offsets are in pixels; the enclosing window bounds are in tiles.
/// The window opens at once, 5 by 5 tiles, or 6 by 5 under Mind Read with the
/// portrait 4 pixels further right. The portrait takes the second portrait
/// slot if it is free, else the first; with both taken the window stays
/// empty. It is the window's first window sprite, which
/// Portrait_CloseWindow looks for.
struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x,
                                   s32 y) {
  struct Window *window;
  s32 negativeFour;
  s32 offset;
  void *work;

  work = gWindowWork[0];
  if (Portrait_GetForSpeaker(speaker) == -1) {
    return 0;
  }
  negativeFour = -4;
  offset = negativeFour;
  if (((struct WindowWork *)work)->mindRead != 0) {
    window = Window_Create(x, y, 6, 5, WINDOW_FLAG_INSTANT);
    offset = 0;
  } else {
    window = Window_Create(x, y, 5, 5, WINDOW_FLAG_INSTANT);
  }
  if (window != 0) {
    Portrait_Create(speaker, portraitOption, -1, window, offset, negativeFour);
  }
  return window;
}

/// Closes the portrait window showing speaker entry, if one is open, at once.
/// portraitPictures selects the portrait slot, then portraitSprites identifies
/// its window sprite, found as the first sprite of some window.
void Portrait_CloseWindow(u32 speaker) {
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
    // Window_Create and the closing update leave empty heads NULL. Skip
    // them before reading kind: dereferencing that NULL pointer is undefined.
    // The plain guard adds four bytes to the native English object.
    if (portraitSprite == NULL)
      continue;
#endif
    if ((u8)portraitSprite->kind == WINDOW_SPRITE_PORTRAIT &&
        (u8)portraitSprite->index == resourceId) {
      Window_Close(window, 2);
      return;
    }
  }
}
