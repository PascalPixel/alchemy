/* 2026-09-29 alchemy permute: score 14624 to 11152 on the permuter's
   scorer (0 is exact); remaining 278 register-only, 22 stack-only, 50
   operand, 51 reordered, 30 inserted, 25 deleted. Kept rewrites: 26x swap
   commutative operands, 25x reorder independent statements, 20x reorder
   local declarations, 15x introduce a temporary, 10x add a same-width
   cast, 10x pointer arithmetic or indexing, 10x split or join a compound
   assignment, 9x change loop form, 8x remove a temporary, 6x drop a
   same-width cast, 5x move an assignment into or out of a condition, 4x
   toggle register, 3x test truth or compare with zero. FAKEMATCH: the
   permuter's temporaries, register hints and swapped operand orders below
   only steer allocation and scheduling; no programmer would write them, so
   they stay tagged until a natural spelling replaces them. */
/* Draft, not exact (2026-09-24): candidate=1796 reference=1796 differing_halfwords=796. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00000200;
extern u8 Value_00008000;
extern u8 Value_00000100;
extern u8 Value_00000300;
extern u8 Value_00007c00;
extern u8 Value_000003e0;
extern u8 Value_0000f800;
extern u8 Value_00000600;
#include "DMA.H"
#include "IWRAM_CALL.H"

/* games/THE BROKEN SEAL/INCLUDE/TYPES.H already aliases this owner; the identical
   definition is repeated here so the entry symbol is visible in this file. */

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
 *  - The three calls to 0x03000118 use the shared ip-linked interface in
 *    IWRAM_CALL.H, matching the reference's `mov ip, pc` / `bx r3` contract.
 *  - The reference issues each DMA request as one three-word block store
 *    (`stmia r3!, {r0,r1,r2}` then `subs r3, #12`), where the shared
 *    Dma_Set call emits three separate word stores.
 *  - The reference also cross-jumps its two DMA request sites into one shared
 *    tail at the end of the function; the two Dma_Set calls here are
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
s32 Math_Div(s32 numerator, s32 denominator);

typedef s32 (*DivideFunc)(s32 num, s32 den);

extern const u16 Data_080f39ee[];
extern const u16 Data_080f3a2e[];
extern const u16 Data_080f3a6e[];

