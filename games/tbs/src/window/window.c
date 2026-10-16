// The UI window system: windows drawn into a shared BG tilemap, the message
// channels that type text into them, the text and number formatting behind
// messages, the sprites (render outputs) that windows carry, icons, the
// selection screens and the option and system menus.
//
// Everything lives in one work block, struct UiRenderWork (heap slot
// HEAP_SLOT_WINDOW, reached as UI_WORK): the tilemap, the window slots, three
// message channels, a pool of 64 render outputs and the decoded text.

#include "unit/window/scales.h"
#include "unit/window/articles.h"
#include "unit/window/text_speed.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/glyph_arm.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "battle_work.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "battle_party.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "field_event.h"
#include "b5_context.h"
#include "battle_type.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resource_menu.h"
#include "battle_summon.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "display_transition.h"
#include "frame.h"

#ifdef MODERN
// In the matching build these calls precede their declarations. Prototypes
// reorder three argument moves for ClearTileAttributesInRect and two for
// PutGlyph in the English image.
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode);
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
#endif

// The UI tilemap in VRAM (BG screen block 4), which UiWork_UploadDirtyBlocks
// copies the work block's tilemap to.
#define UI_TILEMAP_VRAM ((u8 *)0x06002000)

// A tilemap entry of palette 15, the UI palette.
#define UI_TILE(tile) (0xf000 | (tile))
// Two copies of a tilemap entry, for filling a run of entries a word at a time.
#define UI_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

// The window frame's tiles. While UiRenderWork.mode is set (a battle effect
// sets it while it greys palette 15 and scrolls BG0 by scanline) the corners
// are one rounded tile, flipped for each corner.
enum {
  UI_TILE_BLANK = UI_TILE(0x00),
  UI_TILE_FRAME_TOP_LEFT = UI_TILE(0x10),
  UI_TILE_FRAME_TOP = UI_TILE(0x11),
  UI_TILE_FRAME_TOP_RIGHT = UI_TILE(0x12),
  UI_TILE_FRAME_BOTTOM_LEFT = UI_TILE(0x13),
  UI_TILE_FRAME_BOTTOM = UI_TILE(0x14),
  UI_TILE_FRAME_BOTTOM_RIGHT = UI_TILE(0x15),
  UI_TILE_FRAME_LEFT = UI_TILE(0x16),
  UI_TILE_FRAME_RIGHT = UI_TILE(0x17),
  UI_TILE_FRAME_ROUND_CORNER = UI_TILE(0x1c),
  UI_TILE_WINDOW_FILL = UI_TILE(0x20),
  // Divider lines (UiWindow_DrawDividerLine) and their joins.
  UI_TILE_DIVIDER_TEE_DOWN = UI_TILE(0x09),  // a vertical line leaving below
  UI_TILE_DIVIDER_TEE_UP = UI_TILE(0x0a),    // a vertical line leaving above
  UI_TILE_DIVIDER_TEE_RIGHT = UI_TILE(0x0b), // a horizontal line leaving right
  UI_TILE_DIVIDER_TEE_LEFT = UI_TILE(0x0c),  // a horizontal line leaving left
  UI_TILE_DIVIDER_CROSS = UI_TILE(0x0d),
  UI_TILE_DIVIDER_HORIZONTAL = UI_TILE(0x0e),
  UI_TILE_DIVIDER_VERTICAL = UI_TILE(0x0f),
  UI_TILE_FRAME_TOP_TEE = UI_TILE(0x18),    // the top edge meeting a line
  UI_TILE_FRAME_BOTTOM_TEE = UI_TILE(0x19), // the bottom edge meeting a line
  UI_TILE_FRAME_LEFT_TEE = UI_TILE(0x1a),   // the left edge meeting a line
  UI_TILE_FRAME_RIGHT_TEE = UI_TILE(0x1b),  // the right edge meeting a line
};

// The tilemap entry flip bits.
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800

// The DMA control word that fills count words from one fixed source word.
#define DMA_FILL32(count) (DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (count))
// The DMA control word that copies count words.
#define DMA_COPY32(count) (DMA_ENABLE32 | DMA_32BIT | (count))
// The DMA control word that copies count halfwords.
#define DMA_COPY16(count) (DMA_ENABLE32 | (count))

// A glyph resource id meaning no glyph resource is loaded.
#define NO_GLYPH_RESOURCE 99

// Takes the first render output from the free list, or returns NULL when every
// output is in use.
struct RenderOutput *RenderOutput_AcquireFree(void) {
  struct UiRenderWork *work = UI_WORK;
  struct RenderOutput *out = work->freeOutputs.head;

  if (out != NULL) {
    if (out->next == NULL) // the list is now empty
      work->freeOutputs.tailLink = &work->freeOutputs.head;
    work->freeOutputs.head = out->next;
    out->next = NULL;
  }
  return out;
}

// Returns a render output to the end of the free list. An output from outside
// the work block's pool is ignored.
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

// Links all 64 render outputs of the pool into the free list.
void RenderOutput_InitFreeList(void) {
  s32 n;
  struct UiRenderWork *work;
  struct RenderOutput *out;
  struct RenderOutput *next;

  work = UI_WORK;
  out = work->outputs;
  work->freeOutputs.head = out;
  n = UI_OUTPUT_COUNT - 2;
  do {
    next = out + 1;
    n--;
    out->next = next;
    out = next;
  } while (n >= 0);
  next->next = NULL;
  work->freeOutputs.tailLink = &next->next;
}

// Sets up the window system for the field (and the other screens outside
// battle): allocates and clears the work block, blanks the tilemap, frees
// every render output and starts the two per-frame callbacks, the tilemap
// upload and the window update.
void UiWork_InitializeWithResourceCounters(void) {
  struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
  volatile u32 fill;
#endif

  work = Runtime_AllocateBlock(HEAP_SLOT_WINDOW, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
  fill = 0;
  Dma_Set((const void *)&fill, work, DMA_FILL32(RENDER_WORK_SIZE / 4),
          REG_DMA3);
#else
  Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
  work->dirty = 1;
  work->glyphResource = NO_GLYPH_RESOURCE;
  work->level = 15;
#if EDITION_INTERNATIONAL
  fill = UI_TILE_PAIR(UI_TILE_BLANK);
  Dma_Set((const void *)&fill, work, DMA_FILL32(sizeof(work->tilemap) / 4),
          REG_DMA3);
#else
  Iwram_FillWords(work, sizeof(work->tilemap), UI_TILE_PAIR(UI_TILE_BLANK));
#endif
  RenderOutput_InitFreeList();
  UiWork_SetTwoEntriesTo999();
  Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
  UiWork_InitCountersWithResourceAndScheduleRefresh();
}

// Sets up the window system for battle, as
// UiWork_InitializeWithResourceCounters does for the field, and marks the work
// block as in battle (menuState 1). loadGlyphTiles is set only when the Djinn
// tutorial flag is set: it loads the glyph tiles the tutorial's messages use.
void UiWork_Initialize(s32 loadGlyphTiles) {
  struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
  volatile u32 fill;
#endif
  s32 i;

  // Copies pixel rows 4 to 6 of UI tile src to rows 0 to 2 of tile dst and
  // clears the rest of dst.
  s32 UiWork_CopyTileTail(u32 src, u32 dst) {
    src &= 0x3ff;
    dst &= 0x3ff;
    Dma_Set((void *)(0x06000010 + src * 32), (void *)(0x06000000 + dst * 32),
            DMA_COPY16(8), REG_DMA3);
    return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
  }

  work = (struct UiRenderWork *)Runtime_AllocateBlock(HEAP_SLOT_WINDOW,
                                                      RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
  fill = 0;
  Dma_Set(&fill, work, DMA_FILL32(RENDER_WORK_SIZE / 4), REG_DMA3);
#else
  Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
  work->dirty = 1;
  work->glyphResource = NO_GLYPH_RESOURCE;
  work->menuState = 1;
  work->level = 15;
#if EDITION_INTERNATIONAL
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

    for (i = 2; i >= 0; i--)
      work->tileAttributes[i] = mode;
  }
}

// The per-frame tilemap upload: copies each 256-byte band of the tilemap
// (four tile rows) marked in the dirty bits to VRAM. Bit 0 marks the whole
// tilemap, bits 1 to 5 one band each. Nothing is copied while menuBusy holds
// the upload off.
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

// Erases a window's rectangle of the tilemap (x, y, width and height in
// tiles), clamped to the screen. In battle the rows below row 16 take tile
// 0x7f instead of the blank tile.
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

// Draws a window at its full size: its frame, and for a text canvas window
// the canvas, filled with colour 4 under a frame when the window is framed and
// transparent otherwise.
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
  if (UI_WINDOW_TEXT_CANVAS & flags) {
    if (UI_WINDOW_FRAMED_CANVAS & flags) {
      UiWindow_DrawFrame(x, y, width, height);
      Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE, 0x44444444);
    } else {
      Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE, 0);
    }
    UiWindow_MapTextCanvasTiles(x, y, width, height, 0);
  } else {
    UiWindow_DrawFrame(x, y, width, height);
  }
  work->dirty = 1;
}

// Opens a window at tile (x, y) of width by height tiles in the first free
// window slot, and returns it, or NULL when all slots are in use. attrs takes
// the UI_WINDOW_ flags. Unless it is UI_WINDOW_INSTANT the window zooms open
// over eight frames, and this waits until it is fully open.
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs) {
  struct UiWindow *slot;
  struct UiWindow *found;
  s32 i;

  slot = UI_WORK->windows;
  found = 0;
  for (i = 0; i != WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & UI_WINDOW_OPEN) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
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
      found->duration = 8;
      found->frame = 7;
      UiWindow_WaitZoom(found);
      WaitFrames(1);
    }
  }
  return found;
}

// Waits until a zooming window has finished opening or closing.
void UiWindow_WaitZoom(struct UiWindow *window) {
  if (!(UI_WINDOW_INSTANT & window->flags) && window->duration != 0) {
    do {
      WaitFrames(1);
    } while (window->duration != 0);
  }
}

// Closes a window: clears its contents and remembers its last rectangle.
// With instant set the window is erased and its slot freed at once; otherwise
// it zooms closed over four frames first. Does nothing for NULL.
void UiWork_Finalize(struct UiWindow *window, s32 instant) {
  u16 zero;

  if (window == 0)
    return;

  RenderOutput_PrepareForRedraw(window);
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
    window->duration = 4;
  }
}

// Clears a window's contents: redraws its empty frame and releases the render
// outputs it carries. A text canvas window is left alone.
void RenderOutput_PrepareForRedraw(struct UiWindow *window) {
  if (!(UI_WINDOW_TEXT_CANVAS & window->flags)) {
    RenderOutput_RedrawSavedRect(window);
    RenderOutput_ClearList(window);
  }
}

// Redraws a window's empty frame at its current rectangle.
void RenderOutput_RedrawSavedRect(struct UiWindow *window) {
  UiWindow_DrawFrame(window->x, window->y, window->width, window->height);
}

// Releases every render output in a list and empties it. outputs is the list,
// or a window (whose output list comes first); NULL is ignored.
void RenderOutput_ClearList(void *outputs) {
  struct RenderOutputList *list = outputs;
  struct RenderOutput *node;
  struct RenderOutput *next;

  next = NULL;
  if (list != NULL) {
    node = list->head;
    list->tailLink = &list->head;
    list->head = next;
    while (node != NULL) {
      next = node->next;
      RenderOutput_Release(node);
      node = next;
    }
  }
}

// Clears an area of a window's interior to the window fill. left, top, right
// and bottom are pixels inside the window, and the area covers every tile
// they touch.
void UiWindow_ClearInteriorTiles(const struct RenderInput *window, u32 left,
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

// Points a list's tail link back at its last output, after the list has been
// edited by hand.
void RenderOutputList_UpdateTail(struct RenderOutputList *list) {
  struct RenderOutput *node = list->head;
  struct RenderOutput **tail = &list->head;

  while (node != NULL) {
    tail = &node->next;
    node = node->next;
  }
  list->tailLink = tail;
}

// Adds a render output to the end of a list; NULL lists are ignored.
void RenderOutput_AppendToList(struct RenderOutputList *list,
                               struct RenderOutput *node) {
  /* FAKEMATCH: storing the new tail as a RenderOutput * keeps both stores in
   * one alias set, so they stay in order. The typed list->tailLink =
   * &node->next lets the scheduler put the tail store first (differ 2 in all
   * six editions). */
  if (list != NULL) {
    *list->tailLink = node;
    *(struct RenderOutput **)&list->tailLink = node;
  }
}

// Frees a render output: returns it to the free list, releases its resource,
// and for a kind-2 output frees its palette slot too.
void RenderOutput_Release(struct RenderOutput *entry) {
  RenderOutput_ReleaseFree(entry);
  if ((u8)entry->kind != 0) {
    Resource_ResetEntry((u8)entry->index);
    if ((u8)entry->kind == 2) {
      // Free its palette slot (999 marks a free slot).
      u8 *work = gWindowWork[0];
      s32 offset = entry->oam.palette * 2 + RENDER_PALETTE_TABLE_OFFSET;
      *(u16 *)(work + offset) = 999;
    }
  }
  entry->active = 0;
}

// Starts a free message channel typing the decoded text that begins at entry
// into window, with the pen at pixel (x, y). spacing gives each of four lines
// an extra gap between characters (or none for NULL); flags is kept with the
// channel (nonzero keeps the window's size from page to page). Returns the
// channel, or NULL when all three are busy.
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window,
                                                s32 entry, s32 x, s32 y,
                                                u16 *spacing, s32 flags) {
  struct UiChannelSlot *slot = UI_WORK->channels;
  struct UiChannelSlot *found = NULL;
  u32 i;

  for (i = 0; i != 3; i++, slot++) {
    if (slot->work == NULL) {
      found = slot;
      break;
    }
  }
  if (found != NULL) {
    found->work = window;
    found->startX = x << 8;
    found->x = x << 8;
    found->y = y << 8;
    found->entry = entry;
    found->colour = 15;
    found->lineSpacing = 10;
    found->countdown = 0;
    found->outline = 0;
    found->ended = 0;
    found->flags = flags;
    if (spacing != NULL) {
      for (i = 0; i < 4; i++)
        found->spacing[i] = *spacing++;
    } else {
      for (i = 0; i < 4; i++)
        found->spacing[i] = 0;
    }
    found->line = 0;
  }
  return found;
}

// Starts a channel typing the decoded text at entry into the battle message
// window. A channel that already serves a finished window continues below its
// last line (scrolling up once both lines are used) unless keepPosition is
// set. Returns the channel, or NULL when all three are busy.
struct UiChannelSlot *UiText_StartBattleMessage(struct UiWindow *window,
                                                s32 entry, s32 keepPosition) {
  struct UiChannelSlot *slot;
  struct UiChannelSlot *selected;
  u16 *spacing;
  u16 zero;
  u32 index;

  slot = UI_WORK->channels;
  selected = 0;
  for (index = 0; index != 3; slot++, index++) {
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
    selected->colour = 15;
    selected->lineSpacing = 10;
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
    } while (index <= 3);
  }
  return selected;
}

// Clears the text canvas to transparent.
s32 Ui_ClearVramBlock(void) {
  return Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE, 0);
}

// Fills the text canvas with the window background colour (4).
s32 Ui_FillVramBlockPattern(void) {
  return Iwram_FillWords(UI_CANVAS_VRAM, UI_CANVAS_SIZE, 0x44444444);
}

// Resets the pen and text style of the first idle message channel, clearing
// the canvas when the channel still has a window.
void UiWork_ResetFreeChannel(void) {
  struct UiChannelSlot *slot = UI_WORK->channels;
  struct UiChannelSlot *selection = 0;
  s32 i;

  for (i = 0; i != 3; slot++, i++) {
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
    selection->colour = 15;
    selection->outline = 0;
    selection->lineSpacing = 10;
  }
}

// Makes a channel's text colour, outline and line spacing the ones glyphs are
// drawn with.
void UiText_ApplyChannelStyle(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = UI_WORK;

  work->colour = channel->colour;
  work->outline = channel->outline;
  work->lineSpacing = channel->lineSpacing;
}

// Starts a channel scrolling its canvas up a line, over two frames.
void UiText_StartCanvasScroll(struct UiChannelSlot *channel) {
  channel->transition = 2;
}

// The battle message canvas: 30 columns of 4 tiles. In a column each word is
// one 8-pixel row of 4bpp pixels, so a column is 32 pixel rows of one word.
#define CANVAS_COLUMNS 30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS 8 // the top 8 pixel rows stay put

