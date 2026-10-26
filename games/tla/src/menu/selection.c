#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "tla_edition.h"

#include "types.h"
#include "ram_buffer.h"
#include "resource_menu.h"
#include "edition.h"

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);

void Menu_CenterResourceEntries(s32, s32, s32);

/// Lays the entries out in a row three cells apart from x, on row y, and
/// opens the menu's window after them.
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *work;
  s32 i;
  struct ResourceMenuEntry *entry;
  s16 *widthp;

  work = Ram_HeapSlots->menuSelectWork;
  widthp = &work->width;
  *widthp = width + 2;
  work->resourceBase = resourceBase;
  work->row = y;
  /* FAKEMATCH: the empty asm reading the row back makes the count's address
     wait for the row's store; without it, 2 instructions differ in
     tla-en. */
  asm("" : "+r"(work) : "m"(work->row));
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = UiWindow_Create(x, y, *widthp, 3, 2);
}

static __inline__ s32 TableGet(s8 *table, s32 index) { return table[index]; }

s32 Menu_SelectTopEntry(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 triple;
  s32 ret;
  s8 *table;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  triple = group * 3;
  table = gMenu_TopEntryPositionByCommand;
  offset = triple << 1;
  selection = TableGet(table, selection + offset) - 1;
  if (selection < 0) {
    selection = 0;
  }

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  if (group == 0) {
    Menu_AppendResourceEntry(15);
  }
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (ret >= 0) {
    s8 *commands = gMenu_TopEntryCommandByPosition;
    s32 index;

    /* FAKEMATCH: ⚓️ loads the table's address before forming the index */
    asm volatile("" : "+l"(commands));
    index = ret + offset + 1;
    ret = commands[index];
  }

  return ret;
}

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
  UiWindow_OpenMode1AndWaitFrame();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  // Only Japanese centres the entries before selecting.
#if LANGUAGE == LANGUAGE_JAPANESE
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
#endif
  arg1 = Menu_SelectResource(arg0, arg1 - 1);
  Menu_EndResourceSelection();
  UiWork_CloseAndRelease();
  return arg1;
}
