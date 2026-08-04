#include "types.h"

struct GlyphEntry_0802977c {
    s16 id;
    s16 code;
};

struct Window_0802977c;

struct Window_0802977c *Func_08019da8(
    s32 type, s32 top, s32 width, s32 height);
struct Window_0802977c *Func_080162d4(
    s32 type, s32 top, s32 width, s32 height, s32 mode);
s32 Func_080022fc(s32 value, s32 modulus);
void Func_08016478(struct Window_0802977c *window);
void Func_0801a4fc(
    s32 code,
    s32 mode,
    u32 *seed,
    u32 *output,
    s32 width,
    s32 count);
void Func_0801ea08(
    s32 selection,
    s32 mode,
    struct Window_0802977c *window,
    s32 flags);
void Func_0801e7c0(
    s32 resource,
    struct Window_0802977c *window,
    s32 x,
    s32 y);
void Func_080030f8(s32 frames);
void Func_08016418(struct Window_0802977c *window, s32 mode);

s32 Func_0802977c(void)
{
    u8 *runtime = *(u8 *volatile *)0x03001e8c;
    const struct GlyphEntry_0802977c *firstTable =
        (const struct GlyphEntry_0802977c *)0x080367e4;
    const struct GlyphEntry_0802977c *secondTable =
        (const struct GlyphEntry_0802977c *)0x0803680c;
    struct Window_0802977c *firstWindow;
    struct Window_0802977c *secondWindow;
    s32 firstCount = 0;
    s32 secondCount = 0;
    s32 total;
    s32 selection = 0;
    s32 redraw = 1;

    firstWindow = Func_08019da8(0, 0, 10, 5);
    secondWindow = Func_080162d4(10, 10, 14, 3, 2);

    while (firstTable[firstCount].id != -1)
        firstCount++;
    while (secondTable[secondCount].id != -1)
        secondCount++;
    total = firstCount + secondCount;

    for (;;) {
        u32 keys;

        keys = *(volatile u32 *)0x03001b04;
        if (keys & 0x20) {
            redraw = 1;
            selection--;
        }
        keys = *(volatile u32 *)0x03001b04;
        if (keys & 0x10) {
            redraw = 1;
            selection++;
        }
        keys = *(volatile u32 *)0x03001b04;
        if (keys & 0x200) {
            redraw = 1;
            selection -= 10;
        }
        keys = *(volatile u32 *)0x03001b04;
        if (keys & 0x100) {
            redraw = 1;
            selection += 10;
        }

        if ((*(volatile u32 *)0x03001b04 & 1) != 0)
            break;
        if ((*(volatile u32 *)0x03001b04 & 2) != 0)
            break;

        if (redraw != 0) {
            s32 code;
            u32 seed;
            u32 output;

            redraw = 0;
            selection = Func_080022fc(selection + total, total);
            Func_08016478(secondWindow);

            if (selection < firstCount)
                code = firstTable[selection].code;
            else
                code = secondTable[selection - firstCount].code + 0x80;

            seed = *(u16 *)(runtime + 0x12f2);
            Func_0801a4fc(code, 0, &seed, &output, 15, 1);
            Func_0801ea08(selection, 2, secondWindow, 0);
            Func_0801e7c0(selection + 0xdd2, secondWindow, 24, 0);
        }

        Func_080030f8(1);
    }

    Func_08016418(secondWindow, 2);
    Func_08016418(firstWindow, 2);
    Func_080030f8(1);
    return 0;
}
