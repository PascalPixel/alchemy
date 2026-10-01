#include "TYPES.H"
#include "TLA_EDITION.H"

void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 entry);
s32 Menu_CenterResourceEntries(s32, s32, s32);

/* Runs a selection over the four menu entries 0x11 to 0x14. */
s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
    Menu_CenterResourceEntries(0x11, SELECT_ENTRY_11_COLUMN, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}
