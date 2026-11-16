/// @file
/// Window zoom and borders, and opening and waiting on message windows.
// Part of window.c.

#include "window_internal.h"

void Window_DrawZoomBorder(struct Window *window, s32 save_position);
void Window_DrawOpened(struct Window *window);

/// Steps each window's open or close animation: an open window moves toward
/// its geometry and is drawn when it arrives; a closing one is erased frame
/// by frame and cleared when its duration is reached.
void Window_UpdateZooms(void) {
  struct WindowWork *state = WINDOW_WORK;
  struct Window *work = state->windows;
  s32 index = 0;
  u8 dirty;

  do {
    if (work->flags != 0) {
      if ((s16)work->zoomStage != 0) {
        Window_DrawZoomBorder(work, 0);
        work->zoomStage = (s16)work->zoomStage - 1;
      } else if (work->duration != 0) {
        Window_DrawOpened(work);
      }
    } else if (work->duration != 0) {
      if ((s16)work->zoomStage != work->duration) {
        Window_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                               (s16)work->previousWidth,
                               (s16)work->previousHeight);
        Window_DrawZoomBorder(work, 1);
        work->zoomStage++;
        dirty = 1;
        state->dirty = dirty;
      } else {
        Window_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                               (s16)work->previousWidth,
                               (s16)work->previousHeight);
        work->sprites.head = NULL;
        work->sprites.tailLink = NULL;
        work->width = 0;
        work->height = 0;
        work->x = 0;
        work->y = 0;
        work->opened = 0;
        work->animation = 0;
        work->state = 0;
        work->flags = 0;
        work->zoomStage = 0;
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
  } while (index != WINDOW_COUNT);
}

/// Scratch values for a stage of a window's zoom, as in The Broken Seal: the
/// scaled part of a side, the scaled duration and their ratio.
struct WindowZoomScratch {
  s32 scaledPart;
  s32 scaledDuration;
  s32 result;
};

/// Draws a zooming window at its size for this frame: the window grows from
/// its centre as zoomStage counts down to 0. With savePosition set (when
/// closing) the drawn rectangle is remembered, for the next frame to erase.
void Window_DrawZoomBorder(struct Window *window, s32 save_position) {
  struct WindowZoomScratch scratch;
  s32 stage;
  s32 duration;
  s32 remaining;
  s32 x;
  s32 y;
  s32 width;
  s32 height;

  stage = (s16)window->zoomStage;
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

/// Draws an empty window: a border around a filled interior, at tile (x, y) of
/// width by height tiles, and frees the text tiles the rectangle showed.
void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;

  if (width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
      height > DISPLAY_TILE_WIDTH)
    return;
  Window_ClearTextTilesInRect(x, y, width, height);
  if (canvas->mindRead != 0)
    *cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER;
  else
    *cursor++ = WINDOW_TILE_BORDER_TOP_LEFT;
  cursor = Memory_FillHalfwordsDma(
      cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_TOP), width - 2);
  if (canvas->mindRead != 0)
    *cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP;
  else
    *cursor++ = WINDOW_TILE_BORDER_TOP_RIGHT;
  cursor += BG_SCREEN_WIDTH - width;
  for (row = 1; row < height - 1; row++) {
    *cursor++ = WINDOW_TILE_BORDER_LEFT;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(
          cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
    *cursor++ = WINDOW_TILE_BORDER_RIGHT;
    cursor += BG_SCREEN_WIDTH - width;
  }
  if (canvas->mindRead != 0)
    *cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_VFLIP;
  else
    *cursor++ = WINDOW_TILE_BORDER_BOTTOM_LEFT;
  cursor = Memory_FillHalfwordsDma(
      cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_BOTTOM), width - 2);
  if (canvas->mindRead != 0)
    *cursor = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP | TILE_VFLIP;
  else
    *cursor = WINDOW_TILE_BORDER_BOTTOM_RIGHT;
  canvas->dirty = 1;
}

