#include "types.h"

#define U8_AT(base, offset)  (*(u8 *)((u8 *)(base) + (offset)))
#define U16_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define S32_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define PTR_AT(base, offset) (*(void **)((u8 *)(base) + (offset)))

void Func_0800447c(s32 distance, u16 angle, s32 position[3]);
void Func_080090d0(void *object);
void Func_080030f8(u32 frames);
void *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(void *object, void *callback);
s32 Func_080048b0(s32 resource, s32 size);
void Func_08015250(s32 resource);
s32 Func_08003fa4(s32 index, s32 count, s32 destination);
void Func_08002dd8(s32 resource);

s32 Func_0808ef70(s32 slot, s32 resource)
{
    u8 *state = *(u8 **)0x03001ebc;
    u8 *definition = *(u8 **)(state + 0x14 + slot * 4);
    s32 position[3];
    s32 x;
    s32 z;
    s32 index;
    u8 *object;
    u8 *sprite;
    s32 graphics;

    if (definition == 0)
        return 0;

    position[0] = S32_AT(definition, 8);
    position[1] = S32_AT(definition, 0x0c);
    position[2] = S32_AT(definition, 0x10);
    Func_0800447c(0x100000, U16_AT(definition, 6), position);
    x = (position[0] & 0xfff00000) + 0x80000;
    z = (position[2] & 0xfff00000) + 0x80000;

    if (S16_AT(state, 0xcb8) != 0) {
        u8 *entry = *(u8 **)0x03001e64;

        for (index = 0; index < 64; index++, entry += 0x70) {
            s32 callback;

            if (S32_AT(entry, 0) == 0)
                continue;
            callback = S32_AT(entry, 0x6c);
            if (callback == 0x0808f28d)
                Func_080090d0(entry);
            if (S32_AT(entry, 0) == 0x0809e87c)
                Func_080090d0(entry);
        }
    }

    Func_080030f8(3);
    object = Func_080090c8(0x16, x, 0x100000, z);
    if (object == 0)
        return 0;

    Func_08009098(object, (void *)0x0809e6c0);
    sprite = PTR_AT(object, 0x50);
    U8_AT(sprite, 0x26) = 0;
    U8_AT(sprite, 0x27) = 0;
    U8_AT(sprite, 5) &= ~0x20;
    U8_AT(sprite, 9) = (U8_AT(sprite, 9) & 3) | 4;
    S32_AT(object, 0x28) = 0x20000;
    S32_AT(object, 0x48) = 0x4000;

    graphics = Func_080048b0(0x11, 0x608);
    Func_08015250(resource);
    U16_AT(sprite, 8) =
        (U16_AT(sprite, 8) & 0xfc00) |
        (Func_08003fa4(U8_AT(sprite, 0x1c), 0x80,
                       graphics + 0x400) & 0x3ff);
    Func_08002dd8(0x11);
    S32_AT(object, 0x6c) = 0x0808eee5;
    return (s32)object;
}
