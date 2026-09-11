#include "types.h"

/*
 * main:08090a5c, 1816 bytes.
 *
 * Build the per-entry RGB555 "component" table a palette effect feeds to the
 * hardware.  The caller hands in a mode word, a BGR555 source palette, a
 * destination array of three u16 components per entry (stored blue, green,
 * red) and a part selector.
 *
 *   part == 0  whole 448-entry palette
 *   part == 1  first 224 entries
 *   part == 2  second 224 entries (source +224 entries, destination +224 triples)
 *
 * The mode word is interpreted in four ranges:
 *
 *   mode <  0x8000     a literal BGR555 colour.  One triple is expanded and
 *                      DMA3 replicates it over the remaining entries by
 *                      overlapping source and destination.
 *   mode == 0x8000     re-read as the current backdrop colour at 0x05000000
 *                      and then treated as one of the other cases.
 *   0x8000 <= mode
 *       <  0x100000    seven named recolour recipes (0x10001..0x10007);
 *                      anything else expands the source palette unchanged.
 *   otherwise          bit 21 / 22 / 23 select a tint, a saturation blend or
 *                      the plain expansion; with none of them set the mode
 *                      word is a pointer to an already-built table that DMA3
 *                      copies in 32-bit units.
 *
 * Uncertain, and left as evidence rather than interpretation:
 *  - The roles of the three 32-entry ramps at 0x0809e92e / 0x0809e96e /
 *    0x0809e9ae.  They are contiguous and are indexed by a clamped 0..31
 *    channel value, but no case uses one ramp per colour channel: 0x10002 and
 *    0x10004 drive all three components from a single ramp, and the other
 *    cases mix them.  They are declared separately here because that is all
 *    the reference proves; do not read them as "the red/green/blue ramp".
 *  - Func_03000118 is a second ARM-mode IWRAM helper (Func_03000380 is the
 *    signed divide).  It takes two 16.16-looking operands and only the high
 *    halfword of its result is consumed, which is consistent with a scaling
 *    routine, but the operation itself is not established here.  The reference
 *    reaches it with "mov ip, pc / bx r3" rather than the "bl _call_via_rN"
 *    veneer it uses for the divide, and it does so with the callee already in
 *    r3 - the same register the veneer form uses two instructions earlier in
 *    the same loop.  That sequence does not set lr, so it is not an ordinary
 *    Thumb indirect call; the source spelling that produces it is unknown and
 *    both tried spellings (a local function pointer, and a direct call through
 *    the cast literal) compile to the veneer.  Three call sites differ.
 *  - The DMA count in the mode < 0x8000 case is written "* 3 * 2 >> 1"
 *    because the reference literally multiplies (cnt - 1) by six and then
 *    shifts right by one: the shift is a basic block it shares with the
 *    32-bit-unit path at the end of the function.  The transferred count is
 *    (cnt - 1) * 3 halfwords either way.
 *
 * Residual of this draft (1736 candidate bytes against 1816 reference,
 * 881 differing halfwords).  Control flow, calls, jump table, tables, access
 * widths, constants and side effects agree case by case: same 29 direct calls
 * and 7 indirect calls in the same cases, same 36 halfword stores, same 22
 * loop back-edges, and the same destination step per case (2 + 2 + 2 in cases
 * 0x10002..0x10004 and in the mode < 0x8000 expansion, one step of 6 in cases
 * 0x10005..0x10007, in the tint case and in both plain expansions).
 *
 * What is left is register allocation.  The reference holds the destination
 * pointer in r8 for the whole function, so most stores cost an extra
 * "mov rN, r8" and most advances an extra "mov r3, #2 / add r8, r3"; this
 * source lets it live in r4 instead.  That accounts for 35 of the 36
 * instructions the candidate is short (191 "movs" in the reference against
 * 156 here) and for the frame size (40 bytes against 36: the reference carries
 * one more spill slot, and this source reuses six stack stores the reference
 * does not).  No register was forced to chase it.
 *
 * Two smaller unexplained shapes: the reference's final DMA write is one
 * "stmia r3!, {r0,r1,r2}" reached from both paths, where three word stores
 * through a pointer are emitted here (spelling it as a 12-byte aggregate
 * assignment was tried and rejected - it produces an ldmia/stmia pair from a
 * larger frame, which the reference does not have); and two bit tests near
 * the end come out with beq/bne inverted against the reference.
 */

#define PaletteEffect_BuildComponentTable Func_08090a5c

typedef s32 (*SignedDivide)(s32 numerator, s32 denominator);
typedef s32 (*ScaleFixed)(s32 value, s32 scale);

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 BattleFx_ClampRgb555Channel(s32 value);
s32 BattleFx_ClampRgb555Component(s32 value);

