/* Draft, not exact: score 26, 7 differing instructions of 876 (was 206/10).
 * The three channel curves are declared here without const: the listing
 * loads the next curve's address after each store, which const tables let
 * the scheduler hoist above it. PALBUF.H (only declarations use them) should
 * drop const when this adopts.
 * Remaining, register choice and spill slots only:
 *  - 0x10002: the green lower clamp tests blue and its branch skips blue's
 *    own test (jump threading); the three tagged devices give the registers
 *    but block the threading. Device-free forms swap g and b (n2: 220 rows);
 *    keeping only the red device (n7) leaves just this test (29 rows).
 *  - 0x400000 blend: the hoisted tr + tg and the three tint << 16 values take
 *    stack slots sum 4, tb 8, tg 12, tr 16 in the listing; here tg 4, tb 8,
 *    tr 12, sum 16 (slot order follows GCSE's hoisted pseudo numbering).
 * Earlier notes: grey sum exact with its three terms in r, g, b; g/b
 * allocator priorities are 1% apart, so edits to any loop can swap them. */
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"
extern u16 Palette_ChannelCurve[32];
extern u16 Palette_ChannelCurveLight[32];
extern u16 Palette_ChannelCurveContrast[32];

/* Expands a 512-entry (or 256-entry half) BGR555 palette into the engine's
 * three-halfword-per-entry working buffer, applying one of several colour
 * treatments selected by `mode`.
 *
 *   < 0x8000              a literal BGR555 colour; every entry is filled with
 *                         it through a 16-bit DMA replicate.
 *   == 0x8000             take the mode from src[0] and dispatch on that.
 *   0x10001 .. 0x10007    fixed treatments (grey, warm ramp, tinted ramps).
 *   other, < 0x100000     plain channel split.
 *   & 0x200000            scale the tint colour's channels by the source
 *                         entry's weighted luminance.
 *   & 0x400000            fixed-point blend of the source entry towards the
 *                         tint colour.
 *   & 0x800000            plain channel split.
 *   otherwise             `mode` is the address of an already expanded
 *                         buffer, copied in by a 32-bit DMA.
 *
 * Each destination entry is three halfwords holding the blue, green and red
 * components, each scaled into bits 10..14. */

s32 Graphics_ClampRgb555Channel(s32 val);
s32 Graphics_ClampRgb555Component(s32 val);


