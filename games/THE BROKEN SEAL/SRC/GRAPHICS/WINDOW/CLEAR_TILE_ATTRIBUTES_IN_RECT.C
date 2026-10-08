#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "TBS_EDITION.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "MEM.H"
#include "IWRAM_CALL.H"
#include "TEXT_RENDER_RUNTIME.H"


static __inline__ void ClearWords(void *dest, s32 size)
{
    /* FAKEMATCH: needed to match: through this wrapper the pen table's address is reloaded into r0 in each colour arm; a direct call keeps it in r10 (44 differ). */
    Iwram_ClearWords(dest, size);
}

static __inline__ void FillWords(void *dest, s32 size, u32 value)
{
    /* FAKEMATCH: needed to match: through this wrapper both arms load the canvas before the routine and the fill value; a direct call loads the fill value first (8 differ). */
    Iwram_FillWords(dest, size, value);
}

s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x)
{
    /* FAKEMATCH: needed to match: an unread 32-byte reserve, the native frame's extra bytes (the frame is 36 bytes without it). */
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
    canvas = MemAlloc(0x800);
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
#if EDITION_INTERNATIONAL
                    x += UiText_GlyphWidths[0];
#else
                    x += 8;
#endif
                    break;
                case 14:
                case 15:
                case 28:
                    script++;
                case 11:
                case 12:
                case 17:
#if EDITION_INTERNATIONAL
                case 29:
#endif
                    script++;
                    break;
                }
            } else {
                c &= 0xFF;
#if !EDITION_INTERNATIONAL
                if (c == 222 || c == 223) {
                    x -= 3;
                    c += 32;
                }
#endif
                src = (u32 *)(glyphs + c * 32);
                dst = canvas + x;
                for (i = 0; i < 8; i++) {
                    bits = *src++;
                    for (j = 0; j < 4; j++) {
                        u8 pen = pens[bits & 15];

                        if (pen != 0)
                            *dst = pen;
                        dst++;
                        bits >>= 4;
                        pen = pens[bits & 15];
                        if (pen != 0)
                            *dst = pen;
                        dst++;
                        bits >>= 4;
                    }
                    dst += 248;
                }
                if (code == 0xF01D || code == 0xF01F)
                    x += 8;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                else if (code == 0xF01E || code == 0xF005)
#else
                else if (code == 0xF01E)
#endif
                    x += 3;
                else {
#if !EDITION_INTERNATIONAL
                    if (c > 128) {
                        if (c == 254 || c == 255)
                            w = 3;
                        else
                            w = 8;
                    } else
#endif
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

    for (i = 0; i < count; i++) {
        u32 mask = 127;
        u32 tile;
        u32 *p;

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
        p = (u32 *)(0x06000000 + tile * 32);
        *p++ = ((u32 *)canvas)[i];
        *p++ = ((u32 *)canvas)[i + 0x40];
        *p++ = ((u32 *)canvas)[i + 0x80];
        *p++ = ((u32 *)canvas)[i + 0xC0];
        *p++ = ((u32 *)canvas)[i + 0x100];
        *p++ = ((u32 *)canvas)[i + 0x140];
        *p++ = ((u32 *)canvas)[i + 0x180];
        *p++ = ((u32 *)canvas)[i + 0x1C0];
        tiles++;
        mirror++;
    }
    Runtime_BumpFree(canvas);
    return count;
}

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    u32 row;

    for (row = 0; row < height; row++) {
        u32 column;

        for (column = 0; column < width; column++) {
            u32 tile = *cursor++ & 0x3FF;

            if ((tile - 0x80) <= 0x7F ||
                (alternate != 0 &&
                 tile > 0x1FF &&
                 tile <= 0x27F)) {
                u32 index = ((tile & 0xFF) ^ 0x80);
                ((struct UiRenderWork *)base)->tile_attributes[index] &= 0xFC;
            }
        }
        cursor += 32 - width;
    }
}

void UiWindow_MarkVisibleTileAttributes(void)
{
    u8 *base = gWindowWork[0];
    u16 *cursor = (u16 *)base;
    s32 width = 30;
    u32 alternate = ((struct UiRenderWork *)base)->alt;
    s32 row;
    s32 x;

    for (row = 20; row != 0; row--) {
        for (x = 0; x < width; x++) {
            u32 tile = *cursor++ & 0x3ff;

            if ((tile - 0x80) <= 0x7f ||
                (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
                u32 index = ((tile & 0xff) ^ 0x80);

                ((struct UiRenderWork *)base)->tile_attributes[index] |= 2;
            }
        }
    }

    for (row = 0; row < 256; row++, base++) {
        if (((struct UiRenderWork *)base)->tile_attributes[0] == 1)
            ((struct UiRenderWork *)base)->tile_attributes[0] = 0;
    }
}
