#include "types.h"
#include "scene.h"
#include "abi/menu/sel/select_entry_20_to_21.h"

void *Menu_Run(void);

void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_SelectEntry20To21(s32 arg0)
{
    s32 ret;

    Menu_Run();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Menu_Place(0x11, 9, 0);
    ret = Menu_Check(arg0);
    Menu_Run2();
    return ret;
}
