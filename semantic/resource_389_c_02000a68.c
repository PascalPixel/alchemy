#include "types.h"

/* Complete kind-222 spawn/configuration owner and two-word pool. */

extern u8 *Func_0808a080(s32 actor);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
extern void Func_08009080(u8 *object, s32 mode);
extern void Func_08009098(u8 *object, s32 resource);
extern void Func_0808a160(u8 *object, s32 mode);

void Func_02000a68(s32 x, s32 y, s32 z, s32 value,
                   s32 extra, u32 style, u32 packed_mode)
{
    s32 permuted_6;
    u8 *source = Func_0808a080(0);
    u8 *object;
    u8 *display;
    s32 resources[3];
    u32 mode;

    permuted_6 = *(s32 *)0x0200960c;
    resources[1] = *(s32 *)0x02009610;
    resources[0]  = permuted_6;
    resources[2] = *(s32 *)0x02009614;

    object = Func_080090c8(222, x, y, z);
    if (object == 0)
        return;

    Func_08009080(object, (style + 1) & 15);
    Func_08009098(object, resources[style & 15]);
    display = *(u8 **)(object + 80);
    Func_0808a160(object, (style >> 16) & 15);

    object[85] = 0;
    display[38] = 0;
    *(s32 *)(object + 48) = value;
    *(s32 *)(object + 52) = extra;
    *(u16 *)(object + 102) = (u16)packed_mode;
    *(u32 *)(object + 108) = 0x02008a01;

    mode = packed_mode >> 16;
    if (mode == 0) {
        u8 source_flags = *(u8 *)(*(u8 **)(source + 80) + 9);
        display[9] = (display[9] & (u8)~12) | (source_flags & 12);
    } else if (mode <= 3) {
        object[35] &= (u8)~1;
        display[9] = (display[9] & (u8)~12) | ((mode & 3) << 2);
    }
}
