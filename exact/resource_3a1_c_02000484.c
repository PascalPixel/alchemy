#include "types.h"

u16 *Func_02000afe(s32);
void Func_02000aec(void);
void Func_02000b60(s32, s32);
s32 Func_02000af0(s32);
void Func_02000b4a(s32);
void Func_02000b52(s32);
void Func_02000b62(s32, s32);
void Func_02000b2e(void);

void Func_02000484(void)
{
    u16 *state = Func_02000afe(0);
    u32 value = state[3];

    Func_02000aec();
    if (value < 0x2000 || value > 0xe000) {
        Func_02000b60(5, 17);
    } else {
        if (Func_02000af0(0x895) == 0) {
            Func_02000b4a(0x181d);
        } else {
            Func_02000b52(0x1a4e);
        }
        Func_02000b62(17, 0);
    }
    Func_02000b2e();
}
