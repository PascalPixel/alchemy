/// @file
/// Window zoom and borders, and opening and waiting on message windows.
// Part of window.c.

#include "window_internal.h"

/// The pixels between sprite glyphs at a message's start.
#define TEXT_DEFAULT_LETTER_SPACING 9

/// The glyph tiles' size in sprite VRAM: the four tiles of a 16x16 glyph for
/// each window sprite.
#define GLYPH_TILES_SIZE (WINDOW_SPRITE_COUNT * 4 * TILE_SIZE_4BPP)

/// The tile rows a field message is centred in, from the top of the screen.
#define FIELD_MESSAGE_TOP_ROWS 12

/// A tilemap entry's tile number and flip bits, below its palette.
#define TILE_NUMBER_AND_FLIPS (TILE_NUMBER_MASK | TILE_HFLIP | TILE_VFLIP)

/// Scratch values for a stage of a window's zoom: the scaled part of a side,
/// the scaled duration and their ratio. A local of Window_DrawZoomBorder.
struct WindowZoomScratch {
  FIXED scaledPart;     ///< zoom stage (or stages left) times a side in tiles
  FIXED scaledDuration; ///< twice the zoom's duration
  FIXED result;         ///< scaledPart / scaledDuration, in tiles
};

/// The per-frame window update: advances each window's zoom while it opens or
/// closes, draws it in full once open, and frees its slot once closed.
void Window_UpdateZooms(void) {
  struct WindowState *windowState = WINDOW_STATE;
  struct Window *window = windowState->windows;
  s32 index = 0;

loop:
  // Flags 0 with a duration left means closing (Window_Close clears the
  // flags first).
  if (window->flags != 0) {
    // Opening: zoom out one stage a frame, then draw the window in full, which
    // clears the duration.
    if (window->zoomStage != 0) {
      Window_DrawZoomBorder(window, 0);
      window->zoomStage--;
    } else if (window->duration != 0) {
      Window_DrawOpened(window);
    }
  } else if (window->duration != 0) {
    // Closing: zoom in one stage a frame, then erase it and free its slot.
    if (window->zoomStage != window->duration) {
      Window_EraseBorderRect(window->previousX, window->previousY,
                             window->previousWidth, window->previousHeight);
      Window_DrawZoomBorder(window, 1);
      window->zoomStage++;
      /* FAKEMATCH(dead): a once-only block that places allBands, the dirty
       * bit that uploads every band, at its store; as windowState->dirty = 1.
       */
      {
        u8 allBands = 1;

        windowState->dirty = allBands;
      }
    } else {
      Window_EraseBorderRect(window->previousX, window->previousY,
                             window->previousWidth, window->previousHeight);
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
      windowState->dirty = 1;
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

  stage = window->zoomStage;
  duration = window->duration;
  remaining = duration - stage;
  // Iwram_DivFX(d, v) is v / d in 16.16 fixed point: the left edge
  // moves in by stage / duration of half the width, and the width is
  // (duration - stage) / duration of the whole, twice that half; the same for
  // the top and the height.
  scratch.scaledPart = toFIXED(stage * window->width);
  scratch.scaledDuration = toFIXED(2 * duration);
  scratch.result = Iwram_DivFX(scratch.scaledDuration, scratch.scaledPart);
  x = FIXEDtoINT(scratch.result) + window->x;

  scratch.scaledPart = toFIXED(remaining * window->width);
  scratch.result = Iwram_DivFX(scratch.scaledDuration, scratch.scaledPart);
  width = FIXEDtoINT_TWICE(scratch.result);

  scratch.scaledPart = toFIXED(stage * window->height);
  scratch.scaledDuration = toFIXED(2 * window->duration);
  scratch.result = Iwram_DivFX(scratch.scaledDuration, scratch.scaledPart);
  y = FIXEDtoINT(scratch.result) + window->y;

  scratch.scaledPart = toFIXED(remaining * window->height);
  scratch.result = Iwram_DivFX(scratch.scaledDuration, scratch.scaledPart);
  height = FIXEDtoINT_TWICE(scratch.result);

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
  struct WindowState *state = WINDOW_STATE;
  u16 *cursor = (u16 *)((y * BG_SCREEN_WIDTH + x) * 2 + (u32)state->tilemap);
  u32 row;

  if (width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
      height > DISPLAY_TILE_WIDTH)
    return;
  Window_ClearTextTilesInRect(x, y, width, height);
  // The top edge.
  if (state->mindRead != 0)
    *cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER;
  else
    *cursor++ = WINDOW_TILE_BORDER_TOP_LEFT;
  cursor = Memory_FillHalfwordsDma(
      cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_TOP), width - 2);
  if (state->mindRead != 0)
    *cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP;
  else
    *cursor++ = WINDOW_TILE_BORDER_TOP_RIGHT;
  cursor += BG_SCREEN_WIDTH - width;
  // The sides and the fill between them.
  for (row = 1; row < height - 1; row++) {
    *cursor++ = WINDOW_TILE_BORDER_LEFT;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(
          cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
    *cursor++ = WINDOW_TILE_BORDER_RIGHT;
    cursor += BG_SCREEN_WIDTH - width;
  }
  // The bottom edge.
  if (state->mindRead != 0)
    *cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_VFLIP;
  else
    *cursor++ = WINDOW_TILE_BORDER_BOTTOM_LEFT;
  cursor = Memory_FillHalfwordsDma(
      cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_BOTTOM), width - 2);
  if (state->mindRead != 0)
    *cursor = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP | TILE_VFLIP;
  else
    *cursor = WINDOW_TILE_BORDER_BOTTOM_RIGHT;
  state->dirty = 1;
}

/// Maps the text canvas tiles into the interior of a window at tile (x, y).
/// With fullWidth set the canvas covers the side border columns too.
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
                               s32 fullWidth) {
  struct WindowState *state = WINDOW_STATE;
  u16 *cursor = (u16 *)((y * BG_SCREEN_WIDTH + x) * 2 + (u32)state->tilemap);
  u32 row;
  u32 col;

  if (width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
      height > DISPLAY_TILE_WIDTH)
    return;
  // Canvas tiles run down each column of the interior: the tile at interior
  // row r (from 1) of column c (from 0) is TEXT_CANVAS_FIRST_TILE - 1 + r plus
  // c columns of height - 2 tiles.
  cursor += BG_SCREEN_WIDTH;
  if (fullWidth == 0) {
    for (row = 1; row < height - 1; row++) {
      cursor++;
      for (col = 1; col < width - 1; col++)
        *cursor++ = WINDOW_TILE(
            (TEXT_CANVAS_FIRST_TILE - 1 + row + (col - 1) * (height - 2)) &
            TILE_NUMBER_AND_FLIPS);
      cursor++;
      cursor += BG_SCREEN_WIDTH - width;
    }
  } else {
    for (row = 1; row < height - 1; row++) {
      for (col = 0; col < width; col++)
        *cursor++ = WINDOW_TILE(
            (TEXT_CANVAS_FIRST_TILE - 1 + row + col * (height - 2)) &
            TILE_NUMBER_AND_FLIPS);
      cursor += BG_SCREEN_WIDTH - width;
    }
  }
  state->dirty = 1;
}

