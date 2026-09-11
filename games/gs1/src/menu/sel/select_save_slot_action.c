#include "types.h"
#include "scene.h"
#include "abi/menu/sel/select_save_slot_action.h"
#include "gs1_edition.h"

extern void *Menu_Run(void);
extern void Menu_AppendResourceEntry(s32 arg0);

extern s8 gRom[];

s32 Menu_SelectSaveSlotAction(void)
{
    s32 type;
    s32 ret;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = Menu_Check();
    if (type < 0) {
        return -1;
    }
    if (type == 0) {
        return 0;
    }
    if (type == 3) {
        group = 1;
    } else if (type == 0x67) {
        group = 2;
    } else if (type > 0x64) {
        group = 3;
    } else {
        initial = 1;
    }
    Menu_Run();
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x15);
    }
    if (group <= 1U) {
        Menu_AppendResourceEntry(0x16);
    }
    if ((group == 0) || (group == 3)) {
        Menu_AppendResourceEntry(0x17);
    }
    Menu_AppendResourceEntry(0x18);
    if ((*(s16 *)0x0200200C) != 0) {
        Menu_AppendResourceEntry(0x1D);
    }
    if ((*(s16 *)0x02002010) != 0) {
        Menu_AppendResourceEntry(0x1E);
    }
    Menu_Place(0x11, TYPE_MENU_WIDTH, 0);
    ret = Menu_Check2(initial);
    Menu_Run2();
    if (ret >= 0) {
        ret = gRom[ret + (group * 6)];
    }
    return ret;
}
