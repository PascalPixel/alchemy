/// @file
/// Window zoom and borders, and opening and waiting on message windows.
// Part of window.c.

#include "../window_internal.h"

void Func_08039f94(struct Window *window, s32 save_position);
void Window_DrawOpened(struct Window *window);

/// Steps each window's open or close animation: an open window moves toward
/// its geometry and is drawn when it arrives; a closing one is erased frame
/// by frame and cleared when its duration is reached.
void Window_UpdateZooms(void) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct Window *work = state->windows;
  s32 index = 0;
  u8 dirty;

  do {
    if (work->flags != 0) {
      if ((s16)work->frame != 0) {
        Func_08039f94(work, 0);
        work->frame = (s16)work->frame - 1;
      } else if (work->duration != 0) {
        Window_DrawOpened(work);
      }
    } else if (work->duration != 0) {
      if ((s16)work->frame != work->duration) {
        Window_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                                 (s16)work->previousWidth,
                                 (s16)work->previousHeight);
        Func_08039f94(work, 1);
        work->frame++;
        dirty = 1;
        state->dirty = dirty;
      } else {
        Window_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                                 (s16)work->previousWidth,
                                 (s16)work->previousHeight);
        work->output.head = NULL;
        work->output.tailLink = NULL;
        work->width = 0;
        work->height = 0;
        work->x = 0;
        work->y = 0;
        work->unk10 = 0;
        work->effect = 0;
        work->state = 0;
        work->flags = 0;
        work->frame = 0;
        work->duration = 0;
        work->previousX = 0;
        work->previousY = 0;
        work->previousWidth = 0;
        work->previousHeight = 0;
        state->dirty = 1;
      }
    }
    index++;
    work++;
  } while (index != UI_WINDOW_COUNT);
}

/// Scratch values for a stage of a window's zoom, as in The Broken Seal: the
/// scaled part of a side, the scaled duration and their ratio.
struct WindowZoomScratch {
  s32 scaledPart;
  s32 scaledDuration;
  s32 result;
};

void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height);

void Func_08039f94(struct Window *window, s32 save_position) {
  struct WindowZoomScratch scratch;
  s32 stage;
  s32 duration;
  s32 remaining;
  s32 x;
  s32 y;
  s32 width;
  s32 height;

  stage = (s16)window->frame;
  duration = window->duration;
  remaining = duration - stage;
  // Iwram_RatioMulQ14(d, v) is v / d in 16.16 fixed point, as in The Broken
  // Seal: the left edge moves in by stage / duration of half the width
  // (result >> 16), and the width is (duration - stage) / duration of the
  // whole (result >> 15); the same for the top and the height.
  scratch.scaledPart = (stage * window->width) << 16;
  scratch.scaledDuration = duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  x = (scratch.result >> 16) + (s16)window->x;

  scratch.scaledPart = (remaining * window->width) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  {
    s32 part = stage * window->height;
    width = scratch.result >> 15;
    scratch.scaledPart = part << 16;
  }
  scratch.scaledDuration = window->duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  y = (scratch.result >> 16) + (s16)window->y;

  scratch.scaledPart = (remaining * window->height) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  height = scratch.result >> 15;

  Window_DrawBorder(x, y, width, height);
  if (save_position != 0) {
    window->previousX = x;
    window->previousY = y;
    window->previousWidth = width;
    window->previousHeight = height;
  }
}

/// Fills count halfwords at dst with value and returns the position after
/// the fill.
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count) {
  if (count > 0) {
    Dma_Fill16(value, dst, count * 2);
    dst += count;
  }
  return dst;
}

u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);
void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);

void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  Window_ClearTextTilesInRect(x, y, width, height);
  if (canvas->mode != 0)
    *cursor++ = 0xf01c;
  else
    *cursor++ = 0xf010;
  cursor = Memory_FillHalfwordsDma(cursor, 0xf011f011, width - 2);
  if (canvas->mode != 0)
    *cursor++ = 0xf41c;
  else
    *cursor++ = 0xf012;
  cursor += 32 - width;
  for (row = 1; row < height - 1; row++) {
    *cursor++ = 0xf016;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
    *cursor++ = 0xf017;
    cursor += 32 - width;
  }
  if (canvas->mode != 0)
    *cursor++ = 0xf81c;
  else
    *cursor++ = 0xf013;
  cursor = Memory_FillHalfwordsDma(cursor, 0xf014f014, width - 2);
  if (canvas->mode != 0)
    *cursor = 0xfc1c;
  else
    *cursor = 0xf015;
  canvas->dirty = 1;
}