/// Returns 1 when every message has finished its text (or is idle), and 0 while
/// one is still typing.
s32 Message_AllTyped(void) {
  struct Message *message;
  s32 i;

  message = WINDOW_STATE->messages;
  for (i = 0; i != MESSAGE_COUNT; i++, message++) {
    if (message->window != NULL && message->window->state == WINDOW_TYPING)
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

/// Resets the style glyphs are drawn in: the default colour, plain, and the
/// default line and letter spacing.
void Text_ResetStyle(void) {
  struct WindowState *state = WINDOW_STATE;

  state->colour = TEXT_DEFAULT_COLOR;
  state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  state->letterSpacing = TEXT_DEFAULT_LETTER_SPACING;
  state->outline = TEXT_STYLE_PLAIN;
  state->unusedAfterLineSpacing = 1;
}

/// Reserves the glyph sprites' tiles, resets the text style, starts the ring of
/// decoded text from its beginning and starts the per-frame window update
/// (WindowSystem_Update).
void WindowSystem_StartFieldUpdate(void) {
  struct WindowState *state = WINDOW_STATE;

  // The glyph tiles, reserved with nothing to load.
  state->glyphTiles =
      VramCache_Load(VRAM_CACHE_ENTRY_GLYPH_TILES, GLYPH_TILES_SIZE, 0);
  state->letterSpacing = TEXT_DEFAULT_LETTER_SPACING;
  state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  state->outline = TEXT_STYLE_PLAIN;
  state->colour = TEXT_DEFAULT_COLOR;
  state->textBufferEnd = 0;
  Task_Add(WindowSystem_Update, TASK_PASS_FRAME);
}

/// Resets the text style and starts the per-frame window update
/// (WindowSystem_Update), reserving the glyph sprites' tiles first when
/// loadGlyphTiles is set.
void WindowSystem_StartBattleUpdate(s32 loadGlyphTiles) {
  struct WindowState *state = WINDOW_STATE;

  if (loadGlyphTiles != 0)
    state->glyphTiles =
        VramCache_Load(VRAM_CACHE_ENTRY_GLYPH_TILES, GLYPH_TILES_SIZE, 0);
  state->letterSpacing = TEXT_DEFAULT_LETTER_SPACING;
  state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
  state->outline = TEXT_STYLE_PLAIN;
  state->colour = TEXT_DEFAULT_COLOR;
  state->textBufferEnd = 0;
  Task_Add(WindowSystem_Update, TASK_PASS_FRAME);
}

/// Closes the battle message window at once, if one is open, and forgets it,
/// so the next battle message (BattleMessage_Open) opens a new one.
void BattleMessage_Close(void) {
  struct BattleMessageState *display = gBattleMessageState;
  struct Window *window = display->window;

  if (window != NULL) {
    Window_Close(window, 1);
    display->window = NULL;
  }
}

/// Shows a message in the battle message window, opening the window (a
/// full-width text canvas along the bottom of the screen) if it is not open,
/// and returns without waiting.
void BattleMessage_Open(s32 message) {
  s32 entryIndex;
  struct Message *continued;
  struct Window *previousWindow;
  struct Window *messageWindow;
  struct WindowState *state;
  struct BattleMessageState *control;

  state = WINDOW_STATE;
  control = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_MESSAGE);
  continued = NULL;
  state->battleMode = WINDOW_BATTLE_MESSAGE;
  entryIndex = Message_Decode(message, 1);
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
      continued =
          BattleMessage_Continue(messageWindow, entryIndex, control->marked);
      control->offset = (struct BattleMessageOffset *)continued;
      control->marked = 0;
      // With no message free the window is closed, though control->window
      // still names its slot.
      if (continued == NULL) {
        Window_Close(messageWindow, 1);
      }
    }
  }
}

