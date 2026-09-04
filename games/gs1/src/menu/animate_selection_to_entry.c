#include "types.h"

#define Menu_AnimateSelectionToEntry Func_0802899c

void Func_0801c2d0(void);
void *Func_080284dc(void);
void Menu_AppendResourceEntry(s32 arg0);
void Func_08028808(s32, s32, s32);
s32 Menu_SelectResource(s32, s32);
void Func_0802851c(void);
void Func_0801c2e4(void);

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
    Func_0801c2d0();
    Func_080284dc();
    Menu_AppendResourceEntry(1);
    Menu_AppendResourceEntry(0xF);
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Func_08028808(0x11, 7, 0);
    arg1 = Menu_SelectResource(arg0, arg1 - 1);
    Func_0802851c();
    Func_0801c2e4();
    return arg1;
}
