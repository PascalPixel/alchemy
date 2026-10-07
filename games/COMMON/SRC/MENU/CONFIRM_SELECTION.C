#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "text/MSG_IDS.H"
s32 Menu_CenterResourceEntries(s32, s32, s32);

s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(17);
    Menu_AppendResourceEntry(18);
    Menu_AppendResourceEntry(19);
    Menu_AppendResourceEntry(20);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
    Menu_CenterResourceEntries(17, 8, 0);
#elif EDITION_INTERNATIONAL
    Menu_CenterResourceEntries(17, 7, 0);
#else
    Menu_CenterResourceEntries(17, 6, 0);
#endif
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

s32 Menu_SelectEntry19To1c(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(25);
    Menu_AppendResourceEntry(26);
    Menu_AppendResourceEntry(27);
    Menu_AppendResourceEntry(28);
#if EDITION_INTERNATIONAL
    Menu_CenterResourceEntries(17, 10, 0);
#else
    Menu_CenterResourceEntries(17, 8, 0);
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


s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel)
{
    s32 msg = (s32)MsgCommandYes;

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
