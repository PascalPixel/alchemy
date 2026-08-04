#include "types.h"

u16 *Func_02000a96(s32);
void Func_02000a84(void);
void Func_02000aee(s32, s32);
s32 Func_02000a86(s32);
void Func_02000ae0(s32);
void Func_02000af0();
void Func_02000b10(s32, s32);
void Func_02000acc(void);

void Func_0200041c(void)
{
    u16 *state = Func_02000a96(0);
    u32 value = state[3];

    Func_02000a84();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000aee(18, 16);
    } else if (Func_02000a86(0x895) == 0) {
        Func_02000ae0(0x181b);
        Func_02000af0(16, 0);
    } else {
        Func_02000af0(0x1a4a);
        Func_02000b10(16, 0);
    }
    Func_02000acc();
}
