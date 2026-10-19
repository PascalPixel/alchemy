/// @file
/// The render output pool, opening and closing windows, starting messages.
/// Part of window.c, the window object, compiled only through it.

#include "../window_internal.h"

/// Takes the first render output from the free list, or returns NULL when every
/// output is in use.
struct RenderOutput *RenderOutput_AcquireFree(void) {
  struct UiRenderWork *work = UI_WORK;
  struct RenderOutput *out = work->freeOutputs.head;

  if (out != NULL) {
    // Taking the last output leaves the list empty.
    if (out->next == NULL)
      work->freeOutputs.tailLink = &work->freeOutputs.head;
    work->freeOutputs.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Returns a render output to the end of the free list. An output from outside
/// the work block's pool is ignored.
void RenderOutput_ReleaseFree(struct RenderOutput *out) {
  struct UiRenderWork *work = UI_WORK;

  if ((u32)out >= (u32)work->outputs &&
      (u32)out < (u32)&work->freeOutputs.head) {
    struct RenderOutput **tail = work->freeOutputs.tailLink;

    work->freeOutputs.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Links all 64 render outputs of the pool into the free list.
void RenderOutput_InitFreeList(void) {
  s32 remainingLinks;
  struct UiRenderWork *work;
  struct RenderOutput *out;
  struct RenderOutput *next;

  work = UI_WORK;
  out = work->outputs;
  work->freeOutputs.head = out;
  remainingLinks = UI_OUTPUT_COUNT - 2;
  do {
    next = out + 1;
    remainingLinks--;
    out->next = next;
    out = next;
  } while (remainingLinks >= 0);
  next->next = NULL;
  work->freeOutputs.tailLink = &next->next;
}

/// Sets up the window system for the field (and the other screens outside
/// battle): allocates and clears the work block, blanks the tilemap, frees
/// every render output and starts the two per-frame callbacks, the tilemap
/// upload and the window update.
void UiWork_InitializeWithResourceCounters(void) {
  struct UiRenderWork *work;
#if TBS_PATCH_1
  volatile u32 fill;
#endif

  // Clear the work block before linking any of its embedded lists.
  work = Runtime_AllocateBlock(HEAP_SLOT_WINDOW, RENDER_WORK_SIZE);
#if TBS_PATCH_1
  fill = 0;
  Dma_Set((const void *)&fill, work, DMA_FILL32(RENDER_WORK_SIZE / 4),
          REG_DMA3);
#else
  Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
  work->dirty = 1;
  work->glyphResource = NO_GLYPH_RESOURCE;
  work->level = UI_TILEMAP_PALETTE;
#if TBS_PATCH_1
  fill = UI_TILE_PAIR(UI_TILE_BLANK);
  Dma_Set((const void *)&fill, work, DMA_FILL32(sizeof(work->tilemap) / 4),
          REG_DMA3);
#else
  Iwram_FillWords(work, sizeof(work->tilemap), UI_TILE_PAIR(UI_TILE_BLANK));
#endif
  // Make the output and palette pools available, then start the updates.
  RenderOutput_InitFreeList();
  UiWork_ClearPortraitSlots();
  Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
  UiWork_InitCountersWithResourceAndScheduleRefresh();
}

/// Sets up the window system for battle, as
/// UiWork_InitializeWithResourceCounters does for the field, and marks the work
/// block as in battle (menuState 1). loadGlyphTiles is set only when the Djinn
/// tutorial flag is set: it loads the glyph tiles the tutorial's messages use.
void UiWork_Initialize(s32 loadGlyphTiles) {
  struct UiRenderWork *work;
#if TBS_PATCH_1
  volatile u32 fill;
#endif
  s32 tileIndex;

  // Copies pixel rows 4 to 6 of UI tile src to rows 0 to 2 of tile dst and
  // clears the rest of dst.
  s32 UiWork_CopyTileTail(u32 sourceTile, u32 destinationTile) {
    sourceTile &= 0x3ff;
    destinationTile &= 0x3ff;
    Dma_Set(BG_CHAR_BLOCK(0) + sourceTile * TILE_SIZE_4BPP + 16,
            BG_CHAR_BLOCK(0) + destinationTile * TILE_SIZE_4BPP, DMA_COPY16(8),
            REG_DMA3);
    return Iwram_ClearWords(
        BG_CHAR_BLOCK(0) + destinationTile * TILE_SIZE_4BPP + 12, 20);
  }

  // Battle windows share the field's pools but use the canvas tile set.
  work = (struct UiRenderWork *)Runtime_AllocateBlock(HEAP_SLOT_WINDOW,
                                                      RENDER_WORK_SIZE);
#if TBS_PATCH_1
  fill = 0;
  Dma_Set(&fill, work, DMA_FILL32(RENDER_WORK_SIZE / 4), REG_DMA3);
#else
  Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
  work->dirty = 1;
  work->glyphResource = NO_GLYPH_RESOURCE;
  work->menuState = 1;
  work->level = UI_TILEMAP_PALETTE;
#if TBS_PATCH_1
  fill = UI_TILE_PAIR(UI_TILE_BLANK);
  Dma_Set(&fill, work, DMA_FILL32(sizeof(work->tilemap) / 4), REG_DMA3);
#else
  Iwram_FillWords(work, sizeof(work->tilemap), UI_TILE_PAIR(UI_TILE_BLANK));
#endif
  RenderOutput_InitFreeList();
  Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
  UiWork_InitCountersAndScheduleRefresh(loadGlyphTiles);
  // Tiles 128 to 130 take the lower rows of the bottom frame tiles.
  UiWork_CopyTileTail(UI_TILE_FRAME_BOTTOM_LEFT, 128);
  UiWork_CopyTileTail(UI_TILE_FRAME_BOTTOM, 129);
  UiWork_CopyTileTail(UI_TILE_FRAME_BOTTOM_RIGHT, 130);
  {
    u8 mode = 4;

    for (tileIndex = 2; tileIndex >= 0; tileIndex--)
      work->tileAttributes[tileIndex] = mode;
  }
}

/// The per-frame tilemap upload: copies each 256-byte band of the tilemap
/// (four tile rows) marked in the dirty bits to VRAM. Bit 0 marks the whole
/// tilemap, bits 1 to 5 one band each. Nothing is copied while menuBusy holds
/// the upload off.
void UiWork_UploadDirtyBlocks(void) {
  struct UiRenderWork *work = UI_WORK;
  u32 bands;
  u8 *src;
  u8 *dst;
  if (!work->menuBusy) {
    bands = work->dirty;
    if (bands) {
      dst = UI_TILEMAP_VRAM;
      src = (u8 *)work->tilemap;
      if (bands & 1)
        bands = 63;
      bands &= 63;
      bands >>= 1;
      do {
        if (bands & 1)
          Dma_Set(src, dst, DMA_COPY32(64), REG_DMA3);
        bands >>= 1;
        src += 256;
        dst += 256;
      } while (bands);
      work->dirty = bands;
    }
  }
}

/// Erases a window's rectangle of the tilemap (x, y, width and height in
/// tiles), clamped to the screen. In battle the rows below row 16 take tile
/// 0x7f instead of the blank tile.
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height) {
  struct UiRenderWork *canvas = UI_WORK;
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
  s32 tile;
  u32 bottom;
  u32 row;
  u8 *inBattle;

  tile = 240;
  bottom = y + height;
  tile <<= 8;
  if (bottom > 20)
    height = 20 - y;
  if (width <= 1)
    width = 2;
  if (width > 30)
    width = 30;
  if (height <= 1)
    height = 2;
  if (height > 30)
    height = 30;

  // Clear glyph ownership before replacing the corresponding tile entries.
  UiWindow_ClearTileAttributesInRect(x, y, width, height);

  row = 0;
  if (row < height) {
    inBattle = &canvas->menuState;
    do {
      u32 column;

      if (*inBattle != 0) {
        if ((y + row) > 16)
          tile = UI_TILE(0x7f);
        else
          tile = UI_TILE_BLANK;
      }
      column = 0;
      if (column < width) {
        do {
          *cursor++ = tile;
          column++;
        } while (column < width);
      }
      row++;
      cursor += 32 - width;
    } while (row < height);
  }
  canvas->dirty = 1;
}

/// Draws a window at its full size: its frame, and for a text canvas window
/// the canvas, filled with colour 4 under a frame when the window is framed and
/// transparent otherwise.
void UiWindow_DrawOpened(struct UiWindow *window) {
  u32 flags;
  u32 value;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  struct UiRenderWork *work;

  work = UI_WORK;
  value = window->height;
  flags = window->flags;
  height = value;
  value = 0;
  window->duration = value;
  x = window->x;
  y = window->y;
  width = window->width;
  // Canvas windows need both pixel storage and their tilemap rectangle.
  if (UI_WINDOW_TEXT_CANVAS & flags) {
    if (UI_WINDOW_FRAMED_CANVAS & flags) {
      UiWindow_DrawFrame(x, y, width, height);
      Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE,
                      UI_CANVAS_BACKGROUND_WORD);
    } else {
      Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE, 0);
    }
    UiWindow_MapTextCanvasTiles(x, y, width, height, 0);
  } else {
    UiWindow_DrawFrame(x, y, width, height);
  }
  work->dirty = 1;
}

/// Opens a window at tile (x, y) of width by height tiles in the first free
/// window slot, and returns it, or NULL when all slots are in use. attrs takes
/// the UI_WINDOW_ flags. Unless it is UI_WINDOW_INSTANT the window zooms open
/// over eight frames, and this waits until it is fully open.
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs) {
  struct UiWindow *slot;
  struct UiWindow *found;
  s32 i;

  slot = UI_WORK->windows;
  found = 0;
  // A closing slot remains occupied until its animation has finished.
  for (i = 0; i != WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & UI_WINDOW_OPEN) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
    // Reset the slot before applying the caller's canvas and anchor flags.
    found->y = y;
    found->width = width;
    found->height = height;
    found->x = x;
    found->output.head = NULL;
    found->state = 0;
    found->output.tailLink = &slot->output.head;
    found->unk10 = 1;
    found->flags = UI_WINDOW_OPEN;
    UiWork_ResetCounters(x);
    if (attrs & UI_WINDOW_TEXT_CANVAS) {
      found->flags |= UI_WINDOW_TEXT_CANVAS;
    }
    if (attrs & UI_WINDOW_FRAMED_CANVAS) {
      found->flags |= UI_WINDOW_FRAMED_CANVAS;
    }
    if (attrs & UI_WINDOW_OUTLINED_TEXT) {
      found->flags |= UI_WINDOW_OUTLINED_TEXT;
    }
    if (attrs & UI_WINDOW_ANCHOR_BOTTOM) {
      found->flags |= UI_WINDOW_ANCHOR_BOTTOM;
    }
    if (attrs & UI_WINDOW_FIXED_X) {
      found->flags |= UI_WINDOW_FIXED_X;
    }
    if (attrs & UI_WINDOW_INSTANT) {
      found->flags |= UI_WINDOW_INSTANT;
      found->frame = 0;
      found->duration = 1;
      UiWindow_DrawOpened(found);
    } else {
      // Zoom open over eight frames, and wait until it is open.
      found->duration = UI_WINDOW_OPEN_FRAMES;
      found->frame = UI_WINDOW_OPEN_FRAMES - 1;
      UiWindow_WaitZoom(found);
      WaitFrames(1);
    }
  }
  return found;
}

