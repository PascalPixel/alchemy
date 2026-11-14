/// @file
/// Window zoom and borders, and opening and waiting on message windows.
// Part of window.c.

#include "window_internal.h"

/// Scratch values for a stage of a window's zoom: the scaled part of a side,
/// the scaled duration and their ratio. A local of Window_DrawZoomBorder.
struct WindowZoomScratch {
  s32 scaledPart; ///< zoom stage (or stages left) times a side in tiles, << 16
  s32 scaledDuration; ///< the zoom's duration << 17
  s32 result; ///< scaledPart / scaledDuration, 16.16 fixed point (in tiles)
};

/// The per-frame window update: advances each window's zoom while it opens or
/// closes, draws it in full once open, and frees its slot once closed.
void Window_UpdateZooms(void) {
  struct WindowWork *state = WINDOW_WORK;
  struct Window *window = state->windows;
  s32 index = 0;

loop:
  // Flags 0 with a duration left means closing (Window_Close clears the
  // flags first).
  if (window->flags != 0) {
    // Opening: zoom out one stage a frame, then draw the window in full, which
    // clears the duration.
    if ((s16)window->zoomStage != 0) {
      Window_DrawZoomBorder(window, 0);
      window->zoomStage = (s16)window->zoomStage - 1;
    } else if (window->duration != 0) {
      Window_DrawOpened(window);
    }
  } else if (window->duration != 0) {
    // Closing: zoom in one stage a frame, then erase it and free its slot.
    if ((s16)window->zoomStage != window->duration) {
      Window_EraseBorderRect((s16)window->previousX, (s16)window->previousY,
                             (s16)window->previousWidth,
                             (s16)window->previousHeight);
      Window_DrawZoomBorder(window, 1);
      window->zoomStage++;
      /* FAKEMATCH(temporary): the flag through a local. */
      {
        u8 dirty = 1;

        state->dirty = dirty;
      }
    } else {
      Window_EraseBorderRect((s16)window->previousX, (s16)window->previousY,
                             (s16)window->previousWidth,
                             (s16)window->previousHeight);
      window->sprites.head = NULL;
      window->sprites.tailLink = NULL;
      window->width = 0;
      window->height = 0;
      window->x = 0;
      window->y = 0;
      window->opened = 0;
      window->animation = 0;
      window->state = 0;
      window->flags = 0;
      window->zoomStage = 0;
      window->duration = 0;
      window->previousX = 0;
      window->previousY = 0;
      window->previousWidth = 0;
      window->previousHeight = 0;
      state->dirty = 1;
    }
  }
  index++;
  window++;
  if (index != WINDOW_COUNT)
    goto loop;
}

/// Draws a zooming window at its size for this frame: the window grows from its
/// centre as zoomStage counts down to 0.
void Window_DrawZoomBorder(void *windowArg, s32 savePosition) {
  struct Window *window = windowArg;
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
  // Iwram_RatioMulQ14(d, v) is v / d in 16.16 fixed point.
  scratch.scaledPart = (stage * window->width) << 16;
  scratch.scaledDuration = duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  x = (scratch.result >> 16) + window->x;

  scratch.scaledPart = (remaining * window->width) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  width = scratch.result >> 15;

  scratch.scaledPart = (stage * window->height) << 16;
  scratch.scaledDuration = window->duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  y = (scratch.result >> 16) + window->y;

  scratch.scaledPart = (remaining * window->height) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  height = scratch.result >> 15;

  Window_DrawBorder(x, y, width, height);
  if (savePosition != 0) {
    window->previousX = x;
    window->previousY = y;
    window->previousWidth = width;
    window->previousHeight = height;
  }
}

/// Fills count halfwords at dst with value by DMA, and returns the position
/// after them. value holds the halfword in both its halves.
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count) {
  if (count > 0) {
    Dma_Fill16(value, dst, count * 2);
    dst += count;
  }
  return dst;
}

/// Draws an empty window: a border around a filled interior, at tile (x, y) of
/// width by height tiles, and frees the text tiles the rectangle showed.
void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
  u32 row;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  Window_ClearTextTilesInRect(x, y, width, height);
  // The top edge.
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
  cursor += 32 - width;
  // The sides and the fill between them.
  for (row = 1; row < height - 1; row++) {
    *cursor++ = WINDOW_TILE_BORDER_LEFT;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(
          cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
    *cursor++ = WINDOW_TILE_BORDER_RIGHT;
    cursor += 32 - width;
  }
  // The bottom edge.
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

/// Maps the text canvas tiles into the interior of a window at tile (x, y).
/// With fullWidth set the canvas covers the side border columns too.
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
                               s32 fullWidth) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
  u32 row;
  u32 col;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  // Canvas tiles run down each column of the interior: the tile at interior
  // row r (from 1) of column c (from 0) is TEXT_CANVAS_FIRST_TILE - 1 + r plus
  // c columns of height - 2 tiles.
  cursor += 32;
  if (fullWidth == 0) {
    for (row = 1; row < height - 1; row++) {
      cursor++;
      for (col = 1; col < width - 1; col++)
        *cursor++ = WINDOW_TILE(
            (TEXT_CANVAS_FIRST_TILE - 1 + row + (col - 1) * (height - 2)) &
            0xfff);
      cursor++;
      cursor += 32 - width;
    }
  } else {
    for (row = 1; row < height - 1; row++) {
      for (col = 0; col < width; col++)
        *cursor++ = WINDOW_TILE(
            (TEXT_CANVAS_FIRST_TILE - 1 + row + col * (height - 2)) & 0xfff);
      cursor += 32 - width;
    }
  }
  canvas->dirty = 1;
}

