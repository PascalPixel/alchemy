/* Near miss: score 60. The tables take ☀️'s names in ⚓️'s listings. ⚓️
   loads the command table's address before forming the index (ldr r2 ahead
   of adds r3, r5, r7); this draft forms the index first. A table pointer,
   TblGet and asm barriers (which change the addressing) did not fix it, nor
   45 s of permuting. */
#include "TYPES.H"
#include "TLA_EDITION.H"

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
    Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_RunResourceSelectionLoop(sel);
    Menu_EndResourceSelection();

    if (ret >= 0) {
        ret = Menu_TopEntryCommandByPosition[ret + ofs + 1];
    }

    return ret;
}