/* Three contiguous 32-entry ramps indexed by a clamped channel value. */
extern u16 Data_0809e92e[];
extern u16 Data_0809e96e[];
extern u16 Data_0809e9ae[];

void PaletteEffect_BuildComponentTable(u32 mode, u16 *src, u16 *dst, s32 part)
{
    u32 *reg;
    u32 i;
    u32 cnt;
    u32 c;

    cnt = 448;
    if (mode == 0x8000)
        mode = *(u16 *)0x05000000;

    if (part == 1) {
        cnt = 224;
    } else if (part == 2) {
        dst += 672;
        cnt = 224;
        src += 224;
    }

    if (mode < 0x8000) {
        /* One expanded triple, then an overlapping DMA3 fill. */
        *dst++ = mode & 0x7c00;
        *dst++ = (mode & 0x3e0) << 5;
        *dst++ = (mode & 0x1f) << 10;

        reg = (u32 *)0x040000d4;
        reg[0] = (u32)(dst - 3);
        reg[1] = (u32)dst;
        reg[2] = (((cnt - 1) * 3 * 2) >> 1) | 0x80000000;
        return;
    }

    if (mode < 0x100000) {
        switch (mode) {
        case 0x10001: {
            /* Grey: 1024 * (2R + 4G + B) / 7 written to all three components. */
            SignedDivide divide;
            u16 *out;
            s32 v;

            divide = (SignedDivide)0x03000380;
            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                v = divide(((c << 11) & 0xf800) + ((c << 7) & 0x1f000) +
                               (c & 0x7c00),
                           7);
                out[0] = (u16)v;
                out[1] = (u16)v;
                out[2] = (u16)v;
                out += 3;
            }
            return;
        }

        case 0x10002: {
            /* Flat sepia ramp driven by the entry's brightness. */
            SignedDivide divide;
            s32 v;
            s32 r;
            s32 g;
            s32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                divide = (SignedDivide)0x03000380;
                v = divide((s32)((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31)),
                           10);
                r = v * 4 + 5;
                g = v * 3 + 5;
                b = v * 3 + 5;
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
                *dst++ = Data_0809e96e[b];
                *dst++ = Data_0809e96e[g];
                *dst++ = Data_0809e96e[r];
            }
            return;
        }

        case 0x10003: {
            /* Warm shift; black stays black. */
            u32 r;
            u32 g;
            u32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                if ((r | g | b) != 0) {
                    r = r - (r >> 1) + 10;
                    g = (u32)((s32)g - Func_080022ec((s32)g, 3) + 8);
                    b = b - 7;
                    r = (u32)BattleFx_ClampRgb555Channel((s32)r);
                    g = (u32)BattleFx_ClampRgb555Channel((s32)g);
                    b = (u32)BattleFx_ClampRgb555Channel((s32)b);
                }
                *dst++ = Data_0809e9ae[b];
                *dst++ = Data_0809e96e[g];
                *dst++ = Data_0809e92e[r];
            }
            return;
        }

        case 0x10004: {
            /* Wash: each channel is pushed into a narrow band before the ramp. */
            s32 r;
            s32 g;
            s32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = (s32)(c & 31);
                g = (s32)((c >> 5) & 31);
                b = (s32)((c >> 10) & 31);
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
                g = g + 2;
                b = b + 2;
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                *dst++ = Data_0809e92e[b];
                *dst++ = Data_0809e92e[g];
                *dst++ = Data_0809e92e[r];
            }
            return;
        }

        case 0x10005: {
            /* Desaturate halfway towards the entry's mean. */
            s32 r;
            s32 g;
            s32 b;
            s32 v;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = (s32)(c & 31);
                g = (s32)((c >> 5) & 31);
                b = (s32)((c >> 10) & 31);
                v = BattleFx_ClampRgb555Channel(
                    Func_080022ec(r + g + b, 3));
                r = (r >> 1) + v;
                g = (g >> 1) + v;
                b = (b >> 1) + v;
                r = BattleFx_ClampRgb555Channel(r);
                g = BattleFx_ClampRgb555Channel(g);
                b = BattleFx_ClampRgb555Channel(b);
                dst[0] = Data_0809e9ae[b];
                dst[1] = Data_0809e9ae[g];
                dst[2] = Data_0809e9ae[r];
                dst += 3;
            }
            return;
        }

        case 0x10006: {
            /* Push the other two channels into red. */
            s32 r;
            s32 g;
            s32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                g = (s32)((c >> 5) & 31);
                b = (s32)((c >> 10) & 31);
                r = (s32)(c & 31);
                r = BattleFx_ClampRgb555Channel(r + ((g >> 3) + (b >> 3)));
                g = g - Func_080022ec(g, 3);
                b = b - Func_080022ec(b, 3);
                dst[0] = Data_0809e92e[b];
                dst[1] = Data_0809e92e[g];
                dst[2] = Data_0809e96e[r];
                dst += 3;
            }
            return;
        }

        case 0x10007: {
            /* Same shape as 0x10003 with a milder shift and no black guard. */
            u32 r;
            u32 g;
            u32 b;

            for (i = 0; i < cnt; i++) {
                c = *src++;
                r = c & 31;
                g = (c >> 5) & 31;
                b = (c >> 10) & 31;
                r = r - (r >> 1) + 6;
                g = (u32)((s32)g - Func_080022ec((s32)g, 3) + 4);
                b = b - 6;
                r = (u32)BattleFx_ClampRgb555Channel((s32)r);
                g = (u32)BattleFx_ClampRgb555Channel((s32)g);
                b = (u32)BattleFx_ClampRgb555Channel((s32)b);
                dst[0] = Data_0809e9ae[b];
                dst[1] = Data_0809e96e[g];
                dst[2] = Data_0809e92e[r];
                dst += 3;
            }
            return;
        }

        default: {
            /* Plain expansion of the source palette. */
            u16 *out;

            out = dst;
            for (i = 0; i < cnt; i++) {
                c = *src++;
                out[0] = c & 0x7c00;
                out[1] = (c & 0x3e0) << 5;
                out[2] = (c & 0x1f) << 10;
                out += 3;
            }
            return;
        }
        }
    }

    if ((mode & 0x200000) != 0) {
        /* Tint: scale a fixed colour by the entry's brightness. */
        SignedDivide divide;
        s32 red;
        s32 green;
        s32 blue;
        s32 k;
        s32 rr;
        s32 gg;
        s32 bb;

        red = (s32)(mode & 31);
        green = (s32)((mode >> 5) & 31);
        blue = (s32)((mode >> 10) & 31);
        for (i = 0; i < cnt; i++) {
            c = *src++;
            divide = (SignedDivide)0x03000380;
            k = divide(((c << 11) & 0xf800) + ((c << 7) & 0x1f000) +
                           (c & 0x7c00),
                       96);
            rr = red * k;
            gg = green * k;
            bb = blue * k;
            rr = BattleFx_ClampRgb555Component(rr);
            gg = BattleFx_ClampRgb555Component(gg);
            bb = BattleFx_ClampRgb555Component(bb);
            dst[0] = (u16)bb;
            dst[1] = (u16)gg;
            dst[2] = (u16)rr;
            dst += 3;
        }
        return;
    }

    if ((mode & 0x400000) != 0) {
        /* Blend each entry towards a fixed colour, keeping its brightness. */
        SignedDivide divide;
        ScaleFixed scale;
        u32 red;
        u32 green;
        u32 blue;
        s32 k;
        u32 rr;
        u32 gg;
        u32 bb;

        red = mode & 31;
        green = (mode >> 5) & 31;
        blue = (mode >> 10) & 31;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            divide = (SignedDivide)0x03000380;
            k = divide((s32)(((c & 31) + ((c >> 5) & 31) + ((c >> 10) & 31))
                             << 4),
                       (s32)(red + green + blue));
            scale = (ScaleFixed)0x03000118;
            rr = (u32)scale((s32)(((red * (u32)k) >> 4) << 16),
                            (s32)(red << 16) >> 4);
            gg = (u32)scale((s32)(((green * (u32)k) >> 4) << 16),
                            (s32)(green << 16) >> 4);
            bb = (u32)scale((s32)(((blue * (u32)k) >> 4) << 16),
                            (s32)(blue << 16) >> 4);
            rr = (u32)BattleFx_ClampRgb555Channel((s32)(rr >> 16));
            gg = (u32)BattleFx_ClampRgb555Channel((s32)(gg >> 16));
            bb = (u32)BattleFx_ClampRgb555Channel((s32)(bb >> 16));
            *dst++ = Data_0809e92e[bb];
            *dst++ = Data_0809e92e[gg];
            *dst++ = Data_0809e92e[rr];
        }
        return;
    }

    if ((mode & 0x800000) != 0) {
        /* Plain expansion again, reached through the flag word. */
        u16 *out;

        out = dst;
        for (i = 0; i < cnt; i++) {
            c = *src++;
            out[0] = c & 0x7c00;
            out[1] = (c & 0x3e0) << 5;
            out[2] = (c & 0x1f) << 10;
            out += 3;
        }
        return;
    }

    /* The mode word is the address of a prepared table. */
    if (part == 2)
        mode += 1344;

    reg = (u32 *)0x040000d4;
    reg[0] = mode;
    reg[1] = (u32)dst;
    reg[2] = ((cnt * 3) >> 1) | 0x84000000;
}
