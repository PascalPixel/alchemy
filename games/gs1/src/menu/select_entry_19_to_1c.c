#include "types.h"

#define Menu_SelectEntry19To1c Func_08028db4

void *Func_080284dc(void);
void Func_0802851c(void);
s32 Func_08028574(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Func_08028808(s32, s32, s32);

s32 Menu_SelectEntry19To1c(s32 arg0) {
    s32 ret;

    Func_080284dc();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
    Func_08028808(0x11, 0xA, 0);
    ret = Func_08028574(arg0);
    Func_0802851c();
    return ret;
}
