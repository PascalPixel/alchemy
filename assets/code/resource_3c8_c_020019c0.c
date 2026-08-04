#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct_19c0;

extern Struct_19c0 *Func_02006876(s32);
extern void Func_02006842(s32, s32, s32, s32);

void Func_020019c0(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Func_02006876(i + 11);
        if (p->unkC > -0x100000) {
            Func_02006842(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}
