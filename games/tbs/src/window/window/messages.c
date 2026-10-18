// Part of window.c, the window object, compiled only through it.
// Message strings and arguments, symbol decoding and portrait windows.

#include "../window_internal.h"

// Decodes message into buf (a u16 string) with no length limit.
void Ui_AdjustValueWithoutLimit(s32 message, u16 *text) {
  UiText_DecodeMessage(message, text, 0x7fffffff);
}

// Decodes message key into destination as a zero-terminated string of at most
// capacity entries, and returns its length.
// capacity counts halfwords including the terminator and must be at least 1.
s32 UiText_CopyMessageString(s32 message, u16 *destination, u32 capacity) {
  struct UiRenderWork *work;
  u32 count;

  work = UI_WORK;
  work->count = 0;
  UiText_BuildRenderEntries(message, 1);
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

// Decodes message into text as a zero-terminated string of 16-bit codes, with
// at most capacity entries, keeping style codes with their operands.
// capacity includes the terminating halfword; the unbounded wrapper supplies
// -1.
void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity) {
  struct TextReader reader;
  void *original;
  s32 (*decode)(struct TextReader *);
  u32 value;
#if !defined(DECODE_EUROPEAN)
  u16 delta;
#endif

  original = ((union HeapState *)&gWorkSlot)->slots[50];
  if (original == NULL)
    RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
  decode =
      (s32 (*)(struct TextReader *))((union HeapState *)&gWorkSlot)->slots[50];
  UiText_LookupMessage(&reader, message);
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
    Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
  *text = 0;
}

// Clears *p if it is set, and returns p.
s32 *Runtime_ClearWordIfSet(s32 *p) {
  if (*p != 0) {
    *p = 0;
  }
  return p;
}

// Closing the message channels' windows, and the message argument table.

#if !GAME_TLA

