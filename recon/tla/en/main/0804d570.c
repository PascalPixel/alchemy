#include "TYPES.H"
extern u8 gTitleExtraOptionEnabled[];

struct MenuModeLabelState;
extern struct MenuModeLabelState *gMenuSelectWork;
extern u8 MsgPasswordLevel[];

s32 UiWindow_Create(s32, s32, s32, s32, s32);

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    UiWindow_OpenMode1AndWaitFrame();
    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_CenterResourceEntries(0x11, 7, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Menu_EndResourceSelection();
    UiWork_CloseAndRelease();
    return arg1;
}