// Scrolls the battle message canvas up by 6 pixel rows per step, below its
// top 8 rows, and clears the rows this uncovers.
void UiText_ScrollCanvasUp(s32 step) {
  s32 shift = step * 6;
  u32 *base = UI_CANVAS_VRAM;
  s32 n;

  for (n = 0; n < CANVAS_COLUMNS; n++) {
    u32 *column = base + n * CANVAS_COLUMN_WORDS;

    Dma_Set(column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
            DMA_COPY32(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
            REG_DMA3);
    Iwram_FillWords(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
  }
}

// The window effect a message can start (TEXT_WINDOW_STATE): the window's
// frame grows by a tile and its text sprites pulse, for the channel's
// countdown.
#define UI_WINDOW_EFFECT_SWELL 4

// What UiWork_StepChannelScript reports.
enum {
  CHANNEL_STEP_RUNNING = 0, // the text goes on next frame
  CHANNEL_STEP_ENDED = 8,   // the text has ended; the window stays open
  CHANNEL_STEP_CLOSE = 9,   // the text is dismissed: close the window
};

// The per-frame message update: each message channel whose window is fully
// open runs its window effect or types more text, and on the text's end marks
// the window done (state 1) or closes it.
void UiWork_ProcessRenderChannels(void) {
  struct UiChannelSlot *channel = UI_WORK->channels;
  s32 channelNo = 0;

  do {
    struct UiWindow *current = channel->work;

    if (current != 0 && *(s32 *)&current->frame == 0) {
      u16 flags = current->flags;

      if (flags == 0) {
        channel->work = NULL;
      } else {
        s32 pending = current->effect;
        s32 step;

        if (pending != 0) {
          UiWindow_StepSwellEffect(channel);
        } else {
          step = UiWork_StepChannelScript(channel);
          switch (step) {
          case CHANNEL_STEP_ENDED:
            channel->work->state = 1;
            break;
          case CHANNEL_STEP_CLOSE: {
            struct UiWindow *window = channel->work;

            UiWork_Finalize(window, window->flags & UI_WINDOW_INSTANT);
            // pending is 0 here: it clears the channel.
            channel->x = pending;
            channel->y = pending;
            channel->entry = pending;
            channel->countdown = pending;
            channel->colour = pending;
            channel->outline = pending;
            channel->lineSpacing = pending;
            channel->work->state = 1;
            break;
          }
          }
        }
      }
    }
    channelNo++;
    channel++;
  } while (channelNo != 3);
}

// The control codes of decoded message text (UiRenderWork.entries). Codes
// from TEXT_SPACE up are characters.
enum {
  TEXT_END = 0,
  TEXT_PAGE = 1,     // wait for a button, then clear the window for a new page
  TEXT_WAIT_KEY = 2, // wait for a button, then close the window
  TEXT_NEWLINE = 3,
  TEXT_PAUSE_LONG = 4,    // a 60-frame pause
  TEXT_PAUSE_SHORT = 5,   // a 20-frame pause a button can cut short
  TEXT_PAUSE_LONGEST = 6, // a 120-frame pause a button can cut short
  TEXT_COLOURS_RESET = 7,
  TEXT_COLOUR = 8,        // the next entry is the text colour
  TEXT_OUTLINE = 9,       // the next entry is the outline colour
  TEXT_LINE_SPACING = 10, // the next entry is the line spacing
  TEXT_WINDOW_STATE = 15, // the next entry is the window effect to run
  TEXT_END_ALT = 30,
  TEXT_COMMANDS = 31,
  TEXT_SPACE = 32,
  TEXT_VOICED_MARK = 222,    // dakuten
  TEXT_SEMIVOICED_MARK = 223 // handakuten
};

// The kana mark drawn with a character, in the top two bits of its code (the
// index into gUiText_MarkGlyphCodes).
#define TEXT_CODE_MARK_SHIFT 14
#define TEXT_CODE_VOICED (1 << TEXT_CODE_MARK_SHIFT)
#define TEXT_CODE_SEMIVOICED (2 << TEXT_CODE_MARK_SHIFT)

// Types one frame's share of a channel's text: runs control codes (new line,
// new page, pauses, colours) and draws characters, playing the speaker's voice
// blips. Returns a CHANNEL_STEP_ result.
s32 UiWork_StepChannelScript(struct UiChannelSlot *ch) {
  struct UiRenderWork *work = UI_WORK;
  u32 keys = gKeysHeld;
  s32 glyphsLeft = gUiText_SpeedSteps[gGameState.messageSpeed];
  s32 lag;
  u32 code;
  u32 next;
  struct UiWindow *window;
  s32 x;
  s32 y;
  u32 newWidth;
  u32 newHeight;
  s32 oldWidth;
  s32 oldHeight;
  s32 entry;
  s32 nextEntry;
  s32 pixelX;
  s32 pixelY;
  s32 voiceGap;
  s32 advance;
  s32 advanceX;

  // In battle, text runs at 3 glyphs a frame, and 5 more for each frame the
  // game is running behind (up to two).
  if (work->menuState != 0) {
    lag = gLagFramesShown;
    if (lag < 0)
      lag = 0;
    if (lag > 2)
      lag = 2;
    glyphsLeft = lag * 5 + 3;
  }

  // Finish scrolling the canvas up a line before any more text.
  if (ch->transition != 0) {
    UiText_ScrollCanvasUp(1);
    ch->transition = (s16)ch->transition - 1;
    return CHANNEL_STEP_RUNNING;
  }
  // With no button held, wait out the delay after the last glyph.
  if (gKeysHeld == 0 && ch->glyphDelay != 0) {
    ch->glyphDelay--;
    return CHANNEL_STEP_RUNNING;
  }

  // Run control codes and draw characters until this frame's share is done.
  do {
    code = TEXT_END;
    if (ch->ended == 0)
      code = work->entries[ch->entry];

    if (code < TEXT_COMMANDS) {
      switch (code) {
      case TEXT_NEWLINE:
        // Canvas lines are 13 pixels apart, and from the second line on the
        // canvas scrolls up instead; other windows' lines are 15 apart.
        ch->x = ch->startX;
        if (ch->work->flags & UI_WINDOW_TEXT_CANVAS) {
          if (ch->y >= 13 << 8) {
            UiText_StartCanvasScroll(ch);
            glyphsLeft = 1;
          } else {
            ch->y += 13 << 8;
          }
        } else {
          ch->y += 15 << 8;
          if (ch->line <= 2)
            ch->line++;
        }
        break;

      case TEXT_PAGE:
        if (work->mode != 0 && ch->countdown < 900)
          gKeysPressedLatch = 0;
        ch->countdown = 919;
        if (UiText_CheckTextDismissed(ch) == 0) {
          // Until the button: show the next-page arrow in the bottom-right
          // corner.
          struct UiWindow *pane = ch->work;

#if EDITION_INTERNATIONAL
          if (pane->width == 0 || pane->height == 0 ||
              work->pageArrowShown != 0)
            break;
#else
          if (work->pageArrowShown != 0)
            break;
#endif
          UiText_DrawGlyph(pane, 1, pane->width * 4 - 8, pane->height * 8 - 16,
                           1);
          work->pageArrowShown = 1;
          break;
        }
        // Clear the window, and refit it to the next page's text.
        window = ch->work;
        x = window->x;
        y = window->y;
        oldWidth = window->width;
        oldHeight = window->height;
        entry = ch->entry;
        work->pageArrowShown = 0;
        RenderOutput_PrepareForRedraw(window);
        if (ch->flags == 0 && (window->width | window->height) != 0)
          UiWindow_EraseBorderRect(window->x, window->y, window->width,
                                   window->height);
        entry = (entry + 1) & RENDER_ENTRY_MASK;
        if (work->entries[entry] != 0 &&
            (window->width | window->height) != 0) {
          if (ch->flags != 0) {
            UiWindow_EraseBorderRect(window->x, window->y, window->width,
                                     window->height);
          } else {
            UiWindow_FitOnScreen(entry, &x, &y, &newWidth, &newHeight,
                                 ch->spacing, 0);
            if (window->flags & UI_WINDOW_ANCHOR_BOTTOM) {
              if (oldHeight != newHeight)
                y -= newHeight - oldHeight;
              if (y < 0)
                y = 0;
            }
            if (!(window->flags & UI_WINDOW_FIXED_X)) {
              // Keep the window centred where it was.
              x += (s32)(oldWidth - newWidth) / 4;
              UiWindow_FitOnScreen(entry, &x, &y, &newWidth, &newHeight,
                                   ch->spacing, 2);
            }
            window->x = x;
            window->y = y;
            window->width = newWidth;
            window->height = newHeight;
          }
          UiWindow_DrawFrame(window->x, window->y, window->width,
                             window->height);
        }
        ch->x = ch->startX;
        ch->y = 0;
        ch->line = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
        ch->colour = 15;
        ch->outline = 0;
        ch->lineSpacing = 10;
        UiText_ApplyChannelStyle(ch);
#endif
        Resource_ResetEntry(work->glyphResource);
        work->glyphResource = NO_GLYPH_RESOURCE;
        break;

      case TEXT_WAIT_KEY:
        if (work->mode != 0 && ch->countdown < 900)
          gKeysPressedLatch = 0;
        if (UiText_CheckTextDismissed(ch) != 0)
          return CHANNEL_STEP_CLOSE;
        ch->countdown = 919;
        break;

      case TEXT_PAUSE_SHORT:
        if (ch->countdown == 0)
          ch->countdown = 20;
        work->result[1] = 0;
        UiText_CheckPauseSkipped(ch);
        break;

      case TEXT_PAUSE_LONGEST:
        if (ch->countdown == 0)
          ch->countdown = 120;
        work->result[1] = 0;
        UiText_CheckPauseSkipped(ch);
        break;

      case TEXT_PAUSE_LONG:
        if (ch->countdown == 0)
          ch->countdown = 60;
        work->result[1] = 0;
        break;

      case TEXT_COLOUR:
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
        ch->colour = work->entries[ch->entry];
        UiText_ApplyChannelStyle(ch);
        break;

      case TEXT_OUTLINE:
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
        ch->outline = work->entries[ch->entry];
        UiText_ApplyChannelStyle(ch);
        break;

      case TEXT_LINE_SPACING:
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
        ch->lineSpacing = work->entries[ch->entry];
        UiText_ApplyChannelStyle(ch);
        break;

      case TEXT_COLOURS_RESET:
        ch->colour = 15;
        ch->outline = 0;
        ch->lineSpacing = 10;
        UiText_ApplyChannelStyle(ch);
        break;

      case TEXT_WINDOW_STATE:
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
        ch->work->effect = work->entries[ch->entry];
        ch->countdown = 10;
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
        break;

      case TEXT_END:
      case TEXT_END_ALT:
        ch->ended = 1;
        return CHANNEL_STEP_ENDED;
      }

      if (work->menuState == 0)
        glyphsLeft = 1;
    } else {
      // A character: draw it at the pen position, rounded to whole pixels.
      pixelX = (ch->x + 128) / 256;
      pixelY = (ch->y + 128) / 256;
      voiceGap = gUiText_SpeedCueGaps[gGameState.messageSpeed];
      nextEntry = ch->entry;
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
      if (work->mode != 0)
        pixelX += 8;
#endif
      nextEntry = (nextEntry + 1) & RENDER_ENTRY_MASK;
      next = work->entries[nextEntry];
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
      // A kana mark after the character is drawn with it.
      if (next == TEXT_VOICED_MARK) {
        code |= TEXT_CODE_VOICED;
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
      } else if (next == TEXT_SEMIVOICED_MARK) {
        code |= TEXT_CODE_SEMIVOICED;
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
      }
#endif
#if EDITION_INTERNATIONAL
      // Two narrow characters that fit in one 16-pixel cell together are drawn
      // as one pair, the second in the code's high byte.
#if defined(TBS_EDITION_ES)
      if (!(ch->work->flags & UI_WINDOW_TEXT_CANVAS) && code > TEXT_SPACE &&
          next > TEXT_SPACE) {
        s16 pair = gUiText_Glyphs[code - TEXT_SPACE].width +
                   gUiText_Glyphs[next - TEXT_SPACE].width;

        if (work->outline == 1) {
          if ((u16)pair <= 14) {
            code |= next << 8;
            ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
          }
        } else if ((u16)pair <= 15) {
          code |= next << 8;
          ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
        }
      }
#else
      if (!(ch->work->flags & UI_WINDOW_TEXT_CANVAS) && code > TEXT_SPACE &&
          next > TEXT_SPACE &&
          (u16)(gUiText_Glyphs[code - TEXT_SPACE].width +
                gUiText_Glyphs[next - TEXT_SPACE].width) <= 15) {
        code |= next << 8;
        ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
      }
#endif
#endif
      advance = UiText_DrawGlyph(ch->work, code, pixelX, pixelY, 0);
      ch->glyphDelay = gUiText_SpeedDelays[gGameState.messageSpeed];
      if (advance != 0) {
        // The speaker's voice blip (result[0], the first of four sounds) every
        // few characters, and the pen moves on by the glyph's width.
        if (work->result[0] != 0) {
          if (work->result[1] == 0) {
            if (code != TEXT_SPACE) {
              AudioCommand_PlayFar(work->result[0] + (code & 3));
              work->result[1] = voiceGap;
            }
          } else {
            work->result[1]--;
          }
        }
        advanceX = advance << 8;
#if EDITION_INTERNATIONAL
        if (code == TEXT_SPACE)
          advanceX += ch->spacing[ch->line];
#else
        advanceX += ch->spacing[ch->line];
#endif
        ch->x += advanceX;
      }
#if !EDITION_INTERNATIONAL
      else
        return CHANNEL_STEP_CLOSE;
#endif
      if (code == TEXT_SPACE && work->menuState == 0)
        glyphsLeft = 1;
    }

    // A pause holds the channel on this entry until its countdown ends.
    if (ch->countdown != 0) {
      ch->countdown--;
      if (ch->countdown != 0)
        continue;
    }
    ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
  } while (--glyphsLeft != 0);

  return CHANNEL_STEP_RUNNING;
}

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
#if !EDITION_INTERNATIONAL
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
  state->unknownBeforeCount = 9;
  state->outline = 0;
  state->unknownAfterLineSpacing = 1;
}

// Loads the glyph tiles, resets the text style and starts the per-frame
// window update (UiWork_ProcessAll).
void UiWork_InitCountersWithResourceAndScheduleRefresh(void) {
  struct UiRenderWork *state = UI_WORK;
  s32 priority;

  state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->unknownBeforeCount = 9;
  state->lineSpacing = 10;
  state->outline = 0;
  state->colour = 15;
  state->count = 0;
  priority = 200;
  priority <<= 4;
  Scheduler_AddOrUpdateCallback((void *)UiWork_ProcessAll, priority);
}

// Resets the text style and starts the per-frame window update
// (UiWork_ProcessAll), loading the glyph tiles first when loadGlyphTiles is
// set.
void UiWork_InitCountersAndScheduleRefresh(s32 loadGlyphTiles) {
  struct UiRenderWork *state = UI_WORK;
  s32 priority;

  if (loadGlyphTiles != 0)
    state->glyphTiles = VramCache_Load(95, 128 << 6, 0);
  state->unknownBeforeCount = 9;
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

enum { HEAP_SLOT_BATTLE_DISPLAY = 37 };

// Shows a message in the battle message window, opening the window (a
// full-width text canvas along the bottom of the screen) if it is not open.
void UiText_PrepareMessageWork(s32 argument) {
  s32 index;
  struct UiChannelSlot *result;
  struct UiWindow *existing;
  struct UiWindow *work;
  struct UiRenderWork *state;
  struct BattleDisplayWork *control;

  state = UI_WORK;
  control = (struct BattleDisplayWork *)
      gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW];
  // result starts as NULL, though it is assigned again before it is read; the
  // same zero clears marked below
  result = NULL;
  state->menuState = 2;
  index = UiText_BuildRenderEntries(argument, 1);
  state->menuState = 1;

  if (state->entries[index] != 0) {
    existing = control->window;
    if (existing == NULL) {
      work = UiWindow_Create(0, 15, 30, 6,
                             UI_WINDOW_INSTANT | UI_WINDOW_TEXT_CANVAS);
      control->window = work;
      UiWindow_MapTextCanvasTiles(0, 15, 30, 6, 1);
      control->marked = 0;
    } else {
      work = existing;
    }
    if (work != NULL) {
      result = UiText_StartBattleMessage(work, index, control->marked);
      control->offset = (struct BattleDisplayOffset *)result;
      control->marked = 0;
      if (result == NULL) {
        UiWork_Finalize(work, 1);
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

// Types a message into an open window (passed as an integer) from its top
// left. Returns the channel typing it (as an integer), or 0 when the message
// is empty, window is 0 or no channel is free.
s32 UiText_OpenEntryMessage(s32 window, s32 argument) {
  struct UiRenderWork *work = UI_WORK;
  s32 entry;
  s32 result = 0;
  /* FAKEMATCH: never read: the native frame keeps 8 bytes above the two
   * outgoing arguments, exactly these four slots; without them the frame is 8
   * bytes, not 16 (differ 4 in tbs-en). */
  u16 entries[4];

  work->result[0] = 0;
  work->result[1] = 0;
  entry = UiText_BuildRenderEntries(argument, 1);
  if (work->entries[entry] == 0)
    return 0;
  if (window == 0)
    return 0;
  result = (s32)UiText_QueueRenderEntries((struct UiWindow *)window, entry, 0,
                                          0, 0, 1);
  if (result == 0)
    return 0;
  return result;
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
  work->result[0] = (packed << 4) >> 20;
  // the window starts as NULL, though it is assigned before it is read; the
  // same zero serves the zeros below
  window = NULL;
  work->result[1] = 0;
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
void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags) {
  struct UiRenderWork *state = UI_WORK;
  s32 release;
  s32 pos[2] = {0, 0};
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  struct UiWindow *work = NULL;
  s32 zero;

  y = x = 0;
  release = flags & 1;

  if (flags & 2) {
    state->busy = 1;
  }

  UiText_GetResourceDimensions(no, &x, &y, (u32 *)&width, (u32 *)&height);

  // Centre the window across the screen, and in the top 12 rows.
  x = (30 - width) >> 1;
  y = (12 - height) >> 1;

  if (flags & 8) {
    y += 4;
  } else if (flags & 0x40) {
    y += 12;
  } else {
    s32 dy;

    Object_GetScreenPositionFar(gGameState.selectedActor, pos);
    dy = pos[1] >> 3;
    if (dy > 9) {
      y = dy - 5;
    } else {
      y = dy + 4;
    }
  }

  work = UiText_OpenMessageWindow(no, x, y, release);

  if (work != NULL) {
    while (UiWork_IsComplete() == 0) {
      WaitFrames(1);
    }

    if (flags & 0x20) {
      UI_WORK->menuBusy = 1;
    }

    if (!(flags & 4)) {
      UiWork_Finalize(work, release);
      while (UiWork_IsIdle(work) == 0) {
        WaitFrames(1);
      }
    }
  }

  zero = 0;
  state->busy = zero;
  state->result[0] = zero;
  state->result[1] = zero;
  WaitFrames(3);
}

// The window system's per-frame callback: windows, then message channels,
// then sprite animation.
void UiWork_ProcessAll(void) {
  UiWork_ProcessDirectWork();
  UiWork_ProcessRenderChannels();
  UiWork_AnimateSpriteSlots();
}

// UiText_RenderGlyphPair draws its glyphs into a buffer of one byte per pixel
// (Iwram_ExpandBitRuns writes a glyph's rows there in a colour), in rows 24
// bytes apart, then packs it into 4bpp tiles.

#if EDITION_INTERNATIONAL
// Draws a character, and a second one when the code carries it in bits 8 to
// 15, into a 16x16-pixel cell of four 4bpp tiles at out: in the text colour
// over a shadow, or with an outline when UiRenderWork.outline is 1. Returns
// the width of the text drawn in pixels.
s32 UiText_RenderGlyphPair(s32 code, u32 *out) {
  u8 pixels[384];
  struct UiRenderWork *work;
  const u8 *glyph;
  s32 width;
  s32 colour;
  s32 shadowColour;
  s32 firstIndex;
  s32 secondIndex;
  u16 secondWidth;
  u8 *pixel;
  s32 tileColumn;
  s32 tileRow;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  work = UI_WORK;
  secondIndex = (code << 8) >> 16;
  code &= 255;
  Iwram_ClearWords(pixels, 384);
  // While UiRenderWork.mode is set, the text is colour 8 with no shadow.
  if (work->mode != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
  firstIndex = code - 32;
  glyph = firstIndex * sizeof(struct FontGlyph) + (const u8 *)gUiText_Glyphs;
  width = ((const struct FontGlyph *)glyph)->width;
  glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  if (work->outline == 1) {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 50, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    Iwram_ExpandBitRuns(glyph, pixels + 33, colour);
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
  }
  // The second character follows the first.
  if ((u16)secondIndex != 0) {
    glyph = (const u8 *)&gUiText_Glyphs[-32] + ((u16)secondIndex << 5);
    secondWidth = *(const u16 *)glyph;
    glyph += 2;
    if (work->outline == 1) {
      u8 *dst = pixels + width;
      Iwram_ExpandBitRuns(glyph, dst + 49, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 50, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 32, colour);
      Iwram_ExpandBitRuns(glyph, dst + 33, colour);
      secondWidth += 1;
    } else {
      u8 *dst = pixels + width;
      u8 *p;
      p = dst + 49;
      Iwram_ExpandBitRuns(glyph, p, shadowColour);
      p = dst + 32;
      Iwram_ExpandBitRuns(glyph, p, colour);
    }
    width += secondWidth;
  }

  // Pack the cell into four tiles, eight pixels to a word, the leftmost in
  // the lowest nibble.
  pixel = pixels + 7;
  tileColumn = 0;
  do {
    tileRow = 0;
    do {
      line = 0;
      do {
        tileLine = 0;
        pixelsLeft = 7;
        do {
          tileLine = (tileLine << 4) + *pixel--;
          pixelsLeft--;
        } while (pixelsLeft >= 0);
        *out++ = tileLine;
        line++;
        pixel += 24;
      } while (line <= 7);
      tileRow++;
      pixel -= 120;
    } while (tileRow <= 1);
    tileColumn++;
    pixel += 112;
  } while (tileColumn <= 1);
  return width;
}
#else
// Draws a character into a 16x16-pixel cell of four 4bpp tiles at out: in the
// text colour over a shadow, or with an outline when UiRenderWork.outline is
// 1, with the kana mark in the code's top two bits above its top right.
// Codes from 256 are kanji. Returns the character's width in pixels.
s32 UiText_RenderGlyphPair(u32 code, u32 *out) {
  u8 pixels[256];
  struct UiRenderWork *work;
  const u8 *glyph;
  const u8 *markGlyph;
  s32 width;
  s32 colour;
  s32 shadowColour;
  u32 markCode;
  u8 *pixel;
  s32 tileColumn;
  s32 tileRow;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  /* FAKEMATCH: the empty one-pass block ends a scheduling region, keeping the
   * spill of out ahead of the work-address load. Without it the load is
   * scheduled first (differ 2 in tbs-ja, the only edition with this body);
   * assigning work after markGlyph = 0 instead moves the zero too (differ 6).
   */
  do {
  } while (0);
  work = UI_WORK;
  markGlyph = 0;
  markCode = gUiText_MarkGlyphCodes[code >> TEXT_CODE_MARK_SHIFT];
  if (markCode != 0)
    markGlyph =
        (const u8 *)((const struct FontGlyph *)((const u8 *)gUiText_Glyphs +
                                                (markCode - 32) *
                                                    sizeof(struct FontGlyph)))
            ->rows;
  code &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
  if (code <= 0xff) {
    glyph = (const u8 *)gUiText_Glyphs + (code - 32) * sizeof(struct FontGlyph);
    width = ((const struct FontGlyph *)glyph)->width;
    glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  } else {
    glyph = gUiText_KanjiGlyphs + (code - 256) * 24;
    width = 10;
  }
  Iwram_ClearWords(pixels, 256);
  if (work->mode != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
  if (work->outline == 1) {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 50, shadowColour);
    if (markGlyph != 0) {
      Iwram_ExpandBitRuns(markGlyph, pixels + 24, shadowColour);
      Iwram_ExpandBitRuns(markGlyph, pixels + 25, shadowColour);
    }
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    Iwram_ExpandBitRuns(glyph, pixels + 33, colour);
    if (markGlyph != 0) {
      /* FAKEMATCH: assigning pixel in the markGlyph argument computes pixels +
       * 7 in r1 and makes the loop's copy in r5, as the native does. A plain
       * pixels + 7 argument computes it in r5 and makes the copy in r1 (differ
       * 6 in tbs-ja, the only edition with this body). */
      Iwram_ExpandBitRuns(markGlyph, pixel = pixels + 7, colour);
      Iwram_ExpandBitRuns(markGlyph, pixels + 8, colour);
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    if (markGlyph != 0)
      Iwram_ExpandBitRuns(markGlyph, pixels + 24, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    if (markGlyph != 0) {
      /* FAKEMATCH: the same assignment in the argument as the outlined path
       * above; a plain pixels + 7 argument differs the same way (differ 6 in
       * tbs-ja). */
      Iwram_ExpandBitRuns(markGlyph, pixel = pixels + 7, colour);
    }
  }

  pixel = pixels + 7;
  tileColumn = 0;
  do {
    tileRow = 0;
    do {
      line = 0;
      do {
        tileLine = 0;
        pixelsLeft = 7;
        do {
          tileLine = (tileLine << 4) + *pixel--;
          pixelsLeft--;
        } while (pixelsLeft >= 0);
        *out++ = tileLine;
        line++;
        pixel += 24;
      } while (line <= 7);
      tileRow++;
      pixel -= 120;
    } while (tileRow <= 1);
    tileColumn++;
    pixel += 112;
  } while (tileColumn <= 1);
  return width;
}
#endif

// The German, Spanish, French and Italian editions count every character up
// to 0xff as 10 pixels, and those above as nothing.
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) ||                      \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) ||                      \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define WIDE_WIDTH_FIXED 1
#endif

// The main font's glyph n, counted from the space character.
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gUiText_Glyphs))

// Returns the width in pixels of a zero-terminated string of 16-bit character
// codes: 4 for a space, the font width for other codes up to 0xff (in German,
// Spanish, French and Italian a flat 10) and 10 for a kanji. The kana marks
// take no width.
s32 UiText_GetWideStringWidth(u16 *text) {
  s32 width;
  u16 *p;
  u32 c;
#if !defined(WIDE_WIDTH_FIXED)
  u32 idx;
#endif

  c = *text;
  width = 0;
  p = text + 1;
  if (c != 0) {
    do {
      if (c == 0x20) {
        width += 4;
      } else if (c <= 0xffU) {
#if defined(WIDE_WIDTH_FIXED)
        width += 10;
#else
        idx = c - TEXT_VOICED_MARK;
        if (idx > 1U) {
          idx += TEXT_VOICED_MARK - TEXT_SPACE; // the font starts at the space
          width += FONT_GLYPH(idx)->width;
        }
      } else {
        width += 10;
#endif
      }
      c = *p;
      p += 1;
    } while (c != 0);
  }
  return width;
}

// Draws a zero-terminated string of 16-bit character codes into a text canvas
// window at pixel (x, y), running its colour, outline, line spacing and new
// line codes; other control codes are skipped with their arguments. A NULL
// text draws the decoded text in UiRenderWork.entries.
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x,
                                     s32 y) {
  u8 *base;
  struct UiRenderWork *work;
  u32 c;
  u32 next;
  s16 start;

  base = gWindowWork[0];
  work = (struct UiRenderWork *)base;
  c = 0;
  start = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = c;
    work->count = (work->count + 1) & RENDER_ENTRY_MASK;
  }
#if EDITION_INTERNATIONAL
  for (;;)
#else
  while ((c = *text++) != 0)
#endif
  {
#if EDITION_INTERNATIONAL
    c = *text++;
    if (c > 0xff)
      c = 0x40;
    if (c == 0)
      break;
#endif
    if (c <= TEXT_END_ALT) {
      switch (c) {
      case TEXT_COLOUR:
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
      case TEXT_COLOURS_RESET:
        UiWork_ResetCounters();
        break;
      case TEXT_NEWLINE:
        x = start;
        y += 15;
        break;
      case 14:
      case 15:
      case 28:
        text++;
      case 11:
      case 12:
      case 17:
#if EDITION_INTERNATIONAL
      case 29:
#endif
        text++;
        break;
      }
    } else {
#if EDITION_INTERNATIONAL
      // Pair two narrow characters into one cell, as UiWork_StepChannelScript
      // does.
      if ((window->flags & UI_WINDOW_TEXT_CANVAS) == 0) {
        next = *text;
#if defined(TBS_EDITION_ES)
        if (c > 32 && next > 32) {
          s16 width =
              gUiText_Glyphs[c - 32].width + gUiText_Glyphs[next - 32].width;

          if (work->outline == 1) {
            if ((u16)width <= 14) {
              c |= next << 8;
              text++;
            }
          } else if ((u16)width <= 15) {
            c |= next << 8;
            text++;
          }
        }
#else
        if (c > 32 && next > 32 &&
            (u16)(gUiText_Glyphs[c - 32].width +
                  gUiText_Glyphs[next - 32].width) <= 15) {
          c |= next << 8;
          text++;
        }
#endif
      }
#else
      if (*text == TEXT_VOICED_MARK) {
        c |= TEXT_CODE_VOICED;
        text++;
      } else if (*text == TEXT_SEMIVOICED_MARK) {
        c |= TEXT_CODE_SEMIVOICED;
        text++;
      }
#endif
      x += UiText_DrawGlyph(window, c, x, y, 0);
    }
  }
}

// Draws a string of 16-bit character codes straight into the UI tilemap, as
// tiles, inside a window at tile (offsetX, offsetY) of its interior, writing
// both the work block's copy and VRAM. A NULL buffer draws the decoded text
// in UiRenderWork.entries.
void UiText_RenderGlyphTileAtWorkOffset(u16 *buffer,
                                        const struct RenderInput *work,
                                        s32 offsetX, s32 offsetY) {
  struct UiRenderWork *canvas = UI_WORK;
  u32 cell;

  if (buffer == NULL) {
    u16 *counter = &canvas->count;

    buffer = canvas->entries;
    canvas->entries[*counter] = 0;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
  }

  cell = ((work->y + offsetY + 1) << 5) + (work->x + offsetX) + 1;
  if (cell < 32 * 20) { // inside the tilemap
    s32 src;
    s32 dst;

    cell *= 2;
    dst = (s32)UI_TILEMAP_VRAM + cell;
    src = (s32)canvas->tilemap + cell;

    UiText_RenderStringTiles(buffer, (u16 *)src, (u16 *)dst,
                             (s32)UI_TILEMAP_VRAM);
  }
}

// Draws a zero-terminated string of 16-bit character codes into a window's
// tilemap at tile (x, y) of its interior, one tile per character, skipping
// control codes; a NULL text draws the decoded text in UiRenderWork.entries.
void UiText_RenderWideStringInWindow(s16 *text, struct UiWindow *window, s32 x,
                                     s32 y) {
  struct UiRenderWork *work;
  u16 c;
  s16 start;

  work = UI_WORK;
  start = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = 0;
    work->count = (work->count + 1) & RENDER_ENTRY_MASK;
  }
  c = *text++;
  if (c != 0)
    do {
      if (c <= TEXT_END_ALT) {
        switch (c) {
        case TEXT_NEWLINE:
          x = start;
          y++;
          break;
        case 14:
        case 15:
        case 28:
          text++;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 17:
#if EDITION_INTERNATIONAL
        case 29:
#endif
          text++;
          break;
        }
      } else {
        UiWindow_PutGlyph(window, c, x, y, 0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        x++;
#else
        if ((u16)(c - TEXT_VOICED_MARK) > 1) // the kana marks share a tile
          x++;
#endif
      }
      c = *text++;
    } while (c != 0);
  work->dirty = 1;
}

// Writes value as decimal text into buffer (14 bytes) and returns where the
// text starts. With width 0 the number has no padding and a minus sign when
// negative; otherwise it is right-aligned in width characters (up to 12) and
// unsigned.
u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width) {
  s32 offset;
  s32 value;
  s32 negative;
  s32 space;
  s32 minus;
  u8 *leading;
  u8 *trim;

  value = input;
  negative = 0;
  if (value < 0) {
    if (width == 0)
      negative = 1;
    value = 0U - value;
  }

  // Twelve digits, then spaces for the leading zeros and the sign in front.
  buffer[0] = ' ';
  for (offset = 12; offset != 0; offset--) {
    buffer[offset] = value % 10 + '0';
    value = value / 10;
  }

  offset = 0;
  buffer[13] = offset;
  space = ' ';
  offset = 1;
  minus = '-';
  for (leading = buffer; offset != 13; leading++, offset++) {
    if (leading[1] == '0') {
      if (offset != 12)
        leading[1] = space;
    } else {
      if (negative != 0)
        leading[0] = minus;
      break;
    }
  }

  if (width == 0) {
    offset = 0;
    if (buffer[0] == ' ') {
      trim = buffer;
      do {
        offset++;
        if (offset == 12)
          break;
        trim++;
      } while (*trim == ' ');
    }
    return buffer + offset;
  }

  if (width < 0 || width > 12)
    width = 12;
  return buffer + 13 - width;
}

#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_IT)
#define ARTICLE_KINDS 11
#define ARTICLE_KIND_MASK 15
#elif defined(TBS_EDITION_ES)
#define ARTICLE_KINDS 5
#define ARTICLE_KIND_MASK 15
#else
#define ARTICLE_KINDS 8
#define ARTICLE_KIND_MASK 7
#endif

struct ArticleTable {
  s8 *text[ARTICLE_KINDS];
};

#endif

// UiText_AppendArticleName copies a name into the decoded text at pos and
// returns the position after it. A name may start with an article class
// ({article_class:N}, MSG_ARTICLE_CLASS and its class), which picks the article
// written before it. no is the article form the message asks for (its
// argument code's operand); plural says the last number shown was not 1;
// suffix tracks what the text last ended with, for the suffix codes (1: an s,
// or in German s or z; 2: the end of a sentence, so the next word is
// capitalised; 0: anything else). With mode set (battle messages) the name is
// set off with spaces and the style codes around it.

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
// The German, Spanish, French and Italian form: forms 0 and 4 take no article,
// the others the class's article (in Spanish, form bit 3 asks for the
// indefinite article), capitalised at the start of a sentence.
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no,
                             s32 plural, s32 *suffix) {
  struct ArticleTable table;
#if defined(TBS_EDITION_ES)
  struct ArticleTable indefiniteTable;
  u16 count;
  s16 indefinite;
#endif
  s32 kind;
  u16 head;
  u16 c;
  s8 *p;
  s8 c8;
  s32 cnt;

#if defined(TBS_EDITION_ES)
  count = no & 7;
  indefinite = no & 8;
#endif
  if (mode != 0) {
    entry[pos] = ' ';
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
  }
#if defined(TBS_EDITION_ES)
  if (count == 0 || count == 4) {
#else
  if (no == 0 || no == 4) {
#endif
    head = name[0];
    if (head == 29)
      name += 2;
  } else {
    kind = 0;
    table = gUiText_ArticleTable;
#if defined(TBS_EDITION_ES)
    indefiniteTable = gUiText_IndefiniteArticles;
#endif
    if (name[0] == 29) {
      kind = name[1];
      name += 2;
    }
#if defined(TBS_EDITION_ES)
    if (indefinite == 0)
      p = table.text[kind & ARTICLE_KIND_MASK];
    else
      p = indefiniteTable.text[kind & ARTICLE_KIND_MASK];
#else
    p = table.text[kind & ARTICLE_KIND_MASK];
#endif
    for (cnt = 0; cnt < 8; cnt++) {
      if ((c8 = *p++) == 0)
        break;
      if (cnt == 0 && *suffix == 2)
        c8 -= 0x20;
      entry[pos] = c8;
      pos = (pos + 1) & 0x1ff;
    }
  }
  while (*name != 0) {
    c = *name++;
    entry[pos] = c;
    pos = (pos + 1) & 0x1ff;
#if defined(TBS_EDITION_DE)
    if (c == 's' || c == 'S' || c == 'z' || c == 'Z')
      *suffix = 1;
    else
      *suffix = 0;
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
    if (c == 's' || c == 'S')
      *suffix = 1;
    else
      *suffix = 0;
#endif
  }
  if (mode != 0) {
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x08;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = ' ';
    pos = (pos + 1) & 0x1ff;
  }
  return pos;
}
#endif

#if defined(TBS_EDITION_EN)
// The English form: form 1 (or 3 with a single item) takes "a" or "an" (by the
// name's first letter unless its class says otherwise), and form 2 (or 3 with
// several) adds the plural "s" or "es".
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no,
                             s32 plural, s32 *suffix) {
  struct ArticleTable table;
  s32 kind;
  u16 head;
  u16 c;
  u8 *p;
  s8 c8;
  s32 cnt;

  if (mode != 0) {
    entry[pos] = ' ';
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
  }
  if (no == 1 || (no == 3 && plural == 0)) {
    kind = 0;
    table = gUiText_ArticleTable;
    head = name[0];
    if (head == 29) {
      kind = name[1] - 1;
      name += 2;
    }
    if (kind == 0) {
      if (head == 'A' || head == 'I' || head == 'U' || head == 'E' ||
          head == 'O')
        kind = 2;
      else
        kind = 1;
    }
    p = table.text[kind & 7];
    for (cnt = 0; cnt < 8; cnt++) {
      if ((c8 = *p++) == 0)
        break;
      entry[pos] = c8;
      pos = (pos + 1) & 0x1ff;
    }
  } else {
    head = name[0];
    if (head == 29)
      name += 2;
  }
  while (*name != 0) {
    c = *name++;
    entry[pos] = c;
    pos = (pos + 1) & 0x1ff;
    if (c == 'S' || c == 's')
      *suffix = 1;
    else
      *suffix = 0;
  }
  if (no == 2 || (no == 3 && plural != 0)) {
    if (*suffix != 0) {
      entry[pos] = 'e';
      pos = (pos + 1) & 0x1ff;
    }
    entry[pos] = 's';
    pos = (pos + 1) & 0x1ff;
  }
  if (mode != 0) {
    entry[pos] = 0x0a;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = 0x08;
    pos = (pos + 1) & 0x1ff;
    entry[pos] = ' ';
    pos = (pos + 1) & 0x1ff;
  }
  return pos;
}
#endif

