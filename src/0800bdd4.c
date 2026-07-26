#include "types.h"

void Func_08003f3c(u8);
void Func_0800bc48(void *);

void Func_0800bdd4(u8 *object)
{
    u32 zero;
    u32 *source;
    u32 *dma;
    u32 control;
    void **slot;
    s32 remaining;

    if (object != 0) {
        if ((object[29] & 1) == 0) {
            Func_08003f3c(object[28]);
        }
        slot = (void **)(object + 40);
        remaining = 3;
        do {
            Func_0800bc48(*slot++);
            remaining--;
        } while (remaining >= 0);
        source = &zero;
        *source = 0;
        dma = (u32 *)0x040000d4;
        control = 0x8500000e;
        dma[0] = (u32)source;
        dma[1] = (u32)object;
        dma[2] = control;
    }
}