/// Returns 1 when every message has finished its text (or is idle), and 0 while
/// one is still typing.
s32 Message_AllTyped(void) {
  struct Message *message;
  s32 i;

  message = WINDOW_WORK->messages;
  for (i = 0; i != 3; i++, message++) {
    if (message->window != NULL && message->window->state == 0)
      return 0;
  }
  return 1;
}

/// Returns 1 when a window is fully closed (its slot free), and 0 while it is
/// open or closing.
s32 Window_IsClosed(struct Window *window) {
  // Japanese only: a NULL window counts as closed; the other editions read
  // through it.
#if !TBS_PATCH_1
  if (window == NULL)
    return 1;
#endif
  if (window->flags == 0 && window->duration == 0)
    return 1;
  return 0;
}

/// Resets the text style glyphs are drawn with: TEXT_DEFAULT_COLOR, no outline,
/// line spacing 10 and letter spacing 9.
void Text_ResetStyle(void) {
  struct WindowWork *state = WINDOW_WORK;

  state->colour = 15;
  state->lineSpacing = 10;
  state->letterSpacing = 9;
  state->outline = TEXT_STYLE_PLAIN;
  state->unusedAfterLineSpacing = 1;
}

/// Reserves the glyph sprites' tiles, resets the text style, starts the ring of
/// decoded text from its beginning and starts the per-frame window update
/// (WindowSystem_Update).
void WindowSystem_StartFieldUpdate(void) {
  struct WindowWork *state = WINDOW_WORK;
  s32 priority;

  // The glyph sheet, loaded with no source: 128 << 6 is 8192 bytes, 256 sprite
  // tiles, the four of a 16x16 glyph for each of the 64 window sprites.
  state->glyphTiles = VramCache_Load(VRAM_CACHE_ENTRY_GLYPH_SHEET, 128 << 6, 0);
  state->letterSpacing = 9;
  state->lineSpacing = 10;
  state->outline = TEXT_STYLE_PLAIN;
  state->colour = 15;
  state->textBufferEnd = 0;
  // 200 << 4 is TASK_PASS_FRAME: the update runs in the frame
  // pass.
  priority = 200;
  priority <<= 4;
  Task_Add(WindowSystem_Update, priority);
}

/// Resets the text style and starts the per-frame window update
/// (WindowSystem_Update), reserving the glyph sprites' tiles first when
/// loadGlyphTiles is set.
void WindowSystem_StartBattleUpdate(s32 loadGlyphTiles) {
  struct WindowWork *state = WINDOW_WORK;
  s32 priority;

  // As in the field's setup above.
  if (loadGlyphTiles != 0)
    state->glyphTiles =
        VramCache_Load(VRAM_CACHE_ENTRY_GLYPH_SHEET, 128 << 6, 0);
  state->letterSpacing = 9;
  state->lineSpacing = 10;
  state->outline = TEXT_STYLE_PLAIN;
  state->colour = 15;
  state->textBufferEnd = 0;
  priority = 200;
  priority <<= 4;
  Task_Add(WindowSystem_Update, priority);
}

/// Closes the battle message window at once, if one is open, and forgets it,
/// so the next battle message (BattleMessage_Open) opens a new one.
void BattleMessage_Close(void) {
  struct BattleMessageWork *display = gBattleMessageWork;
  struct Window *window = display->window;

  if (window != NULL) {
    Window_Close(window, 1);
    display->window = NULL;
  }
}

/// Shows a message in the battle message window, opening the window (a
/// full-width text canvas along the bottom of the screen) if it is not open,
/// and returns without waiting.
void BattleMessage_Open(s32 argument) {
  s32 entryIndex;
  struct Message *message;
  struct Window *previousWindow;
  struct Window *messageWindow;
  struct WindowWork *state;
  struct BattleMessageWork *control;

  state = WINDOW_WORK;
  control = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_MESSAGE);
  // message starts as NULL, though it is assigned again before it is read; the
  // same zero clears marked below
  message = NULL;
  state->battleMode = WINDOW_BATTLE_MESSAGE;
  entryIndex = Message_Decode(argument, 1);
  state->battleMode = WINDOW_BATTLE;

  if (state->textBuffer[entryIndex] != 0) {
    previousWindow = control->window;
    // A borderless canvas whose interior is rows 16 to 19, the bottom four, and
    // the full 30 columns: all 120 canvas tiles.
    if (previousWindow == NULL) {
      messageWindow = Window_Create(
          0, 15, 30, 6, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
      control->window = messageWindow;
      Window_MapTextCanvasTiles(0, 15, 30, 6, 1);
      control->marked = 0;
    } else {
      messageWindow = previousWindow;
    }
    if (messageWindow != NULL) {
      message =
          BattleMessage_Continue(messageWindow, entryIndex, control->marked);
      control->offset = (struct BattleMessageOffset *)message;
      control->marked = 0;
      // With no message free the window is closed, though control->window
      // still names its slot.
      if (message == NULL) {
        Window_Close(messageWindow, 1);
      }
    }
  }
}

