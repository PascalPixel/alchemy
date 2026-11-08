#include "types.h"
#include "ram_buffer.h"
#include "edition.h"

// The rows one page of the list shows: Japanese fits six, the other editions
// five.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ROWS 6
#else
#define PAGE_ROWS 5
#endif

struct MenuOwnerWork {
  u8 unk000[0x214];
  u8 count;
  u8 unk215;
  u8 owners[0x264 - 0x216];
  s8 selected[8];
};

struct MenuPageResult {
  void *owner;
  s32 unk04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 count;
  s32 selected;
};

void *Unit_Get(s32 owner);

s32 Func_080ff7b4(struct MenuPageResult *result, s32 index) {
  struct MenuOwnerWork *menu = Ram_HeapSlots->menuRuntime;
  void *owner;
  s32 count;
  s32 value;
  s32 page;
  s32 row;
  s32 pages;

  owner = Unit_Get(menu->owners[index]);
  count = menu->count;
  value = menu->selected[menu->owners[index]];

  if (value + 1 > count)
    value = count - 1;
  if (count == 0)
    value = 0;
  page = value / PAGE_ROWS;
  row = value % PAGE_ROWS;
  pages = count / PAGE_ROWS;
  if (count % PAGE_ROWS != 0)
    pages++;
  result->owner = owner;
  result->page = page;
  result->pageCount = pages;
  result->row = row;
  result->count = count;
  result->selected = value;
  return 1;
}
