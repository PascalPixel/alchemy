/* Draft: Japanese confirmation column differs by one immediate. */
#include "TYPES.H"

void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Menu_CenterResourceEntries(s32, s32, s32);

s32 Menu_SelectEntry19To1c(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
    Menu_CenterResourceEntries(0x11, 0xA, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}
