// Part of window.c, the window object, compiled only through it.
// Window zoom and frames, and opening and waiting on message windows.

#include "../window_internal.h"

struct UiWindowInterpolationScratch {
  s32 scaledPart;
  s32 scaledDuration;
  s32 result;
};

// The per-frame window update: steps each window's zoom while it opens or
// closes, draws it in full once open, and frees its slot once closed.
void UiWork_ProcessDirectWork(void) {
  struct UiRenderWork *state = UI_WORK;
  struct UiWindow *work = state->windows;
  s32 index = 0;
  u8 dirty;

loop:
  if (work->flags != 0) {
    // Opening: zoom out one step a frame, then draw the window in full.
    if ((s16)work->frame != 0) {
      UiWindow_DrawZoomFrame(work, 0);
      work->frame = (s16)work->frame - 1;
    } else if (work->duration != 0) {
      UiWindow_DrawOpened(work);
    }
  } else if (work->duration != 0) {
    // Closing: zoom in one step a frame, then erase it and free its slot.
    if ((s16)work->frame != work->duration) {
      UiWindow_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                               (s16)work->previousWidth,
                               (s16)work->previousHeight);
      UiWindow_DrawZoomFrame(work, 1);
      work->frame++;
      dirty = 1;
      state->dirty = dirty;
    } else {
      UiWindow_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
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
  if (index != WINDOW_COUNT)
    goto loop;
}

// Draws the frame of a zooming window at its size for this frame: the window
// grows from its centre as frame counts down to 0. With savePosition set (when
// closing) the drawn rectangle is remembered, for the next frame to erase.
void UiWindow_DrawZoomFrame(void *windowArg, s32 savePosition) {
  struct UiWindow *window = windowArg;
  struct UiWindowInterpolationScratch scratch;
  s32 frame;
  s32 duration;
  s32 remaining;
  s32 x;
  s32 y;
  s32 width;
  s32 height;

  frame = (s16)window->frame;
  duration = window->duration;
  remaining = duration - frame;
  scratch.scaledPart = (frame * window->width) << 16;
  scratch.scaledDuration = duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  x = (scratch.result >> 16) + window->x;

  scratch.scaledPart = (remaining * window->width) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  width = scratch.result >> 15;

  scratch.scaledPart = (frame * window->height) << 16;
  scratch.scaledDuration = window->duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  y = (scratch.result >> 16) + window->y;

  scratch.scaledPart = (remaining * window->height) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  height = scratch.result >> 15;

  UiWindow_DrawFrame(x, y, width, height);
  if (savePosition != 0) {
    window->previousX = x;
    window->previousY = y;
    window->previousWidth = width;
    window->previousHeight = height;
  }
}

// Fills count halfwords at dst with value by DMA, and returns the position
// after them. value holds the halfword in both its halves.
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count) {
  if (count > 0) {
    Dma_Fill16(value, dst, count * 2);
    dst += count;
  }
  return dst;
}