// Closes every window: first the message channels' windows, waiting until
// they have closed, then the windows opened directly.
// Channel references are cleared only after their windows finish closing.
void UiWork_DrainPending(s32 unused) {
  struct UiRenderWork *uw;
  struct UiChannelSlot *ch;
  struct UiWindow *win;
  u32 done;
  struct UiWindow *work;
  struct UiWindow *poll_work;
  s32 i;
  u16 flag;

  uw = UI_WORK;
  ch = uw->channels;
  win = uw->windows;
  i = 0;
  do {
    work = ch->work;
    if (work != 0 && work->flags != 0)
      UiWork_Finalize(work, 0);
    i++;
    ch++;
  } while (i != 3);

  // Free each channel whose window has closed; while one is still closing,
  // wait a frame.
  for (;;) {
    done = 1;
    ch = uw->channels;
    i = 0;
    do {
      poll_work = ch->work;
      if (poll_work != 0) {
        if (poll_work->frame == 0 && poll_work->duration == 0) {
          flag = poll_work->flags;
          if (flag == 0)
            ch->work = (struct UiWindow *)(u32)flag;
          else
            done = 0;
        } else {
          done = 0;
        }
      }
      i++;
      ch++;
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
      UiWork_Finalize(win, 0);
    win++;
    i++;
  }
}

// Steps a channel's window effect (UI_WINDOW_EFFECT_SWELL): the frame is drawn
// a tile bigger on every side until the countdown ends, then back to normal.
void UiWindow_StepSwellEffect(struct UiChannelSlot *channel) {
  struct UiWindow *window = channel->work;
  s32 effect = window->effect;
  s32 x = window->x;
  s32 y = window->y;
  s32 width = window->width;
  s32 height = window->height;

  if (effect != UI_WINDOW_EFFECT_SWELL)
    return;

  UiWindow_DrawFrame(x - 1, y - 1, width + 2, height + 2);
  channel->countdown--;
  if (channel->countdown != 0)
    return;

  channel->work->effect = 0;
  UiWindow_EraseBorderRect(x - 1, y - 1, width + 2, height + 2);
  UiWindow_DrawFrame(x, y, width, height);
}

// Empties the table of message arguments.
void UiWork_ClearValueNameTables(void) {
  s32 no;
  struct UiRenderWork *work;

  work = UI_WORK;
  no = 0;

  do {
    work->values[no] = 0;
    work->names[no] = 0;
    no++;
  } while (no != UI_ARGUMENT_SLOT_COUNT);
}

// Adds a message argument of kind (MSG_ARG_) with value to the first free
// place of the argument table; when all eight are in use it is dropped.
// Kind 0 denotes an empty slot; a full table drops the new value.
void UiWork_PushValueSlot(u32 value, u32 kind) {
  struct UiRenderWork *work = UI_WORK;
  u32 no = 0;
  u32 limit = UI_ARGUMENT_SLOT_COUNT;

  do {
    if (work->names[no] == 0) {
      work->values[no] = value;
      work->names[no] = kind;
      break;
    }
    no++;
  } while (no != limit);
}

// Returns the value of the first message argument of kind (MSG_ARG_), or 0
// when there is none. With clear set the argument is removed from the table.
// Repeated kinds are allowed: only the first matching slot is read or consumed.
u32 UiText_LookupArgument(u32 kind, u32 clear) {
  struct UiRenderWork *work = UI_WORK;
  u32 i;
  u32 value = 0;

  for (i = 0; i < UI_ARGUMENT_SLOT_COUNT; i++) {
    if (work->names[i] == kind) {
      value = work->values[i];
      if (clear != 0) {
        work->values[i] = 0;
        work->names[i] = 0;
      }
      break;
    }
  }
  return value;
}

// Ends a channel's pause early: when the message waits on a sound that has
// finished (busy), or while A, B, L or R is held. Clears the countdown and
// returns 1 if it ended.
// This path uses held keys; dismissal below uses newly pressed keys instead.
s32 UiText_CheckPauseSkipped(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = UI_WORK;
  s32 cancelKeys = 0;
  s32 zero;

  if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
    cancelKeys = 1;
  zero = 0;
  if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (cancelKeys != zero) {
    channel->countdown = zero;
    return 1;
  }
  return zero;
}

// As UiText_CheckPauseSkipped, but on a newly pressed button: the latched
// presses while UiRenderWork.mode is set. Returns 1 when the player has
// dismissed the text.
// Mode chooses latched presses rather than changing which buttons dismiss text.
s32 UiText_CheckTextDismissed(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = UI_WORK;
  s32 pressedKeys;
  s32 cancelKeys = 0;
  u8 zero;

  if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
    cancelKeys = 1;
  pressedKeys = gKeysPressed;
  zero = 0;
  if (work->mode != zero)
    pressedKeys = gKeysPressedLatch;
  if (pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancelKeys = 1;
  if (cancelKeys != 0) {
    channel->countdown = zero;
    return 1;
  }
  return 0;
}

// Closes each message channel's window whose text has ended, then waits ten
// frames.
void UiWork_FinalizePendingCore(void) {
  s32 i;
  struct UiChannelSlot *ch;
  struct UiWindow *work;

  ch = UI_WORK->channels;
  i = 0;
  do {
    work = ch->work;
    if (work != NULL && work->frame == 0 && work->duration == 0 &&
        work->flags != 0 && work->state != 0) {
      UiWork_Finalize(work, UI_WINDOW_INSTANT & work->flags);
    }
    i++;
    ch++;
  } while (i != 3);
  WaitFrames(10);
}
#endif

// Shows a message centred on the screen (yOffset tiles lower) and waits until
// it is typed out, then closes it. With mode 0 the window has no frame or
// size and opens and closes at once; otherwise it is a zooming window sized
// to the message.
void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 yOffset) {
  struct UiRenderWork *render;
  struct UiWindow *messageWindow;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entryIndex;

  render = UI_WORK;
  x = 8;
  y = 8;
  // the window starts as NULL, though every path that reads it assigns it
  // first; the same zero is the style argument below
  messageWindow = NULL;
  entryIndex = UiText_BuildRenderEntries(message, 1);
  if (render->entries[entryIndex] != 0) {
    UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width,
                                 (u32 *)&height);
    x = (30 - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != 0)
      messageWindow = UiWindow_Create(x, y, width, height, 0);
    else {
      messageWindow = UiWindow_Create(x, y, 0, 0, 2);
      messageWindow->width = mode;
      messageWindow->height = mode;
    }
    if (UiText_QueueRenderEntries(messageWindow, entryIndex, 0, 0, 0, 0) == 0)
      UiWork_Finalize(messageWindow, 1);
    else {
      while (UiWork_IsComplete() == 0)
        WaitFrames(1);
      if (mode != 0) {
        UiWork_Finalize(messageWindow, 0);
        while (UiWork_IsIdle(messageWindow) == 0)
          WaitFrames(1);
      } else
        UiWork_Finalize(messageWindow, 1);
      render->speakerVoiceSound = 0;
      render->speakerVoiceCountdown = 0;
    }
  }
}