/// Waits until a zooming window has finished opening or closing.
void UiWindow_WaitZoom(struct UiWindow *window) {
  if (!(UI_WINDOW_INSTANT & window->flags) && window->duration != 0) {
    do {
      WaitFrames(1);
    } while (window->duration != 0);
  }
}

/// Closes a window: clears its contents and remembers its last rectangle.
/// With instant set the window is erased and its slot freed at once; otherwise
/// it zooms closed over four frames first. Does nothing for NULL.
void UiWork_Finalize(struct UiWindow *window, s32 instant) {
  u16 zero;

  if (window == 0)
    return;

  RenderOutput_PrepareForRedraw(window);
  // Keep the old rectangle available while the closing animation shrinks it.
  window->previousX = window->x;
  window->previousY = window->y;
  window->previousWidth = window->width;
  zero = 0;
  window->flags = zero;
  window->previousHeight = window->height;

  if (instant != 0) {
    UiWindow_EraseBorderRect(window->x, window->y, window->width,
                             window->height);
    window->output.head = NULL;
    window->output.tailLink = NULL;
    window->width = zero;
    window->height = zero;
    window->x = zero;
    window->y = zero;
    window->unk10 = zero;
    window->effect = zero;
    window->state = zero;
    window->flags = zero;
    window->frame = zero;
    window->duration = zero;
    window->previousX = zero;
    window->previousY = zero;
    window->previousWidth = zero;
    window->previousHeight = zero;
  } else {
    // Zoom closed over four frames (UiWork_ProcessDirectWork).
    window->frame = instant;
    window->duration = UI_WINDOW_CLOSE_FRAMES;
  }
}