// Draws an empty window: a frame around a filled interior, at tile (x, y) of
// width by height tiles. Sizes outside 2 to 30 tiles draw nothing.
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height) {
  struct UiRenderWork *canvas = UI_WORK;
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
  u32 row;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  UiWindow_ClearTileAttributesInRect(x, y, width, height);
  // The top edge.
  if (canvas->mode != 0)
    *cursor++ = UI_TILE_FRAME_ROUND_CORNER;
  else
    *cursor++ = UI_TILE_FRAME_TOP_LEFT;
  cursor = Memory_FillHalfwordsDma(cursor, UI_TILE_PAIR(UI_TILE_FRAME_TOP),
                                   width - 2);
  if (canvas->mode != 0)
    *cursor++ = UI_TILE_FRAME_ROUND_CORNER | TILE_HFLIP;
  else
    *cursor++ = UI_TILE_FRAME_TOP_RIGHT;
  cursor += 32 - width;
  // The sides and the fill between them.
  for (row = 1; row < height - 1; row++) {
    *cursor++ = UI_TILE_FRAME_LEFT;
    if (width != 2)
      cursor = Memory_FillHalfwordsDma(
          cursor, UI_TILE_PAIR(UI_TILE_WINDOW_FILL), width - 2);
    *cursor++ = UI_TILE_FRAME_RIGHT;
    cursor += 32 - width;
  }
  // The bottom edge.
  if (canvas->mode != 0)
    *cursor++ = UI_TILE_FRAME_ROUND_CORNER | TILE_VFLIP;
  else
    *cursor++ = UI_TILE_FRAME_BOTTOM_LEFT;
  cursor = Memory_FillHalfwordsDma(cursor, UI_TILE_PAIR(UI_TILE_FRAME_BOTTOM),
                                   width - 2);
  if (canvas->mode != 0)
    *cursor = UI_TILE_FRAME_ROUND_CORNER | TILE_HFLIP | TILE_VFLIP;
  else
    *cursor = UI_TILE_FRAME_BOTTOM_RIGHT;
  canvas->dirty = 1;
}

// Maps the text canvas tiles into the interior of a window at tile (x, y).
// With fullWidth set the canvas covers the side frame columns too.
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
                                 s32 fullWidth) {
  struct UiRenderWork *canvas = UI_WORK;
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
  u32 row;
  u32 col;

  if (width <= 1 || height <= 1 || width > 30 || height > 30)
    return;
  // Canvas tiles run down each column of the interior.
  cursor += 32;
  if (fullWidth == 0) {
    for (row = 1; row < height - 1; row++) {
      cursor++;
      for (col = 1; col < width - 1; col++)
        *cursor++ = UI_TILE(
            (UI_CANVAS_FIRST_TILE - 1 + row + (col - 1) * (height - 2)) &
            0xfff);
      cursor++;
      cursor += 32 - width;
    }
  } else {
    for (row = 1; row < height - 1; row++) {
      for (col = 0; col < width; col++)
        *cursor++ = UI_TILE(
            (UI_CANVAS_FIRST_TILE - 1 + row + col * (height - 2)) & 0xfff);
      cursor += 32 - width;
    }
  }
  canvas->dirty = 1;
}

// Returns 1 when every message channel has finished its text (or is idle),
// and 0 while one is still typing.
s32 UiWork_IsComplete(void) {
  struct UiChannelSlot *ch;
  s32 i;

  ch = UI_WORK->channels;
  for (i = 0; i != 3; i++, ch++) {
    if (ch->work != NULL && ch->work->state == 0)
      return 0;
  }
  return 1;
}

// Returns 1 when a window is fully closed (its slot free), and 0 while it is
// open or closing.
s32 UiWork_IsIdle(struct UiWindow *window) {
#if !TBS_PATCH_1
  if (window == NULL)
    return 1;
#endif
  if (window->flags == 0 && window->duration == 0)
    return 1;
  return 0;
}

// Resets the text style glyphs are drawn with: colour 15, no outline, line
// spacing 10.
void UiWork_ResetCounters(void) {
  struct UiRenderWork *state = UI_WORK;

  state->colour = 15;
  state->lineSpacing = 10;
  state->letterSpacing = 9;
  state->outline = 0;
  state->unknownAfterLineSpacing = 1;
}

// Loads the glyph tiles, resets the text style and starts the per-frame
// window update (UiWork_ProcessAll).
void UiWork_InitCountersWithResourceAndScheduleRefresh(void) {
  struct UiRenderWork *state = UI_WORK;
  s32 priority;

  state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->letterSpacing = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  priority = 200;
  priority <<= 4;
  Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, priority);
}

// Resets the text style and starts the per-frame window update
// (UiWork_ProcessAll), loading the glyph tiles first when loadGlyphTiles is
// set.
void UiWork_InitCountersAndScheduleRefresh(s32 loadGlyphTiles) {
  struct UiRenderWork *state = UI_WORK;
  s32 priority;

  if (loadGlyphTiles != 0)
    state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->letterSpacing = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  priority = 200;
  priority <<= 4;
  Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, priority);
}

