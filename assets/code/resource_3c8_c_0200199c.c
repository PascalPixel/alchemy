#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_199c;

extern Struct_199c *Func_02006852(s32);
extern void Func_02006818(s32, s32, s32, s32);

void Func_0200199c(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Func_02006852(i + 11);
        i++;
        Func_02006818(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}
