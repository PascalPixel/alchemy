#include "types.h"

extern u8 *Data_03001ebc;
extern u8 *Func_0808a080(s32 id);
extern s32 Func_02000890(s32 *a, s32 *b);

s32 Func_020008cc(void)
{
    s32 best_distance = 0x280;
    u8 *target = Func_0808a080(0);
    s32 best_slot = 0;
    s32 slot;

    for (slot = 8; slot <= 65; slot++) {
        u8 *candidate = ((u8 **)(Data_03001ebc + 0x34))[slot - 8];

        if (candidate != 0) {
            u8 *owner = *(u8 **)(candidate + 0x50);
            s16 kind = *(s16 *)(*(u8 **)(owner + 0x28));

            if (kind == 0xf2) {
                s32 distance = Func_02000890(
                    (s32 *)(target + 8), (s32 *)(candidate + 8));

                if (distance < best_distance) {
                    best_distance = distance;
                    best_slot = slot;
                }
            }
        }
    }

    return best_slot;
}
