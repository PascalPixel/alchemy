/* 2026-10-04 channel-curve ownership recovery, still uncredited.
 * The three physical scaffold labels name 32-entry channel-index curves:
 * the ordinary curve, a curve whose interior entries are all lighter, and
 * a contrast curve reaching the BGR555 maximum at index25. Their values
 * remain private-input scaffolding; no editable data or code credit added.
 * PALBUF.H owns the transform declaration and callers pass palette fields.
 * Removed the duplicate unnamed alias at the same function entry.
 * Fresh EN baseline was 223: 3 register, 8 stack, 1 operand, 3 reordered.
 * Natural blue-first/shared-value variants:223/828; lower-clamp test via
 * shared blue:898 with two deleted instructions, rejected. A real consumed
 * four-field tint record scores7516/frame44, versus native40; rejected.
 * Whole-channel register bindings score15520; removed, no device retained.
 * A 35-second focused search scores 4280 candidates, none improve. A fresh
 * 216-form tint declaration/assignment/sum search recovers two stack slots
 * with the tr, tb, tg declaration order (221). Explicit consumed scaled
 * tint locals (576 declaration/assignment forms) do not improve it; neither
 * do 216 global/scale/blend declaration permutations.
 * Scoped blue variables score 483..1343, or insert/delete instructions.
 * Tied channel lifetime tests mostly score 223..1323. Blue-first plus a
 * tied, already-defined red value changes only the intended computation/
 * copy register choice: 223 -> 213 before the tint declaration recovery.
 * A scoped lower-clamp value scores 331 or 13682..13892; rejected.
 * Lower-clamp variants using blue delete two instructions (393..913),
 * and an intervening tied channel barrier is at best 213; rejected.
 * Combined retained source scores 211: 1 register, 6 stack, 1 operand,
 * 3 reordered instructions. It remains uncredited; no adoption claim.
 */
/* Draft, not exact: score 211, 11 differing instructions of 831 (was 10457
 * and 447). Rewritten from the listing in plain C: no link-time constants,
 * no permuter temporaries. Same size, same frame, every loop in place.
 * Green and blue have the listing's registers (b r5, g r6) because the
 * 0x10004 loop takes green before red; the other four loops take red first.
 * The allocator's priorities for g and b are 1% apart, so most edits to any
 * loop swap them back (about 100 instructions): check that first.
 * Remaining, all register choice or order:
 *  - 0x10002 tests blue for green's lower clamp; here green is tested.
 *    Ordinary blue-first assignment alone still computes green. Every spelling
 *    of the two assignments compiles to the same code or swaps g and b back
 *    (b = g = x computes blue first but loses the registers).
 *  - in the 0x400000 blend the listing spills tr + tg last (sp+4, below the
 *    three shifted tints); here it is spilled first (sp+16). The four are
 *    GCSE's hoisted temporaries (regs 475..478 in the .gcse dump), numbered
 *    in hash order of their expressions, which depends on the pseudo numbers
 *    of tr, tg and tb: so on how many pseudos the code above the blend
 *    makes, not on the order the blend is written in. Shifted tints in
 *    variables, a sum variable and other operand orders leave it unchanged.
 *  - three reordered instructions that follow from those two.
 * The grey sum is exact now: its three terms go into r, g and b and are
 * added from there (c takes the quotient). */
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"
#include "PALBUF.H"

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
