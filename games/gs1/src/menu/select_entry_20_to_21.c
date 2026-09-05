#include "types.h"

#define Menu_SelectEntry20To21 Func_08028ea8

void *Func_080284dc(void);
void Func_0802851c(void);
s32 Func_08028574(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Func_08028808(s32, s32, s32);

s32 Menu_SelectEntry20To21(s32 arg0) {
    s32 ret;

    Func_080284dc();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Func_08028808(0x11, 9, 0);
    ret = Func_08028574(arg0);
    Func_0802851c();
    return ret;
}
