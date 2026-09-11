#include "types.h"
#include "scene.h"
#include "abi/menu/sel/animate_selection_to_entry.h"

void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectResource(s32, s32);

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
    Menu_Run2();
    Menu_Run();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_Place(0x11, 7, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Menu_Run3();
    Menu_Run4();
    return arg1;
}
