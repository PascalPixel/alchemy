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

// Lays the entries out in a row three cells apart from x, on row y, and
// opens the menu's window after them.
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
  /* FAKEMATCH: the empty asm that reads the row back and passes the work
     pointer on makes the count's address wait for the row's store, as the
     native does; plainly the count's address (movs r7, #142) is started
     first (one reordered pair, differ 2). The width is read back through
     a pointer taken before it, which the asm leaves alone (differ 43
     through the work pointer). Measured against it: operand-tied asms
     with a memory clobber on x (differ 2), y (20) and the work pointer
     (43). */
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

// The Japanese function differs and remains scaffolded (S4).
#if (LANGUAGE != LANGUAGE_JAPANESE)
s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
  UiWindow_OpenMode1AndWaitFrame();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  arg1 = Menu_SelectResource(arg0, arg1 - 1);
  Menu_EndResourceSelection();
  UiWork_CloseAndRelease();
  return arg1;
}

extern s16 gData_020036d8;
extern s16 gData_02005350;
extern s8 gMenu_SaveSlotActionByPosition[];

s32 Func_080432a4(void);

// Offers the actions the save records allow (plus two optional entries
// behind EWRAM flags) and maps the chosen position to an action, as ☀️'s
// Menu_SelectSaveSlotAction. The Lost Age's second optional entry is 31, and
// with the first option off, action 4 becomes 5. The Japanese function
// differs too and remains scaffolded (S4).
s32 Menu_SelectSaveSlotAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = Func_080432a4();
  if (type < 0)
    return -1;
  if (type == 0)
    return 0;
  if (type == 3)
    group = 1;
  else if (type == 103)
    group = 2;
  else if (type > 100)
    group = 3;
  else
    initial = 1;
  AffineEffect_InitializeWork();
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(21);
  if (group <= 1)
    Menu_AppendResourceEntry(22);
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(23);
  Menu_AppendResourceEntry(24);
  if (gData_020036d8 != 0)
    Menu_AppendResourceEntry(29);
  if (gData_02005350 != 0)
    Menu_AppendResourceEntry(31);
  Menu_CenterResourceEntries(17, 7, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0) {
    u32 twice = group * 2;

    /* FAKEMATCH: the one-pass block makes the table's pool load wait for
       group * 2, as the native issues it between the shift and the add;
       written plainly as ret + group * 6 the load comes after the add
       (differ 2), and without the block the split index measures the
       same. */
    do {
    } while (0);
    ret = gMenu_SaveSlotActionByPosition[ret + (twice + group) * 2];
  }
  if (gData_020036d8 == 0 && ret == 4)
    ret = 5;
  return ret;
}
#endif
