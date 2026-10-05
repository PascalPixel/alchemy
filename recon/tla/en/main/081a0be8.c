/* Draft, differ 43: the intro crawl's line drawer. Renders the message
   into a cleared 0x900-byte 8bpp canvas through Func_08038418, packs it
   to 4bpp in place and copies its two halves to object VRAM a tile column
   at a time; returns the renderer's busy flag. The native keeps a 32-byte
   frame it never uses (kept here as a tagged unused local), loops with
   the tile counter in r6 and the column base in r12, and loads the copy
   source byte pair second byte first. */
#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"

s32 Func_08038418(s32 message, u8 *canvas, s32 align);

s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align)
{
    /* FAKEMATCH: an unused 32-byte local, the native frame's size. */
    u32 unused[8];
    u8 *canvas;
    u8 *src;
    u8 *dst;
    u32 *tiles;
    u32 *column;
    u32 *top;
    s32 busy;
    s32 i;
    s32 row;
    s32 offset;

    canvas = Runtime_BumpAllocateAlternatePool(0x900);
    Iwram_ClearWords(canvas, 0xc00);
    busy = Func_08038418(message, canvas, align);
    dst = canvas;
    src = canvas;
    for (i = 0x600; i != 0; i--) {
        *dst++ = src[0] | (src[1] << 4);
        src += 2;
    }
    offset = slot * 32;
    top = (u32 *)canvas;
    for (i = 0; i <= 23; i++) {
        tiles = (u32 *)(offset + 0x06010000);
        column = top;
        for (row = 7; row >= 0; row--) {
            *tiles++ = *column;
            column += 24;
        }
        offset += 32;
        top++;
    }
    offset = slot * 32;
    top = (u32 *)(canvas + 0x300);
    for (i = 0; i <= 23; i++) {
        tiles = (u32 *)(offset + 0x06010300);
        column = top;
        for (row = 7; row >= 0; row--) {
            *tiles++ = *column;
            column += 24;
        }
        offset += 32;
        top++;
    }
    Sys_Free(canvas);
    return busy;
}
