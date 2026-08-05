#include "types.h"

/*
 * General object creation and optional-field setup.  The low flags nibble
 * selects a variant record; the upper bits independently enable fields from
 * the caller's extra record.  This owner is the resource_39d member of the
 * same machine-code family already present in resources 39c, 3c9, and 39e.
 */
extern u8 *Func_0808a080(s32 id);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
extern void Func_08009080(u8 *object, s32 variant);
extern void Func_08009098(u8 *object, s32 entry_value);
extern void Func_0808a160(u8 *object, s32 value);
extern s32 Func_03000380(s32 value, s32 scale);

void Func_0200013c(s32 x, s32 y, s32 z, s32 field44,
                   s32 field48, s32 field4c, u32 flags, u8 *extra)
{
    u8 **table = (u8 **)0x0200b92c;
    u32 variant = flags & 0xf;
    u8 *object;
    u8 *owner;
    u8 *service;
    u8 *entry;
    s32 kind;
    u16 *field64;

    service = Func_0808a080(0);

    if ((flags & 0x100000) != 0 && extra != 0)
        kind = *(short *)(extra + 0x18);
    else
        kind = 0xde;

    object = Func_080090c8(kind, x, y, z);
    if (object == 0)
        return;

    owner = *(u8 **)(object + 0x50);
    Func_08009080(object, (variant + 1) & 0xf);

    entry = table[variant];
    Func_08009098(object, (s32)entry);

    object[0x55] = 0;
    *(s32 *)(object + 0x44) = field44;
    owner[0x26] = 0;
    *(u32 *)(object + 0x6c) = 0x02008105;
    *(s32 *)(object + 0x48) = field48;
    *(s32 *)(object + 0x4c) = field4c;

    owner[9] = (owner[9] & ~0x0c)
        | (*(u8 *)(*(u8 **)(service + 0x50) + 9) & 0x0c);

    *(s32 *)(object + 0x30) = 0;
    *(s32 *)(object + 0x34) = 0;
    field64 = (u16 *)(object + 0x64);
    *field64 = 0;

    if ((flags & 0xffff0000) == 0 || extra == 0)
        return;

    if ((flags & 0x10000) != 0)
        Func_0808a160(object, *(s32 *)(extra + 4));

    if ((flags & 0x20000) != 0) {
        object[0x23] &= ~1;
        owner[9] = (owner[9] & ~0x0c) | ((extra[0] & 3) << 2);
    }

    {
        s32 copied_position = (flags & 0x80000) != 0;
        s32 divisor = *(s32 *)(entry + 0xc);
        s32 y_delta;

        if (copied_position) {
            *(s32 *)(object + 0x18) = *(s32 *)(extra + 8);
            *(s32 *)(object + 0x1c) = *(s32 *)(extra + 12);
        }

        if ((flags & 0x40000) != 0) {
            if (copied_position) {
                *(s32 *)(object + 0x30) = Func_03000380(
                    *(s32 *)(extra + 0x10) - *(s32 *)(object + 0x18),
                    divisor);
                y_delta = *(s32 *)(extra + 0x14) - *(s32 *)(object + 0x1c);
            } else {
                *(s32 *)(object + 0x30) = Func_03000380(
                    *(s32 *)(extra + 0x10) - 0x10000, divisor);
                y_delta = *(s32 *)(extra + 0x14) - 0x10000;
            }
            *(s32 *)(object + 0x34) = Func_03000380(y_delta, divisor);
        }
    }

    if ((flags & 0x200000) != 0) {
        Func_08009080(object, 1);
        Func_08009098(object, *(s32 *)(extra + 0x1c));
    }

    if ((flags & 0x400000) != 0)
        *(u16 *)(owner + 0x1e) = *(u16 *)(extra + 0x20);

    if ((flags & 0x800000) != 0)
        *field64 = *(u16 *)(extra + 0x22);

    if ((flags & 0x1000000) != 0)
        *(u32 *)(object + 0x6c) = *(u32 *)(extra + 0x24);
}
