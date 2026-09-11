#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_confirm_selection.h"

void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 flag;

    flag = 0;

    Menu_Run();

    if (arg2 == 0) {
        arg2 = 3;
    }
    if (arg0 != 0) {
        flag = 17;
    }

    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_Place(flag, arg2, arg1);

    arg3 = Menu_Check(arg3);
    Menu_Run2();
    if (arg3 == -1) {
        arg3 = 1;
    }
    return arg3;
}