/// Clears a window's contents: redraws its empty frame and releases the render
/// outputs it carries. A text canvas window is left alone.
void RenderOutput_PrepareForRedraw(struct UiWindow *window) {
  if (!(UI_WINDOW_TEXT_CANVAS & window->flags)) {
    RenderOutput_RedrawSavedRect(window);
    RenderOutput_ClearList(window);
  }
}

/// Redraws a window's empty frame at its current rectangle.
void RenderOutput_RedrawSavedRect(struct UiWindow *window) {
  UiWindow_DrawFrame(window->x, window->y, window->width, window->height);
}

/// Releases every render output in a list and empties it. outputs is the list,
/// or a window (whose output list comes first); NULL is ignored.
void RenderOutput_ClearList(void *outputs) {
  struct RenderOutputList *list = outputs;
  struct RenderOutput *node;
  struct RenderOutput *next;

  next = NULL;
  if (list != NULL) {
    node = list->head;
    // Detach the chain before returning its nodes to the shared free pool.
    list->tailLink = &list->head;
    list->head = next;
    while (node != NULL) {
      next = node->next;
      RenderOutput_Release(node);
      node = next;
    }
  }
}

/// Clears an area of a window's interior to the window fill. left, top, right
/// and bottom are pixels inside the window, and the area covers every tile
/// they touch.
void UiWindow_ClearInteriorTiles(const struct UiWindow *window, u32 left,
                                 u32 top, u32 right, u32 bottom) {
  struct UiRenderWork *work;
  u32 x;
  u32 y;
  u32 width;
  u32 height;
  u16 *tiles;
  u32 row;

  right += 7;
  bottom += 7;
  right >>= 3;
  bottom >>= 3;
  x = left >> 3;
  work = UI_WORK;
  y = top >> 3;
  x += window->x;
  y += window->y;
  right += window->x;
  bottom += window->y;
  left = x + 1;
  top = y + 1;
  width = right - x;
  height = bottom - y;
  UiWindow_ClearTileAttributesInRect(left, top, width, height);
  tiles = work->tilemap + top * 32 + left;
  for (row = 0; row < height; row++) {
    for (left = 0; left < width; left++)
      *tiles++ = UI_TILE_WINDOW_FILL;
    tiles += 32 - width;
  }
  work->dirty = 1;
}