// Closes the battle message window, if one is open.
void UiWork_FinalizeSharedSlot(void) {
  struct BattleDisplayWork *display = gBattleDisplayWork;
  struct UiWindow *window = display->window;

  if (window != NULL) {
    UiWork_Finalize(window, 1);
    display->window = NULL;
  }
}

// Shows a message in the battle message window, opening the window (a
// full-width text canvas along the bottom of the screen) if it is not open.
void UiText_PrepareMessageWork(s32 argument) {
  s32 entryIndex;
  struct UiChannelSlot *channel;
  struct UiWindow *previousWindow;
  struct UiWindow *messageWindow;
  struct UiRenderWork *state;
  struct BattleDisplayWork *control;

  state = UI_WORK;
  control = (struct BattleDisplayWork *)
      gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW];
  // result starts as NULL, though it is assigned again before it is read; the
  // same zero clears marked below
  channel = NULL;
  state->menuState = 2;
  entryIndex = UiText_BuildRenderEntries(argument, 1);
  state->menuState = 1;

  if (state->entries[entryIndex] != 0) {
    previousWindow = control->window;
    if (previousWindow == NULL) {
      messageWindow = UiWindow_Create(
          0, 15, 30, 6, UI_WINDOW_INSTANT | UI_WINDOW_TEXT_CANVAS);
      control->window = messageWindow;
      UiWindow_MapTextCanvasTiles(0, 15, 30, 6, 1);
      control->marked = 0;
    } else {
      messageWindow = previousWindow;
    }
    if (messageWindow != NULL) {
      channel =
          UiText_StartBattleMessage(messageWindow, entryIndex, control->marked);
      control->offset = (struct BattleDisplayOffset *)channel;
      control->marked = 0;
      if (channel == NULL) {
        UiWork_Finalize(messageWindow, 1);
      }
    }
  }
}

// Shows a message in the battle message window and waits until it has been
// typed out.
void UiText_ShowMessageAndWaitCore(s32 argument) {
  UiText_PrepareMessageWork(argument);
  while (UiWork_IsComplete() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

// Types a message into an open window from its top
// left. Returns the channel typing it (as an integer), or 0 when the message
// is empty, window is NULL or no channel is free. The result is the channel
// pointer encoded as a word, not a message-completion status.
s32 UiText_OpenEntryMessage(struct UiWindow *window, s32 message) {
  struct UiRenderWork *work = UI_WORK;
  s32 entry;
  s32 channel = 0;
  /* FAKEMATCH: never read: the native frame keeps 8 bytes above the two
   * outgoing arguments, exactly these four slots; without them the frame is 8
   * bytes, not 16 (differ 4 in tbs-en). */
  u16 frameReserve[4];

  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  entry = UiText_BuildRenderEntries(message, 1);
  if (work->entries[entry] == 0)
    return 0;
  if (window == 0)
    return 0;
  channel = (s32)UiText_QueueRenderEntries(window, entry, 0, 0, 0, 1);
  if (channel == 0)
    return 0;
  return channel;
}

// Sets the effects the next message is decoded with
// (UiText_BuildRenderEntries): flags bit 0 (spacingBefore) makes it halting
// speech, a short pause before each character, broken off with an ellipsis
// after the first word; bit 1 (spacingAfter) puts code 222 after certain
// characters in English and Japanese. UiText_OpenMessageWindow clears both.
void UiWork_SetBusyFlags(s32 flags) {
  struct UiRenderWork *work;

  work = UI_WORK;
  if (work != NULL) {
    if (flags & 1) {
      work->spacingBefore = 1;
    }
    if (2 & flags) {
      work->spacingAfter = 1;
    }
  }
}

// Opens a window sized to a message near tile (x, y) and starts typing the
// message into it. packed holds the window options in its low bits (bit 0:
// zoom open rather than appear at once; bit 3: a text canvas; bit 4: keep the
// bottom edge; bit 5: keep the left edge) and the speaker's voice sound in
// bits 16 to 27. Returns the window, or NULL when the message is empty or no
// window or channel is free.
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y,
                                          u32 packed) {
  s32 entry;
  u32 height;
  u32 width;
  u16 bounds[4];
  s32 flags;
  u16 *out;
  struct UiWindow *window;
  struct UiRenderWork *work;

  work = UI_WORK;
  work->speakerVoiceSound = (packed << 4) >> 20;
  // the window starts as NULL, though it is assigned before it is read; the
  // same zero serves the zeros below
  window = NULL;
  work->speakerVoiceCountdown = 0;
  packed &= 0xffff;
  flags = 0;
  entry = UiText_BuildRenderEntries(message, 1);
  if (work->entries[entry] == 0)
    return NULL;
  UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
  if (width == 0 && height == 0)
    return NULL;
  if (!(packed & 1))
    flags |= UI_WINDOW_INSTANT;
  if (packed & 8)
    flags |= UI_WINDOW_TEXT_CANVAS;
  if (packed & 16)
    flags |= UI_WINDOW_ANCHOR_BOTTOM;
  if (packed & 32)
    flags |= UI_WINDOW_FIXED_X;
  window = UiWindow_Create(x, y, width, height, flags);
  if (window == NULL)
    return NULL;
  if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
    UiWork_Finalize(window, 1);
    return NULL;
  }
  work->spacingBefore = 0;
  work->spacingAfter = 0;
  return window;
}