// The control codes of a message as stored (the PO catalogs' names), which
// UiText_BuildRenderEntries expands into decoded text.
enum {
  MSG_END = 0,
  MSG_PAGE_BREAK = 1,
  MSG_END_WAIT = 2,
  MSG_LINE_BREAK = 3,
  MSG_TEXT_COLOR = 8,
  MSG_TEXT_EFFECT = 9,
  // German: a counted noun; Italian: a gender ending; Spanish and French: the
  // plural suffix
  MSG_LOCALIZED_WORD = 13,
  MSG_PROTAGONIST_NAME = 16,
  MSG_ACTOR_NAME = 17,
  MSG_ARGUMENT_ACTOR_NAME = 18,
  MSG_ARGUMENT_CLASS_NAME = 19,
  MSG_ARGUMENT_ITEM_NAME = 20,
  MSG_ARGUMENT_ABILITY_NAME = 21,
  MSG_ARGUMENT_SIGNED_NUMBER = 22,
  MSG_ARGUMENT_LOCATION_NAME = 23,
  MSG_EM_DASH = 24,
  // English: the plural suffix; German, Spanish, French and Italian: the
  // item's possessive
  MSG_PLURAL_SUFFIX = 25,
  MSG_BUTTON_ICON = 26,
  MSG_POSSESSIVE_SUFFIX = 27,
  MSG_ARTICLE_CLASS = 29,
  MSG_END_NOW = 30,
};

// What halting speech (UiWork_SetBusyFlags) puts between characters.
#define ENTRY_NARROW_GAP TEXT_PAUSE_SHORT
#define ENTRY_WIDE_GAP TEXT_VOICED_MARK

// The heap block the message decoder routine is copied into to run.
#define TEXT_WORK_BLOCK 50

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) ||                      \
    defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#if defined(TBS_EDITION_FR)
extern s8 gUiText_ItemPronoun[];
#else
struct ItemPossessiveTable {
#if defined(TBS_EDITION_ES)
  s8 *text[2];
#else
  s8 *text[4];
#endif
};
extern const struct ItemPossessiveTable gUiText_ItemPossessives;
extern const s8 gUiText_ItemGenders[];
extern u8 *gUiText_NumberWords[];
#endif
#endif

// Decodes message script into the ring of decoded text (UiRenderWork.entries)
// and returns where it starts; script -1 returns the last message's start
// again. Argument codes take their values from the argument table
// (UiWork_PushValueSlot), and clear empties the table afterwards. Names,
// numbers and articles are written out; style codes are kept with their
// operands for the message channel to run.
s32 UiText_BuildRenderEntries(s32 script, s32 clear) {
  struct UiRenderWork *work = UI_WORK;
  u32 ch = 0;
#if defined(TBS_EDITION_EN)
  s32 head = 1;
#endif
  s32 quote = 0;
  u32 start = work->count;
  u32 cnt = 0;
  s32 plural = 0;
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_IT)
  s32 running = 1;
#endif
#if defined(TBS_EDITION_EN)
  u32 pos = start;
  u32 prev;
  s32 suffix = 0;
#else
#if defined(TBS_EDITION_ES)
  struct ItemPossessiveTable possessives;
  s32 form;
#endif
  s32 suffix = 2;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
  s32 running = 1;
#endif
  s8 *word;
  s8 c8;
  u32 pos = start;
#endif
  u16 *entry = work->entries;
  s32 overflow = 0;
  s32 (*decode)(struct TextReader *);
  u32 next;
  s32 no;
  u32 j;
  u32 i;
  s32 value;
  s32 tmp;
#if defined(TBS_EDITION_IT)
  s32 ending;
#endif
  s32 mag;
  u8 *num;
  u8 *src;
  u16 *dst;
  u16 *p;
  u16 name[24];
  u8 numbuf[16];
  struct TextReader st;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
  struct ItemPossessiveTable possessives;
  s32 kind;
#endif

  if (script == -1) {
    start = work->messageStart;
  } else {
    {
      u32 size;

      /* FAKEMATCH: instruction order only: the size in a block of its own, so
       * the slot number goes to r0 before the size's copy to r1. 2026-10-13,
       * tbs-en: RamCode_LoadSlot, the size and the block as initialized locals,
       * and the size written at both uses with no local all differ 2 (the copy
       * to r1 first). */
      do {
        size = (u32)Text_DecodeSymbolCodeSize;
      } while (0);
      Dma_Set((const void *)Text_DecodeSymbol,
              Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, size),
              DMA_COPY32(size >> 2), REG_DMA3);
    }
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(
        struct TextReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
    UiText_LookupMessage(&st, script);

    do {
#if defined(TBS_EDITION_EN)
      prev = ch;
#endif
      ch = (u32)decode(&st);
      if (ch > 255)
        ch = 64;

      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (ch < 32) {
          switch (ch) {
          case MSG_ARGUMENT_CLASS_NAME:
            decode(&st);
            UiText_LookupArgument(MSG_ARG_CLASS, clear);
            break;
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            decode(&st);
            UiText_LookupArgument(MSG_ARG_ITEM, clear);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_LookupArgument(MSG_ARG_LOCATION, clear);
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
            decode(&st);
            break;
          case MSG_ACTOR_NAME:
            decode(&st);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            decode(&st);
            break;
          case MSG_ARTICLE_CLASS:
            decode(&st);
            break;
          case MSG_PAGE_BREAK:
            running = 0;
            ch = 2;
            break;
          case MSG_PROTAGONIST_NAME:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        // Halting speech pauses before each character.
        if (work->spacingBefore != 0
#if defined(TBS_EDITION_EN)
            && head == 0 && ch != 222 && ch != 223
#endif
        ) {
          entry[pos] = ENTRY_NARROW_GAP;
          pos = (pos + 1) & RENDER_ENTRY_MASK;
        }

#if defined(TBS_EDITION_EN)
        if (work->spacingAfter != 0 && head == 0 && ch != 222 && ch != 223 &&
            prev <= 256 && prev > 127 && prev != 222 && prev != 223 &&
            prev != 32 && prev != 165 && prev != 161 && prev != 164) {
          entry[pos] = ENTRY_WIDE_GAP;
          pos = (pos + 1) & RENDER_ENTRY_MASK;
        }

#endif
        // A character. Halting speech breaks off with an ellipsis at the
        // first space or after 11 characters.
        if (ch > 31) {
          if (work->spacingBefore != 0 && (ch == 32 || cnt > 10)) {
            entry[pos] = '.';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = '.';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = '.';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            overflow = 1;
            if (cnt > 10)
              ch = 32;
          }
          if (ch == 34) {
            quote ^= 1;
            if (quote != 0)
              ch = 142;
          }
          entry[pos] = (u16)ch;
          pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_EN)
          head = 0;
#else
          if (ch != ' '
#if defined(TBS_EDITION_ES)
              && ch != 161 && ch != 191
#endif
          ) {
            suffix = 0;
            if (ch == '.' || ch == '!' || ch == '?')
              suffix = 2;
            if (ch == 's' || ch == 'S'
#if defined(TBS_EDITION_DE)
                || ch == 'z' || ch == 'Z'
#endif
            )
              suffix = 1;
          }
#endif
        } else {
          // A control code: write out names, numbers and articles, and keep
          // style codes with their operands.
          switch (ch) {
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
          case MSG_ARTICLE_CLASS:
            entry[pos] = (u16)ch;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = decode(&st) + 0xFFFFU;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            value = (s32)UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            mag = value;
            if (value < 0)
              mag = -value;
            plural = 1;
            if (mag <= 1)
              plural = 0;
            num = UiText_FormatNumber(numbuf, value, 0);
            i = num - numbuf;
            while (i != 16 && numbuf[i] != 0) {
              entry[pos] = numbuf[i];
              pos = (pos + 1) & RENDER_ENTRY_MASK;
              i++;
            }
            break;
          case MSG_ARGUMENT_CLASS_NAME:
            no = decode(&st) - 1;
            tmp = UiText_LookupArgument(MSG_ARG_CLASS, clear);
            UiText_DecodeMessage(tmp + MsgClassName, name, 24);
            pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                           &suffix);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            no = decode(&st) - 1;
#if defined(TBS_EDITION_ES)
            form = no & 7;
#endif
            tmp = UiText_LookupArgument(MSG_ARG_ITEM, clear);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            tmp &= RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_ES)
            if (form == 2 || (form == 3 && plural != 0) || form == 4)
#else
            if (no == 2 || (no == 3 && plural != 0) || no == 4)
#endif
              UiText_DecodeMessage(tmp + MsgItemPluralName, name, 24);
            else
              UiText_DecodeMessage(tmp + MsgItemName, name, 24);
#else
            UiText_DecodeMessage((tmp & RENDER_ENTRY_MASK) + MsgItemName, name,
                                 24);
#endif
            pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                           &suffix);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            tmp = UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            UiText_DecodeMessage(tmp + MsgAbilityName, name, 24);
            p = name;
            next = pos;
            while (*p != 0) {
              entry[next] = *p;
              p++;
              next = (next + 1) & RENDER_ENTRY_MASK;
            }
            pos = next;
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_DecodeMessage(
                Scene_GetAreaNameIndexFar(
                    UiText_LookupArgument(MSG_ARG_LOCATION, clear), 1) +
                    MsgDebugEntryName,
                name, 24);
            p = name;
            next = pos;
            while (*p != 0) {
              entry[next] = *p;
              p++;
              next = (next + 1) & RENDER_ENTRY_MASK;
            }
            pos = next;
            break;
          case MSG_PROTAGONIST_NAME:
            src = UnitGetFar(gGameState.selectedActor)->name;
            dst = name;
            i = 0;
            do {
              *dst = *src;
              i++;
              src++;
              dst++;
            } while (i <= 14);
            pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0, &suffix);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            no = decode(&st) - 1;
            src = UnitGetFar(UiText_LookupArgument(MSG_ARG_ACTOR, clear))->name;
            dst = name;
            j = 0;
            do {
              *dst = *src;
              j++;
              src++;
              dst++;
            } while (j <= 14);
            pos = UiText_AppendArticleName(0, name, pos, entry, no, plural,
                                           &suffix);
            break;
          case MSG_ACTOR_NAME:
            tmp = decode(&st) - 1;
            src = UnitGetFar(tmp)->name;
            dst = name;
            j = 0;
            do {
              *dst = *src;
              j++;
              src++;
              dst++;
            } while (j <= 14);
            pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0, &suffix);
            break;
          case MSG_BUTTON_ICON:
            tmp = ((decode(&st) - 1) << 1) + 128;
            entry[pos] = tmp;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = tmp + 1;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
          case MSG_EM_DASH:
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            entry[pos] = ' ';
#else
            entry[pos] = 143;
#endif
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = '-';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            entry[pos] = ' ';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
#endif
            break;
#if defined(TBS_EDITION_DE)
          case MSG_LOCALIZED_WORD:
            no = decode(&st) - 1;
            src = gUiText_NumberWords[(no & 7) * 2 + plural];
            while (*src != 0) {
              entry[pos] = *src++;
              pos = (pos + 1) & RENDER_ENTRY_MASK;
            }
            break;
#endif
#if defined(TBS_EDITION_IT)
          case MSG_LOCALIZED_WORD:
            ending = decode(&st) - 1;
            if (ending == 0) {
              if (plural == 0)
                entry[pos] = 'o';
              else
                entry[pos] = 'i';
            } else if (plural == 0)
              entry[pos] = 'a';
            else
              entry[pos] = 'e';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
          case MSG_LOCALIZED_WORD:
#elif defined(TBS_EDITION_EN)
          case MSG_PLURAL_SUFFIX:
#endif
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_IT)
            if (plural != 0) {
              if (suffix != 0) {
                entry[pos] = 'e';
                pos = (pos + 1) & RENDER_ENTRY_MASK;
              }
              entry[pos] = 's';
              pos = (pos + 1) & RENDER_ENTRY_MASK;
            }
            break;
#endif
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
          case MSG_PLURAL_SUFFIX:
#if defined(TBS_EDITION_ES)
            UiText_LookupArgument(MSG_ARG_ITEM, 0);
            possessives = gUiText_ItemPossessives;
            word = possessives.text[plural];
#else
            tmp = UiText_LookupArgument(MSG_ARG_ITEM, 0);
            tmp &= RENDER_ENTRY_MASK;
            possessives = gUiText_ItemPossessives;
            kind = gUiText_ItemGenders[tmp];
            word = possessives.text[kind & 3];
#endif
            /* FAKEMATCH: value counts the letters, compared unsigned, keeping
             * the allocation of Spanish and Italian. With the function's u32 j
             * or i as the counter: differ 44 or 58 in tbs-es, 476 or 486 in
             * tbs-it. */
            for (value = 0; (u32)value < 16; value++) {
              if ((c8 = *word++) == 0)
                break;
              if (value == 0 && suffix == 2)
                c8 -= 0x20;
              entry[pos] = c8;
              pos = (pos + 1) & RENDER_ENTRY_MASK;
            }
            break;
#endif
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
          case MSG_PLURAL_SUFFIX:
#if defined(TBS_EDITION_DE)
            tmp = UiText_LookupArgument(MSG_ARG_ITEM, 0);
            tmp &= RENDER_ENTRY_MASK;
            possessives = gUiText_ItemPossessives;
            kind = gUiText_ItemGenders[tmp];
            word = possessives.text[kind & 3];
#else
            UiText_LookupArgument(MSG_ARG_ITEM, 0);
            word = gUiText_ItemPronoun;
#endif
            for (i = 0; i < 16; i++) {
              if ((c8 = *word++) == 0)
                break;
              if (i == 0 && suffix == 2)
                c8 -= 0x20;
              entry[pos] = c8;
              pos = (pos + 1) & RENDER_ENTRY_MASK;
            }
            suffix = 0;
            break;
#endif
          case MSG_POSSESSIVE_SUFFIX:
#if defined(TBS_EDITION_DE)
            if (suffix == 1)
              entry[pos] = '\'';
            else
              entry[pos] = 's';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
#elif defined(TBS_EDITION_EN)
            entry[pos] = '\'';
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            if (suffix == 0) {
              entry[pos] = 's';
              pos = (pos + 1) & RENDER_ENTRY_MASK;
            }
#endif
            break;
          case -1:
            break;
          case MSG_PAGE_BREAK:
          case MSG_LINE_BREAK:
#if defined(TBS_EDITION_EN)
            head = 1;
#endif
          default:
            entry[pos] = (u16)ch;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_EN)
            if (ch == 's' || ch == 'S')
              suffix = 1;
            else
              suffix = 0;
#endif
            break;
          }
        }
      }
      cnt++;
    } while (running != 0 && cnt <= RENDER_ENTRY_MASK);

    entry[pos] = (u16)ch;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    entry[pos] = 0;
    next = (pos + 1) & RENDER_ENTRY_MASK;
    work->count = (u16)next;
    Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
    work->messageStart = (u16)start;
  }

  if (clear != 0)
    UiWork_ClearValueNameTables();

  return start;
}

#endif

#if defined(TBS_EDITION_JA)

// The Japanese form: Japanese has no articles, so the name is copied as it is.
extern __inline__ u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos,
                                               u16 *entry, s32 no, s32 plural,
                                               s32 *suffix) {
  u16 c;

  if (mode != 0) {
    entry[pos] = ' ';
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    entry[pos] = 10;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    entry[pos] = 10;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
  }
  while (*name != 0) {
    c = *name++;
    entry[pos] = c;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
  }
  if (mode != 0) {
    entry[pos] = 10;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    entry[pos] = 8;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    entry[pos] = ' ';
    pos = (pos + 1) & RENDER_ENTRY_MASK;
  }
  return pos;
}

// The Japanese UiText_BuildRenderEntries, as above with no articles or
// plurals.
s32 UiText_BuildRenderEntries(s32 script, s32 clear) {
  struct UiRenderWork *work = UI_WORK;
  u32 ch = 0;
  s32 head = 1;
  u32 start = work->count;
  u32 cnt = 0;
  s32 running = 1;
  u32 pos = start;
  u16 *entry = work->entries;
  u8 mode = work->menuState & 2;
  s32 (*decode)(struct TextReader *);
  s32 overflow = 0;
  u32 prev;
  u32 next;
  u32 i;
  u32 j;
  s32 tmp;
  s32 message;
  s32 value;
  u8 *num;
  u8 *src;
  u16 *dst;
  struct BattleUnit *actor;
  u16 name[24];
  u8 numbuf[16];
  struct TextReader st;

  if (script == -1) {
    start = work->messageStart;
  } else {
    {
      u32 size;

      /* FAKEMATCH: instruction order only: the size in a block of its own, as
       * in the international copy above; RamCode_LoadSlot differs 2 (the copy
       * to r1 first). */
      do {
        size = (u32)Text_DecodeSymbolCodeSize;
      } while (0);
      Dma_Set((const void *)Text_DecodeSymbol,
              Runtime_AllocateHeapBlock(TEXT_WORK_BLOCK, size),
              DMA_COPY32(size >> 2), REG_DMA3);
    }
    // The decoder runs from a copy in RAM, one symbol at a time.
    decode = (s32 (*)(
        struct TextReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
    UiText_LookupMessage(&st, script);
    do {
      prev = ch;
      ch = (u32)decode(&st);
      // Once halting speech has broken off, the rest of the message is
      // skipped, though its arguments are still taken from the table.
      if (overflow != 0) {
        if (ch < 32) {
          switch (ch) {
          case MSG_ARGUMENT_CLASS_NAME:
            UiText_LookupArgument(MSG_ARG_CLASS, clear);
            break;
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
            decode(&st);
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            UiText_LookupArgument(MSG_ARG_ITEM, clear);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_LookupArgument(MSG_ARG_LOCATION, clear);
            break;
          case MSG_ACTOR_NAME:
            decode(&st);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            break;
          case MSG_PAGE_BREAK:
            running = 0;
            ch = 2;
            break;
          case MSG_PROTAGONIST_NAME:
            break;
          case -1:
            break;
          default:
            break;
          }
        }
      } else {
        // Halting speech pauses before each character.
        if (work->spacingBefore != 0 && head == 0 && ch != 222 && ch != 223) {
          entry[pos] = 5;
          pos = (pos + 1) & RENDER_ENTRY_MASK;
        }
        if (work->spacingAfter != 0 && head == 0 && ch != 222 && ch != 223 &&
            prev <= 256 && prev > 127 && prev != 222 && prev != 223 &&
            prev != 32 && prev != 165 && prev != 161 && prev != 164) {
          entry[pos] = 222;
          pos = (pos + 1) & RENDER_ENTRY_MASK;
        }
        // A character. Halting speech breaks off with an ellipsis at the
        // first space or after 11 characters.
        if (ch > 31) {
          if (mode != 0 && ch == ' ') {
            entry[pos] = ch;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
          }
          if (work->spacingBefore != 0 && (ch == 32 || cnt > 10)) {
            entry[pos] = 165;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = 165;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = 165;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = 161;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            overflow = 1;
            if (cnt > 10)
              ch = 32;
          }
          entry[pos] = (u16)ch;
          pos = (pos + 1) & RENDER_ENTRY_MASK;
          head = 0;
        } else {
          // A control code: write out names and numbers, and keep style
          // codes with their operands.
          switch (ch) {
          case MSG_END:
          case MSG_END_WAIT:
          case MSG_END_NOW:
            running = 0;
            break;
          case MSG_TEXT_COLOR:
          case MSG_TEXT_EFFECT:
            entry[pos] = (u16)ch;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            entry[pos] = decode(&st) + 0xffffU;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
          case MSG_ARGUMENT_SIGNED_NUMBER:
            value = UiText_LookupArgument(MSG_ARG_NUMBER, clear);
            num = UiText_FormatNumber(numbuf, value, 0);
            i = num - numbuf;
            while (i != 16 && numbuf[i] != 0) {
              entry[pos] = numbuf[i];
              pos = (pos + 1) & RENDER_ENTRY_MASK;
              i++;
            }
            break;
          case MSG_ARGUMENT_CLASS_NAME:
            tmp = UiText_LookupArgument(MSG_ARG_CLASS, clear);
            UiText_DecodeMessage(tmp + MsgClassName, name, 24);
            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_ITEM_NAME:
            tmp = UiText_LookupArgument(MSG_ARG_ITEM, clear);
            UiText_DecodeMessage((tmp & 511) + MsgItemName, name, 24);
            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_ABILITY_NAME:
            tmp = UiText_LookupArgument(MSG_ARG_ABILITY, clear);
            UiText_DecodeMessage(tmp + MsgAbilityName, name, 24);
            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_LOCATION_NAME:
            UiText_DecodeMessage(
                Scene_GetAreaNameIndexFar(
                    UiText_LookupArgument(MSG_ARG_LOCATION, clear), 1) +
                    MsgDebugEntryName,
                name, 24);
            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            break;
          case MSG_PROTAGONIST_NAME:
            src = UnitGetFar(gGameState.selectedActor)->name;
            dst = name;
            i = 0;
            do {
              *dst = *src;
              i++;
              src++;
              dst++;
            } while (i <= 14);
            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            break;
          case MSG_ARGUMENT_ACTOR_NAME:
            actor = UnitGetFar(UiText_LookupArgument(MSG_ARG_ACTOR, clear));
            if (work->menuState != 0 &&
                (actor->classId == 122 || actor->classId == 125)) {
              message = MsgMysteryPersonName;
              if (actor->classId == 125)
                message++;
              UiText_DecodeMessage(message, name, 10);
              pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            } else {
              src = actor->name;
              dst = name;
              j = 0;
              do {
                *dst = *src;
                j++;
                src++;
                dst++;
              } while (j <= 14);
              pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            }
            break;
          case MSG_ACTOR_NAME:
            tmp = decode(&st) - 1;
            src = UnitGetFar(tmp)->name;
            dst = name;
            j = 0;
            do {
              *dst = *src;
              j++;
              src++;
              dst++;
            } while (j <= 14);
            pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
            break;
          case -1:
            break;
          case MSG_PAGE_BREAK:
          case MSG_LINE_BREAK:
            head = 1;
          default:
            entry[pos] = (u16)ch;
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
          }
        }
      }
      cnt++;
    } while (running != 0 && cnt <= RENDER_ENTRY_MASK);
    entry[pos] = (u16)ch;
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    entry[pos] = 0;
    next = (pos + 1) & RENDER_ENTRY_MASK;
    work->count = (u16)next;
    Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
    work->messageStart = (u16)start;
  }
  if (clear != 0)
    UiWork_ClearValueNameTables();
  return start;
}

#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define FIT_FIXED_LIMIT 1
#endif

// Sizes a window for decoded text no and moves (*px, *py) so the window fits
// on the screen. flags: bit 0 measures every page (UiText_MeasureAllPages)
// rather than the first; bit 1 takes the size in *pw and *ph as given, in
// tiles, instead of measuring. Measuring returns the size in tiles in *pw and
// *ph and fills spacing with each line's extra character gap.
void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                          u16 *spacing, u32 flags) {
#if !defined(FIT_FIXED_LIMIT)
  u8 *base;
#endif
  s32 x;
  s32 y;
  s32 limit;
  s32 right;
  s32 bottom;
  s32 over;
  s32 pos;

#if !defined(FIT_FIXED_LIMIT)
  base = gWindowWork[0];
#endif
  x = *px;
  y = *py;
  limit = 30;

  if (!(flags & 2)) {
    if (flags & 1)
      UiText_MeasureAllPages(no, pw, ph, spacing);
    else
      UiText_MeasureEntryDimensions(no, pw, ph, spacing);
  }

  if (*pw == 0 && *ph == 0)
    return;

  // Pixels to tiles, with room for the frame and a margin.
  if (!(flags & 2)) {
    *pw = (*pw + 19) >> 3;
    *ph = (*ph + 15) >> 3;
#if !defined(FIT_FIXED_LIMIT)
    if (((struct UiRenderWork *)base)->mode != 0) {
      *pw += 2;
      limit = 29;
    }
#endif
  }

  // Slide the window back onto the screen (limit tiles by 20).
  right = x + *pw;
  if (right > limit) {
    over = right - limit;
    pos = x - over;
    if (pos >= 0)
      x = pos;
    else
      x = 0;
  }
  bottom = y + *ph;
  if (bottom > 20) {
    over = bottom - 20;
    pos = y - over;
    if (pos >= 0)
      y = pos;
    else
      y = 0;
  }
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (x > limit - *pw)
    x = limit - *pw;
  if (y > 20 - *ph)
    y = 20 - *ph;
  *px = x;
  *py = y;
}

// Decodes message no and returns its text's width and height in pixels.
void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y) {
  UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(no, 0), (u32 *)x,
                                (u32 *)y, 0);
}

DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensions, 0)
DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensionsAlt, 1)