void Func_0803a1c0(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  Window_ClearTextTilesInRect(x, y, width, height);
  *cursor++ = 0xf008;
  cursor = Memory_FillHalfwordsDma(cursor, 0xf009f009, width - 2);
  *cursor++ = 0xf00a;
  cursor += 32 - width;
  for (row = 1; row < height - 1; row++) {
    *cursor++ = 0xf00e;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
    *cursor++ = 0xf00f;
    cursor += 32 - width;
  }
  *cursor++ = 0xf00b;
  cursor = Memory_FillHalfwordsDma(cursor, 0xf00cf00c, width - 2);
  *cursor = 0xf00d;
  canvas->dirty = 1;
}

void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 whole) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;
  u32 column;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  cursor += 32;
  if (whole == 0) {
    for (row = 1; row < height - 1; row++) {
      cursor++;
      for (column = 1; column < width - 1; column++)
        *cursor++ =
            ((0x127 + row + (column - 1) * (height - 2)) & 0xfff) | 0xf000;
      cursor++;
      cursor += 32 - width;
    }
  } else {
    for (row = 1; row < height - 1; row++) {
      for (column = 0; column < width; column++)
        *cursor++ = ((0x127 + row + column * (height - 2)) & 0xfff) | 0xf000;
      cursor += 32 - width;
    }
  }
  canvas->dirty = 1;
}

/// Returns 0 if any of the three channels has a window still writing (state
/// is zero), else 1.
s32 Message_AllTyped(void) {
  struct UiChannelSlot *ch;
  s32 i;

  ch = ((struct WindowWork *)Ram_HeapSlots->windowTiles)->channels;
  for (i = 0; i != 3; i++, ch++) {
    if (ch->work != NULL && ch->work->state == 0)
      return 0;
  }
  return 1;
}

s32 Window_IsClosed(struct Window *window) {
  if (window == NULL)
    return 1;
  if (window->flags == 0 && window->duration == 0)
    return 1;
  return 0;
}

void Text_ResetStyle(void) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;

  state->colour = 15;
  state->lineSpacing = 10;
  state->glyphSpacing = 9;
  state->outline = 0;
  state->unknownAfterLineSpacing = 1;
}

void WindowSystem_Update(void);

void WindowSystem_StartFieldUpdate(void) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;

  state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->glyphSpacing = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  Task_Add(WindowSystem_Update, TASK_PASS_VBLANK);
}

void WindowSystem_Update(void);
void WindowSystem_UpdateBattle(void);

void WindowSystem_StartBattleUpdate(s32 initialize) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;

  if (initialize != 0)
    state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->glyphSpacing = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  if (initialize != 0)
    Task_Add(WindowSystem_Update, TASK_PASS_VBLANK);
  else
    Task_Add(WindowSystem_UpdateBattle, TASK_PASS_VBLANK);
}

/// The message window's state in heap slot unk84[4].
struct UiMessageSlot {
  struct Window *window;
  s32 result; ///< what the last BattleMessage_Continue returned
  s32 cursor; ///< where the next message starts
};

void Message_Close(void) {
  struct UiMessageSlot *slot = Ram_HeapSlots->unk84[4];

  if (slot->window != NULL) {
    Window_Close(slot->window, 1);
    slot->window = NULL;
  }
}

s32 Message_Decode(s32 message, s32 mode);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);

/// Decodes message and starts it in the message window along the bottom of
/// the screen (tiles 0, 15, 30 by 6), creating the window first if it is not
/// open; a message that decodes to nothing shows nothing.
void Message_Show(s32 message) {
  s32 entry;
  struct UiMessageSlot *slot;
  struct Window *previous;
  struct Window *window;
  struct WindowWork *work;
  s32 result;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  slot = Ram_HeapSlots->unk84[4];
  // Default: 0 until the message starts; the same zero clears a new window's
  // cursor.
  result = 0;
  // The menu state reads 2 while the message decodes.
  work->menuState = 2;
  entry = Message_Decode(message, 1);
  work->menuState = 1;
  if (work->entries[entry] != 0) {
    previous = slot->window;
    if (previous == NULL) {
      window = Window_Create(0, 15, 30, 6, 10);
      slot->window = window;
      Window_MapTextCanvasTiles(0, 15, 30, 6, 1);
      slot->cursor = result;
    } else {
      window = previous;
    }
    if (window != NULL) {
      result = (s32)BattleMessage_Continue(window, entry, slot->cursor);
      slot->result = result;
      slot->cursor = 0;
      if (result == 0)
        Window_Close(window, 1);
    }
  }
}

void wait(s32 frames);
s32 Message_AllTyped(void);

void Message_ShowAndWaitCore(s32 message) {
  while (Message_AllTyped() == 0)
    wait(1);
  Message_Show(message);
  while (Message_AllTyped() == 0)
    wait(1);
}

