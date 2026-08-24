#include "types.h"

void *Func_080284dc(void);
void Func_0802851c(void);
s32 Func_08028574(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Func_08028808(s32, s32, s32);

s32 Func_08028d74(s32 arg0) {
    s32 result;

    Func_080284dc();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
    Func_08028808(0x11, 7, 0);
    result = Func_08028574(arg0);
    Func_0802851c();
    return result;
}