// Shows a message in a window centred on the screen, waits until it is typed
// out and closes it. flags: bit 0 zooms the window, bit 1 sets busy while it
// shows, bit 2 leaves the window open, bit 3 puts it 4 tiles lower, bit 5
// holds the tilemap upload off afterwards, bit 6 puts it 12 tiles lower.
// Without bit 3 or 6 the window goes above or below the player's character,
// away from where it stands on screen.
void UiText_ShowPositionedMessageAndWait(s32 message, s32 flags) {
  struct UiRenderWork *state = UI_WORK;
  s32 zoom;
  s32 actorPosition[2] = {0, 0};
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  struct UiWindow *window = NULL;
  s32 zero;

  y = x = 0;
  zoom = flags & 1;

  if (flags & 2) {
    state->busy = 1;
  }

  UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width, (u32 *)&height);

  // Centre the window across the screen, and in the top 12 rows.
  x = (30 - width) >> 1;
  y = (12 - height) >> 1;

  if (flags & 8) {
    y += 4;
  } else if (flags & 0x40) {
    y += 12;
  } else {
    s32 actorTileY;

    Object_GetScreenPositionFar(gGameState.selectedActor, actorPosition);
    actorTileY = actorPosition[1] >> 3;
    if (actorTileY > 9) {
      y = actorTileY - 5;
    } else {
      y = actorTileY + 4;
    }
  }

  window = UiText_OpenMessageWindow(message, x, y, zoom);

  if (window != NULL) {
    while (UiWork_IsComplete() == 0) {
      WaitFrames(1);
    }

    if (flags & 0x20) {
      UI_WORK->menuBusy = 1;
    }

    if (!(flags & 4)) {
      UiWork_Finalize(window, zoom);
      while (UiWork_IsIdle(window) == 0) {
        WaitFrames(1);
      }
    }
  }

  zero = 0;
  state->busy = zero;
  state->speakerVoiceSound = zero;
  state->speakerVoiceCountdown = zero;
  WaitFrames(3);
}

// The window system's per-frame callback: windows, then message channels,
// then sprite animation.
void UiWork_ProcessAll(void) {
  UiWork_ProcessDirectWork();
  UiWork_ProcessRenderChannels();
  UiWork_AnimateSpriteSlots();
}