// Measures decoded text from pos to its first page's end: returns the width of
// the longest line and the height (15 pixels a line) in pixels. With spacing
// set, fills one entry per line with the extra gap between characters, in
// 1/256 pixel, that spreads the line to fill the window.
void UiText_MeasureEntryDimensions(s32 pos, u32 *outWidth, u32 *outHeight,
                                   u16 *spacing) {
  struct UiRenderWork *work;
  u32 lines;
  u32 width;
  u32 height;
  u32 lineWidth;
  u32 count;
  u32 c;
  u32 glyph;
  u32 i;
  u16 counts[4];
  u16 widths[4];
  s32 gap;
  u32 style;
#if !EDITION_INTERNATIONAL
  s32 trailing;
#endif

  work = UI_WORK;
#if !EDITION_INTERNATIONAL
  trailing = 0;
#endif
  width = 0;
  height = 15;
  lines = 0;
  lineWidth = 0;
  count = 0;
  for (;;) {
    c = work->entries[pos];
    pos = (pos + 1) & RENDER_ENTRY_MASK;
    // A character adds its width. Spaces (in Japanese every character) are
    // counted as the points the line can be stretched at.
    if (c > 31) {
#if EDITION_INTERNATIONAL
      if (c == 32) {
        lineWidth += 5;
        count++;
      } else {
        glyph = gUiText_Glyphs[c - 32].width;
        style = work->outline;
        if (style == 1 || style == 5)
          glyph++;
        lineWidth += glyph;
      }
#else
      if (c != TEXT_VOICED_MARK && c != TEXT_SEMIVOICED_MARK) {
        if (c == 32) {
          lineWidth += 7;
        } else {
          // Punctuation at the end of a line takes less room than its cell.
          if (c == 0xa5)
            trailing = -1;
          else if (c == 0x21)
            trailing = -3;
          else if (c == 0xa1 || c == 0xa4)
            trailing = -6;
          else
            trailing = 0;
          glyph = 10;
          if (c <= 0xff)
            glyph = gUiText_Glyphs[c - 32].width;
          style = work->outline;
          if (style == 1 || style == 5)
            glyph++;
          lineWidth += glyph;
        }
      }
      count++;
#endif
      continue;
    }
    switch (c) {
    case TEXT_END:
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) &&                    \
    !defined(TBS_EDITION_IT)
    case TEXT_PAGE:
#endif
      goto done;
    case TEXT_NEWLINE:
#if EDITION_INTERNATIONAL
      count++;
#else
      lineWidth += trailing;
#endif
      counts[lines] = count;
      widths[lines] = lineWidth;
      if (width < lineWidth)
        width = lineWidth;
      if (lines < 3)
        lines++;
      count = 0;
      lineWidth = 0;
      height += 15;
      break;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
    case TEXT_PAGE:
      work->outline = 0;
      goto done;
#endif
    case 14:
    case 28:
      pos = (pos + 1) & RENDER_ENTRY_MASK;
    case TEXT_COLOUR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case 17:
      pos = (pos + 1) & RENDER_ENTRY_MASK;
      break;
    case TEXT_OUTLINE:
      work->outline = work->entries[pos];
      pos = (pos + 1) & RENDER_ENTRY_MASK;
      break;
    }
  }
done:
#if EDITION_INTERNATIONAL
  count++;
#else
  lineWidth += trailing;
#endif
  counts[lines] = count;
  widths[lines] = lineWidth;
  if (width < lineWidth)
    width = lineWidth;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
  if (work->mode)
    width = width + 2;
#endif
  *outWidth = width;
  *outHeight = height;
  // Each line's extra gap between characters, in 1/256 pixel, to spread it
  // across the window's inner width.
  width = (width + 19) >> 3;
  width = (width << 3) - 16;
  if (spacing != NULL) {
    i = 0;
    while (i <= lines) {
      if (counts[i] <= 1) {
        *spacing++ = 0;
      } else {
        gap = width - widths[i];
        gap -= 4;
        if (gap < 0)
          gap = 0;
        gap = ((gap << 8) / (counts[i] - 1));
#if EDITION_INTERNATIONAL
        if (gap < 0 || gap > 0xc00)
          gap = 0x200;
#else
        if (gap < 0 || gap > 0x800)
          gap = 0;
        if (gap < 0 || gap > 0x100)
          gap = 0x100;
#endif
        *spacing++ = gap;
      }
      i++;
    }
  }
}

// Measures decoded text from start over all its pages: the width of the first
// page's longest line and the height of its tallest page, in pixels, with each
// line's extra character gap in spacing as UiText_MeasureEntryDimensions.
void UiText_MeasureAllPages(s32 start, u32 *outWidth, u32 *outHeight,
                            u16 *spacing) {
  u32 pageHeights[16];
  u16 counts[4];
  u16 widths[4];
  struct UiRenderWork *work;
  u32 maxWidth;
  u32 line;
  u32 width;
  u32 count;
  u32 page;
  u32 code;
  u32 glyph;
  u32 outline;
  u32 i;
  u32 j;
  s32 gap;
#if !EDITION_INTERNATIONAL
  s32 trailing;

  trailing = 0;
#endif
  work = UI_WORK;
  pageHeights[0] = 15;
  pageHeights[1] = 15;
  pageHeights[2] = 15;
  pageHeights[3] = 15;
  pageHeights[4] = 15;
  pageHeights[5] = 15;
  pageHeights[6] = 15;
  pageHeights[7] = 15;
  pageHeights[8] = 15;
  pageHeights[9] = 15;
  pageHeights[10] = 15;
  pageHeights[11] = 15;
  pageHeights[12] = 15;
  maxWidth = 0;
  pageHeights[13] = 15;
  pageHeights[14] = 15;
  pageHeights[15] = 15;
  line = 0;
  width = 0;
  count = 0;
  page = 0;
  for (;;) {
    code = work->entries[start];
    start = (start + 1) & RENDER_ENTRY_MASK;
    if (code >= 32) {
#if EDITION_INTERNATIONAL
      if (code == 32) {
        width += 5;
        count++;
      } else {
        glyph = gUiText_Glyphs[code - 32].width;
        outline = work->outline;
        if (outline == 1 || outline == 5)
          glyph++;
        width += glyph;
      }
#else
      if (code != TEXT_VOICED_MARK && code != TEXT_SEMIVOICED_MARK) {
        if (code == 32) {
          width += 7;
        } else {
          if (code == 0xa5)
            trailing = -1;
          else if (code == 0x21)
            trailing = -3;
          else if (code == 0xa1 || code == 0xa4)
            trailing = -6;
          else
            trailing = 0;
          glyph = 10;
          if (code <= 0xff)
            glyph = gUiText_Glyphs[code - 32].width;
          outline = work->outline;
          if (outline == 1 || outline == 5)
            glyph++;
          width += glyph;
        }
      }
      count++;
#endif
      continue;
    }
    switch (code) {
    case TEXT_NEWLINE:
#if EDITION_INTERNATIONAL
      counts[line] = ++count;
#else
      width += trailing;
      counts[line] = count;
#endif
      widths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      if (line < 3)
        line++;
      count = 0;
      width = 0;
      pageHeights[page] += 15;
      break;
    case 0:
#if EDITION_INTERNATIONAL
      counts[line] = ++count;
#else
      width += trailing;
      counts[line] = count;
#endif
      widths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      page++;
      goto done;
    case TEXT_PAGE:
#if EDITION_INTERNATIONAL
      counts[line] = ++count;
#else
      width += trailing;
      counts[line] = count;
#endif
      widths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
      work->outline = 0;
#endif
      page++;
      break;
    case 14:
    case 28:
      start = (start + 1) & RENDER_ENTRY_MASK;
    case TEXT_COLOUR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case 17:
      start = (start + 1) & RENDER_ENTRY_MASK;
      break;
    case TEXT_OUTLINE:
      work->outline = work->entries[start];
      start = (start + 1) & RENDER_ENTRY_MASK;
      break;
    }
  }
done:
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_FR)
  if (work->mode != 0)
    maxWidth += 2;
#endif
  for (i = 0; i < page; i++) {
    if (i == 0)
      *outHeight = pageHeights[0];
    else if (*outHeight < pageHeights[i])
      *outHeight = pageHeights[i];
  }
  *outWidth = maxWidth;
  maxWidth = (((maxWidth + 19) >> 3) << 3) - 16;
  if (spacing != NULL) {
    for (j = 0; j <= line; j++) {
      if (counts[j] <= 1) {
        *spacing++ = 0;
      } else {
        gap = maxWidth - widths[j];
        gap -= 4;
        if (gap < 0)
          gap = 0;
        gap = (gap << 8) / (counts[j] - 1);
#if !EDITION_INTERNATIONAL
        if (gap < 0 || gap > 0x800)
          gap = 0;
        if (gap < 0 || gap > 0x100)
          gap = 0x100;
#endif
        *spacing++ = gap;
      }
    }
  }
}

typedef s32 (*GlyphRoutine)(struct UiWindow *, s32, s32, s32, u8 *);

// A render output's sprite cell: the link and OAM attributes in its 12 bytes
// from offset 0x10.
#define SPRITE_CELL(output) ((struct CursorSprite *)(output)->unk10)

// Draws a character of a window's text with its top-left corner at pixel
// (x, y) of the window's interior and returns how far the text advances. A
// text canvas window draws it into the canvas; other windows show it as a
// sprite carried by the window. mode 1 shows the next-page arrow instead.
s32 UiText_DrawGlyph(struct UiWindow *window, s32 character, s32 x, s32 y,
                     s32 mode) {
  struct UiRenderWork *work = UI_WORK;
  u32 spacingX = work->unknownBeforeCount;
  s32 width;
  u32 spacingY = work->lineSpacing;

  // A text canvas window draws the glyph into its canvas.
  if (mode != 1 && (window->flags & UI_WINDOW_TEXT_CANVAS) != 0) {
    u8 *font;
    GlyphRoutine draw;
    s32 result;
#if !EDITION_INTERNATIONAL
    s32 mark;

    font = NULL;
    if (((struct BattleDisplayWork *)
             gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW])
            ->window == window) {
      font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
      width = 3;
      if (character == ' ')
        goto done;
    }
    mark = (s16)gUiText_MarkGlyphCodes[(u32)character >> TEXT_CODE_MARK_SHIFT];
    character &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
    width = 4;
#else
    if (((struct BattleDisplayWork *)
             gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW])
            ->window == window) {
      Resource_GetTableEntry((s32)&ResourceId_SmallFont);
      Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    width = 4;
#endif
    if (character != ' ') {
      RamCode_Load(draw, UiText_DrawGlyphArm, UiText_DrawGlyphCodeSize);
      result = draw(window, character, x, y, font);
#if !EDITION_INTERNATIONAL
      if ((u16)mark != 0)
        result += draw(window, (u16)mark, x + result, y, font);
#endif
      MemFree(draw);
      return result;
    }
  } else if (character == ' ') {
#if !EDITION_INTERNATIONAL
    width = 7;
#else
    width = 5;
#endif
  } else {
    // Other windows show each glyph as a 16x16 sprite, animated by the text
    // style (UiRenderWork.outline 2 to 5) or, in mode 1, as the page arrow.
    u32 tiles[32];
    struct RenderOutput *output;
    struct CursorSprite *sprite;
    s32 index;

    output = RenderOutput_AcquireFree();
    if (output == NULL)
      return 0;
    index = (output - work->outputs) * 4;
    output->active = UI_SPRITE_STILL;
    output->kind = 0;
    if (mode == 1) {
      width = 1;
      output->active = UI_SPRITE_PAGE_ARROW;
    } else {
      switch (work->outline) {
      case 3:
        output->active = UI_SPRITE_SHAKE;
        break;
      case 4:
        output->active = UI_SPRITE_POP;
        output->animStep = 8;
        break;
      case 5:
        output->active = UI_SPRITE_SPIN;
        output->animStep = 0;
        break;
      case 2:
        output->active = UI_SPRITE_WOBBLE;
        output->animStep = 0;
        break;
      }
#if !EDITION_INTERNATIONAL
      width = UiText_RenderGlyphPair(character, tiles);
#else
      {
        s32 drawn = UiText_RenderGlyphPair(character, tiles);

        if (drawn == 0)
          drawn = 1;
        width = drawn;
      }
#endif
    }

    if ((u8)output->active == UI_SPRITE_PAGE_ARROW) {
      // The page arrow sits in the window's bottom-right corner.
      sprite = SPRITE_CELL(output);
      if (work->glyphResource == NO_GLYPH_RESOURCE)
        work->glyphResource = Resource_FindFreeEntry();
      sprite->x = (window->x << 3) + ((window->width - 2) << 3) + 4;
#if !EDITION_INTERNATIONAL
      sprite->y = (window->y << 3) + ((window->height - 2) << 3) + 1;
#else
      sprite->y = (window->y << 3) + ((window->height - 2) << 3) - 1;
#endif
    } else {
      // Copy the glyph's tiles to its sprite tiles (OBJ VRAM) and place it.
      Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
              DMA_COPY32(32), REG_DMA3);
#if !EDITION_INTERNATIONAL
      ((u16 *)&output->oam)[0] = y + (spacingY >> 1) + window->y * 8;
#else
      ((u16 *)&output->oam)[0] = y + (spacingY >> 1) + window->y * 8 - 2;
#endif
      ((u16 *)&output->oam)[1] =
          (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
      ((u16 *)&output->oam)[2] = work->glyphTiles + index;
      sprite = SPRITE_CELL(output);
    }
    output->sentinel = 254;
    output->x = sprite->x;
    output->y = sprite->y;
    output->index = index;
    output->next = NULL;
    RenderOutput_AppendToList(&window->output, output);
  }
done:
  return width;
}

// A view of struct RenderOutput with its sprite cell (from offset 0x10) as the
// member attr. Read through a cast of RenderOutput.unk10 instead,
// UiWork_AnimateSpriteSlots does not match (578 instructions become 605).
struct SpriteOutput {
  struct SpriteOutput *next;
  u8 kind;
  u8 active;
  u16 x;
  u16 y;
  u16 unk0a;
  u16 animStep;
  u8 index;
  u8 sentinel;
  struct CursorSprite attr;
};

LAYOUT_OFFSET_GUARD(SpriteOutput_X, struct SpriteOutput, x,
                    (u32)&((struct RenderOutput *)0)->x);
LAYOUT_OFFSET_GUARD(SpriteOutput_Y, struct SpriteOutput, y,
                    (u32)&((struct RenderOutput *)0)->y);
LAYOUT_OFFSET_GUARD(SpriteOutput_Active, struct SpriteOutput, active,
                    (u32)&((struct RenderOutput *)0)->active);
LAYOUT_OFFSET_GUARD(SpriteOutput_AnimStep, struct SpriteOutput, animStep,
                    (u32)&((struct RenderOutput *)0)->animStep);
LAYOUT_OFFSET_GUARD(SpriteOutput_Index, struct SpriteOutput, index,
                    (u32)&((struct RenderOutput *)0)->index);
LAYOUT_OFFSET_GUARD(SpriteOutput_Attr, struct SpriteOutput, attr,
                    (u32)&((struct RenderOutput *)0)->unk10);

#if EDITION_INTERNATIONAL

// Puts a character into a window's tilemap as a tile at tile (x, y) of its
// interior. mode 1 shows the next-page arrow sprite in the window's corner
// instead.
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode) {
  void *work = gWindowWork[0];
  u8 *base = work;
  struct SpriteOutput *out;
  s32 idx;
  u16 *slot;
  struct CursorSprite *attr;
  u32 pos;
  u16 row;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == 1) {
    s32 column;
    work = RenderOutput_AcquireFree();
    if (work == NULL)
      return;
    out = work;
    idx =
        ((struct RenderOutput *)out - ((struct UiRenderWork *)base)->outputs) *
        4;
    out->active = UI_SPRITE_PAGE_ARROW;
    attr = &out->attr;
    slot = &((struct UiRenderWork *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = Resource_FindFreeEntry();
    column = 0xfffe;
    attr->x = (win->x + (column + *(u16 *)&win->width)) * 8 + 4;
    row = (u8)win->y + (row = (u8)win->height + 254);
    attr->y = row * 8 - 1;
    /* FAKEMATCH: needed to match: the embedded OAM view preserves
     * packed/logical position store order changed by byte-span and scalar
     * views. */
    out->x = attr->x;
    out->y = attr->y;
    out->next = NULL;
    out->index = idx;
    if (out->active == 0)
      out->active = mode;
    RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
  } else if (tile <= 0xff) {
    x++;
    y++;
    pos = (win->y + y) * 32 + (win->x + x);
    if (pos < 32 * 20)
      ((struct UiRenderWork *)work)->tilemap[pos] = UI_TILE(tile);
  }
}

#else

// The Japanese UiWindow_PutGlyph, which also places the kana marks.
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode) {
  u8 *work = gWindowWork[0];
  /* FAKEMATCH: needed to match: this XOR pair changes no pointer bits and folds
   * without instructions, preserving work and base as independent lifetimes. A
   * direct copy merges both into r8, omits the native r8/r12 copy, and differs
   * 17 aligned instructions (0x144 versus 0x148 bytes). */
  u8 *base = (u8 *)((unsigned long)work ^ tile);
  struct SpriteOutput *out;
  s32 idx;
  u16 *slot;
  struct CursorSprite *attr;
  u32 pos;
  u16 row;

  base = (u8 *)((unsigned long)base ^ tile);

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == 1) {
    s32 column;
    u16 left;
    out = (struct SpriteOutput *)RenderOutput_AcquireFree();
    if (out == NULL)
      return;
    idx =
        ((struct RenderOutput *)out - ((struct UiRenderWork *)base)->outputs) *
        4;
    out->active = UI_SPRITE_PAGE_ARROW;
    attr = &out->attr;
    slot = &((struct UiRenderWork *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = Resource_FindFreeEntry();
    column = 0xfffe;
    left = win->x;
    attr->x = (left + (column + *(u16 *)&win->width)) * 8 + 4;
    row = (u8)win->y + (row = (u8)win->height + 254);
    attr->y = row * 8 + 1;
    /* FAKEMATCH: needed to match: the embedded OAM view preserves
     * packed/logical position store order changed by byte-span and scalar
     * views. */
    out->x = attr->x;
    out->y = attr->y;
    out->next = NULL;
    out->index = idx;
    if (out->active == 0)
      out->active = mode;
    RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
  } else if (tile <= 0xff) {
    if (tile - TEXT_VOICED_MARK <= 1) {
      // A kana mark goes in the tile above the character before it. Over the
      // frame's top edge or a divider line, it takes the mark tile drawn over
      // that background.
      u32 line;

      line = (win->y + y) * 32;
      switch (
          *(((struct UiRenderWork *)base)->tilemap + (line + (win->x + x)))) {
      case UI_TILE_FRAME_TOP:
        tile -= 0xc0;
        break;
      case UI_TILE_DIVIDER_HORIZONTAL:
        tile -= 0xd0;
        break;
      }
    } else {
      x++;
      y++;
    }
    pos = (win->y + y) * 32 + (win->x + x);
    if (pos < 32 * 20)
      ((struct UiRenderWork *)work)->tilemap[pos] = UI_TILE(tile);
  }
}

#endif

enum {
  TILEMAP_ENTRY_PLAIN,
  TILEMAP_ENTRY_NONE,
  TILEMAP_ENTRY_PALETTE_14,
  TILEMAP_ENTRY_PALETTE_15,
  TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

// Writes value into a window's tilemap at tile (x, y) of its interior, with
// the palette mode names (TILEMAP_ENTRY_), or as it is for
// TILEMAP_ENTRY_PLAIN; TILEMAP_ENTRY_NONE writes nothing.
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y,
                              u32 mode) {
  struct UiRenderWork *work = UI_WORK;
  s32 palette;
  s32 index;

  y += 1;
  x += 1;
  if (y > (u32)(window->height - 1))
    return;
  if (x > (u32)(window->width - 1))
    return;

  switch (mode) {
  case TILEMAP_ENTRY_PALETTE_15:
    palette = 0xf000;
    break;
  case TILEMAP_ENTRY_PALETTE_14:
    palette = 0xe000;
    break;
  case TILEMAP_ENTRY_PALETTE_1:
    palette = 0x1000;
    break;
  default:
    palette = 0;
    break;
  }

  switch (mode) {
  case TILEMAP_ENTRY_NONE:
    return;
  case TILEMAP_ENTRY_PALETTE_14:
  case TILEMAP_ENTRY_PALETTE_15:
  case TILEMAP_ENTRY_PALETTE_1:
    index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    work->tilemap[index] = palette | value;
    return;
  case TILEMAP_ENTRY_PLAIN:
  default:
    index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    work->tilemap[index] = value;
    return;
  }
}

// Steps a scaling sprite's animation (UI_SPRITE_SWELL to
// UI_SPRITE_SHRINK_HALF) and sets its affine scale for this frame.
void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output) {
  s32 scale = 256;
  struct CursorSprite *sprite = SPRITE_CELL(output);
  struct AffineTransform effect;

  switch ((u8)output->active) {
  case UI_SPRITE_SWELL:
    scale = gUi_BounceScales[output->animStep++ & 31];
    break;
  case UI_SPRITE_SWELL_HALF:
    scale = gUi_BounceScales[output->animStep++ & 31] >> 1;
    break;
  case UI_SPRITE_SHRINK:
    if (output->animStep <= 7)
      scale = gUi_BounceScales[output->animStep++ * 2 + 16];
    break;
  case UI_SPRITE_SHRINK_HALF:
    if (output->animStep <= 7)
      scale = gUi_BounceScales[output->animStep++ * 2 + 16] >> 1;
    break;
  }
  if (scale == 256) {
    sprite->affineIndex = 0;
    sprite->affine = 0;
    sprite->x = output->x;
    sprite->y = output->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
    if (scale > 256) {
      sprite->affine = 3;
      /* FAKEMATCH: needed to match: the unsigned halfword read of the x cell; a
       * cast of the signed member swaps the x load and the -8 pool load. */
      sprite->x = *(u16 *)&output->x - 8;
      sprite->y = (u16)output->y - 8;
    } else {
      sprite->affine = 1;
      sprite->x = output->x;
      sprite->y = output->y;
    }
  }
}

// The per-frame sprite update: moves each sprite that an open window carries
// by its animation mode (UI_SPRITE_) and queues it for drawing.
void UiWork_AnimateSpriteSlots(void) {
  struct UiRenderWork *work = UI_WORK;
  struct UiWindow *window = work->windows;
  s32 no;
  struct AffineTransform effect;

  for (no = 0; no != WINDOW_COUNT; window++, no++) {
    struct SpriteOutput *output;
    u32 phase;

    if (!(window->flags & UI_WINDOW_OPEN))
      continue;
    output = (struct SpriteOutput *)window->output.head;
    phase = (gFrameTick >> 2) & 7;
    while (output != NULL) {
      struct CursorSprite *sprite = &output->attr;

      if (window->effect == UI_WINDOW_EFFECT_SWELL) {
        output->animStep = 2;
        output->active = UI_SPRITE_PULSE;
      }
      switch (output->active) {
      case UI_SPRITE_PAGE_ARROW:
        if (work->glyphResource != 0x60) {
          const u8 *tiles = &gUiSprite_CursorFrameTiles[phase * 128];

          sprite->tile = VramCache_Load(work->glyphResource, 128, tiles);
          output->index = sprite->tile;
          sprite->mode = 0;
          sprite->mosaic = 0;
          sprite->colors = 1;
          sprite->size = 0;
          sprite->shape = 2;
          sprite->y =
              output->y + gUiSprite_CursorBobOffsets[gFrameTick % 80] + 2;
          sprite->affine = 0;
          sprite->affineIndex = 0;
        }
        break;
      case UI_SPRITE_SHAKE:
        if (gFrameTick & 1) {
          sprite->x =
              output->x +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
          sprite->y =
              output->y +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
        }
        break;
      case UI_SPRITE_POP:
        if (output->animStep == 0) {
          sprite->affineIndex = 0;
          sprite->affine = 0;
          sprite->x = output->x;
          sprite->y = output->y;
          break;
        }
        effect.scaleX = 512;
        effect.scaleY = 512;
        effect.angle = 0;
        sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
        sprite->affine = 3;
        sprite->x = output->x - 5;
        sprite->y = output->y - 5;
        output->animStep--;
        break;
      case UI_SPRITE_SPIN:
        effect.scaleX = 256;
        effect.scaleY = 256;
        output->animStep += 768;
        effect.angle = output->animStep;
        sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
        sprite->affine = 1;
        sprite->x = output->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
        sprite->y = output->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
        break;
      case UI_SPRITE_WOBBLE:
        if (gFrameTick & 1)
          output->animStep++;
        sprite->x =
            output->x + gUiSprite_WobbleOffsets[output->animStep % 20][0];
        sprite->y =
            output->y + gUiSprite_WobbleOffsets[output->animStep % 20][1] - 2;
        break;
      case UI_SPRITE_HOP_UP:
        output->animStep++;
        sprite->y = output->y - gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case UI_SPRITE_HOP_DOWN:
      case UI_SPRITE_HOP_DOWN + 1:
      case UI_SPRITE_HOP_DOWN + 2:
        output->animStep++;
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case UI_SPRITE_HOP_DOWN_LEFT:
        output->animStep++;
        sprite->x = output->x - gUiSprite_BounceOffsets[output->animStep & 15];
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case UI_SPRITE_PULSE:
        if (output->animStep != 0) {
          effect.scaleX = 320;
          effect.scaleY = 320;
          effect.angle = 0;
          sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
          sprite->affine = 3;
          sprite->x = output->x - 8;
          sprite->y = output->y - 8;
          output->animStep--;
          break;
        }
        sprite->affineIndex = 0;
        sprite->affine = 0;
        sprite->x = output->x;
        sprite->y = output->y;
        break;
      case UI_SPRITE_SWELL:
      case UI_SPRITE_SWELL_HALF:
      case UI_SPRITE_SHRINK:
      case UI_SPRITE_SHRINK_HALF:
        RenderOutput_UpdateScaleAnimation((struct RenderOutput *)output);
        break;
      }
      if (output->active == UI_SPRITE_PAGE_ARROW) {
        if (work->glyphResource != 0x60)
          Runtime_PushSlotEntry(sprite, output->sentinel);
      } else if (output->active != UI_SPRITE_HIDDEN) {
        Runtime_PushSlotEntry(sprite, output->sentinel);
      }
      output = output->next;
      phase = (gFrameTick >> 2) & 7;
    }
  }
}

// Decodes message into buf (a u16 string) with no length limit.
void Ui_AdjustValueWithoutLimit(s32 message, s32 buf) {
  UiText_DecodeMessage(message, buf, 0x7fffffff);
}

