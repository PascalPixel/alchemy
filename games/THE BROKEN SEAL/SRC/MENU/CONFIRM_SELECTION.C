#include "TYPES.H"
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Menu_CenterResourceEntries(s32, s32, s32);

s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_JA)
    Menu_CenterResourceEntries(0x11, 6, 0);
#elif defined(TBS_EDITION_DE)
    Menu_CenterResourceEntries(0x11, 8, 0);
#else
    Menu_CenterResourceEntries(0x11, 7, 0);
#endif
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

s32 Menu_SelectEntry19To1c(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
#if defined(TBS_EDITION_JA)
    Menu_CenterResourceEntries(0x11, 8, 0);
#else
    Menu_CenterResourceEntries(0x11, 0xA, 0);
#endif
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 flag;

    flag = 0;

    AffineEffect_InitializeWork();

    if (arg2 == 0) {
        arg2 = 3;
    }
    if (arg0 != 0) {
        flag = 17;
    }

    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_CenterResourceEntries(flag, arg2, arg1);

    arg3 = Menu_RunResourceSelectionLoop(arg3);
    Menu_EndResourceSelection();
    if (arg3 == -1) {
        arg3 = 1;
    }
    return arg3;
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

extern u8 MsgCommandYes;
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h);

/* Asks Yes or No at (x, y), starting on SEL; cancelling answers No. */
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel)
{
    /* Entries 5 and 6: the command names Yes and No. */
    s32 msg = (s32)&MsgCommandYes;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_LayoutResourceEntries(x, y, 3, msg);
    sel = Menu_RunResourceSelectionLoop(sel);
    Menu_EndResourceSelection();
    if (sel == -1) {
        sel = 1;
    }
    return sel;
}
#endif
