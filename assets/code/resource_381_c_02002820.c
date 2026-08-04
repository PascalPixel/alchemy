#include "types.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
} Ent;

extern Ent *Func_02005d12(s32);
extern void Func_02005d6a(s32, s32, s32);
extern void Func_02005de8(s32, s32);

void Func_02002820(s32 a)
{
    Ent *p;

    p = Func_02005d12(a);
    Func_02005d6a(a, 0xe80000, 0x7d0000);
    p->unk6 = 0x4000;
    Func_02005de8(a, 3);
}
