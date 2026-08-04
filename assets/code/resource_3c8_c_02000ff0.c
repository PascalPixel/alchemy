#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_0ff0;

extern Struct_0ff0 *Func_02005ea2(s32);
extern void Func_02005e90(void);
extern void Func_02005e8e(s32);
extern void Func_02005eb2(void);

void Func_02000ff0(void)
{
    Struct_0ff0 *s;

    s = Func_02005ea2(12);
    Func_02005e90();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        Func_02005e8e(0x987);
    }
    Func_02005eb2();
}
