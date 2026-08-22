#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);
void *Func_08009048(s32, s32);
s32 Func_08009070(void *, s32);
void **Func_080b7dd0();

void Func_080b8418(void) {
    void *object;
    void *effect;

    object = *Func_080b7dd0();
    if ((object != NULL) && ((0xF & M2C_FIELD(object, u8 *, 0x54)) == 1)) {
        effect = Func_08009048(M2C_FIELD(object, s32 *, 0x50), 0x11B);
        if (effect != NULL) {
            Func_08009070(effect, 1);
            M2C_FIELD(effect, s8 *, 6) = 3;
        }
        Func_080030f8(0xA);
    }
}
