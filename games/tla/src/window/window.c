/// @file
/// The window sprite pool, setting up the window system, uploading its
/// tilemap and opening windows: the functions The Broken Seal's window object
/// opens with (its window/windows.c), in the same order.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "heap_release.h"
#include "window.h"
#include "uiwindow.h"
#include "task.h"
#include "dma.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "ram_buffer.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

void Window_DrawBorder(s32 x, s32 y, s32 width, s32 height);
void Func_0803a2b0(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Text_ResetStyle(s32 x);
void Window_DrawOpened(struct Window *window);
void Window_WaitZoom(void *work);

/// Removes and returns the first entry of the free list, or NULL if it is
/// empty.
struct WindowSprite *WindowSprite_AcquireFree(void) {
  struct WindowWork *work = UI_WORK;
  struct WindowSprite *out = work->freeOutputs.head;

  if (out != NULL) {
    if (out->next == NULL)
      work->freeOutputs.tailLink = &work->freeOutputs.head; // Now empty.
    work->freeOutputs.head = out->next;
    out->next = NULL;
  }
  return out;
}

/// Puts an entry back at the free list's tail; entries outside outputs[] are
/// ignored.
void WindowSprite_ReleaseFree(struct WindowSprite *out) {
  struct WindowWork *work = UI_WORK;

  if ((u32)out >= (u32)work->outputs &&
      (u32)out < (u32)&work->freeOutputs.head) {
    struct WindowSprite **tail = work->freeOutputs.tailLink;

    work->freeOutputs.tailLink = &out->next;
    *tail = out;
    out->next = NULL;
  }
}

/// Link all 64 outputs[] entries into the free list.
void WindowSprite_InitializeFreeList(void) {
  s32 n;
  struct WindowWork *work;
  struct WindowSprite *out;
  struct WindowSprite *next;

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

typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size,
                                u32 value) {
  /* FAKEMATCH: a direct call loads the fill value before the routine and
   * reverses pool order, as in ☀️'s Japanese build. */
  return fill(dst, size, value);
}

void WindowSystem_InitField(void) {
  struct WindowWork *work;

  work = (struct WindowWork *)HeapSlot_AllocEwram(
      60, sizeof(struct WindowWork));
  Iwram_ClearWords(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->level = 15;
  FillWords(Iwram_FillWords, work->tilemap, sizeof(work->tilemap), 0xf000f000);
  WindowSprite_InitializeFreeList();
  Portrait_ClearSlots();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartFieldUpdate();
}

void WindowSystem_InitBattle(s32 kind) {
  struct WindowWork *work;
  s32 i;
  s32 WindowSystem_CopyTileLowerRows(u32 src, u32 dst) {
    src &= 0x3ff;
    dst &= 0x3ff;
    Dma_Set((void *)(0x06000010 + src * 32), (void *)(0x06000000 + dst * 32),
            DMA_COPY_HALFWORDS(8), REG_DMA3);
    return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
  }

  work = (struct WindowWork *)HeapSlot_AllocEwram(
      60, sizeof(struct WindowWork));
  Iwram_ClearWords(work, sizeof(struct WindowWork));
  work->dirty = 1;
  work->glyphResource = 99;
  work->menuState = 1;
  work->level = 15;
  Iwram_FillWords(work->tilemap, sizeof(work->tilemap), 0xf000f000);
  WindowSprite_InitializeFreeList();
  Task_Add((s32)WindowSystem_UploadTilemap, TASK_PASS_FRAME);
  WindowSystem_StartBattleUpdate(kind);
  WindowSystem_CopyTileLowerRows(0xf013, 128);
  WindowSystem_CopyTileLowerRows(0xf014, 129);
  WindowSystem_CopyTileLowerRows(0xf015, 130);
  {
    u8 mode = 4;

    for (i = 2; i >= 0; i--)
      work->tileAttributes[i] = mode;
  }
}

void WindowSystem_UploadTilemap(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 flags;
  u8 *src;
  u8 *dst;

  flags = work->dirty;
  if (flags != 0) {
    if (!work->menuBusy) {
      src = (u8 *)work->tilemap;
      dst = (u8 *)0x06002000;
      if (flags & 1)
        flags = 63;
      flags &= 63;
      flags >>= 1;
      do {
        if (flags & 1)
          Dma_Set(src, dst, DMA_COPY_WORDS(64), REG_DMA3);
        flags >>= 1;
        src += 256;
        dst += 256;
      } while (flags);
      work->dirty = flags;
    }
  }
}

void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height) {
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor = &canvas->tilemap[y][x];
  s32 tile;
  u32 bottom;
  u32 row;

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

  Window_ClearTextTilesInRect(x, y, width, height);

  row = 0;
  if (row < height) {
    do {
      u32 column;

      if (canvas->menuState != 0) {
        if ((u32)(y + row) > 16)
          tile = 0xF07F;
        else
          tile = 0xF000;
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

static inline void Window_FillWords(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to the
   * fixed entry, 10 of 68 instructions differ in tla-en. */
  Iwram_FillWordsEntry(destination, size, value);
}

/// Draws a window as its flags ask: with flag 8 its inside is filled (with
/// colour 4 under flag 0x20, which also draws the border, else cleared), and
/// without it only the border is drawn. Clears the window's duration.
void Window_DrawOpened(struct Window *window) {
  u32 flags;
  s32 x;
  s32 y;
  u32 width;
  u32 height;
  struct WindowWork *work;

  work = UI_WORK;
  x = (s16)window->x;
  y = (s16)window->y;
  width = window->width;
  height = window->height;
  flags = window->flags;
  window->duration = 0;
  if (8 & flags) {
    if (0x20 & flags) {
      Window_DrawBorder(x, y, width, height);
      Window_FillWords((void *)0x06002500, 0xF00, 0x44444444);
    } else {
      Window_FillWords((void *)0x06002500, 0xF00, 0);
    }
    Func_0803a2b0(x, y, width, height, 0);
  } else {
    Window_DrawBorder(x, y, width, height);
  }
  work->dirty = 1;
}

/// Opens the first free window at x, y with width and height; attrs bits 8,
/// 32, 64, 128 and 0x100 become window flags, and bit 2 draws it at once
/// instead of waiting for it to open.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs) {
  struct Window *slot;
  struct Window *found;
  s32 i;

  slot = UI_WORK->windows;
  found = 0;
  for (i = 0; i != UI_WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & 1) == 0 && slot->duration == 0) {
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
    found->flags = 1;
    Text_ResetStyle(x);
    if (attrs & 8) {
      found->flags |= 8;
    }
    if (attrs & 32) {
      found->flags |= 32;
    }
    if (attrs & 64) {
      found->flags |= 64;
    }
    if (attrs & 128) {
      found->flags |= 128;
    }
    if (attrs & 0x100) {
      found->flags |= 0x100;
    }
    if (attrs & 2) {
      found->flags |= 2;
      found->frame = 0;
      found->duration = 1;
      Window_DrawOpened(found);
    } else {
      found->duration = 8;
      found->frame = 7;
      Window_WaitZoom(found);
      wait(1);
    }
  }
  return found;
}

void Window_WaitZoom(void *work) {
  if (!(2 & ((struct Window *)work)->flags) &&
      (((struct Window *)work)->duration != 0)) {
    do {
      wait(1);
    } while (((struct Window *)work)->duration != 0);
  }
}

#undef UI_WORK
