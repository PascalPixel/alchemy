#include "TYPES.H"
#include "IO_REG.H"

/*
 * A developer palette editor over the background palettes. Shows the chosen
 * palette's fifteen colours as swatches with their red, green and blue
 * levels in hex digits; up and down pick the channel, left and right the
 * colour, L and R the palette, A and B raise and lower the level, holding
 * Start blinks the colour and Select leaves.
 */

extern volatile u32 gKeysRepeat;
extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001e40;

/* The compressed swatch tiles, one filled with each colour index. */
extern const u8 Debug_PaletteSwatchTiles[];

void Resource_DecompressHalfwords(const void *source, void *destination);
void WaitFrames(s32 count);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);

#define PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0

void Debug_RunPaletteEditor(void)
{
    s32 palette = 0;
    s32 channel = 1;
    s32 color = 1;
    u16 *map;
    u16 *p;
    u16 *colors;
    u32 tile;
    u32 value;
    u32 red;
    u32 green;
    u32 blue;
    u32 i;

    Resource_DecompressHalfwords(Debug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
    tile = (palette << 12) + 0xd1;
    colors = &PALETTE[palette * 16 + 1];
    map = EDITOR_MAP;
    p = map;
    *p = HEX_DIGIT + palette;
    p += 32;
    *p = 0xf000 | 'R';
    p += 32;
    *p = 0xf000 | 'G';
    p += 32;
    *p = 0xf000 | 'B';
    map++;
    for (i = 1; i < 16; i++) {
        p = map;
        *p = tile++;
        value = *colors++;
        p += 32;
        *p = (value & 31) + HEX_DIGIT;
        p += 32;
        *p = ((value >> 5) & 31) + HEX_DIGIT;
        p += 32;
        *p = ((value >> 10) & 31) + HEX_DIGIT;
        map++;
    }
    WaitFrames(1);
    for (;;) {
        if (gKeysRepeat & KEY_UP) {
            channel--;
            if (channel <= 0)
                channel = 3;
        }
        if (gKeysRepeat & KEY_DOWN) {
            channel++;
            if (channel > 3)
                channel = 1;
        }
        if (gKeysRepeat & KEY_LEFT) {
            color--;
            if (color <= 0)
                color = 15;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            color++;
            if (color > 15)
                color = 1;
        }
        if (gKeysRepeat & KEY_L) {
            palette--;
            if (palette < 0)
                palette = 13;
            goto redraw;
        }
        if (gKeysRepeat & KEY_R) {
            palette++;
            if (palette > 13)
                palette = 0;
            goto redraw;
        }
        if (gKeysRepeat & KEY_A) {
            colors = &PALETTE[palette * 16 + color];
            value = *colors;
            red = value & 31;
            green = (value >> 5) & 31;
            blue = (value >> 10) & 31;
            if (channel == 1 && red < 31)
                red++;
            if (channel == 2 && green < 31)
                green++;
            if (channel == 3 && blue < 31)
                blue++;
            *colors = (blue << 10) | (green << 5) | red;
            goto redraw;
        }
        if (gKeysRepeat & KEY_B) {
            colors = &PALETTE[palette * 16 + color];
            value = *colors;
            red = value & 31;
            green = (value >> 5) & 31;
            blue = (value >> 10) & 31;
            if (channel == 1 && red != 0)
                red--;
            if (channel == 2 && green != 0)
                green--;
            if (channel == 3 && blue != 0)
                blue--;
            *colors = (blue << 10) | (green << 5) | red;
            goto redraw;
        }
        if (gKeysRepeat & KEY_START) {
            colors = &PALETTE[palette * 16 + color];
            value = *colors;
            red = 0;
            for (;;) {
                WaitFrames(1);
                if (!(Data_03001ae8 & 8))
                    break;
                if (red == 0)
                    *colors = 0x7fff;
                if (red == 10)
                    *colors = value;
                if (red == 20)
                    *colors = 0;
                if (red == 30)
                    *colors = value;
                if (++red >= 40)
                    red = 0;
            }
            *colors = value;
        }
        if (gKeysRepeat & KEY_SELECT)
            break;
        Data_03001e40;
        WaitFrames(1);
    }
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
}
