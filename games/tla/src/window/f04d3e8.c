#include "heap_release.h"
#include "types.h"
#include "resource.h"
#include "resource_menu.h"
#include "ram_buffer.h"

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = Ram_HeapSlots->menuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  width += 2;
  menu->width = width;
  menu->resourceBase = resourceBase;
  menu->row = row;
  /* FAKEMATCH(barrier): an empty asm tying width's register and the row
   * field after the row store; as the plain stores, 2 of 75 instructions
   * differ in tla-en. */
  asm("" : "+r"(width), "+m"(menu->row));
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct ResourceMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = Window_Create(x, row, menu->width, 3, 2);
}
