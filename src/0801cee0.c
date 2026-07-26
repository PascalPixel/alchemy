#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 Data_02000240[];

void Func_0801cee0(s32 arg0) {
    u8 *sp;
    u16 sel;
    s32 off;

    sel = M2C_FIELD(arg0, u16, 0x574);
    switch (sel) {
    case 0:
        off = 0x20C;
        sp = &Data_02000240[off];
        if (*sp <= 1) {
            break;
        }
        return;
    case 1:
        off = 0x205;
        sp = &Data_02000240[off];
        if (*sp <= 23) {
            break;
        }
        return;
    case 2:
        off = 0x206;
        sp = &Data_02000240[off];
        if (*sp <= 14) {
            break;
        }
        return;
    default:
        return;
    }
    (*sp)++;
}

void Func_0801cf44(void) {
}
