#include "types.h"
#include "scene.h"
#include "abi/menu/sel/run_confirm_selection_at.h"

extern u8 gVal[];

void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

s32 Menu_RunConfirmSelectionAt(s32 arg0, s32 arg1, s32 arg2)
{
    s32 ret;
    u8 *p;

    ret = arg2;
    p = gVal;
    Menu_Run();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_SetMode(arg0, arg1, 3, p);
    ret = Menu_Check(ret);
    Menu_Run2();
    if (ret == -1) {
        ret = 1;
    }
    return ret;
}
