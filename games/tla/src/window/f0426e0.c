#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

extern const s8 gWindow_PartyColumnOffsets[];

void Func_080426e0(struct Window *window, u32 flags) {
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 first = 1;
  s32 bias = 0;
  u32 max = window->width - 1;
  s32 i;
  u8 state = canvas->menuState;
  u32 rows = window->height;
  u32 col;
  u32 row;
  u16 *dest;
  const s8 *table;

  if ((flags & 1) == 0)
    flags &= ~2;
  if (flags & 2) {
    bias = 5;
    first = 0;
  }
  {
    /* FAKEMATCH(temporary): the table's address passes through a block-local
     * copy set before i; as table = gWindow_PartyColumnOffsets after
     * i = first, 41 of 137 instructions differ in tla-en. */
    const s8 *columns = gWindow_PartyColumnOffsets;

    i = first;
    table = columns;
  }
  if (gWindow_PartyColumnOffsets[i] >= 0) {
    do {
      col = table[i] + bias;
      if (col < max) {
        for (row = 0; row != rows; row++) {
          dest = canvas->tilemap[(s16)window->y + row] + ((s16)window->x + col);
          if (row == 0)
            *dest = 0xf018;
          else if (row == rows - 1)
            *dest = 0xf019;
          else
            *dest = 0xf00f;
        }
      }
      i++;
      table = gWindow_PartyColumnOffsets;
    } while (gWindow_PartyColumnOffsets[i] >= 0);
    state = canvas->menuState;
  }
  {
    /* FAKEMATCH(barrier): the state reaches its test through an empty asm
     * into a word local; as if (state) with no asm, 11 of 137 instructions
     * differ in tla-en. */
    u32 shown;

    asm("" : "=r"(shown) : "0"(state));
    if (shown) {
      dest =
          canvas->tilemap[(s16)window->y + window->height - 1] + (s16)window->x;
      col = 1;
      *dest++ = 0xf080;
      while (col < max) {
        col++;
        *dest++ = 0xf081;
      }
      *dest = 0xf082;
    }
  }
  canvas->dirty = 1;
}
