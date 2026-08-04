/* Spawn an object and apply the optional fields selected by the flag word. */
#include "types.h"

u8 *Func_0808a080(s32 slot);
u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_08009080(u8 *object, s32 mode);
void Func_08009098(u8 *object, s32 value);
void Func_0808a160(u8 *object, s32 mode);
s32 Func_03000380(s32 value, s32 divisor);

void Func_0200013c(
    s32 x,
    s32 y,
    s32 z,
    s32 field44,
    s32 field48,
    s32 field4c,
    u32 flags,
    u8 *extra)
{
    u8 **table = (u8 **)0x02009f60;
    u32 index = flags & 15;
    u8 *object;
    u8 *owner;
    u8 *source;
    u8 *entry;
    s32 kind;
    u16 *field64;

    source = Func_0808a080(0);

    if ((flags & 0x100000) != 0 && extra != 0)
        kind = *(short *)(extra + 24);
    else
        kind = 222;

    object = Func_080090c8(kind, x, y, z);
    if (object == 0)
        return;

    owner = *(u8 **)(object + 80);
    Func_08009080(object, (index + 1) & 15);

    entry = table[index];
    Func_08009098(object, (s32)entry);

    object[85] = 0;
    owner[38] = 0;
    *(u32 *)(object + 108) = 0x02008105;
    *(s32 *)(object + 68) = field44;
    *(s32 *)(object + 72) = field48;
    *(s32 *)(object + 76) = field4c;

    owner[9] = (owner[9] & ~12) | (*(u8 *)(*(u8 **)(source + 80) + 9) & 12);

    *(s32 *)(object + 48) = 0;
    *(s32 *)(object + 52) = 0;
    field64 = (u16 *)(object + 100);
    *field64 = 0;

    if ((flags & 0xffff0000) == 0 || extra == 0)
        return;

    if ((flags & 0x10000) != 0)
        Func_0808a160(object, *(s32 *)(extra + 4));

    if ((flags & 0x20000) != 0) {
        object[35] &= ~1;
        owner[9] = (owner[9] & ~12) | ((extra[0] & 3) << 2);
    }

    {
        s32 copiedPosition = (flags & 0x80000) != 0;
        s32 scale = *(s32 *)(entry + 12);

        if (copiedPosition) {
            *(s32 *)(object + 24) = *(s32 *)(extra + 8);
            *(s32 *)(object + 28) = *(s32 *)(extra + 12);
        }

        if ((flags & 0x40000) != 0) {
            s32 deltaY;

            if (copiedPosition) {
                *(s32 *)(object + 48) =
                    Func_03000380(*(s32 *)(extra + 16) - *(s32 *)(object + 24), scale);
                deltaY = *(s32 *)(extra + 20) - *(s32 *)(object + 28);
            } else {
                *(s32 *)(object + 48) =
                    Func_03000380(*(s32 *)(extra + 16) - 0x10000, scale);
                deltaY = *(s32 *)(extra + 20) - 0x10000;
            }
            *(s32 *)(object + 52) = Func_03000380(deltaY, scale);
        }
    }

    if ((flags & 0x200000) != 0) {
        Func_08009080(object, 1);
        Func_08009098(object, *(s32 *)(extra + 28));
    }

    if ((flags & 0x400000) != 0)
        *(u16 *)(owner + 30) = *(u16 *)(extra + 32);

    if ((flags & 0x800000) != 0)
        *field64 = *(u16 *)(extra + 34);

    if ((flags & 0x1000000) != 0)
        *(u32 *)(object + 108) = *(u32 *)(extra + 36);
}