/// Shows a message in the battle message window and waits until it has been
/// typed out.
void BattleMessage_ShowAndWait(s32 message) {
  BattleMessage_Open(message);
  while (Message_AllTyped() == 0)
    wait(1);
  wait(1);
}

/// Types a message into an open window from its top left, keeping the window's
/// size from page to page, with no voice blips.
s32 Message_OpenDecoded(struct Window *window, s32 message) {
  struct WindowState *state = WINDOW_STATE;
  s32 entry;
  s32 typing = 0;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u16 frameReserve[4];

  state->voiceSound = 0;
  state->voiceCountdown = 0;
  entry = Message_Decode(message, 1);
  if (state->textBuffer[entry] == 0)
    return 0;
  if (window == NULL)
    return 0;
  typing = (s32)Message_Start(window, entry, 0, 0, 0, 1);
  if (typing == 0)
    return 0;
  return typing;
}

/// Sets the decode flags the next message is decoded with (Message_Decode):
/// MESSAGE_DECODE_HALTING_SPEECH makes it halting speech, a short pause before
/// each character, broken off with an ellipsis after the first word;
/// MESSAGE_DECODE_SPACING_AFTER puts a wide gap (ENTRY_WIDE_GAP) after certain
/// characters in English and Japanese.
void Message_SetDecodeFlags(s32 flags) {
  struct WindowState *state;

  state = WINDOW_STATE;
  if (state != NULL) {
    if (flags & MESSAGE_DECODE_HALTING_SPEECH) {
      state->haltingSpeech = 1;
    }
    if (MESSAGE_DECODE_SPACING_AFTER & flags) {
      state->spacingAfter = 1;
    }
  }
}