/// Shows a message in the battle message window and waits until it has been
/// typed out.
void BattleMessage_ShowAndWait(s32 argument) {
  BattleMessage_Open(argument);
  while (Message_AllTyped() == 0)
    wait(1);
  wait(1);
}

/// Types a message into an open window from its top left, keeping the window's
/// size from page to page, with no voice blips.
s32 Message_OpenDecoded(struct Window *window, s32 message) {
  struct WindowWork *work = WINDOW_WORK;
  s32 entry;
  s32 typing = 0;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u16 frameReserve[4];

  work->voiceSound = 0;
  work->voiceCountdown = 0;
  entry = Message_Decode(message, 1);
  if (work->textBuffer[entry] == 0)
    return 0;
  if (window == 0)
    return 0;
  typing = (s32)Message_Start(window, entry, 0, 0, 0, 1);
  if (typing == 0)
    return 0;
  return typing;
}

/// Sets the decode flags the next message is decoded with (Message_Decode):
/// flags bit 0 (haltingSpeech) makes it halting speech, a short pause before
/// each character, broken off with an ellipsis after the first word; bit 1
/// (spacingAfter) puts code 222 after certain characters in English and
/// Japanese.
void Message_SetDecodeFlags(s32 flags) {
  struct WindowWork *work;

  work = WINDOW_WORK;
  if (work != NULL) {
    if (flags & 1) {
      work->haltingSpeech = 1;
    }
    if (2 & flags) {
      work->spacingAfter = 1;
    }
  }
}

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
  work->voiceSound = (packed << 4) >> 20;
  // the window starts as NULL, though it is assigned before it is read; the
  // same zero serves the zeros below
  window = NULL;
  work->voiceCountdown = 0;
  packed &= 0xffff;
  flags = 0;
  entry = Message_Decode(message, 1);
  if (work->textBuffer[entry] == 0)
    return NULL;
  // bounds receives each line's extra gap between characters, which the
  // message spreads the line with.
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

/// Shows a message in a window centred on the screen, waits until it is typed
/// out and closes it, as its enum FieldMessageFlag flags say.
void FieldMessage_Show(s32 message, s32 flags) {
  struct WindowWork *state = WINDOW_WORK;
  s32 zoom;
  s32 actorPosition[2] = {0, 0};
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  struct Window *window = NULL;

  y = x = 0;
  zoom = flags & FIELD_MESSAGE_ZOOM;

  if (flags & FIELD_MESSAGE_WAITING_FOR_SOUND) {
    state->waitingForSound = 1;
  }

  Text_GetMessageSize(message, &x, &y, (u32 *)&width, (u32 *)&height);

  // Centre the window across the screen, and in the top 12 rows.
  x = (30 - width) >> 1;
  y = (12 - height) >> 1;

  if (flags & FIELD_MESSAGE_LOWER) {
    y += 4;
  } else if (flags & FIELD_MESSAGE_LOWEST) {
    y += 12;
  } else {
    s32 actorTileY;

    // The character's screen y in pixels, as a tile row.
    Actor_GetScreenPositionFar(gMainRoutineWork.leaderActorId, actorPosition);
    actorTileY = actorPosition[1] >> 3;
    if (actorTileY > 9) {
      y = actorTileY - 5;
    } else {
      y = actorTileY + 4;
    }
  }

  window = Message_OpenWindow(message, x, y, zoom);

  if (window != NULL) {
    while (Message_AllTyped() == 0) {
      wait(1);
    }

    if (flags & FIELD_MESSAGE_HOLD_UPLOAD) {
      WINDOW_WORK->holdTilemapUpload = 1;
    }

    if (!(flags & FIELD_MESSAGE_KEEP_OPEN)) {
      Window_Close(window, zoom);
      while (Window_IsClosed(window) == 0) {
        wait(1);
      }
    }
  }

  state->waitingForSound = 0;
  state->voiceSound = 0;
  state->voiceCountdown = 0;
  wait(3);
}

/// The window system's per-frame task: windows, then messages, then sprite
/// animation.
void WindowSystem_Update(void) {
  Window_UpdateZooms();
  Message_UpdateAll();
  WindowSprite_UpdateAll();
}
