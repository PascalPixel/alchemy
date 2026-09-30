#include "TYPES.H"
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Menu_CenterResourceEntries(s32, s32, s32);

s32 Menu_SelectEntry20To21(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Menu_CenterResourceEntries(0x11, 9, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}