/// Window_DrawBorder in the flash border's tiles (8 to 15), which
/// Window_ShowFlashFrame loads: the frame a window flashes with as it
/// swells.
void Window_DrawFlashBorder(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;

  if (width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
      height > DISPLAY_TILE_WIDTH)
    return;
  Window_ClearTextTilesInRect(x, y, width, height);
  *cursor++ = WINDOW_TILE_FLASH_TOP_LEFT;
  cursor = Memory_FillHalfwordsDma(
      cursor, WINDOW_TILE_PAIR(WINDOW_TILE_FLASH_TOP), width - 2);
  *cursor++ = WINDOW_TILE_FLASH_TOP_RIGHT;
  cursor += BG_SCREEN_WIDTH - width;
  for (row = 1; row < height - 1; row++) {
    *cursor++ = WINDOW_TILE_FLASH_LEFT;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(
          cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
    *cursor++ = WINDOW_TILE_FLASH_RIGHT;
    cursor += BG_SCREEN_WIDTH - width;
  }
  *cursor++ = WINDOW_TILE_FLASH_BOTTOM_LEFT;
  cursor = Memory_FillHalfwordsDma(
      cursor, WINDOW_TILE_PAIR(WINDOW_TILE_FLASH_BOTTOM), width - 2);
  *cursor = WINDOW_TILE_FLASH_BOTTOM_RIGHT;
  canvas->dirty = 1;
}

/// Maps the text canvas tiles into the interior of a window at tile (x, y).
/// With fullWidth set the canvas covers the side border columns too.
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
                               s32 fullWidth) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *cursor = &canvas->tilemap[y][x];
  u32 row;
  u32 column;

  if (width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
      height > DISPLAY_TILE_WIDTH)
    return;
  cursor += BG_SCREEN_WIDTH;
  if (fullWidth == 0) {
    for (row = 1; row < height - 1; row++) {
      cursor++;
      for (column = 1; column < width - 1; column++)
        *cursor++ = WINDOW_TILE(
            (TEXT_CANVAS_FIRST_TILE - 1 + row + (column - 1) * (height - 2)) &
            0xfff);
      cursor++;
      cursor += BG_SCREEN_WIDTH - width;
    }
  } else {
    for (row = 1; row < height - 1; row++) {
      for (column = 0; column < width; column++)
        *cursor++ = WINDOW_TILE(
            (TEXT_CANVAS_FIRST_TILE - 1 + row + column * (height - 2)) & 0xfff);
      cursor += BG_SCREEN_WIDTH - width;
    }
  }
  canvas->dirty = 1;
}

/// Returns 0 if any of the three channels has a window still writing (state
/// is zero), else 1.
s32 Message_AllTyped(void) {
  struct UiChannelSlot *ch;
  s32 i;

  ch = WINDOW_WORK->channels;
  for (i = 0; i != 3; i++, ch++) {
    if (ch->work != NULL && ch->work->state == 0)
      return 0;
  }
  return 1;
}

/// Returns 1 when a window is fully closed (its slot free), and 0 while it is
/// open or closing.
s32 Window_IsClosed(struct Window *window) {
  if (window == NULL)
    return 1;
  if (window->flags == 0 && window->duration == 0)
    return 1;
  return 0;
}

/// Resets the text style glyphs are drawn with: colour 15, no outline, line
/// spacing 10 and letter spacing 9.
void Text_ResetStyle(void) {
  struct WindowWork *state = WINDOW_WORK;

  state->colour = TEXT_COLOR_WHITE;
  state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  state->glyphSpacing = 9;
  state->outline = TEXT_STYLE_PLAIN;
  state->unusedAfterLineSpacing = 1;
}

void WindowSystem_Update(void);

/// Reserves the glyph sprites' tiles, resets the text style, starts the ring of
/// decoded text from its beginning and starts the per-frame window update
/// (WindowSystem_Update).
void WindowSystem_StartFieldUpdate(void) {
  struct WindowWork *state = WINDOW_WORK;

  state->glyphTiles = VramCache_Load(VRAM_CACHE_ENTRY_GLYPH_SHEET, 128 << 6, 0);
  state->glyphSpacing = 9;
  state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  state->outline = TEXT_STYLE_PLAIN;
  state->colour = TEXT_COLOR_WHITE;
  state->textBufferEnd = 0;
  Task_Add(WindowSystem_Update, TASK_PASS_VBLANK);
}

