#include "video_dma_family.h"

/* games/gs1/include/types.h already aliases this owner; the identical
   definition is repeated here so the entry symbol is visible in this file. */
#define Graphics_TransformPaletteBuffer Func_080f3078

/* Expands a 512-entry (or 256-entry half) BGR555 palette into the engine's
 * three-halfword-per-entry working buffer, applying one of several colour
 * treatments selected by `mode`.
 *
 * `mode` is a tagged word:
 *   < 0x8000              a literal BGR555 colour; every entry is filled with
 *                         it through a 16-bit DMA replicate.
 *   == 0x8000             re-read the tag from src[0] and dispatch again on
 *                         that value; because src[0] is a halfword it can
 *                         still land on any branch below 0x10000, so this is
 *                         a re-dispatch and not a jump into the literal path.
 *   0x10001 .. 0x10007    fixed treatments (grey, warm ramp, tinted ramps).
 *   other, < 0x100000     plain channel split.
 *   & 0x200000            scale the tint colour's channels by the source
 *                         entry's weighted luminance.
 *   & 0x400000            fixed-point blend of the source entry towards the
 *                         tint colour.
 *   & 0x800000            plain channel split (a duplicate of the default
 *                         body, kept separate as in the reference).
 *   otherwise             `mode` is the address of an already expanded
 *                         buffer, copied in by a 32-bit DMA.
 *
 * Each destination entry is three halfwords holding the blue, green and red
 * components, each scaled into bits 10..14.
 *
 * Uncertain: the three 32-entry halfword ramps that follow this function in
 * ROM are only reachable by address, and the role of the IWRAM ARM helper at
 * 0x03000118 is not established beyond its 16.16 argument shapes.
 *
 * Known residuals against the reference, measured at 1796/1796 bytes and 766
 * differing halfwords:
 *
 *  - The three calls to 0x03000118 are reached in the reference by an inline
 *    `mov ip, pc` / `bx r3` pair, which leaves the return address in ip rather
 *    than lr. The approved route compiles an indirect call as `bl
 *    _call_via_r3`; both forms are two halfwords, so each of the three sites
 *    differs in both of them. This is a compiler-route difference, not a
 *    source spelling: a typed indirect call is the sanctioned model for a
 *    call-via slot, and no ordinary C reaches the inline form here.
 *  - The reference issues each DMA request as one three-word block store
 *    (`stmia r3!, {r0,r1,r2}` then `subs r3, #12`), where the shared
 *    StartDmaTransfer helper emits three separate word stores.
 *  - The reference also cross-jumps its two DMA request sites into one shared
 *    tail at the end of the function; the two StartDmaTransfer calls here are
 *    expanded separately.
 *  - In cases 0x10003 and 0x10007 the reference emits Math_Div before the
 *    three channel clamps, while the spelling kept here schedules the red
 *    clamp first. Splitting those statements to force the reference call order
 *    was measured and costs 8 extra bytes and about 57 more differing
 *    halfwords, because a channel then spills across the calls.
 *
 * The remaining bulk of the residual is register assignment and instruction
 * scheduling inside the eleven loop bodies: the reference keeps the literal 31
 * and the loop-invariant ramp pointers in callee-saved high registers, where
 * this candidate keeps them in low registers and reloads or spills them around
 * the calls. Every reference branch, loop, call and store is represented, and
 * the call inventory agrees exactly (17 clamp-channel, 3 clamp-component,
 * 5 Math_Div, 7 indirect). This is a draft, not exact C. */

s32 Graphics_ClampRgb555Channel(s32 val);
s32 Graphics_ClampRgb555Component(s32 val);
s32 Func_080022ec(s32 numerator, s32 denominator);

#define Math_Div Func_080022ec

typedef s32 (*DivideFunc)(s32 num, s32 den);
typedef u32 (*BlendFunc)(s32 a, s32 b);

extern const u16 Data_080f39ee[];
extern const u16 Data_080f3a2e[];
extern const u16 Data_080f3a6e[];

#define COLOR_RAMP_A Data_080f39ee
#define COLOR_RAMP_B Data_080f3a2e
#define COLOR_RAMP_C Data_080f3a6e

