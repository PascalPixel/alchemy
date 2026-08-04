#include "types.h"

u16 *Func_02000a36(s32);
void Func_02000a24(void);
void Func_02000a8e(s32, s32);
s32 Func_02000a26(s32);
void Func_02000a80(s32);
void Func_02000a88(s32);
void Func_02000a98(s32, s32);
void Func_02000a64(void);

void Func_020003bc(void)
{
    u16 *state = Func_02000a36(0);
    u32 value = state[3];

    Func_02000a24();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000a8e(17, 15);
    } else {
        if (Func_02000a26(0x895) == 0) {
            Func_02000a80(0x1819);
        } else {
            Func_02000a88(0x1a48);
        }
        Func_02000a98(15, 0);
    }
    Func_02000a64();
}