void WindowSystem_Update(void);
void WindowSystem_UpdateBattle(void);

/// Resets the text style and starts the per-frame window update
/// (WindowSystem_Update), reserving the glyph sprites' tiles first when
/// initialize is set.
void WindowSystem_StartBattleUpdate(s32 initialize) {
  struct WindowWork *state = WINDOW_WORK;

  if (initialize != 0)
    state->glyphTiles =
        VramCache_Load(VRAM_CACHE_ENTRY_GLYPH_SHEET, 128 << 6, 0);
  state->glyphSpacing = 9;
  state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  state->outline = TEXT_STYLE_PLAIN;
  state->colour = TEXT_COLOR_WHITE;
  state->textBufferEnd = 0;
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

/// Closes the battle message window at once, if one is open, and forgets it,
/// so the next battle message (BattleMessage_Open) opens a new one.
void BattleMessage_Close(void) {
  struct UiMessageSlot *slot = Ram_HeapSlots->battleMessage;

  if (slot->window != NULL) {
    Window_Close(slot->window, 1);
    slot->window = NULL;
  }
}

s32 Message_Decode(s32 message, s32 mode);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

/// Decodes message and starts it in the message window along the bottom of
/// the screen (tiles 0, 15, 30 by 6), creating the window first if it is not
/// open; a message that decodes to nothing shows nothing.
void BattleMessage_Open(s32 message) {
  s32 entry;
  struct UiMessageSlot *slot;
  struct Window *previous;
  struct Window *window;
  struct WindowWork *work;
  s32 result;

  work = WINDOW_WORK;
  slot = Ram_HeapSlots->battleMessage;
  // Default: 0 until the message starts; the same zero clears a new window's
  // cursor.
  result = 0;
  // The menu state reads 2 while the message decodes.
  work->battleMode = WINDOW_BATTLE_MESSAGE;
  entry = Message_Decode(message, 1);
  work->battleMode = WINDOW_BATTLE;
  if (work->textBuffer[entry] != 0) {
    previous = slot->window;
    if (previous == NULL) {
      window = Window_Create(0, 15, 30, 6,
                             WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
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

s32 Message_AllTyped(void);

/// Shows a message in the battle message window and waits until it has been
/// typed out.
void BattleMessage_ShowAndWait(s32 message) {
  while (Message_AllTyped() == 0)
    wait(1);
  BattleMessage_Open(message);
  while (Message_AllTyped() == 0)
    wait(1);
}

/// Builds the render entries of a message and queues them in the window;
/// returns 0 when the message is empty or there is no window.
s32 Message_OpenDecoded(s32 window, s32 argument) {
  struct WindowWork *work = WINDOW_WORK;
  s32 entry;
  s32 result = 0;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u16 frameReserve[4];

  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  entry = Message_Decode(argument, 1);
  if (work->textBuffer[entry] == 0)
    return 0;
  if (window == 0)
    return 0;
  result = (s32)Message_Start((struct Window *)window, entry, 0, 0, 0, 1);
  if (result == 0)
    return 0;
  return result;
}

/// Sets the decode flags the next message is decoded with
/// (Message_Decode): MESSAGE_DECODE_HALTING_SPEECH makes it halting
/// speech, a short pause before each character, broken off with an ellipsis
/// after the first word; MESSAGE_DECODE_SPACING_AFTER puts a wide gap
/// (ENTRY_WIDE_GAP) after certain characters in English and Japanese. Flags
/// already set stay set: only Message_OpenWindow clears both, once it has
/// started a message typing, so they also apply to every message decoded before
/// that.
void Message_SetDecodeFlags(u32 flags) {
  struct WindowWork *work;

  work = WINDOW_WORK;
  if (work == NULL)
    return;
  if (flags & MESSAGE_DECODE_HALTING_SPEECH)
    work->haltingSpeech = 1;
  if (flags & MESSAGE_DECODE_SPACING_AFTER)
    work->spacingAfter = 1;
}

s32 Message_Decode(s32 key, s32 mode);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
/// Opens a window sized to a message near tile (x, y) and starts typing the
/// message into it. packed holds the window options in its low bits (bit 0:
/// zoom open rather than appear at once; bit 3: a text canvas; bit 4: keep the
/// bottom edge; bit 5: keep the left edge) and the speaker's voice sound in
/// bits 16 to 27.
struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed) {
  s32 entry;
  u32 height;
  u32 width;
  u16 bounds[4];
  s32 flags;
  u16 *out;
  struct Window *window;
  struct WindowWork *work;

  work = WINDOW_WORK;
  work->speakerVoiceSound = (packed << 4) >> 20;
  window = NULL;
  work->speakerVoiceCountdown = 0;
  packed &= 0xffff;
  flags = 0;
  entry = Message_Decode(message, 1);
  if (work->textBuffer[entry] == 0)
    return NULL;
  Window_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
  if (width == 0 && height == 0)
    return NULL;
  if (!(packed & 1))
    flags |= WINDOW_FLAG_INSTANT;
  if (packed & 8)
    flags |= WINDOW_FLAG_TEXT_CANVAS;
  if (packed & 16)
    flags |= WINDOW_FLAG_ANCHOR_BOTTOM;
  if (packed & 32)
    flags |= WINDOW_FLAG_FIXED_X;
  window = Window_Create(x, y, width, height, flags);
  if (window == NULL)
    return NULL;
  if (Message_Start(window, entry, 0, 0, out, 0) == 0) {
    Window_Close(window, 1);
    return NULL;
  }
  work->haltingSpeech = 0;
  work->spacingAfter = 0;
  return window;
}

s32 Message_AllTyped(void);

/// Shows a message in a window centred on the screen, waits until it is typed
/// out and closes it, as its flags say.
void FieldMessage_Show(s32 no, s32 flags) {
  struct WindowWork *state = WINDOW_WORK;
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
  release = flags & FIELD_MESSAGE_ZOOM;

  if (flags & FIELD_MESSAGE_WAITING_FOR_SOUND) {
    state->waitingForSound = 1;
  }

  Text_GetMessageSize(no, &x, &y, (u32 *)&width, (u32 *)&height);

  margin = 30 - width;
  rows = height;
  x = margin >> 1;
  y = (12 - rows) >> 1;

  if (flags & FIELD_MESSAGE_LOWER) {
    y = y + 4;
  } else if (flags & FIELD_MESSAGE_LOWEST) {
    y = y + 12;
  } else {
    s32 dy;

    Actor_GetScreenPositionFar(Party_GetLeaderActorFar(), pos);
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

    if (flags & FIELD_MESSAGE_HOLD_UPLOAD) {
      WINDOW_WORK->holdTilemapUpload = 1;
    }

    if (!(flags & FIELD_MESSAGE_KEEP_OPEN)) {
      Window_Close(work, release);
      while (Window_IsClosed(work) == 0) {
        wait(1);
      }
    }
  }

  zero = 0;
  state->waitingForSound = zero;
  state->speakerVoiceSound = zero;
  state->speakerVoiceCountdown = zero;
  wait(3);
}

/// The window system's per-frame task: windows, then messages, then sprite
/// animation.
void WindowSystem_Update(void) {
  Window_UpdateZooms();
  Message_UpdateAll();
  WindowSprite_UpdateAll();
}

/// The window system's per-frame task in battle: windows, messages and sprite
/// animation while the battle menu's work exists, otherwise messages alone.
void WindowSystem_UpdateBattle(void) {
  if (Ram_HeapSlots->battleMenuWork != 0) {
    Window_UpdateZooms();
    Message_UpdateAll();
    WindowSprite_UpdateAll();
  } else {
    Message_UpdateAll();
  }
}