void Graphics_TransformPaletteBuffer(u32 mode, u16 *src, u16 *dst, s32 half)
{
    const u16 *tbl;
    register u16 *out;
    DivideFunc divide;
    register u32 cnt;
    register s32 r;
    u32 i;
    u32 c;
    register s32 g;
    u32 tint_r;
    s32 b;
    u32 tint_g;
    u32 tint_b;
    s32 v;

    cnt = 512;
    if (mode == 0x8000) {
        (s32)(mode = src[0]);
    }
    if (half == 1) {
        cnt = 256;
    } else if (half == 2) {
        dst += 768;
        src = src + 256;
        cnt = 256;
    }
    if (mode < 0x8000) {
        u32 tmp8;
        *dst++ = mode & (u32)0x7c00;
        dst++[0] = (mode & (s32)&Value_000003e0) << 5;
        *dst++ = (mode & 0x001f) << 10;
        tmp8 = ((cnt - 1) * 6) >> 1;
        Dma_Set(dst - 3, dst, tmp8 | 0x80000000, (volatile u32 *)0x040000d4);
    } else if (0x100000 > mode) {
        switch (mode) {
            u16 *tmp7;
        case 0x10001:
            divide = (DivideFunc)0x03000380;
            out = dst;
            for (i = 0; i < cnt; i++) {
                u32 tmp2;
                c = *src++;
                tmp2 = 0x1f000 & (c << 7);
                v = divide((0xf800 & (c << 11)) + tmp2 + (0x7c00 & c), 7);
                out[0] = v;
                out[1] = v;
                out[2] = v;
                out += 3;
            }
            break;
        case 0x10002:
            divide = (DivideFunc)0x03000380;
            tbl = Data_080f3a2e;
            for (i = 0; i < cnt; i++) {
                u32 tmp3;
                c = *src++;
                tmp3 = c & 31;
                v = divide((31 & (c >> 10)) + (((c >> 5) & 31) + tmp3), 10);
                r = v * 4 + 5;
                b = 5 + v * 3;
                if (8 > (g = (u32)(3 * v + 5))) {
                    g = 8;
                }
                if (b < 8) {
                    b = 8;
                }
                if (r < 8) {
                    r = 8;
                }
                if (r > 28) {
                    r = 28;
                }
                if (b > 28) {
                    b = 28;
                }
                if (g > 28) {
                    g = 28;
                }
                *dst++ = tbl[b];
                *dst++ = tbl[g];
                *dst++ = tbl[r];
            }
            break;
        case 0x10003:
            i = 0;
            if (i < cnt) {
                do {
                    c = *src++;
                    r = c & 31;
                    r = Graphics_ClampRgb555Channel(r - ((u32)r >> 1) + 6);
                    g = (c >> 5) & 31;
                    b = (c >> 10) & 31;
                    g = Graphics_ClampRgb555Channel(4 + (g - Math_Div(g, 3)));
                    b = Graphics_ClampRgb555Channel(b - 6);
                    dst[0] = Data_080f3a6e[b];
                    dst[1] = *(Data_080f3a2e + g);
                    dst[2] = Data_080f39ee[r];
                    dst += 3;
                    ++i;
                } while (i < cnt);
            }
            break;
        case 0x10004:
            i = 0;
            tbl = Data_080f39ee;
            while (i < cnt) {
                c = src++[0];
                r = c & 31;
                g = (c >> 5) & 31;
                b = 31 & (c >> 10);
                if (r < 10) {
                    r = 10;
                }
                if (16 > b) {
                    b = 16;
                }
                if (g < 16) {
                    g = 16;
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
                i++;
            }
            break;
        case 0x10005:
            tmp7 = Data_080f3a6e;
            i = 0;
            tbl = tmp7;
            for (; i < cnt; dst += 3) {
                i++;
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                v = Graphics_ClampRgb555Channel(Math_Div(r + (g + b), 3));
                r = Graphics_ClampRgb555Channel((r >> 1) + v);
                g = Graphics_ClampRgb555Channel((g >> 1) + v);
                b = Graphics_ClampRgb555Channel((b >> 1) + v);
                dst[0] = tbl[b];
                dst[1] = *&tbl[g];
                dst[2] = tbl[r];
            }
            break;
        case 0x10006:
            i = 0;
            while (i < cnt) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = Graphics_ClampRgb555Channel(r + ((b >> 3) + (g >> 3)));
                g = g - Math_Div(g, 3);
                b = b - Math_Div(b, 3);
                *dst = Data_080f39ee[b];
                dst[1] = Data_080f39ee[g];
                dst[2] = *(Data_080f3a2e + r);
                dst = 3 + dst;
                i++;
            }
            break;
        case 0x10007:
            i = 0;
            if (i < cnt) {
                do {
                    c = *src++;
                    r = c & 31;
                    r = Graphics_ClampRgb555Channel(r - ((u32)r >> 1) + 6);
                    g = (c >> 5) & 31;
                    b = (c >> 10) & 31;
                    g = Graphics_ClampRgb555Channel(4 + (g - Math_Div(g, 3)));
                    b = Graphics_ClampRgb555Channel(b - 6);
                    dst[0] = Data_080f3a6e[b];
                    dst[1] = ((u16 *)Data_080f3a2e)[g];
                    dst[2] = Data_080f39ee[r];
                    dst = dst + 3;
                    i++;
                } while (i < cnt);
            }
            break;
        default:
            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                out[0] = c & 0x7c00;
                *(out + 1) = (c & 0x03e0) << 5;
                out[2] = (0x001f & c) << 10;
                out += 3;
            }
            break;
        }
    } else if ((s32)(mode & 0x200000) != 0) {
        divide = (DivideFunc)0x03000380;
        tint_r = 31 & mode;
        tint_g = (mode >> 5) & 31;
        tint_b = 31 & (mode >> 10);
        for (i = 0; i < cnt; i++) {
            u8 *tmp5;
            c = *src++;
            tmp5 = &Value_00007c00;
            v = divide(((c << 11) & 0xf800) + (((c << 7) & 0x1f000) + (c & (s32)tmp5)), 96);
            b = tint_b * v;
            g = tint_g * v;
            r = v * tint_r;
            r = Graphics_ClampRgb555Component(r);
            g = Graphics_ClampRgb555Component(g);
            b = Graphics_ClampRgb555Component(b);
            dst[0] = b;
            dst[1] = g;
            dst[2] = r;
            dst += 3;
        }
    } else if ((mode & 0x400000) != 0) {
        tbl = Data_080f39ee;
        divide = (DivideFunc)0x03000380;
        tint_r = mode & 31;
        tint_g = (mode >> 5) & 31;
        tint_b = (mode >> 10) & 31;
        i = 0;
        while (i < cnt) {
            s32 tmp;
            s32 tmp4;
            c = src++[0];
            v = divide(((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4, tint_r + tint_g + tint_b);
            r = Iwram_MulQ16(((v * tint_r) >> 4) << 16, (s32)(tint_r << 16) >> 4);
            g = Iwram_MulQ16(((v * tint_g) >> 4) << 16, (s32)(tint_g << 16) >> 4);
            b = Iwram_MulQ16(((tint_b * v) >> 4) << 16, (s32)(tint_b << 16) >> 4);
            tmp4 = Graphics_ClampRgb555Channel((u32)r >> 16);
            g = Graphics_ClampRgb555Channel((u32)g >> 16);
            tmp = Graphics_ClampRgb555Channel((u32)b >> 16);
            b = tmp;
            *dst++ = tbl[b];
            r = tmp4;
            *dst++ = tbl[g];
            *dst++ = tbl[r];
            i++;
        }
    } else if (mode & 0x800000) {
        out = dst;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            out[0] = c & 0x7c00;
            out[1] = (c & 0x03e0) << 5;
            *(out + 2) = (0x001f & c) << 10;
            out += 3;
        }
    } else {
        if (2 == half) {
            mode += (s32)&Value_00000600;
        }
        Dma_Set((const void *)mode, dst, ((cnt * 6) >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    }
}
