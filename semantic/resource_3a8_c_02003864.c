#include "types.h"

/*
 * resource_3a8 owner at 0x02003864, 168 bytes: create a kind-222 effect near
 * the caller's 16.16 position, configure its handle flags and presentation,
 * seed two random signed velocity components, and attach descriptor
 * 0x0200d120.  The preceding reviewed owner calls it with the current x/y/z
 * fields of followers 8 and 9, establishing all three inputs.
 *
 * Complete owner: saved-register prologue at 0x02003864 through the sole
 * interworking return at 0x020038fa-0x02003902, followed by two referenced
 * pool words through 0x0200390b.  Eight calls across seven targets match the
 * independently resolved machine sequence.
 */








extern u32 Func_0200739e(void);
extern u8 * Func_020073fc();
extern void Func_02007566();
extern void Func_0200746e();
extern u32 Func_020073ea(void);
extern u32 Func_020073f8(void);
extern void Func_0200744c();
extern void Func_0200745c();
void Func_02003864(s32 x, s32 y, s32 z)
{
    u8 *object;
    u8 *handle;
    u32 random;
    s32 offset;

    random = Func_0200739e();
    offset = (s32)(((random << 3) >> 16) << 16);

    object = Func_020073fc(222, x - 0x80000,
                           y + offset + 0x100000, z);
    if (object == 0) {
        return;
    }

    object[0x55] = 0;
    handle = *(u8 **)(object + 0x50);
    handle[9] = (u8)((handle[9] & ~0x0c) | 8);

    Func_02007566(object, 9);
    Func_0200746e(object, 0);

    random = Func_020073ea();
    *(s32 *)(object + 0x24) = (s32)(((random << 1) >> 16) - 1) * 0x10000;

    random = Func_020073f8();
    *(s32 *)(object + 0x28) = (s32)(((random * 6) >> 16) - 3) * 0x10000;

    *(u16 *)(object + 0x64) = 20;
    object[0x61] = 1;

    Func_0200744c(object, 1);
    Func_0200745c(object, (void *)0x0200d120);
}
