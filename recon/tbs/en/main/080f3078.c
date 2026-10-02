/* Draft, not exact: score 863, 120 differing instructions of 831 (was 10457
 * and 447). Rewritten from the listing in plain C: no link-time constants,
 * no permuter temporaries. Same size, same frame, every loop in place.
 * Remaining, all register choice or order:
 *  - green and blue are swapped throughout (listing b r5, g r6; here g r5,
 *    b r6). The allocator's priorities are 4.46 for g and 4.43 for b; blue
 *    needs 128 weighted references (it has 118) or two fewer instructions of
 *    green's life. No natural spelling found that moves it without changing
 *    code elsewhere; this is about 100 of the 122.
 *  - the grey sum (0x10001 and the 0x200000 tint) adds its first two terms
 *    and then the third in the listing; here the last two are added first.
 *    (c takes the quotient: that keeps c out of the 0x7c00 and, as listed.)
 *  - 0x10002 computes blue first and copies it to green, and tests blue for
 *    green's lower clamp; here green is computed and tested.
 *  - in the 0x400000 blend the listing spills tr + tg last (sp+4, below the
 *    three shifted tints); here it is spilled first (sp+16).
 * The permuter reaches 206 from here only with duplicated statements. */
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

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

extern const u16 Data_080f39ee[];
extern const u16 Data_080f3a2e[];
extern const u16 Data_080f3a6e[];

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
                c = Iwram_SignedDivide(((c << 11) & 0xf800) + (((c << 7) & 0x1f000) + (c & 0x7c00)), 7);
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
                g = c * 3 + 5;
                b = g;
                if (r < 8)
                    r = 8;
                if (g < 8)
                    g = 8;
                if (b < 8)
                    b = 8;
                if (r > 28)
                    r = 28;
                if (g > 28)
                    g = 28;
                if (b > 28)
                    b = 28;
                *dst++ = Data_080f3a2e[b];
                *dst++ = Data_080f3a2e[g];
                *dst++ = Data_080f3a2e[r];
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
                dst[0] = Data_080f3a6e[b];
                dst[1] = Data_080f3a2e[g];
                dst[2] = Data_080f39ee[r];
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
                *dst++ = Data_080f39ee[b];
                *dst++ = Data_080f39ee[g];
                *dst++ = Data_080f39ee[r];
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
                dst[0] = Data_080f3a6e[b];
                dst[1] = Data_080f3a6e[g];
                dst[2] = Data_080f3a6e[r];
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
                dst[0] = Data_080f39ee[b];
                dst[1] = Data_080f39ee[g];
                dst[2] = Data_080f3a2e[r];
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
                dst[0] = Data_080f3a6e[b];
                dst[1] = Data_080f3a2e[g];
                dst[2] = Data_080f39ee[r];
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
            c = Iwram_SignedDivide(((c << 11) & 0xf800) + (((c << 7) & 0x1f000) + (c & 0x7c00)), 96);
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
        u32 tg;
        u32 tb;

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
            *dst++ = Data_080f39ee[b];
            *dst++ = Data_080f39ee[g];
            *dst++ = Data_080f39ee[r];
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