// Decodes message key into destination as a zero-terminated string of at most
// capacity entries, and returns its length.
s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity) {
  struct UiRenderWork *work;
  u32 count;

  work = UI_WORK;
  work->count = 0;
  UiText_BuildRenderEntries(key, 1);
  for (count = 0;
       count < capacity - 1 && (destination[count] = work->entries[count]) != 0;
       count++) {
  }
  destination[count] = 0;
  return count;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define DECODE_EUROPEAN
#endif

// Decodes message into text as a zero-terminated string of 16-bit codes, with
// at most capacity entries, keeping style codes with their operands.
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
        if ((*(s32 *)&poll_work->frame) == 0) // frame and duration both 0
        {
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
void UiWindow_StepSwellEffect(struct UiChannelSlot *ch) {
  struct UiWindow *work = ch->work;
  s32 effect = work->effect;
  s32 x = work->x;
  s32 y = work->y;
  s32 w = work->width;
  s32 h = work->height;

  if (effect != UI_WINDOW_EFFECT_SWELL)
    return;

  UiWindow_DrawFrame(x - 1, y - 1, w + 2, h + 2);
  ch->countdown--;
  if (ch->countdown != 0)
    return;

  ch->work->effect = 0;
  UiWindow_EraseBorderRect(x - 1, y - 1, w + 2, h + 2);
  UiWindow_DrawFrame(x, y, w, h);
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
  } while (no != 8);
}

// Adds a message argument of kind (MSG_ARG_) with value to the first free
// place of the argument table; when all eight are in use it is dropped.
void UiWork_PushValueSlot(u32 value, u32 kind) {
  struct UiRenderWork *work = UI_WORK;
  u32 no = 0;
  u32 limit = 8;

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
u32 UiText_LookupArgument(u32 kind, u32 clear) {
  struct UiRenderWork *work = UI_WORK;
  u32 i;
  u32 value = 0;

  for (i = 0; i < 8; i++) {
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
s32 UiText_CheckPauseSkipped(struct UiChannelSlot *ch) {
  struct UiRenderWork *work = UI_WORK;
  s32 cancel = 0;
  s32 zero;

  if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
    cancel = 1;
  zero = 0;
  if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (cancel != zero) {
    ch->countdown = zero;
    return 1;
  }
  return zero;
}

// As UiText_CheckPauseSkipped, but on a newly pressed button: the latched
// presses while UiRenderWork.mode is set. Returns 1 when the player has
// dismissed the text.
s32 UiText_CheckTextDismissed(struct UiChannelSlot *ch) {
  struct UiRenderWork *work = UI_WORK;
  s32 key;
  s32 cancel = 0;
  u8 zero;

  if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
    cancel = 1;
  key = gKeysPressed;
  zero = 0;
  if (work->mode != zero)
    key = gKeysPressedLatch;
  if (key & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (cancel != 0) {
    ch->countdown = zero;
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
    if (work != NULL && (*(s32 *)&work->frame) == 0 && work->flags != 0 &&
        work->state != 0) {
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
  struct UiRenderWork *work;
  struct UiWindow *window;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entry;

  work = UI_WORK;
  x = 8;
  y = 8;
  // the window starts as NULL, though every path that reads it assigns it
  // first; the same zero is the style argument below
  window = NULL;
  entry = UiText_BuildRenderEntries(message, 1);
  if (work->entries[entry] != 0) {
    UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width,
                                 (u32 *)&height);
    x = (30 - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != 0)
      window = UiWindow_Create(x, y, width, height, 0);
    else {
      window = UiWindow_Create(x, y, 0, 0, 2);
      window->width = mode;
      window->height = mode;
    }
    if (UiText_QueueRenderEntries(window, entry, 0, 0, 0, 0) == 0)
      UiWork_Finalize(window, 1);
    else {
      while (UiWork_IsComplete() == 0)
        WaitFrames(1);
      if (mode != 0) {
        UiWork_Finalize(window, 0);
        while (UiWork_IsIdle(window) == 0)
          WaitFrames(1);
      } else
        UiWork_Finalize(window, 1);
      work->result[0] = 0;
      work->result[1] = 0;
    }
  }
}

// Decodes message arg0 and empties the argument table; returns where its
// decoded text starts.
s32 UiText_BuildRenderEntriesMode1(s32 arg0) {
  return UiText_BuildRenderEntries(arg0, 1);
}

// Points reader at the start of message (its compressed bits and first
// context), running the ARM lookup routine from a RAM copy.
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
s32 UiText_DecodeSymbol(struct TextReader *reader) {
  u32 code;
  u32 hi;
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
  u32 half;

  code = reader->previousCode;
  text = (const u8 *)reader->words;
  textBits = reader->bits;
  treeBits = 1;
  hi = code >> 8;
  code &= 0xff;
  base = Text_MessageContexts[hi][0];
  base += ((const u16 *)Text_MessageContexts[hi][1])[code];
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
    half = leaves[-(index >> 1)];
    half &= 0xf;
    code = leaves[-(index >> 1) - 1] | (half << 8);
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
  s32 i = 0;

  if (value < 20) {
    for (;;) {
      s32 key = gPortrait_CharacterMap[i];

      if (key == -1)
        break;
      if (key == value) {
        i++;
        result = gPortrait_CharacterMap[i];
        break;
      }
      i += 2;
    }
  } else {
    for (;;) {
      s32 key = gPortrait_SpriteMap[i];

      if (key == -1)
        break;
      if (key == value) {
        i++;
        result = gPortrait_SpriteMap[i];
        result += 128;
        break;
      }
      i += 2;
    }
  }
  return result;
}

// Opens a portrait window at tile (x, y) showing the portrait of speaker arg0
// (arg1 is passed on to the portrait object), and returns it as an integer,
// or 0 when the speaker has no portrait.
s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y) {
  s32 win;
  s32 minus_four;
  s32 offset;
  void *work;

  work = gWindowWork[0];
  if (Localization_LookupEntryId(arg0) == -1) {
    return 0;
  }
  minus_four = -4;
  offset = minus_four;
  if (((struct UiRenderWork *)work)->mode != 0) {
    win = (s32)UiWindow_Create(x, y, 6, 5, 2);
    offset = 0;
  } else {
    win = (s32)UiWindow_Create(x, y, 5, 5, 2);
  }
  if (win != 0) {
    CreateSideObject(arg0, arg1, -1, win, offset, minus_four);
  }
  return win;
}

// Closes the portrait window showing speaker entry, if one is open.
void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry) {
  struct UiRenderWork *state;
  struct UiWindow *work;
  s32 id;
  s32 index;
  s32 i;
  struct RenderOutput *entity;

  state = UI_WORK;
  work = state->windows;
  id = Localization_LookupEntryId(entry);
  if (id == -1)
    return;

  if (state->resourceIds[1] == id) {
    index = 1;
  } else if (state->resourceIds[0] == id) {
    index = 0;
  } else {
    return;
  }

  id = state->outputIds[index];

  for (i = 0; i != WINDOW_COUNT; i++, work++) {
    entity = work->output.head;
    if ((u8)entity->kind == 2 && (u8)entity->index == id) {
      UiWork_Finalize(work, 2);
      return;
    }
  }
}

// Returns how many item icons there are.
s32 UiIcon_CountItemIcons(void) {
  return gUiIcon_ItemIconPointersEnd - gUiIcon_ItemIconPointers;
}

// Returns how many Psynergy icons there are.
s32 UiIcon_CountPsynergyIcons(void) {
  return gUiIcon_PsynergyIconPointersEnd - gUiIcon_PsynergyIconPointers;
}

// Builds a command icon (overlay no0 over frame no1) and loads it into a
// sprite tile slot: *slot, or a free one when flag is 0. Returns the tile in
// *ret.
void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret,
                                  s32 flag) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  work->encoded = gUiIcon_FramePointerTable[no1];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, 0);
  work->encoded = gUiIcon_OverlayPointerTable[no0];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, 1);
  if (flag == 0) {
    *slot = Resource_FindFreeEntry();
  }
  *ret = VramCache_Load(*slot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
}

// Builds the icon of a battle action's item (its first operand) into a slot,
// as UiIcon_BuildItemIconTiles.
void Ability_RequestGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                          s32 reuse) {
  UiIcon_BuildItemIconTiles(BattleActGet(action)->unk04[0], withBase, slot,
                            tile, reuse);
}

// Builds item icon glyph, over the icon frame when withBase is set, and loads
// it into sprite tile slot *src (a free slot unless reuse is set). Returns the
// tile in *dst.
void UiIcon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *src, s32 *dst,
                               s32 reuse) {
  GlyphTransfer *work;
  s32 slot;

  slot = 0;
  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));

  if (glyph >= UiIcon_CountItemIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 0);
    slot = 1;
  }

  work->encoded = gUiIcon_ItemIconPointers[glyph];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, slot);

  if (reuse == 0)
    *src = Resource_FindFreeEntry();

  *dst = VramCache_Load(*src, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
}

extern GlyphTransfer *gGlyphWork;

#define ICON_FRAME 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

// Draws an item's icon into the glyph work buffer from its inventory code
// (item id in the low 9 bits, broken 0x200, equipped 0x400, count less one in
// the top 5 bits) with the layers asked for (ICON_ flags): the frame, the
// equipped, broken and artifact marks, and the count. Returns 256, or -1 when
// there is no glyph work buffer.
s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  GlyphTransfer *work;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_GetDirectFar(code & 0x1ff);
  work = gGlyphWork;
  if (work == NULL)
    return -1;
  if (layers & ICON_FRAME) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 0);
    overlay = 1;
  }
  work->encoded = gUiIcon_ItemIconPointers[item->icon];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, overlay);
  if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
    work->encoded = gUiIcon_MarkPointers[1];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
  }
  if ((layers & ICON_BROKEN) && (code & 0x200)) {
    work->encoded = gUiIcon_MarkPointers[0];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) &&
      (item->flags & 2)) {
    work->encoded = gUiIcon_MarkPointers[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
  }
  if (layers & ICON_COUNT_ABOVE_ONE) {
    count = (code & 0xf800) >> 11;
    count++;
    if (count <= 1)
      count = 0;
  }
  if (layers & ICON_COUNT) {
    count = (code & 0xf800) >> 11;
    count++;
  }
  if (count != 0 && count <= 30) {
    s32 ones = count % 10;

    work->encoded = gUiIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 1);
    if (count / 10 != 0) {
      work->encoded = gUiIcon_DigitPointers[count / 10 + 9];
      work->width = 2;
      work->height = 2;
      UiIcon_Decode(work, 1);
    }
  }
  return 256;
}

// Composes an item's icon (inventory code arg0, layers arg1) and loads it into
// sprite tile slot arg2.
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  ItemIcon_Compose(arg0, arg1);
  VramCache_Load(arg2, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return 1;
}

// Builds one of the other icons and loads it into sprite tile slot slot.
s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  Ui_PrepareTransferFromTableEntry(icon);
  VramCache_Load(slot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return 1;
}
extern const u8 *gRenderResource_PairSourceTable[];

// Loads one of four small fixed images into sprite tile slot slot.
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot) {
  /* FAKEMATCH: three more names of the one table keep each case's own literal
   * slot; with the one name in every case the switch folds away, differ 21 on
   * tbs-en (9 instructions for 24). */
  extern const u8 *PairSource1[] __asm__("gRenderResource_PairSourceTable"),
      *PairSource2[] __asm__("gRenderResource_PairSourceTable"),
      *PairSource3[] __asm__("gRenderResource_PairSourceTable");
  void *source;
  switch (value) {
  case 1:
    source = PairSource1;
    break;
  case 2:
    source = PairSource2;
    break;
  case 3:
    source = PairSource3;
    break;
  case 0:
  default:
    source = gRenderResource_PairSourceTable;
    break;
  }
  VramCache_Load(slot, 32, source);
  return 1;
}

// Draws an item's icon from its inventory code into the glyph work buffer.
void Ui_PrepareTransferForItem(s32 code) {
  GlyphTransfer *state = gGlyphWork;
  struct ItemDefinition *item = Item_GetDirectFar(0x1ff & code);

  if (code != 0) {
    state->encoded = gUiIcon_ItemIconPointers[item->icon];
  } else {
    state->encoded = gUiIcon_ItemIconPointers[0];
  }
  state->width = 2;
  state->height = 2;
  UiIcon_Decode(state, 0);
}

// Builds the Psynergy icon of a battle action, as
// UiIcon_BuildAbilityIconTiles.
void Ability_LoadGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                       s32 reuse) {
  UiIcon_BuildAbilityIconTiles(BattleActGet(action)->unk04[0], withBase, slot,
                               tile, reuse);
}

#define ROUTINE_BLOCK 49

// Builds Psynergy icon glyph, over the icon frame when withBase is set, and
// loads it into sprite tile slot *src (a free slot unless reuse is set).
// Returns the tile in *dst.
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *src, s32 *dst,
                                  s32 reuse) {
  GlyphTransfer *work;
  s32 slot;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  slot = 0;

  if (glyph >= UiIcon_CountPsynergyIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiIcon_Decode(work, 0);
    slot = 1;
  }

  work->encoded = gUiIcon_PsynergyIconPointers[glyph];
  work->width = 2;
  work->height = 2;
  UiIcon_Decode(work, slot);

  if (reuse == 0)
    *src = Resource_FindFreeEntry();

  *dst = VramCache_Load(*src, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
}

// Draws one of the other icons into the glyph work buffer.
void Ui_PrepareTransferFromTableEntry(u32 index) {
  GlyphTransfer *state = gGlyphWork;

  state->encoded = gUiIcon_MiscIconPointers[index];
  state->width = 2;
  state->height = 2;
  UiIcon_Decode(state, 0);
}

// Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a
// free slot unless reuse is set) and its colours into sprite palette palette.
// Returns the tile in *tile.
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse) {
  GlyphTransfer *work;
  u16 *table;
  u8 *entry;
  u32 index;

  work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  table = Resource_GetTableEntry((s32)&ResourceId_Icons);
  if (icon <= 127)
    index = icon;
  else
    index = icon - 112;
  entry = (u8 *)table + table[index];
  work->encoded = entry + 32;
  work->width = 4;
  work->height = 4;
  UiIcon_Decode(work, 0);
  if (reuse == 0)
    *slot = Resource_FindFreeEntry();
  *tile = VramCache_Load(*slot, 0x200, work->tiles);
  Runtime_ReleaseHeapBlock(17);
  Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(palette, 0), 0x80000010, REG_DMA3);
}

// Does nothing.
void UiGlyph_ReservedNoOp(void) {}

// Decodes a compressed icon into the glyph buffer and expands it to tiles,
// opaque for the first layer or masked over what is there when overlay is set.
void UiIcon_Decode(GlyphTransfer *glyph, s32 outlined) {
  RamCode_LoadSlot(ROUTINE_BLOCK, Tile_Decompress4bpp,
                   Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))gWorkSlot[ROUTINE_BLOCK])(glyph->encoded,
                                                           glyph->input);
  Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
  if (outlined)
    RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandMasked,
                     Tile_ExpandMaskedCodeSize);
  else
    RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandOpaque,
                     Tile_ExpandOpaqueCodeSize);
  ((void (*)(u8 *, u8 *, u32, u32))gWorkSlot[ROUTINE_BLOCK])(
      glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
  Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}

struct GlyphCursor {
  u16 unused;
  u16 cursor;
};

// Clears a selection record's cursor.
static __inline__ void ResetCursor(struct GlyphCursor *cursor) {
  /* FAKEMATCH: the helper takes the node's base field as its record, so each
   * row store forms base + 372 and stores the cursor at #2, as the native does.
   * A plain work->records[7].kind = 0 folds the offset into 374 and stores at
   * #0 (differ 6). */
  cursor->cursor = 0;
}

// Allocates and resets the selection screen: the row of icons the player picks
// from, with arrows at each end and a pointing hand over the chosen target.
void UiGlyph_ResetWorkState(void) {
  struct SelectionScreen *work;
  struct SelectionSprite *visual;
  const u8 *table;
  s32 i;
  u16 clear = 0;

  work = Runtime_AllocateBlock(18, sizeof(struct SelectionScreen));
  work->head = NULL;
  work->path = NULL;
  work->window = NULL;
  work->unk39a = clear;
  work->top = clear;
  work->cursorIndex = 128;
  work->verticalScroll = 32;
  work->count = clear;
  work->locked = 999;
  i = 0;
  do {
    work->records[i + 2].kind = 0;
    work->records[i + 9].kind = 0;
    i++;
  } while (i != 5);
  ResetCursor((struct GlyphCursor *)&work->records[7].base);
  ResetCursor((struct GlyphCursor *)&work->records[8].base);
  work->records[0].kind = 0;
  work->records[1].kind = 0;
  work->records[0].y = 0;
  work->records[1].y = 0;
  table = gMenu_AnimatedCursorTiles;
  work->records[14].slot = Resource_FindFreeEntry();
  work->records[14].tile = VramCache_Load(work->records[14].slot, 256, table);
  work->records[14].kind = 0;
  work->records[14].scale = 0;
  work->records[15].kind = 0;
  visual = &work->records[14].sprite;
  visual->mode = 0;
  visual->mosaic = 0;
  visual->colors = 1;
  visual->affine = 0;
  visual->param = 0;
  visual->size = 1;
  visual->shape = 0;
  visual->priority = 0;
}

// Empties the selection screen's list of entries.
void Resource_ClearOwnerListAndCounters(void) {
  struct SelectionScreen *screen;

  screen = gResQueueWork;
  screen->head = NULL;
  screen->unk39a = 0;
  if (screen->cursorIndex & 0x80) {
    screen->top = 0;
    screen->cursorIndex = 0;
  }
  screen->verticalScroll = 0;
  screen->count = 0;
}

// Adds an entry (kind and id) to the selection screen's list, up to 16.
void Resource_PushPendingPair(u32 first, u32 second) {
  struct SelectionScreen *state = gResQueueWork;
  u16 cnt = state->count;

  if (cnt != 16) {
    state->kinds[cnt] = first;
    state->bases[cnt] = second;
    state->count++;
  }
}

// Creates icons for the first five listed entries in a row along the bottom of
// the screen.
void MenuSelection_BuildEntries(u32 unused) {
  struct SelectionScreen *screen = gResQueueWork;
  u32 count = screen->count;
  u32 index = screen->top;
  struct SelectionNode *prev = 0;
  struct SelectionNode *node;
  s32 cnt = 0;

  while (index < count) {
    s32 base = screen->bases[index];
    u32 kind = screen->kinds[index];

    node = SelectionScreen_FindFreeIcon(0);
    if (node == 0)
      break;
    SelectionScreen_SetupIcon(kind, base, node, 0);
    if (screen->head == 0) {
      screen->head = node;
      node->prev = 0;
    } else {
      prev->next = node;
      node->prev = prev;
    }
    node->next = 0;
    cnt++;
    prev = node;
    if (cnt == 5)
      break;
    index++;
  }

  screen->baseX = 100 - cnt * 8;
  screen->baseY = 140;
  for (prev = screen->head, cnt = 0; prev != 0; prev = prev->next) {
    s32 x = screen->baseX + cnt;
    s32 y;

    prev->x = x;
    y = screen->baseY;
    prev->y = y;
    prev->targetX = x;
    prev->targetY = y;
    if (prev->kind == 6 && screen->locked == 0) {
      prev->y = 6;
      prev->targetY = 6;
    }
    prev->dx = 0;
    prev->dy = 0;
    cnt += 16;
  }
  SelectionScreen_LoadLargeIcon();
}

// Does nothing.
void UiWork_ReservedNoOp(void) {}

// Returns a free selection icon record: one of the seven row icons (kind 0) or
// of the five chosen icons (otherwise), or NULL.
struct SelectionNode *SelectionScreen_FindFreeIcon(s32 kind) {
  struct SelectionScreen *state = gResQueueWork;
  s32 i;

  if (kind != 0) {
    for (i = 0; i != 5; i++) {
      if (state->records[i + 9].kind == 0)
        return &state->records[i + 9];
    }
  } else {
    for (i = 0; i != 7; i++) {
      if (state->records[i + 2].kind == 0)
        return &state->records[i + 2];
    }
  }
  return NULL;
}

// Starts drawing the selection screen each frame.
void Resource_ScheduleOwnerResetDelayed(void) {
  Scheduler_AddOrUpdateCallback(SelectionScreen_Draw, 0xC80);
}

// Stops drawing the selection screen.
void Resource_ScheduleOwnerReset(void) {
  Scheduler_RemoveCallback(SelectionScreen_Draw);
}

// The selection screen's per-frame callback: slides and draws the row of
// icons, the large icon over the chosen one, the end arrows, the icons already
// chosen and the pointing hand.
void SelectionScreen_Draw(void) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionSprite *cursorEntry = &state->records[14].sprite;
  struct SelectionNode *cursor = &state->records[14];
  struct SelectionNode *transfer = &state->records[15];
  struct SelectionNode *node;
  struct SelectionSprite *e;
  struct SelectionSprite *te;
  s32 cnt = 0;
  s32 slot;
  s32 tmp;
  struct EffectPosition pos;

  node = state->head;
  while (node != 0) {
    e = &node->sprite;
    e->affine = 0;
    e->param = 0;
    e->x = node->x;
    e->y = node->y;
    slot = 240;
    if (state->verticalScroll != 0) {
      e->y += state->verticalScroll;
    } else if (node->x != node->targetX) {
      if (node->dx > 0) {
        if (node->x + node->dx > node->targetX) {
          node->x = node->targetX;
        } else {
          node->x += node->dx;
        }
      } else {
        if (node->x + node->dx < node->targetX) {
          node->x = node->targetX;
        } else {
          node->x += node->dx;
        }
      }
      e->x = node->x;
    } else if (cnt == state->cursorIndex) {
      slot = 241;
      if (cursor->kind != 0) {
        if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1) {
          cursor->targetX = pos.x;
          cursor->targetY = pos.y;
          if (cursor->scale == 0) {
            cursor->x = pos.x;
            cursor->y = pos.y;
            cursor->scale = 1;
          }
        }
      }
    }
    if (node->scale != 0) {
      if (GameFlag_TestFar(0x103) != 0) {
        if (state->records[14].kind == 1) {
          e->mode = 1;
        } else {
          e->mode = 0;
        }
        if (node->kind == 1) {
          e->mode = 1;
        }
      }
      Runtime_PushSlotEntry((s32 *)e, slot);
    }
    node = node->next;
    cnt++;
  }
  if (transfer->kind != 0) {
    node = NodeChain_GetNodeAtCount(state);
    te = &transfer->sprite;
    te->mode = 0;
    te->affine = 0;
    te->param = 0;
    te->mosaic = 0;
    te->colors = 1;
    te->size = 2;
    te->shape = 0;
    te->priority = 0;
    te->tile = transfer->tile;
    te->x = node->x - 4;
    te->y = node->y + (gMenu_CursorBobOffsets[(gFrameTick >> 1) & 15] >> 1) - 4;
    if (transfer->scale != transfer->scaleEnd) {
      state->matrix[0] = transfer->scale;
      state->matrix[1] = transfer->scale;
      state->matrix[2] = 0;
      te->param =
          AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
      te->affine = 3;
      te->x += 0xfff0;
      te->y += 0xf0;
      transfer->scale += transfer->scaleStep;
    }
    if (GameFlag_TestFar(0x103) != 0) {
      te->mode = 1;
    }
    Runtime_PushSlotEntry((s32 *)te, 248);
  }
  SelectionScreen_DrawArrow(state, 0);
  SelectionScreen_DrawArrow(state, 1);
  node = state->path;
  while (node != 0) {
    e = &node->sprite;
    if (node->x != node->targetX) {
      node->x += node->dx;
    }
    if (node->y != node->targetY) {
      node->y += node->dy;
    }
    e->x = node->x;
    e->y = node->y;
    if (node->scale != node->scaleEnd) {
      node->scale += node->scaleStep;
      state->matrix[0] = node->scale;
      state->matrix[1] = node->scale;
      state->matrix[2] = 0;
      e->param =
          AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
      e->affine = 3;
      e->x += 0xfff8;
      e->y += 0xf8;
    } else {
      e->affine = 0;
      e->param = 0;
    }
    if (GameFlag_TestFar(0x103) != 0) {
      if (state->records[14].kind == 1) {
        e->mode = 1;
      } else {
        e->mode = 0;
      }
      if (node->kind == 1) {
        e->mode = 1;
      }
    }
    Runtime_PushSlotEntry((s32 *)e, 240);
    node = node->next;
  }
  if (cursor->kind != 0) {
    cnt = (s32)gMenu_AnimatedCursorTiles;
    {
      void *src = (void *)(cnt + (((gFrameTick >> 2) & 15) << 8));

      cursorEntry->tile = VramCache_Load(cursor->slot, 0x100, src);
    }
    if (cursor->targetX != cursor->x) {
      tmp = (cursor->targetX - cursor->x) >> 1;
      if (tmp != 0) {
        cursor->x += tmp;
      } else {
        cursor->x = cursor->targetX;
      }
    }
    if (cursor->targetY != cursor->y) {
      if ((tmp = (cursor->targetY - cursor->y) >> 1) != 0) {
        cursor->y += tmp;
      } else {
        cursor->y = cursor->targetY;
      }
    }
    cursorEntry->y =
        cursor->y + gMenu_CursorBobOffsets[(gFrameTick >> 2) & 15] - 32;
    cursorEntry->x = cursor->x - 4;
    if (GameFlag_TestFar(0x103) != 0) {
      if (state->records[14].kind == 1) {
        cursorEntry->mode = 1;
      } else {
        cursorEntry->mode = 0;
      }
    }
    Runtime_PushSlotEntry((s32 *)cursorEntry, 248);
  }
  state->status += 1;
}

// Draws the selection screen's left (index 0) or right (1) arrow, nudged
// outwards while it flashes after a scroll.
void SelectionScreen_DrawArrow(struct SelectionScreen *state, s32 index) {
  u32 frame = (gFrameTick >> 2) & 7;
  struct SelectionSprite *entry;
  const u8 *frames;
  const u8 *tmp;

  if (state->records[index].kind == 0)
    return;
  entry = &state->records[index].sprite;
  entry->x = state->records[index].x;
  entry->y = state->records[index].y;
  if (index != 0) {
    frames = gMenu_CursorObjectTiles;
    if (state->records[1].base != 0)
      entry->x = entry->x + state->records[1].base;
  } else {
    frames = gMenu_CursorLeftObjectTiles;
    if (state->records[0].base != 0)
      entry->x = entry->x - state->records[0].base;
  }
  tmp = frames + frame * 128;
  entry->tile = VramCache_Load(state->records[index].slot, 128, tmp);
  if (GameFlag_TestFar(0x103)) {
    if (state->records[14].kind == 1)
      entry->mode = 1;
    else
      entry->mode = 0;
  }
  Runtime_PushSlotEntry((s32 *)entry, 238);
  if (state->records[index].base != 0)
    state->records[index].base--;
}

