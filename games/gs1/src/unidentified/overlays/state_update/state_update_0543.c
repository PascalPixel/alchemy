#include "types.h"

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

void Func_02007532(s32, s32, s32, s32, s32, s32);
void Func_02007544(s32, s32, s32, s32, s32, s32);
void Func_02007560(Effect *, s32);
Effect *Func_02007594(s32);
void Func_0200759e(s32);

void Func_02003970(void)
{
    Effect *effect = Func_02007594(11);

    Func_02007532(0, 0, 1, 1, 9, 14);
    Func_02007544(0, 0, 1, 1, 9, 45);
    if (effect != 0) {
        Func_02007560(effect, 0);
        effect->y -= 0x200000;
        effect->state23 = 2;
    }
    Func_0200759e(0x201);
}