void Graphics_TransformPaletteBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
    u32 cnt = 512;
    u32 i;
    u32 c;
    s32 r;
    s32 g;
    s32 b;

    if (mode == 0x8000)
        mode = *src;
    if (half == 1) {
        cnt = 256;
    } else if (half == 2) {
        dst += 768;
        src += 256;
        cnt = 256;
    }

    if (mode < 0x8000) {
        *dst++ = mode & 0x7c00;
        *dst++ = (mode & 0x03e0) << 5;
        *dst++ = (mode & 0x001f) << 10;
        Dma_Set(dst - 3, dst, (((cnt - 1) * 6) >> 1) | 0x80000000, (volatile u32 *)0x040000d4);
    } else if (mode < 0x100000) {
        switch (mode) {
        case 0x10001:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = ((c << 11) & 0xf800);
                g = ((c << 7) & 0x1f000);
                b = (c & 0x7c00);
                c = Iwram_SignedDivide(r + g + b, 7);
                dst[i * 3 + 0] = c;
                dst[i * 3 + 1] = c;
                dst[i * 3 + 2] = c;
            }
            break;
        case 0x10002:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                c = Iwram_SignedDivide(r + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
                r = c * 4 + 5;
                b = c * 3 + 5;
                /* FAKEMATCH: tied red selects native blue-first copy; 223 -> 213, unchanged instruction presence after ordinary forms fail. */
                asm("" : "+r"(r));
                g = b;
                /* FAKEMATCH: independent channel lower-clamp lifetime after ordinary forms merge tests. */
                asm("" : "+r"(g));
                if (r < 8)
                    r = 8;
                if (b < 8)
                    g = 8;
                /* FAKEMATCH: retain second independent channel test by tied lifetime. */
                asm("" : "+r"(b));
                if (b < 8)
                    b = 8;
                if (r > 28)
                    r = 28;
                if (g > 28)
                    g = 28;
                if (b > 28)
                    b = 28;
                *dst++ = Palette_ChannelCurveLight[b];
                *dst++ = Palette_ChannelCurveLight[g];
                *dst++ = Palette_ChannelCurveLight[r];
            }
            break;
        case 0x10003:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r -= (u32)r >> 1;
                g -= g / 3;
                r += 6;
                g += 4;
                b -= 6;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                dst[0] = Palette_ChannelCurveContrast[b];
                dst[1] = Palette_ChannelCurveLight[g];
                dst[2] = Palette_ChannelCurve[r];
                dst += 3;
            }
            break;
        case 0x10004:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                if (r < 10)
                    r = 10;
                if (g < 16)
                    g = 16;
                if (b < 16)
                    b = 16;
                if (r > 28)
                    r = 28;
                if (g > 24)
                    g = 24;
                if (b > 26)
                    b = 26;
                g += 2;
                b += 2;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                *dst++ = Palette_ChannelCurve[b];
                *dst++ = Palette_ChannelCurve[g];
                *dst++ = Palette_ChannelCurve[r];
            }
            break;
        case 0x10005:
            for (i = 0; i < cnt; i++) {
                s32 mean;

                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                mean = Graphics_ClampRgb555Channel((r + g + b) / 3);
                r = (r >> 1) + mean;
                g = (g >> 1) + mean;
                b = (b >> 1) + mean;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                dst[0] = Palette_ChannelCurveContrast[b];
                dst[1] = Palette_ChannelCurveContrast[g];
                dst[2] = Palette_ChannelCurveContrast[r];
                dst += 3;
            }
            break;
        case 0x10006:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = r + ((g >> 3) + (b >> 3));
                r = Graphics_ClampRgb555Channel(r);
                g = g - g / 3;
                b = b - b / 3;
                dst[0] = Palette_ChannelCurve[b];
                dst[1] = Palette_ChannelCurve[g];
                dst[2] = Palette_ChannelCurveLight[r];
                dst += 3;
            }
            break;
        case 0x10007:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r -= (u32)r >> 1;
                g -= g / 3;
                r += 6;
                g += 4;
                b -= 6;
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g);
                b = Graphics_ClampRgb555Channel(b);
                dst[0] = Palette_ChannelCurveContrast[b];
                dst[1] = Palette_ChannelCurveLight[g];
                dst[2] = Palette_ChannelCurve[r];
                dst += 3;
            }
            break;
        default:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                dst[i * 3 + 0] = c & 0x7c00;
                dst[i * 3 + 1] = (c & 0x03e0) << 5;
                dst[i * 3 + 2] = (c & 0x001f) << 10;
            }
            break;
        }
    } else if (mode & 0x200000) {
        u32 tr;
        u32 tg;
        u32 tb;

        tr = mode & 31;
        tg = (mode >> 5) & 31;
        tb = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            r = ((c << 11) & 0xf800);
            g = ((c << 7) & 0x1f000);
            b = (c & 0x7c00);
            c = Iwram_SignedDivide(r + g + b, 96);
            r = c * tr;
            g = c * tg;
            b = c * tb;
            r = Graphics_ClampRgb555Component(r);
            g = Graphics_ClampRgb555Component(g);
            b = Graphics_ClampRgb555Component(b);
            dst[0] = b;
            dst[1] = g;
            dst[2] = r;
            dst += 3;
        }
    } else if (mode & 0x400000) {
        u32 tr;
        u32 tb;
        u32 tg;

        tr = mode & 31;
        tg = (mode >> 5) & 31;
        tb = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            r = c & 31;
            c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4, tr + tg + tb);
            r = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
            g = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
            b = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
            r = Graphics_ClampRgb555Channel((u32)r >> 16);
            g = Graphics_ClampRgb555Channel((u32)g >> 16);
            b = Graphics_ClampRgb555Channel((u32)b >> 16);
            *dst++ = Palette_ChannelCurve[b];
            *dst++ = Palette_ChannelCurve[g];
            *dst++ = Palette_ChannelCurve[r];
        }
    } else if (mode & 0x800000) {
        for (i = 0; i < cnt; i++) {
            c = *src++;
            dst[i * 3 + 0] = c & 0x7c00;
            dst[i * 3 + 1] = (c & 0x03e0) << 5;
            dst[i * 3 + 2] = (c & 0x001f) << 10;
        }
    } else {
        if (half == 2)
            mode += 0x600;
        Dma_Set((const void *)mode, dst, ((cnt * 6) >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    }
}
