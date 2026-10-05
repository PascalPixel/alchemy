/*
 * 2026-10-05 (deep finisher 16, later): alchemy diff 2. The VRAM copy
 * writes through one pointer, *p++ = ... eight times: p is set several
 * times, so global-alloc owns it and tile * 32 (r2) outranks the
 * addresses (r1), and CSE turns each p + 4 back into tile * 32 plus the
 * next constant, as listed. (Every form with a fresh address per store
 * left the addresses to local-alloc, which put them in r2 and tile * 32 in
 * r1, and tied the last address to it: 36-38.) Remaining: i = count is
 * scheduled before the hoisted tile-loop constants here and after them in
 * the listing (mov r9, r6 two places later).
 * Tried for that and no better: i = count before dst = canvas (2 lines
 * the same), dst = canvas or i = count outside the if (40-42), a
 * while (i != 0) loop (44), for (0..count) with dst = canvas before it
 * (44: the load lands before the pre-test) or inside if (count != 0)
 * (103-105: the inner pre-test stays), a separate canvas index whose
 * pointer loop.c derives (36 before the copy fix: one place closer),
 * tile = *tiles; tile &= 0x3FF (224).
 *
 * 2026-10-05 (deep finisher 16): alchemy diff 36 (was 349), no register
 * pins. The listing reuses function-scope variables across the loops, as
 * one 2001 function would: i is the glyph row, the pack row and the tile
 * count (r9 in all three), j the glyph column pair, the pack word and the
 * slot search (r4), src the glyph and pack source (r5), dst the glyph
 * destination, the pack destination and the tile copy source (r0), c the
 * code, the pack's left word and (not yet) the tile, bits the glyph row
 * and the pack's right word. The advance is x += 8 and x += 3 apart and
 * w for the table or 1, so x has the refs that put it in r8 before the
 * pen table. FillWords goes through an inline wrapper like ClearWords (the
 * arms' argument order), the glyph source is set before the destination,
 * count is signed so the pack loops are for (0..n) loops that loop.c
 * reverses (j = count after the pre-test), and the slot search is
 * for (j = 0; j <= mask && attributes[tile] != 0; j++), whose body does
 * not end in a jump, so the exit test is rotated and duplicated and 0xDA0
 * is shared by the first test and the store (ip) as listed. In the pack
 * loop src += 2 precedes the store.
 * Remaining: in the tile copy the listing has tile * 32 in r2 and each
 * VRAM address in r1, here the reverse (local-alloc order of the address
 * quantities and tile * 32), and i = count is scheduled last in the tile
 * loop set-up there, first here.
 * Tried this wave and worse: one w for every advance (406 with shared
 * variables), every advance a separate x += (517: x outranks the
 * hoisted 15 and takes r7), w for 3 too (391); the tile loop as
 * for (0..count) (44: dst = canvas lands before the pre-test), the same
 * inside if (count != 0) (105); tile as c (95-99); while (attr) with
 * if (++j > mask) break (128: the body ends in a jump, so no rotation);
 * the attribute test before j <= mask (48); a u32 count (208-238: the
 * unsigned compare stops loop reversal); a and b as their own locals
 * (208), c and w (222), c and code (543); the VRAM copy as
 * ((u32 *)0x06000000)[tile * 8 + k], byte offsets or tile <<= 5 (38 each),
 * through a vram pointer (58); c = *src++, bits = *src++ (61).
 *
 * Earlier waves (before the shared variables):
 * 2026-10-05 (deep finisher 8): alchemy diff 349 (was 395). The glyph
 * advance is a width variable w set in each arm (8, 3, the table, 1) and
 * added once, which gives the listing's shared add r8, r3 and its c - 32
 * in r3 (355); in the pack loop the source steps back before the
 * destination (349). Tried and worse: x += 8 kept apart with w for the
 * rest (356), w = 1 as the default (358); while ((c = *script++) != 0)
 * or code read first (377-379); source declared first, dest = source or
 * source = dest, the source step before the store, a separate dest++,
 * pointer arithmetic in u32 steps (349-389). Still open in the pack loop:
 * the listing keeps the source in r5 (the canvas reload) and the
 * destination in the copy r0, here the reverse, and the row counter is r9
 * there, lr here.
 *
 * Draft (2026-10-05, wave 3): rewritten from the listing in place of the
 * machine-translated draft. Normalized difference 315 of 402 instructions
 * (alchemy diff counts pool and branch offsets too and reads higher). The
 * control flow now follows the listing: the control-code switch before the
 * glyph path, cases 8, 7/9/10, 3, 14/15/28, 11/12/17/29 in that order; the
 * code kept apart from the glyph number masked in place (c &= 0xFF, so
 * if-conversion cannot hoist the glyph set-up above the c <= 30 test); the
 * colour stored and read back (which gives ldrb [rT, rN] for colour 15);
 * the pen table cleared through an inline wrapper, which leaves the call
 * argument in r0 and makes gcse set the table base in each arm as the
 * listing does; widths added in each branch so jump2 shares the add; one
 * tile variable from the tilemap read through the slot search to the VRAM
 * copy; the slot search an if around a do-while.
 * Devices: a 32-byte frame reserve (the frame is 68 bytes), x pinned to r8
 * and the glyph source to r5. Remaining: register choice in the arms (the
 * listing's per-arm table base is r3 then r7, here r2 then r0), c in r1
 * (here r2), the glyph row counter in r9 with the table copy in lr (here
 * swapped), the pack loop's source in r5 and destination in r0 (swapped),
 * and the tile loop, where the listing hoists 0xDA0 into ip and keeps the
 * cursor address in r7. UiText_GlyphWidths (0x080370d4, 0xe0 bytes) and
 * UiText_ColourPens (0x080371b4, 0x10 bytes) are the tail of
 * UiSprite_CursorFrameTiles and still need their labels.
 */
