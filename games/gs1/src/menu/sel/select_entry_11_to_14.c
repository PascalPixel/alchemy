#include "types.h"
#include "scene.h"
#include "abi/menu/sel/select_entry_11_to_14.h"

void *Menu_Run(void);

void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    Menu_Run();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
    Menu_Place(0x11, 7, 0);
    ret = Menu_Check(arg0);
    Menu_Run2();
    return ret;
}
