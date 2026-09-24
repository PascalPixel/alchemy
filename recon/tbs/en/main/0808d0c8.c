/* Draft, not exact (2026-09-25): 716 of 716 bytes, 16 differing halfwords.
   Written from the listing after splitting it from BattleMap_ApplyEntranceView.
   What made it line up: the header and swatch columns written through a
   pointer stepped by 32 halfwords, the tile and colour cursors set before
   the header, and the Start blink counting in the red level local.
   Remaining: in the swatch loop the tile, colour cursor, 0xf0e0 digit base
   and 31 mask take r4/r0/r5/r7 where the ROM has r0/r7/r4/r5 (global
   allocation order; declaration order, cursor placement and loop spellings
   moved nothing). */
#include "TYPES.H"

/*
 * A developer palette editor over the background palettes. Shows the chosen
 * palette's fifteen colours as swatches with their red, green and blue
 * levels in hex digits; up and down pick the channel, left and right the
 * colour, L and R the palette, A and B raise and lower the level, holding
 * Start blinks the colour and Select leaves.
 */

extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001e40;

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
    u16 *entry;
    u16 *p;
    u16 *colors;
    u32 tile;
    u32 value;
    u32 red;
    u32 green;
    u32 blue;
    u32 i;

    Resource_DecompressHalfwords((const void *)0x0809e4ce, (void *)0x06001a00);
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
        if (Data_03001b04 & 0x40) {
            channel--;
            if (channel <= 0)
                channel = 3;
        }
        if (Data_03001b04 & 0x80) {
            channel++;
            if (channel > 3)
                channel = 1;
        }
        if (Data_03001b04 & 0x20) {
            color--;
            if (color <= 0)
                color = 15;
        }
        if (Data_03001b04 & 0x10) {
            color++;
            if (color > 15)
                color = 1;
        }
        if (Data_03001b04 & 0x200) {
            palette--;
            if (palette < 0)
                palette = 13;
            goto redraw;
        }
        if (Data_03001b04 & 0x100) {
            palette++;
            if (palette > 13)
                palette = 0;
            goto redraw;
        }
        if (Data_03001b04 & 1) {
            entry = &PALETTE[palette * 16 + color];
            value = *entry;
            red = value & 31;
            green = (value >> 5) & 31;
            blue = (value >> 10) & 31;
            if (channel == 1 && red < 31)
                red++;
            if (channel == 2 && green < 31)
                green++;
            if (channel == 3 && blue < 31)
                blue++;
            *entry = (blue << 10) | (green << 5) | red;
            goto redraw;
        }
        if (Data_03001b04 & 2) {
            entry = &PALETTE[palette * 16 + color];
            value = *entry;
            red = value & 31;
            green = (value >> 5) & 31;
            blue = (value >> 10) & 31;
            if (channel == 1 && red != 0)
                red--;
            if (channel == 2 && green != 0)
                green--;
            if (channel == 3 && blue != 0)
                blue--;
            *entry = (blue << 10) | (green << 5) | red;
            goto redraw;
        }
        if (Data_03001b04 & 8) {
            entry = &PALETTE[palette * 16 + color];
            value = *entry;
            red = 0;
            for (;;) {
                WaitFrames(1);
                if (!(Data_03001ae8 & 8))
                    break;
                if (red == 0)
                    *entry = 0x7fff;
                if (red == 10)
                    *entry = value;
                if (red == 20)
                    *entry = 0;
                if (red == 30)
                    *entry = value;
                if (++red >= 40)
                    red = 0;
            }
            *entry = value;
        }
        if (Data_03001b04 & 4)
            break;
        Data_03001e40;
        WaitFrames(1);
    }
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
}
