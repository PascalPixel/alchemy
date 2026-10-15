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
#include "resmenu.h"

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);


void Menu_CenterResourceEntries(s32, s32, s32);

// Lays the entries out in a row three cells apart from x, on row y, and
// opens the menu's window after them.
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *work;
    s32 i;
    struct ResourceMenuEntry *entry;
    s16 *widthp;

    work = Ram_HeapSlots->menu_select_work;
    widthp = &work->width;
    *widthp = width + 2;
    work->resource_base = resource_base;
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


static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
    return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
    s32 group = 0;
    s32 ofs;
    s32 triple;
    s32 ret;
    s8 *tbl;

    if (Party_SumDjinnCountsFar(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = Menu_TopEntryPositionByCommand;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(sel);
    Menu_EndResourceSelection();

    if (ret >= 0) {
        s8 *commands = Menu_TopEntryCommandByPosition;
        s32 index;

        /* FAKEMATCH: ⚓️ loads the table's address before forming the index */
        asm volatile("" : "+l"(commands));
        index = ret + ofs + 1;
        ret = commands[index];
    }

    return ret;
}

// 日本語版はこの関数が違うので、まだ scaffold のまま (S4)
#if !defined(TLA_EDITION_JA)
s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
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
#endif