/// Points a list's tail link back at its last output, after the list has been
/// edited by hand.
/// Nothing calls it.
UNUSED void RenderOutputList_UpdateTail(struct RenderOutputList *list) {
  struct RenderOutput *node = list->head;
  struct RenderOutput **tail = &list->head;

  while (node != NULL) {
    tail = &node->next;
    node = node->next;
  }
  list->tailLink = tail;
}

/// Adds a render output to the end of a list; NULL lists are ignored.
void RenderOutput_AppendToList(struct RenderOutputList *list,
                               struct RenderOutput *node) {
  /* FAKEMATCH: storing the new tail as a RenderOutput * keeps both stores in
   * one alias set and in order; as list->tailLink = &node->next, 2
   * instructions differ in tbs-en. */
  if (list != NULL) {
    *list->tailLink = node;
#ifdef UBFIX
    // Store the address of next through the tail link's actual pointer type.
    list->tailLink = &node->next;
#else
    *(struct RenderOutput **)&list->tailLink = node;
#endif
  }
}

/// Frees a render output: returns it to the free list, releases its resource,
/// and for a kind-2 output frees its palette slot too.
void RenderOutput_Release(struct RenderOutput *entry) {
  RenderOutput_ReleaseFree(entry);
  if ((u8)entry->kind != 0) {
    Resource_ResetEntry((u8)entry->index);
    if ((u8)entry->kind == 2) {
      // Free its palette slot (999 marks a free slot).
      u8 *work = gWindowWork[0];
      s32 offset = entry->oam.palette * 2 + RENDER_PALETTE_TABLE_OFFSET;
      *(u16 *)(work + offset) = UI_PALETTE_SLOT_FREE;
    }
  }
  entry->active = 0;
}

