#include "types.h"
#include "gs1_edition.h"

extern s32 Func_0801f77c(void);
extern void *Func_080284dc(void);
extern void Menu_AppendResourceEntry(s32 arg0);
extern void Func_08028808(s32, s32, s32);
extern s32 Func_08028574(s32);
extern void Func_0802851c(void);
extern s8 Data_0803740f[];

s32 Func_080289e8(void) {
    s32 type;
    s32 result;
    s32 initial;
    u32 group;

    group = 0;
    initial = 0;
    type = Func_0801f77c();
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
    Func_080284dc();
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
    Func_08028808(0x11, TYPE_MENU_WIDTH, 0);
    result = Func_08028574(initial);
    Func_0802851c();
    if (result >= 0) {
        result = Data_0803740f[result + (group * 6)];
    }
    return result;
}
