#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u32 Data_02000434[];

void *Func_0808ba1c(u32);
void *Func_08009048(s32, s32);
void Func_08009080(s32, s32);
void Func_080030f8(u32);

void Func_080942e0(s32 arg0)
{
    void *object = Func_0808ba1c(Data_02000434[125]);
    s32 actor = FIELD(object, s32, 0x50);
    void *effect = Func_08009048(actor, 0x1b);

    FIELD(actor, s8, 0x26) = 0;
    FIELD(effect, s8, 5) = 0xf;
    FIELD(object, s32, 8) =
        (FIELD(object, s32, 8) & 0xfff00000) + 0x80000;
    FIELD(object, s32, 0x10) =
        (FIELD(object, s32, 0x10) & 0xfff00000) + 0x100000;
    FIELD(object, s32, 0x24) = 0;
    FIELD(object, s32, 0x2c) = 0;
    FIELD(object, s32, 0x38) = 0x80000000;
    FIELD(object, s32, 0x40) = 0x80000000;
    Func_08009080((s32)object, arg0);
    Func_080030f8(0x12);
}
