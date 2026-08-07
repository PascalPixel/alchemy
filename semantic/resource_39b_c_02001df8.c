#include "types.h"

/* Complete two-spawn owner and its pools, 0x02001df8-0x02001ec7. */
extern u8 *Data_03001ebc;
extern u32 Data_02000434;
extern u8 *Func_080090c8(s32, s32, s32, s32);
extern void Func_08009098(u8 *, void *);
extern void Func_08009020(u8 *, s32);
extern void Func_080f9010(s32);

void Func_02001df8(void)
{
    s32 permuted_4;
    s32 *slot = ((s32 **)(Data_03001ebc + 0x14))[Data_02000434];
    u8 *object;
    u8 *owner;

    permuted_4 = Func_080090c8(26, slot[2], slot[3], slot[4]);
    if (object != 0) {
        *(s32 *)(object + 20) = slot[5];
        owner = *(u8 **)(object + 0x50);
        *(s32 **)(object + 0x68) = slot;
        Func_08009098(object, (void *)0x0200a7c4);
        object[0x55] = 0;
        if (owner != 0) {
            owner[0x26] = 0;
            Func_08009020(owner, 2);
            owner[9] = (owner[9] & ~0x0c) | 4;
        }
        *(short *)(object + 0x64) = 0;
    }
    object  = permuted_4;

    object = Func_080090c8(26, slot[2], slot[3], slot[4]);
    if (object != 0) {
        *(s32 *)(object + 20) = slot[5];
        owner = *(u8 **)(object + 0x50);
        Func_08009098(object, (void *)0x0200a7c4);
        *(short *)(object + 0x64) = 0;
        object[0x55] = 0;
        object[0x23] = 2;
        *(s32 **)(object + 0x68) = slot;
        if (owner != 0) {
            Func_08009020(owner, 1);
            owner[0x26] = 0;
        }
    }
    Func_080f9010(130);
}
