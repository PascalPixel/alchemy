/* DRAFT: 596 of 620 bytes, 165 aligned halfword edits. Renders a string in the
 * 8x8 font (0x080F1770, widths at 0x080F11BD) into a 256-wide 8bpp buffer
 * with a drop shadow, aligned left, right (2) or centred (1) in 192 pixels,
 * packs it to 4bpp and copies 24 tiles to OBJ VRAM from slot; flag 0x200
 * marks the first call, later calls keep the previous top row.
 * Remaining: the frame is 44 bytes (the unused tile array reproduces that);
 * the reference hoists 96 twice (r7 for the pre-check and pointer rewind,
 * r8 as the reversed counter's start), 192 into lr and 256 into r6 for the
 * pack loop, and counts the tile loop down from sl = 24 with the VRAM offset
 * and source as separate pointers. A countdown with separate source and VRAM
 * cursors reproduces those increments, but the count remains in a low register;
 * the first character is also shared across the width and drawing scans. */
#include "TYPES.H"

typedef s32 (*FillFn)(void *dst, s32 size, s32 value);
typedef s32 (*CopyFn)(void *src, void *dst, s32 size);

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    ((FillFn)0x03000168)(dst, size, value);
}

static __inline__ void CopyWords(void *src, void *dst, s32 size)
{
    ((CopyFn)0x03001388)(src, dst, size);
}

extern u8 Data_080f11bd[];
extern u8 Data_080f1770[];

u8 *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *block);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);

s32 Func_080f07f0(u8 *text, s32 slot, s32 align)
{
    u8 *buf = Runtime_BumpAllocateAlternatePool(0x900);
    u32 tile[8];
    s32 x = 0;
    s32 width = 192;
    s32 half;
    s32 tiles;
    s32 pos;
    u8 *p;
    u8 *glyph;
    u8 *dst;
    u8 *src;
    u32 c;
    s32 idx;
    s32 w;
    s32 row;
    s32 bit;
    u32 bits;
    u32 mask;
    s32 n;
    s32 offset;
    u32 *words;

    if (text == NULL)
        return -1;
    if (!GameFlag_TestFar(0x200)) {
        FillWords(buf, 0x900, 0);
        GameFlag_SetBitFar(0x200);
    } else {
        CopyWords(buf, buf + 0x800, 0x100);
        FillWords(buf + 0x100, 0x800, 0);
    }
    p = text;
    pos = 0;
    while ((c = *p++) != 0) {
        if (c > 31)
            pos += Data_080f11bd[c - 32];
    }
    if (align == 2)
        x = width - pos;
    else if (align == 1)
        x = (width - pos) / 2;
    pos = 0;
    while ((c = *text++) != 0) {
        if (c > 31) {
            idx = c - 32;
            glyph = &Data_080f1770[idx * 8];
            dst = buf + x + pos;
            for (row = 0; row < 8; row++) {
                bits = *glyph++;
                mask = 0x80;
                for (bit = 7; bit >= 0; bit--) {
                    if (bits & mask) {
                        dst[0x101] = 1;
                        dst[0] = 15;
                    }
                    dst++;
                    mask >>= 1;
                }
                dst += 248;
            }
            w = 1;
            if (c > 31)
                w = Data_080f11bd[idx];
            pos += w;
        }
    }
    tiles = width / 8;
    half = width / 2;
    dst = buf;
    src = buf;
    for (row = 7; row >= 0; row--) {
        for (n = half; n != 0; n--) {
            *dst++ = src[0] | (src[1] << 4);
            src += 2;
        }
        dst += 256 - half;
        src += 256 - width;
    }
    words = (u32 *)buf;
    offset = slot * 32;
    for (n = tiles; n != 0; n--) {
        *(u32 *)(0x06010000 + offset) = words[0x000];
        *(u32 *)(0x06010004 + offset) = words[0x040];
        *(u32 *)(0x06010008 + offset) = words[0x080];
        *(u32 *)(0x0601000c + offset) = words[0x0c0];
        *(u32 *)(0x06010010 + offset) = words[0x100];
        *(u32 *)(0x06010014 + offset) = words[0x140];
        *(u32 *)(0x06010018 + offset) = words[0x180];
        *(u32 *)(0x0601001c + offset) = words[0x1c0];
        offset += 32;
        words++;
    }
    Runtime_BumpFree(buf);
    return 0;
}
