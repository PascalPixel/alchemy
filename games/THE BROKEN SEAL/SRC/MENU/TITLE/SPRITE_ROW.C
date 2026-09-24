#include "TYPES.H"

struct VramBlock {
    u16 base;
    u16 offset;
};

extern struct VramBlock Data_03001b10[];
extern s16 Data_02008650;
extern s16 Data_0200868c;
extern u32 Data_020086a0[];

#define FrameCounter (*(u32 *)0x03001e40)

struct Sprite {
    u32 words[3];
};

void Main_080001e8(struct Sprite *sprite, s32 value);

/* Rebuild the row of eighteen title sprites; each frame reveals one more
 * every two frames, and the newest two blink with the frame counter. */
void Title_RevealSpriteRow(void)
{
    u32 *w;
    struct Sprite *p;
    s32 tile;
    s32 i;
    s32 n;
    s32 y;
    s32 x;

    p = (struct Sprite *)Data_020086a0;
    w = Data_020086a0;
    tile = Data_03001b10[Data_02008650].offset >> 5;
    i = 0;
    y = 0x88;
loop:
    {
        x = 232 - (18 - i) * 8;
        *w++ = 0;
        *w++ = (x << 16) | y | 0x8400;
        *w++ = 0xf000 | tile;
        n = Data_0200868c / 2 - i;
        if (n < 0)
            n = 0;
        if (n <= 2 && (FrameCounter & 1))
            n = 0;
        if (n != 0)
            Main_080001e8(p++, 255);
        tile += 2;
    }
    if (++i <= 17)
        goto loop;
    Data_0200868c++;
}