#include "TYPES.H"
#include "WINDOW.H"
#include "TBS_EDITION.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"

extern const u8 UiText_GlyphWidths[];
extern const u8 UiText_ColourPens[];

static __inline__ void ClearWords(void *dest, s32 size)
{
    Iwram_ClearWords(dest, size);
}

static __inline__ void FillWords(void *dest, s32 size, u32 value)
{
    Iwram_FillWords(dest, size, value);
}

s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x)
{
    /* FAKEMATCH: an unread 32-byte reserve, the native frame's extra bytes. */
    u32 reserve[8];
    u8 pens[16];
    struct UiRenderWork *work;
    u8 *canvas;
    u8 *glyphs;
    u32 attr;
    u32 *src;
    u8 *dst;
    u32 c;
    u32 code;
    s32 count;
    u32 w;
    s32 i;
    s32 j;
    u32 bits;

    work = (struct UiRenderWork *)gWindowWork[0];
    canvas = Runtime_BumpAllocate(0x800);
    glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    attr = work->level << 12;
    ClearWords(pens, 16);
    if (attr == 0xF000) {
        pens[1] = UiText_ColourPens[work->colour & 15];
        pens[3] = 3;
        FillWords(canvas, 0x800, 0x04040404);
    } else {
        pens[1] = (u8)work->colour & 15;
        pens[3] = 1;
        FillWords(canvas, 0x800, 0x0E0E0E0E);
    }
    if (script != 0) {
        for (;;) {
            c = *script++;
            code = c;
            if (c == 0)
                break;
            if (c <= 30) {
                switch (c) {
                case 8:
                    work->colour = *script++;
                    pens[1] = UiText_ColourPens[work->colour & 15];
                    break;
                case 7:
                case 9:
                case 10:
                    work->colour = 15;
                    pens[1] = UiText_ColourPens[work->colour];
                    break;
                case 3:
                    x += UiText_GlyphWidths[0];
                    break;
                case 14:
                case 15:
                case 28:
                    script++;
                case 11:
                case 12:
                case 17:
                case 29:
                    script++;
                    break;
                }
            } else {
                c &= 0xFF;
                src = (u32 *)(glyphs + c * 32);
                dst = canvas + x;
                for (i = 0; i < 8; i++) {
                    bits = *src++;
                    for (j = 3; j >= 0; j--) {
                        u8 pen = pens[bits & 15];

                        if (pen != 0)
                            *dst = pen;
                        bits >>= 4;
                        pen = pens[bits & 15];
                        dst++;
                        if (pen != 0)
                            *dst = pen;
                        dst++;
                        bits >>= 4;
                    }
                    dst += 248;
                }
                if (code == 0xF01D || code == 0xF01F)
                    x += 8;
                else if (code == 0xF01E)
                    x += 3;
                else {
                    if (c > 31)
                        w = UiText_GlyphWidths[c - 32];
                    else
                        w = 1;
                    x += w;
                }
            }
        }
    }
    count = (u32)(x + 7) >> 3;
    src = (u32 *)canvas;
    dst = canvas;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < count; j++) {
            c = src[0];
            bits = src[1];
            c |= c << 4;
            bits |= bits >> 4;
            c &= 0x0FF00FF0;
            bits &= 0x00FF00FF;
            c |= c << 8;
            bits |= bits >> 8;
            src += 2;
            *(u32 *)dst = (c << 4) >> 16 | bits << 16;
            dst += 4;
        }
        dst = dst - count * 4 + 0x100;
        src = (u32 *)((u8 *)src - count * 8 + 0x100);
    }
    if (count != 0) {
        dst = canvas;
        i = count;
        do {
            u32 mask = 127;
            u32 tile;

            if (work->alt != 0)
                mask = 255;
            tile = *tiles & 0x3FF;
            if ((tile < 0x80 || tile > 0xFF) &&
                (work->alt == 0 || tile < 0x200 || tile > 0x27F)) {
                tile = work->tile_cursor & mask;
                for (j = 0; j <= mask && work->tile_attributes[tile] != 0; j++)
                    tile = (tile + 1) & mask;
                work->tile_cursor = (tile + 1) & mask;
                work->tile_attributes[tile] = 1;
                if (tile > 127)
                    tile += 0x180;
                else
                    tile |= 0x80;
                *tiles = attr | tile;
                *mirror = attr | tile;
            }
            {
                u32 *p = (u32 *)(0x06000000 + tile * 32);

                *p++ = ((u32 *)dst)[0];
                *p++ = ((u32 *)dst)[0x40];
                *p++ = ((u32 *)dst)[0x80];
                *p++ = ((u32 *)dst)[0xC0];
                *p++ = ((u32 *)dst)[0x100];
                *p++ = ((u32 *)dst)[0x140];
                *p++ = ((u32 *)dst)[0x180];
                *p++ = ((u32 *)dst)[0x1C0];
            }
            tiles++;
            mirror++;
            dst += 4;
        } while (--i != 0);
    }
    Runtime_BumpFree(canvas);
    return count;
}
