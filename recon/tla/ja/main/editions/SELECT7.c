/* Draft: Japanese top-menu width 7 has no complete instruction-extent
 * match. Table/call layout still needs a source structural branch. */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "TLA_EDITION.H"

s32 UiWindow_Create(s32, s32, s32, s32, s32);

/* ☀️'s, reaching the selection work through ⚓️'s heap slot. */
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h)
{
    u8 *state;
    s32 i;

    state = Ram_HeapSlots->menu_select_work;

    *(u16 *)(state + 144) = (u16)((u32)w + 2);
    *(u16 *)(state + 146) = (u16)h;
    *(u16 *)(state + 148) = (u16)y;

    for (i = 0; i < *(s16 *)(state + 142); i++) {
        *(u16 *)(state + i * 20 + 14) = (u16)((u32)y << 3);
        *(u16 *)(state + i * 20 + 12) = (u16)((u32)x << 3);
        x = (s32)((u32)x + 3);
    }

    *(s32 *)(state + 120) =
        UiWindow_Create(x, y, *(s16 *)(state + 144), 3, 2);
}

extern s8 Menu_TopEntryPositionByCommand[];
extern s8 Menu_TopEntryCommandByPosition[];

s32 Party_SumDjinnCountsFar(s32);
void *AffineEffect_InitializeWork(void);
void Menu_AppendResourceEntry(s32 arg0);
void Menu_CenterResourceEntries(s32, s32, s32);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_EndResourceSelection(void);

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
    Menu_CenterResourceEntries(17, 7, 0);
    ret = Menu_RunResourceSelectionLoop(sel);
    Menu_EndResourceSelection();

    if (ret >= 0) {
        s8 *commands = Menu_TopEntryCommandByPosition;
        s32 index;

        asm volatile("" : "+l"(commands)); /* FAKEMATCH: ⚓️ loads the table's address before forming the index */
        index = ret + ofs + 1;
        asm volatile("" : "+l"(index)); /* FAKEMATCH: and indexes it with the whole sum */
        ret = commands[index];
    }

    return ret;
}
