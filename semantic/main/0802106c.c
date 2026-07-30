#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct Window_0802106c {
    u8 unknown_00[0x0c];
    u16 x_0c;
    u16 y_0e;
};

typedef char Assert_0802106c_window_x[
    OFFSET_OF(struct Window_0802106c, x_0c) == 0x0c ? 1 : -1];
typedef char Assert_0802106c_window_y[
    OFFSET_OF(struct Window_0802106c, y_0e) == 0x0e ? 1 : -1];

void Func_080030f8(s32 frames);
void Func_08003f3c(s32 tile);
void Func_08003fa4(s32 tile, s32 size, const void *source);
s32 Func_08004080(void);
struct Window_0802106c *Func_080162d4(
    s32 x, s32 y, s32 width, s32 height, s32 mode);
void Func_08016418(struct Window_0802106c *window, s32 mode);
void Func_0801e7c0(
    s32 text, struct Window_0802106c *window, s32 x, s32 y);
s32 Func_0801eadc(
    s32 tile, u32 flags, struct Window_0802106c *window, s32 x, s32 y);
void Func_08020a60(
    struct Window_0802106c *window,
    s32 column,
    s32 row,
    s32 first_frame,
    s32 mode,
    s32 second_frame);
void Func_080b0020(s32 *sprite);
void Func_080b0030(s32 *sprite, s32 x, s32 y, s32 mode);
void Func_080b0038(s32 *sprite, s32 x, s32 y);
void Func_080f9010(s32 sound);

/*
 * Present the three-option modal selector. The UI tile allocator is required
 * by this subsystem to return a valid tile (0..95); the reference also relies
 * on that invariant before using the cursor handle.
 */
s32 Func_0802106c(void)
{
    struct Window_0802106c *window =
        Func_080162d4(7, 13, 18, 7, 2);
    s32 tile;
    s32 sprite;
    s32 selection = 0;
    s32 dirty = 1;

    Func_0801e7c0(0x2080, window, 8, 0);
    Func_0801e7c0(0x2081, window, 8, 0x10);
    Func_0801e7c0(0x2082, window, 8, 0x20);

    tile = Func_08004080();
    if (tile <= 0x5f) {
        Func_08003fa4(tile, 0x80, (const void *)0x080310a4);
        sprite = Func_0801eadc(tile, 0x40000000, window, 0, 0);
        Func_080b0038(
            &sprite,
            window->x_0c * 8 - 3,
            window->y_0e * 8 + 9);
    }

    {
        volatile u32 *dma3 = (volatile u32 *)0x040000d4;

        dma3[0] = 0x050001e0;
        dma3[1] = 0x050001c0;
        dma3[2] = 0x84000008;
    }
    *(volatile u16 *)0x050001c8 = 0x6318;

    for (;;) {
        Func_08020a60(window, 1, selection * 2, 14, 1, 14);
        Func_080030f8(1);
        Func_08020a60(window, 1, selection * 2, 14, 1, 15);

        if (dirty != 0) {
            dirty = 0;
            Func_080b0030(
                &sprite,
                window->x_0c * 8 - 3,
                (window->y_0e + selection * 2) * 8 + 9,
                3);
        }
        Func_080b0020(&sprite);

        if ((*(volatile u32 *)0x03001b04 & 0x40) != 0) {
            Func_080f9010(0x6f);
            selection--;
            dirty = 1;
            if (selection == -1)
                selection = 2;
        }
        if ((*(volatile u32 *)0x03001b04 & 0x80) != 0) {
            Func_080f9010(0x6f);
            selection++;
            dirty = 1;
            if (selection == 3)
                selection = 0;
        }

        if ((*(volatile u32 *)0x03001b04 & 2) != 0) {
            Func_080f9010(0x71);
            selection = -1;
            break;
        }
        if ((*(volatile u32 *)0x03001b04 & 1) != 0) {
            Func_080f9010(0x70);
            break;
        }
    }

    Func_08016418(window, 2);
    Func_080030f8(1);
    Func_08003f3c(tile);
    return selection;
}
