#include "TYPES.H"
extern u8 gTitleExtraOptionEnabled[];

struct MenuModeLabelState;
extern struct MenuModeLabelState *gMenuSelectWork;
extern u8 MsgPasswordLevel[];

s32 UiWindow_Create(s32, s32, s32, s32, s32);

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