// Opens or refits the name window above the selection row and writes the name
// of the icon under the cursor, or that nothing is known (mode 4, Psynergy)
// or held (mode 2, items). mode 6 is a yes or no question, placed at the
// bottom or top of the screen.
void Menu_OpenSelectionWindow(s32 mode, u32 count) {
  struct SelectionScreen *screen;
  struct SelectionNode *node;
  struct UiWindow **slot;
  struct UiWindow *window;

  screen = gResQueueWork;
  node = NodeChain_GetNodeAtCount(screen);
  slot = &screen->window;
  window = *slot;
  if (window == 0) {
    if (mode == 6) {
      if (screen->locked != 0) {
        *slot = UiWindow_Create(17, 17, 5, 3, mode);
      } else {
        *slot = UiWindow_Create(17, 0, 5, 3, mode);
      }
      screen->verticalScroll = 0;
      screen->locked = 999;
    } else {
      *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    RenderOutput_PrepareForRedraw(screen->window);
  } else {
    if (count != 0 && window->width != count + 2) {
      UiWork_Finalize(window, 2);
      *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
    }
    RenderOutput_PrepareForRedraw(screen->window);
  }
  if (screen->count != 0) {
    UiText_DrawCharacterAtOffset(node->message, screen->window, 0, 0);
  } else {
    switch (mode) {
    case 4:
      UiText_DrawCharacterAtOffset(MsgAbilityNotKnown, screen->window, 0, 0);
      break;
    case 2:
      UiText_DrawCharacterAtOffset(MsgItemNotHeld, screen->window, 0, 0);
      break;
    }
  }
}

// Closes the selection screen and frees its icons, tiles and work block.
void Resource_ResetOwnerEntries(void) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionNode *node;

  Resource_ScheduleOwnerReset();
  UiWork_Finalize(state->window, 2);
  WaitFrames(1);
  node = state->head;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  node = state->path;
  while (node != 0) {
    if (node->kind != 0) {
      Resource_ResetEntry(node->slot);
      node->kind = 0;
    }
    node = node->next;
  }
  Resource_ResetPendingTransfer();
  if (state->records[0].y != 0) {
    Resource_ResetEntry(state->records[0].slot);
    if (state->records[0].y != 0) {
      Resource_ResetEntry(state->records[1].slot);
    }
  }
  Resource_ResetEntry(state->records[14].slot);
  Runtime_ReleaseHeapBlock(18);
}

// Moves the row of icons to start at (first, second), 16 pixels apart.
void Menu_SetNodeCoordinates(u32 first, u32 second) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionNode *node;

  state->baseX = first;
  state->baseY = second;
  node = state->head;
  while (node != 0) {
    node->x = first;
    node->targetX = first;
    node->y = second;
    node->targetY = second;
    node = node->next;
    first += 16;
  }
}

// Sets up both end arrows of the selection row.
void Menu_SetupSelectionBothSides(void) {
  struct SelectionScreen *state;

  state = gResQueueWork;
  SelectionScreen_SetupArrow(state, 0);
  SelectionScreen_SetupArrow(state, 1);
}

// Sets up the left (index 0) or right (1) arrow of the selection row, shown
// when there are entries beyond that end.
void SelectionScreen_SetupArrow(struct SelectionScreen *state, s32 index) {
  struct SelectionSprite *entry = &state->records[index].sprite;
  const u8 *frames = 0;
  u32 count;

  state->records[index].kind = 0;
  if (index != 0) {
    /* FAKEMATCH: the separate local loads the tile pointer early into r5 and
     * makes its copy late, as the native does. Assigning
     * gMenu_CursorObjectTiles directly where right is read: differ 42; the same
     * at the top of the branch: differ 16 (tbs-en). */
    const u8 *right = gMenu_CursorObjectTiles;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[index].kind = 1;
      count = 5;
    }
    frames = right;
    state->records[1].x = state->baseX + 16 * (count - 1) + 17;
  } else {
    frames = gMenu_CursorLeftObjectTiles;
    state->records[0].x = state->baseX - 9;
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[index].y == 0) {
    state->records[index].slot = Resource_FindFreeEntry();
    state->records[index].tile =
        VramCache_Load(state->records[index].slot, 128, frames);
    state->records[index].y = state->baseY;
    state->records[index].base = 0;
    entry->mode = 0;
    entry->mosaic = 0;
    entry->colors = 1;
    entry->affine = 0;
    entry->param = 0;
    entry->size = 0;
    entry->shape = 2;
    entry->priority = 0;
  }
}

// Returns the node st->count links on from st->node.
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st) {
  struct NodeChainNode *node = st->node;
  s32 i;

  for (i = 0; i != st->count; ++i) {
    node = node->next;
  }
  return node;
}

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

// Runs the selection row until the player chooses (the chosen entry's index)
// or, when mode allows it, cancels with B (-1). SELECTION_FIXED allows no
// moving or choosing.
s32 Menu_SelectionLoop(s32 mode) {
  struct SelectionScreen *screen = gResQueueWork;

  SelectionScreen_HighlightIcon(screen, 0);
  for (;;) {
    WaitFrames(1);
    if (screen->verticalScroll != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        Menu_StepRight(screen);
      } else if (gKeysRepeat & KEY_LEFT) {
        Menu_StepLeft(screen);
      } else if (gKeysPressed & KEY_A) {
        return Menu_ConfirmSelection(screen);
      }
    }
    if (mode != 0 && (gKeysPressed & KEY_B)) {
      return -1;
    }
  }
}

// As Menu_SelectionLoop, with the menu sounds.
u32 Menu_WaitForSelectionInput(u32 mode) {
  struct SelectionScreen *screen = gResQueueWork;
  u32 result;

  for (;;) {
    WaitFrames(1);
    if (screen->verticalScroll != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Menu_StepRight(screen);
      } else if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Menu_StepLeft(screen);
      }

      if (gKeysPressed & KEY_A) {
        result = screen->top + screen->cursorIndex;
        if (screen->head->kind == NODE_KIND_YES_NO) {
          if (result == 0)
            AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
          else
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        } else {
          AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gKeysPressed & KEY_B)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      return -1;
    }
  }
}

// Moves the cursor one entry right, scrolling the row at its fourth place.
void Menu_MoveSelectionForward(struct SelectionScreen *screen) {
  u32 end = screen->top + screen->cursorIndex + 1;

  if (end == screen->count) {
    return;
  }
  SelectionScreen_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  screen->cursorIndex++;
  if (screen->cursorIndex == SELECTION_ROWS && end + 1 < screen->count) {
    screen->cursorIndex--;
    screen->records[1].base = ARROW_SCROLL_FRAMES;
    screen->top++;
    Menu_ScrollSelectionList(screen, TRUE);
    if (screen->top + screen->cursorIndex + 2 == screen->count) {
      screen->records[1].kind = FALSE;
    }
    screen->records[0].kind = TRUE;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(screen, screen->cursorIndex);
  WaitFrames(1);
  Menu_OpenSelectionWindow(screen->head->kind, 0);
  WaitFrames(1);
}

// Moves the cursor one entry left, scrolling the row at its second place.
void Menu_MoveSelectionBackward(struct SelectionScreen *screen) {
  u32 cursor;

  if (screen->top == 0 && screen->cursorIndex == 0) {
    return;
  }
  SelectionScreen_RedrawIcon(screen, screen->cursorIndex);
  screen->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  cursor = screen->cursorIndex;
  if (cursor == 1 && screen->top != 0) {
    screen->records[0].base = ARROW_SCROLL_FRAMES;
    screen->top--;
    Menu_ScrollSelectionList(screen, FALSE);
    if (screen->top == 0) {
      screen->records[0].kind = FALSE;
    }
    screen->records[1].kind = cursor;
  } else {
    screen->cursorIndex--;
  }
  screen->status = NODE_STATUS_SHOWN;
  SelectionScreen_HighlightIcon(screen, screen->cursorIndex);
  WaitFrames(1);
  Menu_OpenSelectionWindow(screen->head->kind, 0);
  WaitFrames(1);
}

// Moves the cursor one entry right, scrolling the row at its edge and
// wrapping from the last entry to the first.
void Menu_StepRight(struct SelectionScreen *state) {
  struct SelectionNode *node;
  u16 *ids;
  s32 y;
  u32 end;

  SelectionScreen_RedrawIcon(state, state->cursorIndex);
  state->status = 33;
  WaitFrames(1);
  state->cursorIndex++;
  if (state->count > 5) {
    end = state->top + state->cursorIndex;
    if (end == state->count) {
      node = state->head;
      state->records[0].kind = 0;
      while (node->next != NULL) {
        node->targetX = state->baseX;
        node->targetY = state->baseY;
        node->dx = -12;
        node = node->next;
      }
      node->targetX = state->baseX;
      node->targetY = state->baseY;
      node->dx = -12;
      while (node->targetX != node->x)
        WaitFrames(1);
      node = state->head;
      if (node != NULL) {
        ids = state->kinds;
        do {
          SelectionScreen_SetupIcon(ids[0], ids[16], node, 1);
          node = node->next;
          ids++;
        } while (node != NULL);
      }
      state->cursorIndex = 0;
      state->top = 0;
      node = state->head;
      y = node->x + 16;
      node = node->next;
      while (node != NULL) {
        node->targetX = y;
        node->dx = 12;
        node = node->next;
        y += 16;
      }
      state->records[1].kind = 1;
    } else if (state->cursorIndex == 4 && end + 1 < state->count) {
      state->cursorIndex--;
      state->records[1].base = 8;
      state->top++;
      Menu_ScrollSelectionList(state, 1);
      if (state->top + state->cursorIndex + 2 == state->count)
        state->records[1].kind = 0;
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex == state->count) {
    state->cursorIndex = 0;
  }
  state->status = 1;
  SelectionScreen_HighlightIcon(state, state->cursorIndex);
  WaitFrames(1);
  Menu_OpenSelectionWindow(state->head->kind, 0);
  WaitFrames(1);
}

// Moves the cursor one entry left, scrolling the row at its edge and
// wrapping from the first entry to the last.
void Menu_StepLeft(struct SelectionScreen *state) {
  struct SelectionNode *node;
  s32 i;

  SelectionScreen_RedrawIcon(state, state->cursorIndex);
  state->status = 33;
  WaitFrames(1);
  if (state->count > 5) {
    if ((state->cursorIndex | state->top) != 0) {
      if (state->cursorIndex == 1 && state->top != 0) {
        state->records[0].base = 8;
        state->top--;
        Menu_ScrollSelectionList(state, 0);
        if (state->top == 0)
          state->records[0].kind = 0;
        state->records[1].kind = 1;
      } else {
        state->cursorIndex--;
      }
    } else {
      node = state->head;
      i = 64;
      {
        /* FAKEMATCH: the zero goes through r0. 2026-10-11: CSE follows the beq
         * into this block and reuses the known-zero OR register for a plain
         * store (differ 28); only a loop-end note before the else label stops
         * it, and a fresh zero then takes r3 (differ 4); r0 needs the zero live
         * across the first loop, which no instruction-free second use gives (a
         * second store after the loop: differ 13). */
        register s32 z asm("r0") = 0;
        state->records[1].kind = z;
      }
      while (node->next != NULL) {
        node->targetX = node->x + i;
        node->dx = 12;
        node = node->next;
        i -= 16;
      }
      node = state->head;
      while (node->x != node->targetX)
        WaitFrames(1);
      i = 0;
      while (i != state->count - 5)
        i++;
      node = state->head;
      state->top = i;
      state->cursorIndex = 4;
      while (node != NULL) {
        SelectionScreen_SetupIcon(state->kinds[i], state->bases[i], node, 1);
        node = node->next;
        i++;
      }
      node = state->head;
      i = state->baseX;
      while (node->next != NULL) {
        node->targetX = i;
        node->dx = -12;
        node = node->next;
        i += 16;
      }
      state->records[0].kind = 1;
    }
  } else if (state->cursorIndex != 0) {
    state->cursorIndex--;
  } else {
    state->cursorIndex = state->count - 1;
  }
  state->status = 1;
  SelectionScreen_HighlightIcon(state, state->cursorIndex);
  WaitFrames(1);
  Menu_OpenSelectionWindow(state->head->kind, 0);
  WaitFrames(1);
}

// Redraws the command icon at row place index in its plain state.
void SelectionScreen_RedrawIcon(struct SelectionScreen *state, u32 index) {
  struct SelectionNode *node = state->head;
  u32 output;
  u32 value;

  while (index != 0) {
    index--;
    node = node->next;
  }
  if (node->kind == 1 || node->kind == 6) {
    u32 first = node->message - MsgCommandName;

    value = node->slot;
    Ui_BuildPairedPatternsToSlot(first, 0, &value, &output, 1);
  }
}

// Redraws the command icon at row place index and loads its large icon.
void SelectionScreen_HighlightIcon(struct SelectionScreen *state, u32 index) {
  struct SelectionNode *node = state->head;
  u32 res;
  u32 value;

  while (index != 0) {
    index--;
    node = node->next;
  }
  if (node->kind == 1 || node->kind == 6) {
    u32 id = node->message - MsgCommandName;

    value = node->slot;
    Ui_BuildPairedPatternsToSlot(id, 0, &value, &res, 1);
    SelectionScreen_LoadLargeIcon();
  }
}

// Tells the battle presentation how many icons the row shows.
void Menu_SendNodeCountList(struct SelectionScreen *screen) {
  u16 data[6];
  struct SelectionNode *node = screen->head;
  s32 count = 0;

  while (node != 0) {
    node = node->next;
    count++;
  }
  data[count] = 0xff;
  BattlePres_SetActorModesFar(data, 0);
}

// Scrolls the row one entry: forward brings an icon in from the right as the
// leftmost shrinks away, and back the reverse; waits until it has moved.
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward) {
  struct SelectionNode *p;
  struct SelectionNode *last;
  struct SelectionNode *first;
  s32 base;
  s32 kind;
  u32 index;
  s32 y;
  s32 z;

  if (forward != 0) {
    index = screen->top + SELECTION_ROWS;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = SelectionScreen_FindFreeIcon(0);
    if (p == NULL)
      return;
    SelectionScreen_SetupIcon(kind, base, p, 0);
    y = screen->baseX;
    p->x = y + 80;
    z = screen->baseY;
    p->targetX = y + 64;
    p->y = z;
    p->targetY = z;
    p->scaleStep = 32;
    p->scale = 32;
    p->scaleEnd = 256;
    p->dx = -2;
    last = p;
    p = screen->head;
    p->scaleStep = -32;
    p->scaleEnd = 0;
    for (;;) {
      p->targetX = p->x - 16;
      p->dx = -2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->next = last;
    last->next = NULL;
    last->prev = p;
    p = screen->head;
    do {
      WaitFrames(1);
    } while (p->scale != 0);
    screen->head = p->next;
    Resource_ResetEntry(p->slot);
    p->kind = 0;
    p = p->next;
    p->prev = NULL;
  } else {
    index = screen->top;
    base = screen->bases[index];
    kind = screen->kinds[index];
    p = SelectionScreen_FindFreeIcon(0);
    if (p == NULL)
      return;
    SelectionScreen_SetupIcon(kind, base, p, 0);
    p->x = screen->baseX - 16;
    p->y = screen->baseY;
    p->targetY = p->y;
    p->dx = 2;
    p->scale = 32;
    p->scaleStep = 32;
    p->targetX = p->x + 16;
    p->scaleEnd = 256;
    first = p;
    p = screen->head;
    p->prev = first;
    first->next = p;
    first->prev = NULL;
    screen->head = first;
    p = first;
    for (;;) {
      p->targetX = p->x + 16;
      p->dx = 2;
      if (p->next == NULL)
        break;
      p = p->next;
    }
    p->scaleEnd = 0;
    p->scaleStep = -32;
    p = screen->head;
    do {
      WaitFrames(1);
    } while (p->scale != 256);
    while (p->next != NULL)
      p = p->next;
    Resource_ResetEntry(p->slot);
    p->kind = 0;
    p->prev->next = NULL;
  }
}

// Loads the icon of an entry of kind mode (1 and 6 commands, 2 items, 4
// Psynergy, 9 fixed images) into a free sprite tile slot and returns it, or
// -1.
s32 Resource_LoadByMode(s32 mode, s32 value) {
  s32 output;
  s32 result = -1;

  switch (mode) {
  case 1:
  case 6:
    Ui_BuildPairedPatternsToSlot(value, 0, &result, &output, 0);
    break;
  case 2:
    result = Resource_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    UiIcon_CopyResourceToSlot(value, 0x1a, result);
    break;
  case 9:
    result = Resource_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return -1;
    RenderResource_LoadTableEntry(value, 0, result);
    break;
  case 4:
    Ability_LoadGlyph(value, 1, &result, &output, 0);
    break;
  }
  return result;
}

// Loads the icon of an entry of kind mode into sprite tile slot result (a free
// slot when -1) and returns the slot. Kind 7 is an item icon without its
// count, 8 one of the other icons.
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option) {
  s32 output;
  s32 original = result;

  if (result == -1) {
    result = Resource_FindFreeEntry();
    if (result == VRAM_CACHE_ENTRY_COUNT)
      return original;
  }

  switch (mode) {
  case 1:
  case 6:
    Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
    break;
  case 2:
    UiIcon_CopyResourceToSlot(value, 58, result);
    break;
  case 7:
    UiIcon_CopyResourceToSlot(value, 42, result);
    break;
  case 4:
    Ability_LoadGlyph(value, option, &result, &output, 1);
    break;
  case 8:
    Ui_BuildPatternToSlot(value, 0, result);
    break;
  case 9:
    RenderResource_LoadTableEntry(value, 0, result);
    break;
  }

  return result;
}

// Sets up a selection icon for an entry: its icon tiles (in its old slot when
// reuse is set), its name message and its sprite.
void SelectionScreen_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                               s32 reuse) {
  s32 src;
  s32 tile;
  struct SelectionSprite *obj;

  switch (kind) {
  case 1:
  case 6:
    if (reuse != 0)
      src = node->slot;
    Ui_BuildPairedPatternsToSlot(base, 0, &src, &tile, reuse);
    node->message = base + MsgCommandName;
    break;
  case 2:
    if (reuse != 0)
      src = node->slot;
    UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
    node->message = base + MsgItemName;
    break;
  case 4:
    if (reuse != 0)
      src = node->slot;
    Ability_LoadGlyph(base, 1, &src, &tile, reuse);
    node->message = base + MsgAbilityName;
    break;
  }

  node->base = base;
  node->slot = src;
  node->tile = tile;
  node->kind = kind;
  node->scale = 0x100;
  node->scaleEnd = 0x100;

  obj = &node->sprite;
  obj->mode = 0;
  obj->colors = 0;
  obj->mosaic = 0;

  obj->size = 1;
  obj->shape = 0;
  obj->palette = 0;
  obj->tile = tile;
  obj->priority = 0;
}

// Chooses the entry under the cursor: the other icons gather into it and
// vanish, and it joins the chosen icons at the top left. Returns the entry's
// index.
u32 Menu_ConfirmSelection(struct SelectionScreen *screen) {
  struct SelectionNode *node;
  struct SelectionNode *other = NULL;
  u32 cnt = 0;
  u32 index;
  struct SelectionSprite *sprite;

  index = screen->top + screen->cursorIndex;
  Menu_SendNodeCountList(screen);
  SelectionScreen_RedrawIcon(screen, screen->cursorIndex);
  screen->status = 33;
  WaitFrames(1);
  screen->records[0].kind = 0;
  screen->records[1].kind = 0;
  screen->records[14].kind = 0;
  screen->records[14].scale = 0;
  Resource_ResetPendingTransfer();
  node = screen->head;
  while (node != NULL && screen->cursorIndex != cnt) {
    node = node->next;
    cnt++;
  }
  node->homeX = node->x;
  node->homeY = node->y;
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      other->targetX = node->x;
      other->dx = (node->x - other->x) >> 1;
    }
  }
  WaitFrames(2);
  for (other = screen->head; other != NULL; other = other->next) {
    if (other != node) {
      Resource_ResetEntry(other->slot);
      other->kind = 0;
    }
  }
  screen->head = node;
  node->prev = NULL;
  node->next = NULL;
  node->targetX = 4;
  for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
    node->targetX += 16;
    cnt++;
  }
  screen->pathTop[cnt] = screen->top;
  screen->pathCursor[cnt] = screen->cursorIndex;
  node->dx = (node->targetX - node->x) >> 1;
  screen->unk39a = 0;
  screen->cursorIndex |= 0x80;
  WaitFrames(2);
  other = SelectionScreen_FindFreeIcon(1);
  other->kind = node->kind;
  other->message = node->message;
  other->base = node->base;
  other->slot = node->slot;
  other->tile = node->tile;
  other->x = node->x;
  other->y = node->y;
  other->targetX = other->x;
  other->targetY = other->y;
  other->homeX = node->homeX;
  other->homeY = node->homeY;
  other->dx = 0;
  other->dy = 0;
  other->scale = 0x100;
  other->scaleEnd = 0x100;
  sprite = &other->sprite;
  sprite->mode = 0;
  sprite->colors = 0;
  sprite->mosaic = 0;
  sprite->size = 1;
  sprite->shape = 0;
  sprite->palette = 0;
  sprite->tile = other->tile;
  node->kind = 0;
  screen->head = NULL;
  if (screen->path != NULL) {
    node = screen->path;
    while (node->next != NULL)
      node = node->next;
    node->next = other;
    other->prev = node;
    other->next = NULL;
  } else {
    screen->path = other;
    other->prev = NULL;
    other->next = NULL;
  }
  return index;
}

// Six empty functions.
void UiTextResource_NoOpCallback(void) {}
void UiTextResource_NoOpCallbackBank(void) {}
void UiTextResource_NoOpCallback1(void) {}
void UiTextResource_NoOpCallback2(void) {}
void UiTextResource_NoOpCallback3(void) {}
void UiTextResource_NoOpCallback4(void) {}

// Sets up a sprite as the right-pointing arrow, in a free tile slot (returned
// in *slot).
void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot) {
  const void *data = gMenu_CursorObjectTiles;
  s32 value = Resource_FindFreeEntry();

  *slot = value;
  object->oam.f.tile = VramCache_Load(value, 0x80, data);
  object->oam.f.mode = 0;
  object->oam.f.mosaic = 0;
  object->oam.f.colors = 1;
  object->oam.f.affine = 0;
  object->oam.f.affineIndex = 0;
  object->oam.f.size = 0;
  object->oam.f.shape = 2;
  object->oam.f.priority = 0;
}

// Places an arrow sprite at (x, y) and queues it for drawing.
void UiTextResource_SetPosition(struct MenuSprite *obj, s32 x, s32 y) {
  obj->oam.f.x = x;
  obj->oam.f.y = y;
  Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

// Frees an arrow sprite's tile slot.
void UiTextResource_Release(s32 value) { Resource_ResetEntry(value); }

// Loads the large icon of the command under the cursor, zooming in.
void SelectionScreen_LoadLargeIcon(void) {
  struct SelectionScreen *state = gResQueueWork;
  struct SelectionNode *selection = NodeChain_GetNodeAtCount(state);
  struct SelectionNode *transfer;
  GlyphTransfer *buffer;
  u8 *table;
  void *resource;
  s32 no;

  if (selection->kind != 1 && selection->kind != 6)
    return;

  buffer = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
  transfer = &state->records[15];
  no = selection->base;
  table = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  {
    u8 **destination = &buffer->encoded;
    resource = table + *(u16 *)(table + selection->base * 2);
    *destination = resource;
  }
  Resource_DecodeByteLz(resource, buffer);

  if (transfer->kind == 0)
    transfer->slot = Resource_FindFreeEntry();
  transfer->tile = VramCache_Load(transfer->slot, 0x400, buffer);
  transfer->kind = 1;
  transfer->base = no;
  transfer->scale = 40;
  transfer->scaleStep = 40;
  transfer->scaleEnd = 240;
  Runtime_ReleaseHeapBlock(17);
}

// Frees the large command icon, if it is shown.
void Resource_ResetPendingTransfer(void) {
  struct SelectionNode *transfer = &gResQueueWork->records[15];

  if (transfer->kind != 0) {
    Resource_ResetEntry(transfer->slot);
    transfer->kind = 0;
  }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define HAS_LOCALIZED_MENU_GUARD 1
#endif

// Runs the field menu until the player picks something that leaves it, and
// returns that choice's result.
s32 Menu_RunTopSelection(void) {
  s32 ret;
  s32 selection;
  u8 *state;

  state = (u8 *)gEventWork;
  selection = 0;

  for (;;) {
    UiWindow_OpenMode1AndWaitFrame();
    selection = Menu_SelectTopEntry(selection);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
      BattleFx_ClearRandomParticlesFar();
      WaitFrames(1);
    }
#endif
    ret = UiWork_CloseAndRelease();

    switch (selection) {
    case 0:
      ret = Object_GetTriggerTileAheadOfCurrentFar();
      if (ret == 0)
        ret = 0xff;
      ((struct EventWork *)state)->facingTrigger = ret;
      break;
    case 1:
      ret = Menu_OpenConfirmPromptFar();
      if (ret == -1)
        continue;
      break;
    case 2:
      ret = ItemMenu_RunFar();
      if (ret != 0)
        continue;
      break;
    case 3:
      ret = RunAssetSelectionScreenFar();
      if (ret == -1)
        continue;
      break;
    case 4:
      ret = Menu_OpenActionFlow();
      if (ret == -1)
        continue;
      break;
    default:
      break;
    }
    break;
  }

#if defined(HAS_LOCALIZED_MENU_GUARD)
  state[0xcca] = 0;
#endif
  return ret;
}

// Opens the field menu's window and waits a frame.
void UiWindow_OpenMode1AndWaitFrame(void) {
  UiWindow_CreateWithLayoutBounds(1);
  WaitFrames(1);
}

// Closes the window kept in heap block 16 and frees the block.
void UiWork_CloseAndRelease(void) { UiWork_FinalizeAndReleaseBlock16(); }

// Sets up an empty selection screen and waits a frame.
void SelectionScreen_CreateEmpty(void) {
  UiGlyph_ResetWorkState();
  Resource_ClearOwnerListAndCounters();
  WaitFrames(1);
}

// Runs a selection row with the cursor on entry value, cancellable, and
// returns the entry chosen or -1.
u32 Menu_RunSelectionForValue(u32 value) {
  struct SelectionScreen *state = gResQueueWork;
  u32 result;

  state->cursorIndex = value;
  state->locked = 1;
  MenuSelection_BuildEntries(value);
  Menu_SetupSelectionBothSides();
  Menu_OpenSelectionWindow(0, 5);
  Resource_ScheduleOwnerResetDelayed();
  result = Menu_WaitForSelectionInput(1);
  Resource_ResetOwnerEntries();
  return result;
}

// The area name notice's fields, which live in the event work block.
struct TimedNoticeWork {
  u8 eventWork[0x230]; // the event work's other fields
  struct UiWindow *window;
  u16 countdown;
};

// Shows the area's name in a window near the top of the screen for 90 frames.
void UiTimedNotice_Create(void) {
  struct TimedNoticeWork *work;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 resource;
  struct UiWindow *window;

  work = (struct TimedNoticeWork *)gEventWork;
  x = 8;
  y = 8;
  resource = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance) +
             RENDER_RESOURCE_BASE;
  UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
  x = (30 - width) >> 1;
  y = (10 - height) >> 1;
  window = UiWindow_Create(x, y, width, height, 2);
  work->window = window;
  UiText_DrawResource(resource, (s32)window, 0, 0);
  work->countdown = 90;
  Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}

