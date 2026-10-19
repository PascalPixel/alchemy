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
#include "mem.h"
#include "ram_buffer.h"
#include "dma.h"
#include "ramcode.h"

extern u8 Tile_BuildMetatilesCode[];
extern u8 Tile_BuildMetatilesCodeSize[];

/// Fills the 0x140-byte row buffer with blank tiles, or builds it from the
/// window tiles' map through the metatile routine copied to RAM.
void UiWindow_FillFromScene(void *dst) {
  u8 *scene = Ram_HeapSlots->windowTiles;

  if (scene == 0) {
    /* FAKEMATCH: the fill value is pinned to the r3 it takes, which loads it
       from the pool before the buffer's address is formed; a plain Dma_Fill16
       forms the address first (one reordered pair, differ 2). */
    register u16 value asm("r3") = 0xe0e0;

    Dma_Fill16(value, dst, 0x140);
  } else {
    void (*code)(void *, void *);

    RamCode_Load(code, Tile_BuildMetatilesCode, Tile_BuildMetatilesCodeSize);
    code(dst, scene + 8);
    /* FAKEMATCH: the barrier keeps the second argument's add ahead of the
       first argument's copy; without it they trade places (differ 2). */
    asm("");
    MemFree(code);
  }
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width,
                                  u32 height, s32 value) {
  u32 row = 0;
  s16 *dst = (s16 *)0x06002000;

  if (row < height) {
    do {
      u32 column = 0;
      if (column < width) {
        do {
          column++;
          *dst = value;
          dst++;
        } while (column < width);
      }
      row++;
      dst += 32 - width;
    } while (row < height);
  }
}

struct UiWindowBounds {
  s32 handle;
  u16 left;
  u16 top;
  u16 right;
  u16 height;
  u16 flags;
};

s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

void UiWindow_BuildLayoutBounds(s32 flags) {
  struct UiWindowBounds *state = Ram_HeapSlots->windowLayout;
  u8 *base = Ram_HeapSlots->windowTiles;
  s32 height = 4;
  s32 n;
  s32 right;
  s32 left;

  if (base[5] != 0) {
    n = BattleParty_PrepareActiveOwnersFar(0);
    height = 3;
  } else {
    n = Party_CountActiveOwnersFar();
    if ((u32)n > 4)
      n = 4;
  }
  if (flags & 1)
    height++;
  else
    flags &= -3;

  n *= 6;
  right = n + 1;
  if (flags & 2)
    right = n + 6;

  left = 30;
  left -= right;
  state->left = left;
  state->top = 0;
  state->right = right;
  state->height = height;
  state->flags = flags;
}

void UiWindow_CreateWithLayoutBounds(s32 flags) {
  s32 zero;
  struct UiWindowBounds *window;
  u8 *tiles;

  window = Runtime_AllocateBlock(64, sizeof(struct UiWindowBounds));
  tiles = Ram_HeapSlots->windowTiles;
  zero = 0;
  tiles[6] = 1;
  UiWindow_BuildLayoutBounds(flags);
  window->handle = UiWindow_Create(window->left, window->top, window->right,
                                   window->height, 6);
  PartyStatusDraw(flags);
  tiles[6] = zero;
}