/// Opens a window sized to a message near tile (x, y) and starts typing the
/// message into it. packed holds the window options (MESSAGE_WINDOW_ZOOM and
/// the rest) in its low bits and the speaker's voice sound in bits 16 to 27
/// (MESSAGE_WINDOW_VOICE).
struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed) {
  s32 entry;
  u32 height;
  u32 width;
  u16 bounds[4];
  s32 flags;
  u16 *out;
  struct Window *window;
  struct WindowState *state;

  state = WINDOW_STATE;
  state->voiceSound = MESSAGE_WINDOW_VOICE_OF(packed);
  // Default: no window.
  window = NULL;
  state->voiceCountdown = 0;
  packed &= MESSAGE_WINDOW_OPTIONS_MASK;
  flags = 0;
  entry = Message_Decode(message, 1);
  if (state->textBuffer[entry] == 0)
    return NULL;
  // bounds receives each line's extra gap between characters, which the
  // message spreads the line with.
  Window_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
  if (width == 0 && height == 0)
    return NULL;
  if (!(packed & MESSAGE_WINDOW_ZOOM))
    flags |= WINDOW_FLAG_INSTANT;
  if (packed & MESSAGE_WINDOW_TEXT_CANVAS)
    flags |= WINDOW_FLAG_TEXT_CANVAS;
  if (packed & MESSAGE_WINDOW_ANCHOR_BOTTOM)
    flags |= WINDOW_FLAG_ANCHOR_BOTTOM;
  if (packed & MESSAGE_WINDOW_FIXED_X)
    flags |= WINDOW_FLAG_FIXED_X;
  window = Window_Create(x, y, width, height, flags);
  if (window == NULL)
    return NULL;
  if (Message_Start(window, entry, 0, 0, out, 0) == 0) {
    Window_Close(window, 1);
    return NULL;
  }
  state->haltingSpeech = 0;
  state->spacingAfter = 0;
  return window;
}

/// Shows a message in a window centred on the screen, waits until it is typed
/// out and closes it, as its enum FieldMessageFlag flags say.
void FieldMessage_Show(s32 message, s32 flags) {
  struct WindowState *state = WINDOW_STATE;
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

  // Centre the window across the screen and in its top rows, then lower it
  // by 4 or 12 rows when asked.
  x = (DISPLAY_TILE_WIDTH - width) >> 1;
  y = (FIELD_MESSAGE_TOP_ROWS - height) >> 1;

  if (flags & FIELD_MESSAGE_LOWER) {
    y += 4;
  } else if (flags & FIELD_MESSAGE_LOWEST) {
    y += 12;
  } else {
    s32 actorTileY;

    // Otherwise the window keeps clear of the leader: above them when they
    // stand in the screen's lower half, below them when in its upper half.
    Actor_GetScreenPositionFar(gMainRoutineState.leaderActorId, actorPosition);
    actorTileY = actorPosition[1] >> 3;
    if (actorTileY > DISPLAY_TILE_HEIGHT / 2 - 1) {
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
      WINDOW_STATE->holdTilemapUpload = 1;
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