// Counts the area name down, and closes it at 0.
void UiTimedNotice_Tick(void) {
  struct TimedNoticeWork *work;
  u16 count;

  work = (struct TimedNoticeWork *)gEventWork;
  work->countdown = (count = work->countdown + 0xffff);
  if ((count << 16) == 0) {
    UiWork_Finalize(work->window, 2);
    Scheduler_RemoveCallback(UiTimedNotice_Tick);
  }
}

// Closes the area name at once, if it is shown.
void UiTimedNotice_CloseIfActive(void) {
  struct UiWindow *window;

  window = ((struct TimedNoticeWork *)gEventWork)->window;
  if (window != NULL && window->flags != 0) {
    UiWork_Finalize(window, 2);
    Scheduler_RemoveCallback(UiTimedNotice_Tick);
  }
}

// Removes inventory entry arg1 and returns 0.
s32 Item_CallHandler48(s32 arg0, s32 arg1) {
  PartyInventory_RemoveFar(arg1);
  return 0;
}

// Returns 1.
s32 Item_ReturnTrue(void) { return 1; }

// Steps the window hue one step: back when arg0 has LEFT (0x20), on
// otherwise.
void OptionMenu_StepWindowHue(s32 arg0) {
  u8 value = gGameState.paletteGlow[0];
  if (arg0 & 0x20)
    value += 0xff;
  else
    value += 1;
  gGameState.paletteGlow[0] = value;
}

struct AbilityListEntry {
  u16 owner;
  u16 ability;
};

// Lets the player pick two Psynergy shortcuts from all the party's Psynergy,
// showing each one's cost and description, and stores them in
// gGameState.firstShortcut and secondShortcut (owner << 10 | Psynergy).
void Debug_SelectAbilityPair(void) {
  struct AbilityListEntry *list;
  struct RenderInput *win;
  struct RenderInput *title;
  struct RenderInput *info;
  struct RenderOutput *cursor;
  u32 first;
  u32 second;
  u32 count;
  s32 done = 0;
  s32 row = 0;
  s32 redraw;
  s32 slot;
  s32 y;
  s32 cost;
  s32 ability;

  cursor = NULL;
  redraw = 1;
  list = MemAlloc(0x700);
  Unit_LearnPsynergyFar(0, 140);
  Unit_LearnPsynergyFar(1, 140);
  Unit_LearnPsynergyFar(2, 140);
  Unit_LearnPsynergyFar(2, 141);
  Unit_LearnPsynergyFar(2, 78);
  Unit_LearnPsynergyFar(3, 93);
  Unit_LearnPsynergyFar(5, 140);
  second = 0;
  first = 0;
  count = Party_ListPsynergy(list);
  if (count != 0) {
    Menu_FindShortcutEntries(&first, &second, list);
    win = UiWindow_Create(4, 6, 20, 7, 2);
    title = UiWindow_Create(4, 3, 20, 3, 2);
    info = UiWindow_Create(4, 14, 20, 5, 2);
    slot = Resource_FindFreeEntry();
    if (slot != 0) {
      VramCache_Load(slot, 128, gResource_FixedBlockBTiles);
      cursor = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
    }
    /* FAKEMATCH: needed to match: the title message reuses the slot variable.
     * The message id passed directly: differ 23. */
    slot = MsgPanelHpMaxLabel;
    UiText_DrawCharacterAtOffset(slot, title, 16, 0);
    for (;;) {
      if (redraw != 0) {
        redraw = 0;
        first = (first + count) % count;
        second = (second + count) % count;
        row = (row + 2) % 2;
        y = (row << 4) + (win->y << 3) + 28;
        cursor->y = y;
        cursor->oam.y = y;
        RenderOutput_RedrawSavedRect(win);
        UiWindow_DrawDividerLine(win, 1, 2, 17, 2);
        UiText_DrawCharacterAtOffset(MsgPanelStatusLabel, win, 48, 0);
        UiText_DrawCharacterAtOffset(list[first].ability + MsgAbilityName, win,
                                     56, 16);
        UiText_DrawCharacterAtOffset(list[second].ability + MsgAbilityName, win,
                                     56, 32);
        UiText_DrawCharacterAtOffset(MsgPanelStatusLabel - 2, win, 16, 16);
        UiText_DrawCharacterAtOffset(MsgPanelStatusLabel - 1, win, 16, 32);
        UiText_DrawCharacterAtOffset(list[first].owner + MsgCharacterName, win,
                                     104, 16);
        UiText_DrawCharacterAtOffset(list[second].owner + MsgCharacterName, win,
                                     104, 32);
        RenderOutput_RedrawSavedRect(info);
        UiText_DrawCharacterAtOffset(MsgPsynergyNeedMorePp, info, 0, 16);
        if (row != 0) {
          cost = BattleActGet(list[second].ability)->ppCost;
          ability = list[second].ability;
        } else {
          cost = BattleActGet(list[first].ability)->ppCost;
          ability = list[first].ability;
        }
        UiText_DrawNumberAtOffset(cost, 2, info, 64, 16);
        UiText_DrawCharacterAtOffset(ability + MsgAbilityDescription, info, 0,
                                     0);
      }
      WaitFrames(1);
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(111);
        if (row != 0)
          second--;
        else
          first--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(111);
        if (row != 0)
          second++;
        else
          first++;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_UP) {
        AudioCommand_PlayFar(111);
        row--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_DOWN) {
        AudioCommand_PlayFar(111);
        row++;
        redraw = 1;
      }
      if (gKeysPressed & KEY_A)
        AudioCommand_PlayFar(112);
      else if (gKeysPressed & KEY_B)
        AudioCommand_PlayFar(113);
      else if (gKeysPressed & KEY_START)
        AudioCommand_PlayFar(113);
      else
        continue;
      /* FAKEMATCH: the unread flag forces fresh zeros for both entries and the
       * cursor argument instead of reusing the row register. Without the store,
       * also without the flag: differ 4. */
      done = 1;
      break;
    }
    gGameState.firstShortcut = (list[first].owner << 10) | list[first].ability;
    gGameState.secondShortcut =
        (list[second].owner << 10) | list[second].ability;
    UiWork_Finalize(win, 1);
    UiWork_Finalize(title, 1);
    UiWork_Finalize(info, 1);
    WaitFrames(1);
  }
  MemFree(list);
}

// Lists every Psynergy of the party's active members as (owner, Psynergy)
// pairs in output and returns how many.
s32 Party_ListPsynergy(struct AbilityListEntry *output) {
  u16 objectIds[14];
  s32 outputCount = 0;
  s32 objectCount = Party_ListActiveOwnersFar(objectIds);
  s32 i;

  for (i = 0; i < objectCount; i++) {
    struct BattleUnit *object;
    u32 id;
    s32 index;
    u32 resourceId;

    id = objectIds[i];
    object = UnitGetFar(id);
    for (index = 0; index < (s32)(sizeof(object->actionSlots) /
                                  sizeof(object->actionSlots[0]));
         index++) {
      resourceId = object->actionSlots[index].encodedAction & ACTION_ID_MASK;
      if (resourceId == 0)
        break;
      BattleActGet(resourceId);
      output[outputCount].owner = id;
      output[outputCount].ability = resourceId;
      outputCount++;
    }
  }

  return outputCount;
}

// Finds the two Psynergy shortcuts in a list from Party_ListPsynergy, and
// returns their indexes (0 for one not found).
void Menu_FindShortcutEntries(u32 *firstIndex, u32 *secondIndex,
                              const struct AbilityListEntry *entries) {
  s32 i;

  *firstIndex = 0;
  *secondIndex = 0;

  for (i = 0; i <= 447; i++) {
    if (entries[i].ability == (gGameState.firstShortcut & 0x3ff) &&
        entries[i].owner == (gGameState.firstShortcut >> 10)) {
      *firstIndex = i;
      break;
    }
  }

  for (i = 0; i <= 447; i++) {
    if (entries[i].ability == (gGameState.secondShortcut & 0x3ff) &&
        entries[i].owner == (gGameState.secondShortcut >> 10)) {
      *secondIndex = i;
      break;
    }
  }
}

// Runs the Psynergy shortcut picker.
void Menu_RunSelectionHook(void) { Debug_SelectAbilityPair(); }

// The work block of heap slot 19: two icon flags and resources and a window,
// among fields nothing here uses.
struct SelectionWorkspace {
  u8 unk000[0x46];
  u16 firstActive;
  u16 firstResource;
  u8 unk04a[0x352 - 0x4a];
  u16 secondActive;
  u16 secondResource;
  u8 unk356[0xff4 - 0x356];
  struct UiWindow *window;
  u8 unkff8[12];
};

// Allocates the work block of heap slot 19 and clears its two icon flags.
void Menu_AllocateSelectionWorkspace(void) {
  struct SelectionWorkspace *work;
  u32 zero;

  work = Runtime_AllocateBlock(0x13, 0x1004);
  zero = 0;
  work->firstActive = zero;
  work->secondActive = zero;
}

// Closes the window of heap slot 19's work block, frees its two icons and
// frees the block.
void Ui_FinalizeWorkAndReleaseHeap13(void) {
  struct SelectionWorkspace *state;

  state = *(struct SelectionWorkspace **)gSelectionWorkspace;
  UiWork_Finalize(state->window, 0);
  while (UiWork_IsIdle(state->window) == 0) {
    WaitFrames(1);
  }
  if (state->firstActive != 0) {
    Resource_ResetEntry(state->firstResource);
  }
  if (state->secondActive != 0) {
    Resource_ResetEntry(state->secondResource);
  }
  Runtime_ReleaseHeapBlock(0x13);
}

// Three empty functions.
void GraphicsPalette_ReservedNoOpC9BC(void) {}
void GraphicsPalette_ReservedNoOpC9C0(void) {}
void GraphicsPalette_ReservedNoOpC9C4(void) {}

// One of the seven entries of the option screen's work from offset 0x400;
// only its state is known.
struct MenuWorkspaceEntry {
  u8 unk00[10];
  u16 state;
  u8 unk0c[40];
};

// Clears the option screen's seven entry states, its row, its tint and its
// cursor animation.
void OptionMenu_ResetEntries(struct WorkspaceWork *work) {
  struct MenuWorkspaceEntry *entry =
      (struct MenuWorkspaceEntry *)((u8 *)work + 0x400);
  u16 zero = 0;

  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  entry++;
  entry->state = zero;
  work->page = zero;
  work->tint[0] = zero;
  work->tint[1] = zero;
  work->tint[2] = zero;
  work->cursorFrame = zero;
}

// Works out the window tint (red, green and blue, 0 to 31) from the window
// colour settings: the hue picks a point on the colour wheel and the
// brightness shifts all three channels.
void WindowColor_ComputeTint(struct WorkspaceWork *work) {
  s16 phase;
  s32 bias;
  s32 blue;
  s32 red;
  s32 green;

  phase = (gGameState.paletteGlow[0] + 12) % 24 * 4;
  bias = gGameState.paletteGlow[1] - 7;
  red = gPaletteGlow_WaveTable[(phase % 96)];
  green = gPaletteGlow_WaveTable[(phase + 32) % 96];
  blue = gPaletteGlow_WaveTable[(phase + 64) % 96];
  red += bias;
  green += bias;
  blue += bias;
  if (red < 0)
    red = 0;
  if (green < 0)
    green = 0;
  if (blue < 0)
    blue = 0;
  if (red > 31)
    red = 31;
  if (green > 31)
    green = 31;
  if (blue > 31)
    blue = 31;
  work->tint[0] = red;
  work->tint[1] = green;
  work->tint[2] = blue;
}

// Writes window colours 4 to 10 of BG palette 15, each a fixed scale of the
// window tint.
void WindowColor_WritePalette(struct WorkspaceWork *work) {
  *(s16 *)0x050001E8 = WindowColor_ScaleTint(work, 0xEEEE, 0xCCCC, 0x11110);
  *(s16 *)0x050001EA = WindowColor_ScaleTint(work, 0xD555, 0xBBBB, 0xEEEE);
  *(s16 *)0x050001EC = WindowColor_ScaleTint(work, 0xBBBB, 0xAAAA, 0xCCCC);
  *(s16 *)0x050001EE = WindowColor_ScaleTint(work, 0xA221, 0x9999, 0xAAAA);
  *(s16 *)0x050001F0 = WindowColor_ScaleTint(work, 0x10888, 0xDDDD, 0x13333);
  *(s16 *)0x050001F2 = WindowColor_ScaleTint(work, 0x12221, 0xEEEE, 0x15555);
  *(s16 *)0x050001F4 = WindowColor_ScaleTint(work, 0x13BBB, 0x10000, 0x17777);
}

// Returns the window tint with each channel scaled (16.16 fixed point) and
// clamped to 0 to 31, as a BGR555 colour.
u16 WindowColor_ScaleTint(struct WorkspaceWork *work, s32 scale_r, s32 scale_g,
                          s32 scale_b) {
  s32 r;
  s32 g;
  s32 b;

  r = Iwram_MulQ16(work->tint[0] << 16, scale_r) >> 16;
  g = Iwram_MulQ16(work->tint[1] << 16, scale_g) >> 16;
  b = Iwram_MulQ16(work->tint[2] << 16, scale_b) >> 16;
  if (r < 0)
    r = 0;
  if (g < 0)
    g = 0;
  if (b < 0)
    b = 0;
  if (r > 31)
    r = 31;
  if (g > 31)
    g = 31;
  if (b > 31)
    b = 31;
  return r + ((b << 10) + (g << 5));
}

// Returns rgb with each channel scaled (16.16 fixed point) and clamped to 0 to
// 31, as a BGR555 colour.
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale,
                          s32 blue_scale) {
  s32 red;
  s32 green;
  s32 blue;

  red = Iwram_MulQ16(rgb[0] << 16, red_scale) >> 16;
  green = Iwram_MulQ16(rgb[1] << 16, green_scale) >> 16;
  blue = Iwram_MulQ16(rgb[2] << 16, blue_scale) >> 16;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  return red + ((blue << 10) + (green << 5));
}

// Sets the window colours (BG palette 15, colours 4 to 10) for a window hue
// (0 to 23) and brightness (0 to 14, 7 neutral).
void PaletteGlow_Update(s32 phase, s32 brightness) {
  s16 rgb[3];
  s16 step;
  s16 red;
  s16 green;
  s16 blue;
  s16 offset;

  step = ((phase + 12) % 24) * 4;
  offset = brightness - 7;
  red = gPaletteGlow_WaveTable[(step % 96)] + offset;
  green = gPaletteGlow_WaveTable[(step + 32) % 96] + offset;
  blue = gPaletteGlow_WaveTable[(step + 64) % 96] + offset;
  if (red < 0)
    red = 0;
  if (red > 31)
    red = 31;
  if (green < 0)
    green = 0;
  if (green > 31)
    green = 31;
  if (blue < 0)
    blue = 0;
  if (blue > 31)
    blue = 31;
  rgb[0] = red;
  rgb[1] = green;
  rgb[2] = blue;
  BG_PLTT_COLOR(15, 4) = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
  BG_PLTT_COLOR(15, 5) = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
  BG_PLTT_COLOR(15, 6) = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
  BG_PLTT_COLOR(15, 7) = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
  BG_PLTT_COLOR(15, 8) = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
  BG_PLTT_COLOR(15, 9) = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
  BG_PLTT_COLOR(15, 10) = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}

// Moves the three-row settings cursor up, wrapping to the bottom.
void OptionMenu_PreviousRow(struct WorkspaceWork *work) {
  if (work->page == 0)
    work->page = 2;
  else
    work->page--;
}

// Moves the three-row settings cursor down, wrapping to the top.
void OptionMenu_NextRow(struct WorkspaceWork *work) {
  if (++work->page > 2)
    work->page = 0;
}

// Lowers the setting on the three-row cursor's row: message speed, window hue
// or window brightness.
void OptionMenu_LowerSetting(struct WorkspaceWork *work) {
  switch (work->page) {
  case 0:
    if (gGameState.messageSpeed != 0)
      gGameState.messageSpeed--;
    break;
  case 1:
    if (gGameState.paletteGlow[0] != 0)
      gGameState.paletteGlow[0]--;
    break;
  case 2:
    if (gGameState.paletteGlow[1] != 0)
      gGameState.paletteGlow[1]--;
    break;
  }
}

// Raises the setting on the three-row cursor's row: message speed, window hue
// or window brightness.
void OptionMenu_RaiseSetting(struct WorkspaceWork *work) {
  switch (work->page) {
  case 0:
    if (gGameState.messageSpeed <= 1)
      gGameState.messageSpeed++;
    break;
  case 1:
    if (gGameState.paletteGlow[0] <= 23)
      gGameState.paletteGlow[0]++;
    break;
  case 2:
    if (gGameState.paletteGlow[1] <= 14)
      gGameState.paletteGlow[1]++;
    break;
  }
}

// Does nothing.
void GraphicsPalette_SelectionNoOp(void) {}

// The option screen's per-frame callback: moves the cursor and the two slider
// markers, animates the marker of the slider in use, and pulses the chosen
// setting's icon.
void OptionMenu_AnimateCursors(void) {
  s32 src0;
  s32 src1;
  s32 selection;
  struct WorkspaceWork *work;

  work = gSelectionWork;
  selection = work->page;
  ShopCursor_AdvanceFar(&work->cursor.cursor);
  ShopCursor_MoveTowardTargetFar(&work->marker[0].cursor);
  ShopCursor_MoveTowardTargetFar(&work->marker[1].cursor);

  if (selection == 0) {
    src0 = (work->cursorFrame & 7) + (s32)&ResourceId_ShopCursorFrames;
  } else {
    src0 = (s32)&ResourceId_ShopCursorFrames;
  }
  VramCache_Load((u8)work->marker[0].output->index, 0x100,
                 Resource_GetTableEntry(src0));

  if (selection == 1) {
    src1 = (work->cursorFrame & 7) + (s32)&ResourceId_ShopCursorFrames;
  } else {
    src1 = (s32)&ResourceId_ShopCursorFrames;
  }
  VramCache_Load((u8)work->marker[1].output->index, 0x100,
                 Resource_GetTableEntry(src1));

  if (selection > 1)
    Ui_ApplyTableScaleToObject(
        work->frame[selection - 2][work->option[selection]]);
  work->cursorFrame++;
}

// Allocates the option screen's work block, copies the settings into it with
// how many values each takes, and starts its per-frame callback.
void OptionMenu_InitializeWork(void) {
  struct WorkspaceWork *work;

  work = Runtime_AllocateBlock(20, 0x628);
  Dma_Fill32(0, work, 0x628);
  work->option[0] = gGameState.paletteGlow[0];
  work->optionCount[0] = 24;
  work->option[1] = gGameState.paletteGlow[1];
  work->optionCount[1] = 15;
  work->option[2] = gGameState.messageSpeed;
  work->optionCount[2] = 3;
  work->option[3] = gGameState.speech;
  work->optionCount[3] = 2;
  work->option[4] = gGameState.idleSleepEnabled;
  work->optionCount[4] = 2;
  Scheduler_AddOrUpdateCallback(OptionMenu_AnimateCursors, 0xc80);
}

// Stops the option screen's per-frame callback and frees its work block.
void OptionMenu_Release(void) {
  Scheduler_RemoveCallback(OptionMenu_AnimateCursors);
  Runtime_ReleaseHeapBlock(0x14);
}

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96

// Opens the option screen's window: the labels, the hue and brightness
// sliders with their markers, the cursor and the setting icons. Returns the
// window.
struct RenderInput *OptionMenu_Open(void) {
  /* FAKEMATCH: the shared cursor union keeps pointer stores before window
   * reads; an equal-size struct reorders these moves in all six editions. */
  struct WorkspaceWork *work;
  struct RenderInput *win;
  struct RenderOutput *out;
  s32 x;
  s32 y;
  s32 msg;

  work = gSelectionWork;
  win = UiWindow_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
  UiWindow_DrawDividerLine(win, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
  UiWindow_DrawDividerLine(win, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
  UiWindow_DrawDividerLine(win, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
  UiWindow_DrawDividerLine(win, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
  msg = MsgWindowColorLabel;
  UiText_DrawCharacterAtOffset(msg, win, 8, 0);
  msg++;
  UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
  msg = MsgMessageSpeedLabel;
  UiText_DrawCharacterAtOffset(msg, win, 8, 32);
  msg++;
  UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
  UiText_DrawCharacterAtOffset(MsgMessageSpeedLabel, win, 8, 40);
#endif
  UiText_DrawCharacterAtOffset(MsgSpeechLabel, win, 8, 64);
  UiText_DrawCharacterAtOffset(MsgAutoSleepLabel, win, 8, 88);

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 128, gResource_FixedBlockBTiles);
    out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
    work->cursor.output = out;
    x = win->x * 8;
    y = win->y * 8 + 12;
    ShopCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    Dma_Set(gWorkspaceOptions_SliderPalette,
            (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), 0x80000020, REG_DMA3);
    VramCache_Load(x, 256, gWorkspaceOptions_SliderTiles);
    y = 0;
    out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
    out->oam.palette = SLIDER_PALETTE;
    out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
    out->oam.index += 4;
    out->oam.palette = SLIDER_PALETTE;
    y = 16;
    out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
    out->oam.palette = 15;
    out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
    out->oam.index += 4;
    out->oam.palette = 15;
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
    out->oam.palette256 = 1;
    work->marker[0].output = out;
    x = win->x * 8 + OPTION_SLIDER_X;
    x += work->option[0] * 60 / work->optionCount[0];
    y = win->y * 8 + 4;
    ShopCursor_SetPositionImmediateFar(&work->marker[0].cursor, x, y);
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
    out->oam.palette256 = 1;
    work->marker[1].output = out;
    x = win->x * 8 + OPTION_SLIDER_X;
    x += work->option[1] * 60 / work->optionCount[1];
    y = win->y * 8 + 20;
    ShopCursor_SetPositionImmediateFar(&work->marker[1].cursor, x, y);
  }

  y = 28;
  work->frame[0][0] = RenderResource_CreateFrame(gOptionMenu_SpeedFrames[0], 0,
                                                 win, OPTION_FRAME_X, y);
  work->frame[0][1] = RenderResource_CreateFrame(gOptionMenu_SpeedFrames[1], 0,
                                                 win, OPTION_FRAME_X + 24, y);
  work->frame[0][2] = RenderResource_CreateFrame(gOptionMenu_SpeedFrames[2], 0,
                                                 win, OPTION_FRAME_X + 48, y);
  y = 52;
  work->frame[1][0] =
      RenderResource_CreateFrame(gOptionMenu_SpeechFrames[0], 0, win, 100, y);
  work->frame[1][1] =
      RenderResource_CreateFrame(gOptionMenu_SpeechFrames[1], 0, win, 124, y);
  y = 76;
  work->frame[2][0] =
      RenderResource_CreateFrame(gOptionMenu_SleepFrames[0], 0, win, 100, y);
  work->frame[2][1] =
      RenderResource_CreateFrame(gOptionMenu_SleepFrames[1], 0, win, 124, y);
  return win;
}

// Runs the option screen (window colour and brightness, message speed,
// speech, auto-sleep) until A or START keeps the settings (returns 0) or B
// drops them (returns -1). SELECT cycles through the colour presets.
s32 Menu_RunWorkspaceOptions(void) {
  struct WorkspaceWork *work;
  struct RenderInput *icon;
  struct RenderInput *win;
  struct RenderOutput *pair;
  s8 *pA;
  s8 *pB;
  s32 redraw;
  s32 page;
  s32 i;
  s32 dim;
  s32 index;
  s32 x;
  s32 y;
  s32 result;
  struct RenderOutput *frame;

  redraw = 1;
  page = 0;
  OptionMenu_InitializeWork();
  work = gSelectionWork;
  win = UiWindow_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, 2);
  icon = OptionMenu_Open();
  pair = RenderResource_CreatePair(7, icon, 64, -48);
  WaitFrames(1);
  pA = &work->option[0];
  pB = &work->option[1];

  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      page = (page + 5) % 5;
      work->option[page] = (work->option[page] + work->optionCount[page]) %
                           work->optionCount[page];
      work->page = page;
      if (gSleepDisabled != 0)
        work->option[4] = 0;

      for (i = 0; i <= 2; i++) {
        frame = work->frame[0][i];
        frame->sentinel = 0xfb;
        UiIcon_PrepareObjectFar(frame);
        index = (u8)work->frame[0][i]->index;
        dim = 0;
        if (i != work->option[2])
          dim = 1;
        RenderResource_LoadFrame(gOptionMenu_SpeedFrames[i], index, dim);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->frame[1][i];
        frame->sentinel = 0xfb;
        UiIcon_PrepareObjectFar(frame);
        index = (u8)work->frame[1][i]->index;
        dim = 0;
        if (i != work->option[3])
          dim = 1;
        RenderResource_LoadFrame(gOptionMenu_SpeechFrames[i], index, dim);
      }
      for (i = 0; i <= 1; i++) {
        frame = work->frame[2][i];
        frame->sentinel = 0xfb;
        UiIcon_PrepareObjectFar(frame);
        index = (u8)work->frame[2][i]->index;
        dim = 0;
        if (i != work->option[4])
          dim = 1;
        RenderResource_LoadFrame(gOptionMenu_SleepFrames[i], index, dim);
      }

      x = icon->x * 8 + OPTION_SLIDER_X;
      x += (*pA * 60) / work->optionCount[0];
      y = icon->y * 8 + 4;
      Shop_SetCursorFar(&work->marker[0].cursor, x, y, 1);
      x = icon->x * 8 + OPTION_SLIDER_X;
      x += (*pB * 60) / work->optionCount[1];
      y = icon->y * 8 + 20;
      Shop_SetCursorFar(&work->marker[1].cursor, x, y, 1);

      x = work->option[2] + MsgMessageSpeedSetting;
      UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END,
                                  48);
      UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
      x = work->option[3] + MsgSpeechSetting;
      UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64,
                                  OPTION_SETTING_END, 72);
      UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
      x = work->option[4] + MsgAutoSleepSetting;
      UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88,
                                  OPTION_SETTING_END, 96);
      UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
      PaletteGlow_Update(*pA, *pB);

      x = icon->x * 8;
      y = (page * 3 + icon->y) * 8 + 4;
      if (page == 0)
        y += 8;
      Shop_SetCursorFar(&work->cursor.cursor, x, y, 3);
      RenderOutput_ClearList(win);
      UiText_DrawResource(page + MsgOptionHelp, win, 0, 0);
    }
    Ui_ApplyTableOffsetToPair(pair);
    WaitFrames(1);

    if (gKeysPressed & KEY_SELECT) {
      AudioCommand_PlayFar(112);
      work->preset++;
      redraw = 1;
      if (work->preset > 5)
        work->preset = 0;
      *pA = gOptionMenu_HuePresets[work->preset];
      *pB = gOptionMenu_BrightnessPresets[work->preset];
      continue;
    }
    if (gKeysPressed & (KEY_A | KEY_START)) {
      result = 0;
      AudioCommand_PlayFar(112);
      break;
    }
    if (gKeysPressed & KEY_B) {
      result = -1;
      AudioCommand_PlayFar(113);
      break;
    }
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(111);
      page--;
      redraw = 1;
    } else if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(111);
      page++;
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(111);
        work->option[page]--;
        redraw = 1;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(111);
        work->option[page]++;
        redraw = 1;
      }
    }
  }

  UiWork_Finalize(win, 2);
  UiWork_Finalize(icon, 2);
  if (result == 0) {
    gGameState.paletteGlow[0] = work->option[0];
    gGameState.paletteGlow[1] = work->option[1];
    gGameState.messageSpeed = work->option[2];
    gGameState.speech = work->option[3];
    gGameState.idleSleepEnabled = work->option[4];
    gIdleSleepEnabled = gGameState.idleSleepEnabled;
  } else {
    PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  }
  OptionMenu_Release();
  WaitFrames(1);
  return result;
}

