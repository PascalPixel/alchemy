#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08016418(struct Work *work, s32 release);
void Func_080030f8(u32);

void Func_08019a54(void)
{
    s32 no;
    void **slot;
    void *work;

    slot = (void **)(*(s32 *)0x03001e8c + 0x620);
    no = 0;
    do {
        work = *slot;
        if (work != NULL && M2C_FIELD(work, s32, 0x18) == 0
            && M2C_FIELD(work, u16, 0x16) != 0
            && M2C_FIELD(work, u16, 0x14) != 0) {
            Func_08016418((s32)work,
                (s32)(u16)(2 & M2C_FIELD(work, u16, 0x16)));
        }
        no++;
        slot = (void **)((u8 *)slot + 40);
    } while (no != 3);
    Func_080030f8(10);
}
