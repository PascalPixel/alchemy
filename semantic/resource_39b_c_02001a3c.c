#include "types.h"

extern void *Func_08000148();
extern int Func_080770c0();
extern u8 *Func_0808a400();
extern u8 *Func_080090c8();
extern void Func_08009098();
extern void Func_08009020();

/* Maintain the two scene objects associated with a moving actor record. */
void Func_02001a3c(s32 variant, u8 *state)
{
    u8 *actor;
    u8 *object;
    u8 *owner;
    s32 tile;

    *(u8 **)Func_08000148(35, 4) = state;
    if (Func_080770c0(0x109) == 0) {
        state[0] = 0;
        state[4] = (u8)variant;
        return;
    }

    actor = Func_0808a400(*(int *)(0x02000240 + 500));
    tile = (((*(int *)(actor + 16) & 0x000fffff) >> 20) * 128) +
           ((*(int *)(actor + 8) & 0x000fffff) >> 20);

    if (state[0] != 0 && *(int *)(state + 20) != 0) {
        object = Func_080090c8(26, *(int *)(actor + 8),
                              *(int *)(actor + 12) + 0x600000,
                              *(int *)(actor + 16));
        if (object != 0) {
            owner = *(u8 **)(object + 80);
            *(int *)(object + 20) = *(int *)(actor + 20);
            Func_08009098(object, (void *)0x0200a7e8);
            *(u8 **)(object + 104) = actor;
            object[85] = 4;
            *(int *)(object + 12) -= 0x8000;
            if (owner != 0) {
                Func_08009020(owner, 6 - state[0]);
                owner[38] = 0;
                owner[9] = (owner[9] & (u8)~0x0c) | 4;
            }
            *(u8 **)(state + 20) = object;
        }
    } else {
        *(u8 **)(state + 20) = 0;
    }

    if (((u8 *)0x02010000)[tile * 4 + 2] == variant && *(int *)(state + 24) != 0) {
        object = Func_080090c8(26, *(int *)(actor + 8), *(int *)(actor + 12), *(int *)(actor + 16));
        if (object != 0) {
            owner = *(u8 **)(object + 80);
            *(int *)(object + 20) = *(int *)(actor + 20);
            Func_08009098(object, (void *)0x0200a7d0);
            object[85] = 0;
            *(short *)(object + 100) = 0;
            object[35] = 2;
            *(int *)(object + 48) = 0x600000;
            if (owner != 0) {
                Func_08009020(owner, 6);
                owner[38] = 0;
            }
            *(u8 **)(state + 24) = object;
        }
    } else {
        *(u8 **)(state + 24) = 0;
    }
}
