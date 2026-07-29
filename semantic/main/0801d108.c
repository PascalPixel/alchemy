#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Window_0801d108 {
    u8 unknown_00[0x0c];
    u16 tile_x;
    u16 tile_y;
};

struct Sprite_0801d108 {
    u8 unknown_00[0x15];
    u8 flags_15;
    u8 unknown_16[2];
    u16 tile;
    u8 flags_1a;
};

struct Window_0801d108 *Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e41c(struct Window_0801d108 *, s32, s32, s32, s32);
void Func_0801e7c0(s32, struct Window_0801d108 *, s32, s32);
s32 Func_08004080(void);
void Func_08003fa4(s32, s32, const void *);
struct Sprite_0801d108 *Func_0801eadc(
    s32, u32, struct Window_0801d108 *, s32, s32);
void Func_080b0038(struct Sprite_0801d108 **, s32, s32);
s32 Func_080022ec(s32, s32);
void *Func_08021750(s32, s32, struct Window_0801d108 *, s32, s32);

struct Window_0801d108 *Func_0801d108(void)
{
    u8 *runtime = *(u8 **)0x03001ea0;
    struct Window_0801d108 *window =
        Func_080162d4(1, 5, 0x1c, 0x0e, 2);
    s32 resource;

    Func_0801e41c(window, 0, 2, 0x1b, 2);
    Func_0801e41c(window, 0, 4, 0x1b, 4);
    Func_0801e41c(window, 0, 7, 0x1b, 7);
    Func_0801e41c(window, 0, 0x0a, 0x1b, 0x0a);
    Func_0801e7c0(0xc07, window, 8, 0);
    Func_0801e7c0(0xc08, window, 8, 0x10);
    Func_0801e7c0(0xc0d, window, 8, 0x20);
    Func_0801e7c0(0xc0e, window, 0x20, 0x28);
    Func_0801e7c0(0xc0f, window, 8, 0x40);
    Func_0801e7c0(0xc12, window, 8, 0x58);

    resource = Func_08004080();
    if (resource <= 0x5f) {
        struct Sprite_0801d108 **sprite =
            (struct Sprite_0801d108 **)(runtime + 0x5a4);
        Func_08003fa4(resource, 0x80, (const void *)0x080310a4);
        *sprite = Func_0801eadc(resource, 0x40000000, window, 0, 0);
        Func_080b0038(
            sprite, window->tile_x * 8, window->tile_y * 8 + 0x0c);
    }

    resource = Func_08004080();
    if (resource <= 0x5f) {
        struct Sprite_0801d108 *sprite;

        *(volatile u32 *)0x040000d4 = 0x08073812;
        *(volatile u32 *)0x040000d8 = 0x050003c0;
        *(volatile u32 *)0x040000dc = 0x80000020;
        Func_08003fa4(resource, 0x100, (const void *)0x08029910);

        sprite = Func_0801eadc(resource, 0x40004000, window, 0x86, 0);
        sprite->flags_1a = (sprite->flags_1a & 0x0f) | 0xe0;

        sprite = Func_0801eadc(resource, 0x40004000, window, 0xa6, 0);
        sprite->tile =
            (sprite->tile & 0xfc00) | ((sprite->tile + 4) & 0x03ff);
        sprite->flags_1a = (sprite->flags_1a & 0x0f) | 0xe0;

        sprite = Func_0801eadc(resource, 0x40004000, window, 0x86, 0x10);
        sprite->flags_1a |= 0xf0;

        sprite = Func_0801eadc(resource, 0x40004000, window, 0xa6, 0x10);
        sprite->tile =
            (sprite->tile & 0xfc00) | ((sprite->tile + 4) & 0x03ff);
        sprite->flags_1a |= 0xf0;
    }

    resource = Func_08004080();
    if (resource <= 0x5f) {
        struct Sprite_0801d108 **sprite =
            (struct Sprite_0801d108 **)(runtime + 0x5b4);
        Func_08003fa4(resource, 0x100, 0);
        *sprite = Func_0801eadc(resource, 0x40000000, window, 0, 0);
        (*sprite)->flags_15 |= 0x20;
        Func_080b0038(
            sprite,
            window->tile_x * 8 + 0x8c +
                Func_080022ec(S8_AT(runtime, 0x594) * 0x3c,
                              S8_AT(runtime, 0x599)),
            window->tile_y * 8 + 4);
    }

    resource = Func_08004080();
    if (resource <= 0x5f) {
        struct Sprite_0801d108 **sprite =
            (struct Sprite_0801d108 **)(runtime + 0x5c4);
        Func_08003fa4(resource, 0x100, 0);
        *sprite = Func_0801eadc(resource, 0x40000000, window, 0, 0);
        (*sprite)->flags_15 |= 0x20;
        Func_080b0038(
            sprite,
            window->tile_x * 8 + 0x8c +
                Func_080022ec(S8_AT(runtime, 0x595) * 0x3c,
                              S8_AT(runtime, 0x59a)),
            window->tile_y * 8 + 0x14);
    }

    PTR_AT(runtime, 0x5ec) =
        Func_08021750(S8_AT((void *)0x080367c9, 0), 0, window, 0x54, 0x1c);
    PTR_AT(runtime, 0x5f0) =
        Func_08021750(S8_AT((void *)0x080367c9, 1), 0, window, 0x6c, 0x1c);
    PTR_AT(runtime, 0x5f4) =
        Func_08021750(S8_AT((void *)0x080367c9, 2), 0, window, 0x84, 0x1c);
    PTR_AT(runtime, 0x5f8) =
        Func_08021750(S8_AT((void *)0x080367cc, 0), 0, window, 0x64, 0x34);
    PTR_AT(runtime, 0x5fc) =
        Func_08021750(S8_AT((void *)0x080367cc, 1), 0, window, 0x7c, 0x34);
    PTR_AT(runtime, 0x604) =
        Func_08021750(S8_AT((void *)0x080367ce, 0), 0, window, 0x64, 0x4c);
    PTR_AT(runtime, 0x608) =
        Func_08021750(S8_AT((void *)0x080367ce, 1), 0, window, 0x7c, 0x4c);
    return window;
}
