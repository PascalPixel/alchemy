#include "types.h"
#include "scene.h"
#include "abi/menu/sel/select_entry_19_to_1c.h"

void *Menu_Run(void);

void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectEntry19To1c(s32 arg0)
{
    s32 ret;

    Menu_Run();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
    Menu_Place(0x11, 0xA, 0);
    ret = Menu_Check(arg0);
    Menu_Run2();
    return ret;
}
