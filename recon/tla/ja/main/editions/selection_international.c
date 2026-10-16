#include "tla_edition.h"
#undef SELECT_MENU_WIDTH
#define SELECT_MENU_WIDTH 7
#include "types.h"
#include "ram_buffer.h"
#include "tla_edition.h"

s32 UiWindow_Create(s32, s32, s32, s32, s32);

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h) {
  u8 *state;
  s32 i;

  state = Ram_HeapSlots->menuSelectWork;

  *(u16 *)(state + 144) = (u16)((u32)w + 2);
  *(u16 *)(state + 146) = (u16)h;
  *(u16 *)(state + 148) = (u16)y;

  for (i = 0; i < *(s16 *)(state + 142); i++) {
    *(u16 *)(state + i * 20 + 14) = (u16)((u32)y << 3);
    *(u16 *)(state + i * 20 + 12) = (u16)((u32)x << 3);
    x = (s32)((u32)x + 3);
  }

  *(s32 *)(state + 120) = UiWindow_Create(x, y, *(s16 *)(state + 144), 3, 2);
}

extern s8 gMenu_TopEntryPositionByCommand[];
extern s8 gMenu_TopEntryCommandByPosition[];

s32 Party_SumDjinnCountsFar(s32);
void *AffineEffect_InitializeWork(void);
void Menu_AppendResourceEntry(s32 arg0);
void Menu_CenterResourceEntries(s32, s32, s32);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_EndResourceSelection(void);

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

    asm volatile(""
                 : "+l"(commands)); /* FAKEMATCH: ⚓️ loads the table's address
                                       before forming the index */
    index = ret + offset + 1;
    asm volatile(
        ""
        : "+l"(index)); /* FAKEMATCH: and indexes it with the whole sum */
    ret = commands[index];
  }

  return ret;
}
