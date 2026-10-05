/*
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

s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 column)
{
    /* FAKEMATCH: an unread 32-byte reserve, the native frame's extra bytes. */
    u32 reserve[8];
    u8 *canvas;
    u8 *glyphs;
    u32 attr;
    u8 pens[16];
    u8 *work;
    u32 c;
    u32 code;
    u32 count;
    u32 w;
    /* FAKEMATCH: x pinned to r8, as the native allocates it. */
    register s32 x asm("r8");

    x = column;
    work = gWindowWork[0];
    canvas = Runtime_BumpAllocate(0x800);
    glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    attr = ((struct UiRenderWork *)work)->level << 12;
    ClearWords(pens, 16);
    if (attr == 0xF000) {
        pens[1] = UiText_ColourPens[((struct UiRenderWork *)work)->colour & 15];
        pens[3] = 3;
        Iwram_FillWords(canvas, 0x800, 0x04040404);
    } else {
        pens[1] = (u8)((struct UiRenderWork *)work)->colour & 15;
        pens[3] = 1;
        Iwram_FillWords(canvas, 0x800, 0x0E0E0E0E);
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
                    ((struct UiRenderWork *)work)->colour = *script++;
                    pens[1] = UiText_ColourPens[((struct UiRenderWork *)work)->colour & 15];
                    break;
                case 7:
                case 9:
                case 10:
                    ((struct UiRenderWork *)work)->colour = 15;
                    pens[1] = UiText_ColourPens[((struct UiRenderWork *)work)->colour];
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
                /* FAKEMATCH: source pinned to r5, as the native allocates it. */
                register u32 *source asm("r5");
                u8 *dest;
                s32 row;

                c &= 0xFF;
                dest = canvas + x;
                source = (u32 *)(glyphs + c * 32);
                for (row = 0; row < 8; row++) {
                    u32 bits = *source++;
                    s32 n;

                    for (n = 3; n >= 0; n--) {
                        u8 pen = pens[bits & 15];

                        if (pen != 0)
                            *dest = pen;
                        bits >>= 4;
                        pen = pens[bits & 15];
                        dest++;
                        if (pen != 0)
                            *dest = pen;
                        dest++;
                        bits >>= 4;
                    }
                    dest += 248;
                }
                if (code == 0xF01D || code == 0xF01F)
                    w = 8;
                else if (code == 0xF01E)
                    w = 3;
                else if (c > 31)
                    w = UiText_GlyphWidths[c - 32];
                else
                    w = 1;
                x += w;
            }
        }
    }
    count = (u32)(x + 7) >> 3;
    {
        u32 *dest = (u32 *)canvas;
        u32 *source = (u32 *)canvas;
        s32 row;

        for (row = 7; row >= 0; row--) {
            u32 n;

            for (n = count; n != 0; n--) {
                u32 a = source[0];
                u32 b = source[1];

                a |= a << 4;
                b |= b >> 4;
                a &= 0x0FF00FF0;
                b &= 0x00FF00FF;
                a |= a << 8;
                b |= b >> 8;
                *dest++ = (a << 4) >> 16 | b << 16;
                source += 2;
            }
            source = (u32 *)((u8 *)source - count * 8 + 0x100);
            dest = (u32 *)((u8 *)dest - count * 4 + 0x100);
        }
    }
    if (count != 0) {
        u32 *source = (u32 *)canvas;
        u32 n = count;

        do {
            u32 mask = 127;
            u32 tile;

            if (((struct UiRenderWork *)work)->alt != 0)
                mask = 255;
            tile = *tiles & 0x3FF;
            if ((tile < 0x80 || tile > 0xFF) &&
                (((struct UiRenderWork *)work)->alt == 0 || tile < 0x200 || tile > 0x27F)) {
                u32 tried;

                tile = ((struct UiRenderWork *)work)->tile_cursor & mask;
                tried = 0;
                if (((struct UiRenderWork *)work)->tile_attributes[tile] != 0) {
                    do {
                        tile = (tile + 1) & mask;
                        if (++tried > mask)
                            break;
                    } while (((struct UiRenderWork *)work)->tile_attributes[tile] != 0);
                }
                ((struct UiRenderWork *)work)->tile_cursor = (tile + 1) & mask;
                ((struct UiRenderWork *)work)->tile_attributes[tile] = 1;
                if (tile > 127)
                    tile += 0x180;
                else
                    tile |= 0x80;
                *tiles = attr | tile;
                *mirror = attr | tile;
            }
            ((u32 *)(0x06000000 + tile * 32))[0] = source[0];
            ((u32 *)(0x06000000 + tile * 32))[1] = source[0x40];
            ((u32 *)(0x06000000 + tile * 32))[2] = source[0x80];
            ((u32 *)(0x06000000 + tile * 32))[3] = source[0xC0];
            ((u32 *)(0x06000000 + tile * 32))[4] = source[0x100];
            ((u32 *)(0x06000000 + tile * 32))[5] = source[0x140];
            ((u32 *)(0x06000000 + tile * 32))[6] = source[0x180];
            ((u32 *)(0x06000000 + tile * 32))[7] = source[0x1C0];
            tiles++;
            mirror++;
            source++;
        } while (--n != 0);
    }
    Runtime_BumpFree(canvas);
    return count;
}