void Graphics_TransformPaletteBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
    DivideFunc divide;
    BlendFunc blend;
    const u16 *tbl;
    u16 *out;
    u32 cnt;
    u32 i;
    u32 c;
    s32 r;
    s32 g;
    s32 b;
    s32 v;
    u32 tint_r;
    u32 tint_g;
    u32 tint_b;

    cnt = 512;
    if (mode == 0x8000) {
        mode = src[0];
    }
    if (half == 1) {
        cnt = 256;
    } else if (half == 2) {
        dst += 768;
        cnt = 256;
        src += 256;
    }

    if (mode < 0x8000) {
        *dst++ = mode & 0x7c00;
        *dst++ = (mode & 0x03e0) << 5;
        *dst++ = (mode & 0x001f) << 10;
        StartDmaTransfer(dst - 3, dst, (((cnt - 1) * 6) >> 1) | 0x80000000);
    } else if (mode < 0x100000) {
        switch (mode) {
        case 0x10001:
            divide = (DivideFunc)0x03000380;
            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                v = divide(((c << 11) & 0xf800) + ((c << 7) & 0x1f000) +
                               (c & 0x7c00),
                           7);
                out[0] = v;
                out[1] = v;
                out[2] = v;
                out += 3;
            }
            break;
        case 0x10002:
            divide = (DivideFunc)0x03000380;
            tbl = COLOR_RAMP_B;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                v = divide((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
                r = v * 4 + 5;
                g = v * 3 + 5;
                b = v * 3 + 5;
                if (r < 8) {
                    r = 8;
                }
                if (g < 8) {
                    g = 8;
                }
                if (b < 8) {
                    b = 8;
                }
                if (r > 28) {
                    r = 28;
                }
                if (g > 28) {
                    g = 28;
                }
                if (b > 28) {
                    b = 28;
                }
                *dst++ = tbl[b];
                *dst++ = tbl[g];
                *dst++ = tbl[r];
            }
            break;
        case 0x10003:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = Graphics_ClampRgb555Channel(r - ((u32)r >> 1) + 6);
                g = Graphics_ClampRgb555Channel(g - Math_Div(g, 3) + 4);
                b = Graphics_ClampRgb555Channel(b - 6);
                dst[0] = COLOR_RAMP_C[b];
                dst[1] = COLOR_RAMP_B[g];
                dst[2] = COLOR_RAMP_A[r];
                dst += 3;
            }
            break;
        case 0x10004:
            tbl = COLOR_RAMP_A;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                if (r < 10) {
                    r = 10;
                }
                if (g < 16) {
                    g = 16;
                }
                if (b < 16) {
                    b = 16;
                }
                if (r > 28) {
                    r = 28;
                }
                if (g > 24) {
                    g = 24;
                }
                if (b > 26) {
                    b = 26;
                }
                r = Graphics_ClampRgb555Channel(r);
                g = Graphics_ClampRgb555Channel(g + 2);
                b = Graphics_ClampRgb555Channel(b + 2);
                *dst++ = tbl[b];
                *dst++ = tbl[g];
                *dst++ = tbl[r];
            }
            break;
        case 0x10005:
            tbl = COLOR_RAMP_C;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                v = Graphics_ClampRgb555Channel(Math_Div(r + g + b, 3));
                r = Graphics_ClampRgb555Channel((r >> 1) + v);
                g = Graphics_ClampRgb555Channel((g >> 1) + v);
                b = Graphics_ClampRgb555Channel((b >> 1) + v);
                dst[0] = tbl[b];
                dst[1] = tbl[g];
                dst[2] = tbl[r];
                dst += 3;
            }
            break;
        case 0x10006:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = Graphics_ClampRgb555Channel(r + ((g >> 3) + (b >> 3)));
                g = g - Math_Div(g, 3);
                b = b - Math_Div(b, 3);
                dst[0] = COLOR_RAMP_A[b];
                dst[1] = COLOR_RAMP_A[g];
                dst[2] = COLOR_RAMP_B[r];
                dst += 3;
            }
            break;
        case 0x10007:
            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = Graphics_ClampRgb555Channel(r - ((u32)r >> 1) + 6);
                g = Graphics_ClampRgb555Channel(g - Math_Div(g, 3) + 4);
                b = Graphics_ClampRgb555Channel(b - 6);
                dst[0] = COLOR_RAMP_C[b];
                dst[1] = COLOR_RAMP_B[g];
                dst[2] = COLOR_RAMP_A[r];
                dst += 3;
            }
            break;
        default:
            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                out[0] = c & 0x7c00;
                out[1] = (c & 0x03e0) << 5;
                out[2] = (c & 0x001f) << 10;
                out += 3;
            }
            break;
        }
    } else if ((mode & 0x200000) != 0) {
        divide = (DivideFunc)0x03000380;
        tint_r = mode & 31;
        tint_g = (mode >> 5) & 31;
        tint_b = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            v = divide(((c << 11) & 0xf800) + ((c << 7) & 0x1f000) +
                           (c & 0x7c00),
                       96);
            r = tint_r * v;
            g = tint_g * v;
            b = tint_b * v;
            r = Graphics_ClampRgb555Component(r);
            g = Graphics_ClampRgb555Component(g);
            b = Graphics_ClampRgb555Component(b);
            dst[0] = b;
            dst[1] = g;
            dst[2] = r;
            dst += 3;
        }
    } else if ((mode & 0x400000) != 0) {
        divide = (DivideFunc)0x03000380;
        blend = (BlendFunc)0x03000118;
        tint_r = mode & 31;
        tint_g = (mode >> 5) & 31;
        tint_b = (mode >> 10) & 31;
        tbl = COLOR_RAMP_A;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            v = divide((((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4),
                       tint_r + tint_g + tint_b);
            r = blend(((tint_r * v) >> 4) << 16, (s32)(tint_r << 16) >> 4);
            g = blend(((tint_g * v) >> 4) << 16, (s32)(tint_g << 16) >> 4);
            b = blend(((tint_b * v) >> 4) << 16, (s32)(tint_b << 16) >> 4);
            r = Graphics_ClampRgb555Channel((u32)r >> 16);
            g = Graphics_ClampRgb555Channel((u32)g >> 16);
            b = Graphics_ClampRgb555Channel((u32)b >> 16);
            *dst++ = tbl[b];
            *dst++ = tbl[g];
            *dst++ = tbl[r];
        }
    } else if ((mode & 0x800000) != 0) {
        out = dst;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            out[0] = c & 0x7c00;
            out[1] = (c & 0x03e0) << 5;
            out[2] = (c & 0x001f) << 10;
            out += 3;
        }
    } else {
        if (half == 2) {
            mode += 0x600;
        }
        StartDmaTransfer((const void *)mode, dst, ((cnt * 6) >> 2) | 0x84000000);
    }
}