// Decodes message and empties the argument table; returns where its
// decoded text starts.
s32 UiText_BuildRenderEntriesMode1(s32 message) {
  return UiText_BuildRenderEntries(message, 1);
}

// Points reader at the start of message (its compressed bits and first
// context), running the ARM lookup routine from a RAM copy.
// The RAM routine initializes reader state; symbols are decoded separately.
void UiText_LookupMessage(struct TextReader *reader, s32 message) {
  void (*routine)(struct TextReader *, s32);

  RamCode_Load(routine, UiText_LookupMessageArm, UiText_LookupMessageCodeSize);
  routine(reader, message);
  MemFree(routine);
}

extern const void *const Text_MessageContexts[][2];

// Decodes the next symbol of a message. Messages are Huffman coded with one
// tree per previous symbol: the previous code picks the tree, the message's
// bits walk it to a leaf, and the leaf's 12-bit symbol is returned and
// becomes the next context.
// Tree traversal consumes bytes; reader->words retains that byte position
// between calls.
// Nothing calls it.
UNUSED s32 UiText_DecodeSymbol(struct TextReader *reader) {
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
  contextBank = code >> 8;
  code &= 0xff;
  base = Text_MessageContexts[contextBank][0];
  base += ((const u16 *)Text_MessageContexts[contextBank][1])[code];
  tree = base;
  leaves = base - 1;
  rank = 0;
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

// Marks both portrait slots free (999).
void UiWork_SetTwoEntriesTo999(void) {
  struct UiRenderWork *work = UI_WORK;
  s32 index = 0;

  do {
    work->resourceIds[index] = 999;
    index++;
  } while (index != 2);
}

// Returns the portrait of a character (ids below 20) or another speaker's
// sprite (128 and up for those), or -1 when it has none.
s32 Localization_LookupEntryId(u32 value) {
  s32 result = -1;
  s32 portraitIndex = 0;

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

// Opens a portrait window at tile (x, y) showing the speaker's portrait.
// portraitOption is passed on to the portrait object. Returns the window,
// or 0 when the speaker has no portrait.
// Portrait offsets are in pixels; the enclosing window bounds are in tiles.
struct UiWindow *UiWindow_CreateWithSideObject(s32 speaker, s32 portraitOption,
                                               s32 x, s32 y) {
  struct UiWindow *window;
  s32 negativeFour;
  s32 offset;
  void *work;

  work = gWindowWork[0];
  if (Localization_LookupEntryId(speaker) == -1) {
    return 0;
  }
  negativeFour = -4;
  offset = negativeFour;
  if (((struct UiRenderWork *)work)->mode != 0) {
    window = UiWindow_Create(x, y, 6, 5, 2);
    offset = 0;
  } else {
    window = UiWindow_Create(x, y, 5, 5, 2);
  }
  if (window != 0) {
    CreateSideObject(speaker, portraitOption, -1, window, offset, negativeFour);
  }
  return window;
}

// Closes the portrait window showing speaker entry, if one is open.
// resourceIds selects the portrait slot, then outputIds identifies its render
// output.
void UiWork_FinalizeEntityMatchingLocalizedId(u32 speaker) {
  struct UiRenderWork *state;
  struct UiWindow *window;
  s32 resourceId;
  s32 portraitSlot;
  s32 windowIndex;
  struct RenderOutput *portraitOutput;

  state = UI_WORK;
  window = state->windows;
  resourceId = Localization_LookupEntryId(speaker);
  if (resourceId == -1)
    return;

  if (state->resourceIds[1] == resourceId) {
    portraitSlot = 1;
  } else if (state->resourceIds[0] == resourceId) {
    portraitSlot = 0;
  } else {
    return;
  }

  resourceId = state->outputIds[portraitSlot];

  for (windowIndex = 0; windowIndex != WINDOW_COUNT; windowIndex++, window++) {
    portraitOutput = window->output.head;
#ifdef UBFIX
    // UiWindow_Create and the closing update leave empty heads NULL. Skip
    // them before reading kind: dereferencing that NULL pointer is undefined.
    // The plain guard adds four bytes to the native English object.
    if (portraitOutput == NULL)
      continue;
#endif
    if ((u8)portraitOutput->kind == 2 &&
        (u8)portraitOutput->index == resourceId) {
      UiWork_Finalize(window, 2);
      return;
    }
  }
}