#define FLAG_WORKSPACE_SETTINGS_ONLY 0x17e
#define WORKSPACE_ROWS 3
#define WORKSPACE_DEBUG_ROWS 3
#define WORKSPACE_ROW_HEIGHT 24
#define WORKSPACE_TEXT_X 48

// The system menu's per-frame callback: moves the cursor and pulses the
// chosen entry's icon.
void SystemMenu_AnimateCursor(void) {
  struct WorkspaceWork *work = gSelectionWork;
  u32 index;

  ShopCursor_AdvanceFar(&work->cursor.cursor);
  index = work->page;
  Ui_ApplyTableScaleToObject(work->icon[index]);
}

// Allocates the system menu's work block and starts its per-frame callback.
void SystemMenu_Initialize(void) {
  void *work;
  work = Runtime_AllocateBlock(20, 0x628);
  Dma_Fill32(0, work, 0x628);
  Scheduler_AddOrUpdateCallback(SystemMenu_AnimateCursor, 3200);
}

// Stops the system menu's per-frame callback and frees its work block.
void SystemMenu_Release(void) {
  Scheduler_RemoveCallback(&SystemMenu_AnimateCursor);
  Runtime_ReleaseHeapBlock(0x14);
}

// Opens the system menu's window with its entries (Save your quest, Sleep,
// Change settings, and the debug entries in debug mode; only Change settings
// while flag 0x17e is set), their icons and the cursor. Returns the window.
struct RenderInput *SystemMenu_Open(void) {
  struct WorkspaceWork *work;
  s32 rows;
  s32 first;
  s32 settingsOnly;
  s32 top;
  s32 height;
  struct RenderInput *win;
  s32 y;
  s32 slot;
  s32 msg;
  s32 i;
  s32 x;

  rows = WORKSPACE_ROWS;
  work = gSelectionWork;
  settingsOnly = GameFlag_TestFar(FLAG_WORKSPACE_SETTINGS_ONLY);
  first = 0;
  if (settingsOnly != 0) {
    rows = 1;
    first = 2;
  }
  if (gDebugMode != 0)
    rows += WORKSPACE_DEBUG_ROWS;
  top = 8 - rows;
  height = rows * 3 + 1;
  if (top + height > 19) {
    top = 1;
    height = 19;
  }
  win = UiWindow_Create(5, top, 20, height, 2);
  for (i = 1; i < rows; i++)
    UiWindow_DrawDividerLine(win, 0, i * 3, 19, i * 3);
  y = 4;
  if (settingsOnly == 0) {
    msg = MsgWorkspaceSaveQuest;
    UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
    msg++;
    UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
    y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
  }
  UiText_DrawResource(MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
  y += WORKSPACE_ROW_HEIGHT;
  if (gDebugMode != 0) {
    msg = MsgWorkspaceRoughMenu;
    UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
    y += WORKSPACE_ROW_HEIGHT;
    UiText_DrawResource(msg + 1, win, WORKSPACE_TEXT_X, y);
    y += WORKSPACE_ROW_HEIGHT;
    msg += 2;
    UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
  }
  slot = Resource_FindFreeEntry();
  if (slot < RESOURCE_SLOTS) {
    VramCache_Load(slot, 128, gResource_FixedBlockBTiles);
    work->cursor.output = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
    x = win->x * 8;
    y = win->y * 8 + 16;
    ShopCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
  }
  y = -4;
  for (i = 0; i < rows; i++) {
    work->icon[i] = RenderResource_CreateFrame(
        gMenu_WorkspaceIconFrames[first + i], 0, win, 12, y);
    y += WORKSPACE_ROW_HEIGHT;
  }
  return win;
}

// Runs the system menu until the player picks an entry (its number, counting
// Save your quest as 0) or cancels with B or START (-1).
s32 Menu_RunWorkspaceSelectionLoop(void) {
  struct RenderInput *window;
  s32 redraw;
  struct RenderOutput *pair;
  s32 first;
  s32 i;
  s32 j;
  s32 rows;
  s32 settingsOnly;
  s32 dim;
  s32 y;
  s32 result;
  s32 page;
  s32 index;
  s32 x;
  s32 top;
  s8 *table;
  struct RenderOutput *icon;
  struct WorkspaceWork *work;

  redraw = 1;
  rows = 3;
  settingsOnly = GameFlag_TestFar(0x17E);
  first = 0;
  SystemMenu_Initialize();
  work = gSelectionWork;
  window = SystemMenu_Open();
#if EDITION_INTERNATIONAL
  y = -0x18;
  if (gDebugMode != 0) {
    y = -0x10;
  }
  pair = RenderResource_CreatePair(6, window, 0x28, y);
#else
  pair = RenderResource_CreatePair(6, window, 0x28, -0x18);
#endif
  WaitFrames(1);
  page = work->page;
  if (settingsOnly != 0) {
    rows = 1;
    first = 2;
  }
  if (gDebugMode != 0) {
    rows += 3;
  }
  for (;;) {
    if (redraw != 0) {
      redraw = 0;
      page = (page + rows) % rows;
      work->page = page;
      i = 0;
      if (i < rows) {
        j = first;
        table = gMenu_WorkspaceIconFrames;
        do {
          icon = work->icon[i];
          icon->sentinel = 0xFB;
          UiIcon_PrepareObjectFar(icon);
          index = (u8)icon->index;
          dim = 0;
          if (i != work->page) {
            dim = 1;
          }
          RenderResource_LoadFrame(*(s8 *)(j + (s32)table), index, dim);
          i++;
          j++;
        } while (i < rows);
      }
      top = window->y;
      x = window->x * 8;
      y = (((page * 3) + top) * 8) + 0x10;
      Shop_SetCursorFar(&work->cursor.cursor, x, y, 3);
    }
    Ui_ApplyTableOffsetToPair(pair);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      result = page;
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    } else if (gKeysPressed & (KEY_B | KEY_START)) {
      result = -1;
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    } else {
      if (gKeysRepeat & KEY_UP) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        page -= 1;
        redraw = 1;
      } else if (gKeysRepeat & KEY_DOWN) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        page += 1;
        redraw = 1;
      }
      continue;
    }
    break;
  }
  UiWork_Finalize(window, 2);
  SystemMenu_Release();
  WaitFrames(1);
  if (result >= 0) {
    result += first;
  }
  return result;
}

// Draws window tile pattern over the tile of tilemap entry *entry, with its
// colours mapped through remap (0 leaves a pixel as it was). A shared tile
// first gets a free tile of its own, which entry and mirror then point to.
void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
                                 u8 *remap) {
  struct UiRenderWork *work = UI_WORK;
  u8 pixels[128];
  u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  u32 slot = *(u8 *)entry;
  u8 *src;
  u8 *dst;
  u32 n;
  u32 i;

  dst = pixels;
  src = (u8 *)(0x06000000 + 32 * slot);
  for (i = 0; i < 32; i++) {
    n = *src++;
    *dst++ = n & 15;
    *dst++ = n >> 4;
  }

  dst = pixels;
  src = sheet + pattern * 32;
  for (i = 0; i < 32; i++) {
    u32 value = *src++;
    u32 color = remap[value & 15];

    if (color != 0)
      *dst = color;
    dst++;
    color = remap[value >> 4];
    if (color != 0)
      *dst = color;
    dst++;
  }

  {
    u8 *p;

    dst = pixels;
    for (i = 0, p = dst; i < 32; i++) {
      u32 value = p[0] | p[1] << 4;

      p += 2;
      *dst++ = value;
    }
  }

  if ((s8)slot >= 0) {
    for (n = 0; n < 128; n++) {
      u32 value = work->tileCursor;
      u32 following = (value + 1) % 128;

      slot = (u8)value;
      work->tileCursor = following;
      if (work->tileAttributes[slot] == 0)
        break;
    }
    work->tileAttributes[slot] = 1;
    slot |= 0x80;
    *entry = slot | 0xf000;
    *mirror = slot | 0xf000;
  }
  Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}

// Draws a string of 16-bit character codes with the window font into tiles
// of their own, starting x pixels into the first tile, and points tiles (and
// the same entries in mirror) at them. Colour codes change the text colour.
// Returns how many tiles the text covers.
s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x) {
  /* FAKEMATCH: needed to match: an unread 32-byte reserve, the native frame's
   * extra bytes (the frame is 36 bytes without it). */
  u32 reserve[8];
  u8 pens[16];
  struct UiRenderWork *work;
  u8 *canvas;
  u8 *glyphs;
  u32 attr;
  u32 *src;
  u8 *dst;
  u32 c;
  u32 code;
  s32 count;
  u32 w;
  s32 i;
  s32 j;
  u32 bits;

  work = UI_WORK;
  canvas = MemAlloc(0x800);
  glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  attr = work->level << 12;
  Iwram_ClearWords(pens, 16);
  if (attr == 0xF000) {
    pens[1] = gUiText_ColourPens[work->colour & 15];
    pens[3] = 3;
    Iwram_FillWords(canvas, 0x800, 0x04040404);
  } else {
    pens[1] = (u8)work->colour & 15;
    pens[3] = 1;
    Iwram_FillWords(canvas, 0x800, 0x0E0E0E0E);
  }
  if (script != 0) {
    for (;;) {
      c = *script++;
      code = c;
      if (c == 0)
        break;
      if (c <= 30) {
        switch (c) {
        case 8:
          work->colour = *script++;
          pens[1] = gUiText_ColourPens[work->colour & 15];
          break;
        case 7:
        case 9:
        case 10:
          work->colour = 15;
          pens[1] = gUiText_ColourPens[work->colour];
          break;
        case 3:
#if EDITION_INTERNATIONAL
          x += gUiText_GlyphWidths[0];
#else
          x += 8;
#endif
          break;
        case 14:
        case 15:
        case 28:
          script++;
        case 11:
        case 12:
        case 17:
#if EDITION_INTERNATIONAL
        case 29:
#endif
          script++;
          break;
        }
      } else {
        c &= 0xFF;
#if !EDITION_INTERNATIONAL
        if (c == 222 || c == 223) {
          x -= 3;
          c += 32;
        }
#endif
        src = (u32 *)(glyphs + c * 32);
        dst = canvas + x;
        for (i = 0; i < 8; i++) {
          bits = *src++;
          for (j = 0; j < 4; j++) {
            u8 pen = pens[bits & 15];

            if (pen != 0)
              *dst = pen;
            dst++;
            bits >>= 4;
            pen = pens[bits & 15];
            if (pen != 0)
              *dst = pen;
            dst++;
            bits >>= 4;
          }
          dst += 248;
        }
        if (code == 0xF01D || code == 0xF01F)
          x += 8;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
        else if (code == 0xF01E || code == 0xF005)
#else
        else if (code == 0xF01E)
#endif
          x += 3;
        else {
#if !EDITION_INTERNATIONAL
          if (c > 128) {
            if (c == 254 || c == 255)
              w = 3;
            else
              w = 8;
          } else
#endif
              if (c > 31)
            w = gUiText_GlyphWidths[c - 32];
          else
            w = 1;
          x += w;
        }
      }
    }
  }

  count = (u32)(x + 7) >> 3;
  src = (u32 *)canvas;
  dst = canvas;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < count; j++) {
      c = src[0];
      bits = src[1];
      c |= c << 4;
      bits |= bits >> 4;
      c &= 0x0FF00FF0;
      bits &= 0x00FF00FF;
      c |= c << 8;
      bits |= bits >> 8;
      src += 2;
      *(u32 *)dst = (c << 4) >> 16 | bits << 16;
      dst += 4;
    }
    dst = dst - count * 4 + 0x100;
    src = (u32 *)((u8 *)src - count * 8 + 0x100);
  }

  for (i = 0; i < count; i++) {
    u32 mask = 127;
    u32 tile;
    u32 *p;

    if (work->alt != 0)
      mask = 255;
    tile = *tiles & 0x3FF;
    if ((tile < 0x80 || tile > 0xFF) &&
        (work->alt == 0 || tile < 0x200 || tile > 0x27F)) {
      tile = work->tileCursor & mask;
      for (j = 0; j <= mask && work->tileAttributes[tile] != 0; j++)
        tile = (tile + 1) & mask;
      work->tileCursor = (tile + 1) & mask;
      work->tileAttributes[tile] = 1;
      if (tile > 127)
        tile += 0x180;
      else
        tile |= 0x80;
      *tiles = attr | tile;
      *mirror = attr | tile;
    }
    p = (u32 *)(0x06000000 + tile * 32);
    *p++ = ((u32 *)canvas)[i];
    *p++ = ((u32 *)canvas)[i + 0x40];
    *p++ = ((u32 *)canvas)[i + 0x80];
    *p++ = ((u32 *)canvas)[i + 0xC0];
    *p++ = ((u32 *)canvas)[i + 0x100];
    *p++ = ((u32 *)canvas)[i + 0x140];
    *p++ = ((u32 *)canvas)[i + 0x180];
    *p++ = ((u32 *)canvas)[i + 0x1C0];
    tiles++;
    mirror++;
  }
  MemFree(canvas);
  return count;
}

// Marks the text tiles under a tilemap rectangle (x, y, width and height in
// tiles) as no longer shown, for reuse.
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height) {
  u8 *base = gWindowWork[0];
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
  u32 alternate = ((struct UiRenderWork *)base)->alt;
  u32 row;

  for (row = 0; row < height; row++) {
    u32 column;

    for (column = 0; column < width; column++) {
      u32 tile = *cursor++ & 0x3FF;

      if ((tile - 0x80) <= 0x7F ||
          (alternate != 0 && tile > 0x1FF && tile <= 0x27F)) {
        u32 index = ((tile & 0xFF) ^ 0x80);
        ((struct UiRenderWork *)base)->tileAttributes[index] &= 0xFC;
      }
    }
    cursor += 32 - width;
  }
}

// Marks the text tiles the tilemap shows, and frees those it no longer shows.
void UiWindow_MarkVisibleTileAttributes(void) {
  u8 *base = gWindowWork[0];
  u16 *cursor = (u16 *)base;
  s32 width = 30;
  u32 alternate = ((struct UiRenderWork *)base)->alt;
  s32 row;
  s32 x;

  for (row = 20; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tile = *cursor++ & 0x3ff;

      if ((tile - 0x80) <= 0x7f ||
          (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
        u32 index = ((tile & 0xff) ^ 0x80);

        ((struct UiRenderWork *)base)->tileAttributes[index] |= 2;
      }
    }
  }

  for (row = 0; row < 256; row++, base++) {
    if (((struct UiRenderWork *)base)->tileAttributes[0] == 1)
      ((struct UiRenderWork *)base)->tileAttributes[0] = 0;
  }
}

// Frees every text tile, and with flag set lets text tiles take the second
// tile range (0x200 to 0x27f) too.
void UiWork_SetAltFlagAndClearTable(s32 flag) {
  s32 i;
  s32 j;
  s8 *p;
  s8 *q;
  u8 *work;

  work = gWindowWork[0];
  if (flag != 0) {
    ((struct UiRenderWork *)work)->alt = 1;
    flag = 0;
    for (i = 0x80,
        p = (s8 *)((struct UiRenderWork *)work)->tileAttributes + 0x80;
         i <= 0xFF; i += 1) {
      *p = flag;
      p += 1;
    }
    return;
  }
  ((struct UiRenderWork *)work)->alt = 0;
  flag = 0;
  q = (s8 *)((struct UiRenderWork *)work)->tileAttributes + 0x80;
  j = 0x7F;
  do {
    j -= 1;
    *q = flag;
    q += 1;
  } while (j >= 0);
}

// Does nothing.
void UiRender_ReservedNoOp(void) {}

// Draws a straight divider line inside a window from tile (x1, y1) to
// (x2, y2), horizontal or vertical: its ends join the frame or the lines they
// meet, and it crosses the lines it passes over.
void UiWindow_DrawDividerLine(struct RenderInput *win, u32 x1, u32 y1, u32 x2,
                              u32 y2) {
  struct UiRenderWork *work = UI_WORK;
  u16 *cursor;
  u32 pos;
  u32 tile;

  if (x1 == x2) {
    if (y1 == y2)
      return;
    if (y1 > y2) {
      pos = y1;
      y1 = y2;
      y2 = pos;
    }
    UiWindow_ClearTileAttributesInRect(win->x + x2, win->y + y1, 1, y2 - y1);
    cursor =
        (u16 *)((((win->y + y1) << 5) + win->x + x2) * 2 + (u32)work->tilemap);
    for (pos = y1; pos <= y2; pos++) {
      tile = *cursor;
      if (pos == y1) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_TOP_TEE:
          break;
        case UI_TILE_FRAME_TOP:
          tile = UI_TILE_FRAME_TOP_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_UP:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_HORIZONTAL:
          tile = UI_TILE_DIVIDER_TEE_DOWN;
          break;
        default:
          tile = UI_TILE_DIVIDER_VERTICAL;
          break;
        }
      } else if (pos == y2) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_BOTTOM_TEE:
          break;
        case UI_TILE_FRAME_BOTTOM:
          tile = UI_TILE_FRAME_BOTTOM_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_DOWN:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_HORIZONTAL:
          tile = UI_TILE_DIVIDER_TEE_UP;
          break;
        default:
          tile = UI_TILE_DIVIDER_VERTICAL;
          break;
        }
      } else {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
          break;
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_HORIZONTAL:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        default:
          tile = UI_TILE_DIVIDER_VERTICAL;
          break;
        }
      }
      *cursor = tile;
      cursor += 32;
    }
  } else if (y1 == y2) {
    if (x1 == x2)
      return;
    if (x1 > x2) {
      pos = x1;
      x1 = x2;
      x2 = pos;
    }
    UiWindow_ClearTileAttributesInRect(win->x + x1, win->y + y1, x2 - x1, 1);
    cursor =
        (u16 *)((((win->y + y1) << 5) + win->x + x1) * 2 + (u32)work->tilemap);
    for (pos = x1; pos <= x2; pos++) {
      tile = *cursor;
      if (pos == x1) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_LEFT_TEE:
          break;
        case UI_TILE_FRAME_LEFT:
          tile = UI_TILE_FRAME_LEFT_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_LEFT:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_VERTICAL:
          tile = UI_TILE_DIVIDER_TEE_RIGHT;
          break;
        default:
          tile = UI_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      } else if (pos == x2) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_RIGHT_TEE:
          break;
        case UI_TILE_FRAME_RIGHT:
          tile = UI_TILE_FRAME_RIGHT_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_RIGHT:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_VERTICAL:
          tile = UI_TILE_DIVIDER_TEE_LEFT;
          break;
        default:
          tile = UI_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      } else {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_CROSS:
          break;
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_VERTICAL:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        default:
          tile = UI_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      }
      *cursor = tile;
      cursor++;
    }
  }
}

// Sets the text colour (0 to 15) of drawn text.
void UiWork_SetParamNibble(s32 param) {
  struct UiRenderWork *work = UI_WORK;

  work->colour = param & 15;
}

// Sets the text style (UiRenderWork.outline) of drawn text.
void UiWork_SetRenderWord(u16 value) {
  struct UiRenderWork *work = UI_WORK;

  work->outline = value;
}

// Decodes a message and draws it into a text canvas window (passed as an
// integer) at pixel (x, y).
void UiText_DrawResource(s32 message, s32 window, s32 x, s32 y) {
  struct UiRenderWork *canvas = UI_WORK;
  u16 *counter = &canvas->count;
  s32 zero = 0;

  *counter = zero;
  UiText_BuildRenderEntries(message, 1);
  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & RENDER_ENTRY_MASK;
  UiText_RenderWideStringAtOffset(canvas->entries, (struct UiWindow *)window, x,
                                  y);
}

// Decodes a message and draws it as tiles in a window at pixel (offsetX,
// offsetY) of its interior.
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window,
                                  u32 offsetX, u32 offsetY) {
  s32 byteOffset;
  s32 vramAddress;
  u16 *text;
  s32 zero;
  u32 cell;
  struct UiRenderWork *canvas;
  u16 *counter;

  canvas = UI_WORK;
  counter = &canvas->count;
  zero = 0;
  *counter = zero;
  UiText_BuildRenderEntries(message, 1);

  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & RENDER_ENTRY_MASK;

  cell = ((window->y + (offsetY >> 3) + 1) << 5) +
         (window->x + (offsetX >> 3)) + 1;
  if (cell < 32 * 20) { // inside the tilemap
    byteOffset = cell * 2;
    vramAddress = byteOffset + (s32)UI_TILEMAP_VRAM;
    text = canvas->entries;
    UiText_RenderStringTiles(text, (u16 *)((u8 *)canvas->tilemap + byteOffset),
                             (u16 *)vramAddress, 7 & offsetX);
  }
}

// Draws a byte string into a text canvas window (passed as an integer) at
// pixel (x, y).
void UiText_DrawString(u8 *text, s32 window, s32 x, s32 y) {
  s16 *buffer;
  s16 *output;
  u8 *input;

  input = text;
  buffer = Runtime_BumpAllocateAlternatePool(0x200);
  output = buffer;
  if (*input != 0) {
    do {
      *output = *input;
      input++;
      output++;
    } while (*input != 0);
  }
  *output = 0;
  UiText_RenderWideStringAtOffset((u16 *)buffer, (struct UiWindow *)window, x,
                                  y);
  MemFree(buffer);
}

// Draws a byte string as tiles in window work at pixel (offsetX, offsetY) of
// its interior.
void UiText_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX,
                               s32 offsetY) {
  s16 *buffer;
  s16 *output;
  struct UiRenderWork *canvas;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = Runtime_BumpAllocateAlternatePool(0x200);
  canvas = UI_WORK;
  output = buffer;
  if (*text != 0) {
    do {
      *output = *text;
      text++;
      output++;
    } while (*text != 0);
  }
  *output = 0;

  cell = ((work->y + ((u32)offsetY >> 3) + 1) << 5) +
         (work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 32 * 20) { // inside the tilemap
    vram = (s32)UI_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    UiText_RenderStringTiles((u16 *)buffer, (u16 *)src, (u16 *)dst, phase);
    MemFree(buffer);
  }
}

// Draws a byte string into a window's tilemap at pixel (x, y) of its
// interior, rounded down to tiles.
void UiText_DrawStringInWindow(u8 *text, s32 window, u32 x, u32 y) {
  u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  *output = 0;
  x >>= 3;
  y >>= 3;
  UiText_RenderWideStringInWindow((s16 *)buffer, (struct UiWindow *)window, x,
                                  y);
  MemFree(buffer);
}

// Draws a number (width as UiText_FormatNumber) into a text canvas window at
// pixel (x, y).
void UiText_DrawNumber(s32 value, s32 width, s32 window, s32 x, s32 y) {
  u8 data[16];

  UiText_DrawString(UiText_FormatNumber(data, value, width), window, x, y);
}

// Draws a number (width as UiText_FormatNumber) as tiles in a window at pixel
// (x, y) of its interior.
void UiText_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *window,
                               s32 x, s32 y) {
  u8 data[16];

  UiText_DrawStringAtOffset(UiText_FormatNumber(data, value, width), window, x,
                            y);
}

// Draws a number (width as UiText_FormatNumber) into a window's tilemap at
// pixel (x, y) of its interior.
void UiText_DrawNumberInWindow(s32 value, s32 width, s32 window, s32 x, s32 y) {
  u8 data[16];

  UiText_DrawStringInWindow(UiText_FormatNumber(data, value, width), window, x,
                            y);
}

// Draws a four-digit number after a two-glyph sign (window tiles 0x1d and
// 0x1e, or 0x1f and 0x1e with variant set) as tiles in window work at pixel
// (offsetX, offsetY) of its interior.
void UiText_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work,
                                       s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[16];
  s16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct UiRenderWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  canvas = UI_WORK;
  text = UiText_FormatNumber(formatted, value, 4);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
  if (variant == 0) {
    output[0] = 0xf01d;
    output[1] = 0xf01e;
  } else {
    output[0] = 0xf01f;
    output[1] = 0xf005;
  }
#else
  if (variant == 0) {
    output[0] = 0xf01d;
  } else {
    output[0] = 0xf01f;
  }
  output[1] = 0xf01e;
#endif
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;

  cell = ((work->y + ((u32)offsetY >> 3) + 1) << 5) +
         (work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 32 * 20) { // inside the tilemap
    vram = (s32)UI_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    UiText_RenderStringTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
