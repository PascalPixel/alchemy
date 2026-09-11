#include "types.h"

extern u8 Data_00000024[];

void *Func_080284dc(void);
void Menu_AppendResourceEntry(s32 arg0);
void Func_080288a8(s32 a0, s32 a1, s32 a2, s32 a3);
s32 Func_08028574(s32);
void Func_0802851c(void);

s32 Menu_RunConfirmSelectionAt(s32 arg0, s32 arg1, s32 arg2)
{
    s32 ret;
    u8 *p;

    ret = arg2;
    p = Data_00000024;
    Func_080284dc();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Func_080288a8(arg0, arg1, 3, p);
    ret = Func_08028574(ret);
    Func_0802851c();
    if (ret == -1) {
        ret = 1;
    }
    return ret;
}