/// Builds the render entries of a message and queues them in the window;
/// returns 0 when the message is empty or there is no window.
s32 Message_OpenDecoded(s32 window, s32 argument) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 entry;
  s32 result = 0;
  // Unused: gives the stack frame its 8 bytes above the outgoing arguments.
  UNUSED u16 frameReserve[4];

  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  entry = Message_Decode(argument, 1);
  if (work->entries[entry] == 0)
    return 0;
  if (window == 0)
    return 0;
  result = (s32)Message_Start((struct Window *)window, entry, 0,
                                          0, 0, 1);
  if (result == 0)
    return 0;
  return result;
}

void Window_SetBusyFlags(u32 flags) {
  u8 *work;

  work = Ram_HeapSlots->windowTiles;
  if (work == 0)
    return;
  if (flags & 1)
    work[WINDOW_BUSY_FLAGS_OFFSET] = 1;
  if (flags & 2)
    work[WINDOW_BUSY_FLAGS_OFFSET + 1] = 1;
}

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

s32 Message_Decode(s32 key, s32 mode);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs);
struct Window *Message_OpenWindow(s32 message, s32 x, s32 y,
                                          u32 packed) {
  s32 entry;
  u32 height;
  u32 width;
  u16 bounds[4];
  s32 flags;
  u16 *out;
  struct Window *window;
  struct WindowWork *work;

  work = UI_WORK;
  work->speakerVoiceSound = (packed << 4) >> 20;
  window = NULL;
  work->speakerVoiceCountdown = 0;
  packed &= 0xffff;
  flags = 0;
  entry = Message_Decode(message, 1);
  if (work->entries[entry] == 0)
    return NULL;
  Window_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
  if (width == 0 && height == 0)
    return NULL;
  if (!(packed & 1))
    flags |= 2;
  if (packed & 8)
    flags |= 8;
  if (packed & 16)
    flags |= 128;
  if (packed & 32)
    flags |= 256;
  window = Window_Create(x, y, width, height, flags);
  if (window == NULL)
    return NULL;
  if (Message_Start(window, entry, 0, 0, out, 0) == 0) {
    Window_Close(window, 1);
    return NULL;
  }
  work->unk138a[0] = 0;
  work->unk138a[1] = 0;
  return window;
}

// Text-workspace tail: the Japanese edition's table is 0x200 bytes shorter.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define TEXT_WORK_TAIL 0x1184
#else
#define TEXT_WORK_TAIL 0x1384
#endif

s32 Party_GetLeaderActorFar(void);
s32 Func_080c82a8(s32 object, s32 *out);
s32 Message_AllTyped(void);

void FieldMessage_Show(s32 no, s32 flags) {
  u8 *base = Ram_HeapSlots->windowTiles;
  s32 release;
  s32 pos[2] = {0, 0};
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  struct Window *work = NULL;
  s32 zero;
  s32 margin;
  s32 rows;

  y = x = 0;
  release = flags & 1;

  if (flags & 2) {
    *(u8 *)(base + TEXT_WORK_TAIL + 5) = 1;
  }

  Text_GetMessageSize(no, &x, &y, (u32 *)&width, (u32 *)&height);

  margin = 30 - width;
  rows = height;
  x = margin >> 1;
  y = (12 - rows) >> 1;

  if (flags & 8) {
    y = y + 4;
  } else if (flags & 0x40) {
    y = y + 12;
  } else {
    s32 dy;

    Func_080c82a8(Party_GetLeaderActorFar(), pos);
    dy = pos[1] >> 3;
    if (dy > 9) {
      y = dy - 5;
    } else {
      y = dy + 4;
    }
  }

  work = Message_OpenWindow(no, x, y, release);

  if (work != NULL) {
    while (Message_AllTyped() == 0) {
      wait(1);
    }

    if (flags & 0x20) {
      Ram_HeapSlots->windowTiles[6] = 1;
    }

    if (!(flags & 4)) {
      Window_Close(work, release);
      while (Window_IsClosed(work) == 0) {
        wait(1);
      }
    }
  }

  zero = 0;
  *(u8 *)(base + TEXT_WORK_TAIL + 5) = zero;
  *(u16 *)(base + TEXT_WORK_TAIL) = zero;
  *(u16 *)(base + TEXT_WORK_TAIL + 2) = zero;
  wait(3);
}

void WindowSystem_Update(void) {
  Window_UpdateZooms();
  Message_UpdateAll();
  WindowSprite_UpdateAll();
}

void WindowSystem_UpdateBattle(void) {
  if (Ram_HeapSlots->unke4[0] != 0) {
    Window_UpdateZooms();
    Message_UpdateAll();
    WindowSprite_UpdateAll();
  } else {
    Message_UpdateAll();
  }
}

#undef UI_WORK
#undef TEXT_WORK_TAIL