/// Starts a free message channel typing the decoded text that begins at entry
/// into window, with the pen at pixel (x, y). spacing gives each of four lines
/// an extra gap between characters (or none for NULL); flags is kept with the
/// channel (nonzero keeps the window's size from page to page). Returns the
/// channel, or NULL when all three are busy.
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window,
                                                s32 entry, s32 x, s32 y,
                                                u16 *spacing, s32 flags) {
  struct UiChannelSlot *slot = UI_WORK->channels;
  struct UiChannelSlot *found = NULL;
  u32 i;

  for (i = 0; i != UI_MESSAGE_CHANNEL_COUNT; i++, slot++) {
    if (slot->work == NULL) {
      found = slot;
      break;
    }
  }
  if (found != NULL) {
    // The channel stores pen coordinates in 1/256 pixels.
    found->work = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->entry = entry;
    found->colour = UI_TEXT_DEFAULT_COLOUR;
    found->lineSpacing = UI_TEXT_DEFAULT_LINE_SPACING;
    found->countdown = 0;
    found->outline = 0;
    found->ended = 0;
    found->flags = flags;
    if (spacing != NULL) {
      for (i = 0; i < UI_MESSAGE_LINE_COUNT; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < UI_MESSAGE_LINE_COUNT; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}

/// Starts a channel typing the decoded text at entry into the battle message
/// window. A channel that already serves a finished window continues below its
/// last line (scrolling up once both lines are used) unless keepPosition is
/// set. Returns the channel, or NULL when all three are busy.
struct UiChannelSlot *UiText_StartBattleMessage(struct UiWindow *window,
                                                s32 entry, s32 keepPosition) {
  struct UiChannelSlot *slot;
  struct UiChannelSlot *selected;
  u16 *spacing;
  u16 zero;
  u32 index;

  slot = UI_WORK->channels;
  selected = 0;
  for (index = 0; index != UI_MESSAGE_CHANNEL_COUNT; slot++, index++) {
    if (slot->work == 0 || slot->work->state != 0) {
      selected = slot;
      break;
    }
  }

  if (selected != 0) {
    // Positions are in 1/256 pixel: the first line is 10 pixels down, the
    // second 13 pixels below it, and after the second the text scrolls up.
    if (selected->work == 0) {
      selected->y = 10 << 8;
      selected->work = window;
      selected->x = 3 << 8;
    } else if (keepPosition == 0) {
      if (selected->y == 0) {
        selected->y = 10 << 8;
      } else if (selected->y < 13 << 8) {
        selected->y += 13 << 8;
      } else {
        UiText_StartCanvasScroll(selected);
      }
      selected->x = 3 << 8;
    }

    zero = 0;
    selected->startX = 3 << 8;
    selected->work->state = zero;
    // Start the next text run without retaining the previous run's style.
    selected->colour = UI_TEXT_DEFAULT_COLOUR;
    selected->lineSpacing = UI_TEXT_DEFAULT_LINE_SPACING;
    selected->entry = entry;
    selected->countdown = zero;
    selected->outline = zero;
    selected->line = zero;
    selected->ended = zero;
    index = 0;
    spacing = selected->spacing;
    do {
      index++;
      *spacing++ = zero;
    } while (index < UI_MESSAGE_LINE_COUNT);
  }
  return selected;
}

/// Clears the text canvas to transparent.
s32 Ui_ClearVramBlock(void) {
  return Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE, 0);
}

/// Fills the text canvas with the window background colour (4).
s32 Ui_FillVramBlockPattern(void) {
  return Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE,
                         UI_CANVAS_BACKGROUND_WORD);
}

/// Resets the pen and text style of the first idle message channel, clearing
/// the canvas when the channel still has a window.
void UiWork_ResetFreeChannel(void) {
  struct UiChannelSlot *slot = UI_WORK->channels;
  struct UiChannelSlot *selection = 0;
  s32 i;

  for (i = 0; i != UI_MESSAGE_CHANNEL_COUNT; slot++, i++) {
    if (slot->work == 0 || slot->work->state != 0) {
      selection = slot;
      break;
    }
  }
  if (selection != 0) {
    if (selection->work != 0) {
      Ui_ClearVramBlock();
      selection->y = 0;
    }
    selection->x = 0;
    selection->countdown = 0;
    selection->colour = UI_TEXT_DEFAULT_COLOUR;
    selection->outline = 0;
    selection->lineSpacing = UI_TEXT_DEFAULT_LINE_SPACING;
  }
}

/// Copies the channel's text colour, outline and line spacing into the shared
/// glyph renderer before that channel draws its next character.
void UiText_ApplyChannelStyle(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = UI_WORK;

  work->colour = channel->colour;
  work->outline = channel->outline;
  work->lineSpacing = channel->lineSpacing;
}

/// Starts a channel scrolling its canvas up a line, over two frames.
void UiText_StartCanvasScroll(struct UiChannelSlot *channel) {
  channel->transition = 2;
}
