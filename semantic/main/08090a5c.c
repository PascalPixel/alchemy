#include "types.h"

/*
 * Core-drive row 0x08090a5c, 1816 bytes. Structural twin of
 * 0x080f3078 (Ivan's diff: same skeleton, same three shared callees,
 * near-1:1 control flow with a couple of registers renamed but not
 * restructured) -- drafted second per Kraden's call, adapted from that
 * row's own corrected draft rather than re-deriving each blend body
 * from scratch, then spot-checked (case 1 fully re-traced here) to
 * confirm the shared bodies really do carry over unchanged.
 *
 * Confirmed deltas from 0x080f3078, all re-checked against this row's
 * own addressed disassembly rather than assumed from the twin:
 *
 *   - Bucket count: default AND mode 1 both resolve to 448 (0xe0 << 1)
 *     here -- unlike 0x080f3078 where mode 1 genuinely changes the
 *     count (512 -> 256), this row's mode-1 store is a same-value
 *     no-op. Mode 2: count becomes 224 (raw, not doubled -- a real
 *     shape difference from 0x080f3078's mode-2 count of 256), dest
 *     advances by 1344 halfwords-worth (168 << 3) rather than 1536,
 *     source by 448 rather than 512, and the DMA mode-2 selector
 *     offset (bottom of the function) also uses 1344 -- internally
 *     consistent with the dest offset, unlike 0x080f3078 where the two
 *     mode-2 constants (r8 offset, DMA offset) happen to share one
 *     value (1536) by coincidence of both being 0xc0 << 3.
 *   - Sentinel fallback: selector == 0x8000 reads a FIXED palette-RAM
 *     address (0x05000000, BG palette slot 0 -- `movs r3,#160; lsls
 *     r3,r3,#19`) instead of re-reading through source like
 *     0x080f3078 does.
 *   - Local clamp helpers are named Func_08091294 (clamp [0,31]) and
 *     Func_080912a8 (clamp to max 31744) here, byte-identical bodies to
 *     0x080f3078's Func_080f3898/Func_080f38ac (both already exact,
 *     src/08091294.c, src/080912a8.c).
 *   - The three shared lookup tables are Table_0809e96e,
 *     Table_0809e9ae, Table_0809e92e -- confirmed by pool-word position
 *     to play the exact same per-case roles as 0x080f3078's
 *     Table_080f3a2e, Table_080f3a6e, Table_080f39ee respectively.
 *
 * Everything else -- the broadcast repack formula, all seven jump-table
 * bodies including the two identical case-2/case-6 tints, the plain
 * repack fallback, both tint-by-luminance/ratio branches (including the
 * 0x8000 IWRAM-multiply one 0x080f3078's own first draft got wrong
 * before a second read caught it), and the DMA fast path shape -- is
 * unchanged from 0x080f3078; see that file's header comment for the
 * full structural writeup. Not yet checked byte-exact.
 */

extern const u16 Table_0809e96e[];
extern const u16 Table_0809e9ae[];
extern const u16 Table_0809e92e[];

extern s32 Func_080022ec(s32 value, s32 divisor);
extern s32 Func_08091294(s32 value); /* clamp [0,31] */
extern s32 Func_080912a8(s32 value); /* clamp to max 31744 */

/* IWRAM-relocated fixed-point multiply, opaque -- same helper already
 * treated as opaque throughout this dig. */
extern s32 Func_03000118(s32 value, s32 multiplier);

static u16 BroadcastBlend(u16 color)
{
    s32 term1 = ((s32)color << 11) & 0xf800;
    s32 term2 = ((s32)color << 7) & 0x1f000;
    s32 term3 = color & 0x7c00;

    return (u16)Func_080022ec(term1 + term2 + term3, 7);
}

/* Case 2 and case 6 are byte-for-byte the same body, as in 0x080f3078. */
static void TintTriple(u16 color, u16 *dest)
{
    s32 r = color & 0x1f;
    s32 g = (color >> 5) & 0x1f;
    s32 b = (color >> 10) & 0x1f;
    s32 rOut = Func_08091294(r - (r >> 1) + 6);
    s32 gOut = Func_08091294(g - Func_080022ec(g, 3) + 4);
    s32 bOut = Func_08091294(b - 6);

    dest[0] = Table_0809e9ae[bOut];
    dest[1] = Table_0809e96e[gOut];
    dest[2] = Table_0809e92e[rOut];
}

void Func_08090a5c(s32 selector, u16 *source, u16 *dest, s32 mode)
{
    s32 count = 448;
    s32 i;

    if (mode == 1) {
        count = 448; /* same value as default -- a no-op store in the
                        original, kept for faithfulness */
    } else if (mode == 2) {
        dest += 1344;
        count = 224;
        source += 448;
    }

    if (selector == 0x8000) {
        selector = *(u16 *)0x05000000;
    }

    if ((u32)selector < 0x8000) {
        for (i = 0; i < count; i++) {
            u16 blended = BroadcastBlend(source[i]);

            dest[i * 3 + 0] = blended;
            dest[i * 3 + 1] = blended;
            dest[i * 3 + 2] = blended;
        }
        return;
    }

    if ((u32)selector < 0x100000) {
        u32 caseIndex = (u32)(selector - 0x10001);

        if (caseIndex > 6) {
            for (i = 0; i < count; i++) {
                u16 color = source[i];

                dest[i * 3 + 0] = (u16)(color & 0x7c00);
                dest[i * 3 + 1] = (u16)((color & 0x3e0) << 5);
                dest[i * 3 + 2] = (u16)((color & 0x1f) << 10);
            }
            return;
        }

        switch (caseIndex) {
        case 0:
            for (i = 0; i < count; i++) {
                dest[i * 3 + 0] = dest[i * 3 + 1] = dest[i * 3 + 2] =
                    BroadcastBlend(source[i]);
            }
            break;

        case 1:
            for (i = 0; i < count; i++) {
                u16 color = source[i];
                s32 r = color & 0x1f;
                s32 g = (color >> 5) & 0x1f;
                s32 b = (color >> 10) & 0x1f;
                s32 level = Func_080022ec(r + g + b, 10);
                s32 idxHigh = level * 4 + 5;
                s32 idxLow = level * 3 + 5;

                if (idxHigh <= 7) idxHigh = 8;
                if (idxLow <= 7) idxLow = 8;
                if (idxHigh > 28) idxHigh = 28;
                if (idxLow > 28) idxLow = 28;

                dest[i * 3 + 0] = Table_0809e96e[idxLow];
                dest[i * 3 + 1] = Table_0809e96e[idxLow];
                dest[i * 3 + 2] = Table_0809e96e[idxHigh];
            }
            break;

        case 2:
            for (i = 0; i < count; i++) {
                TintTriple(source[i], dest + i * 3);
            }
            break;

        case 3:
            for (i = 0; i < count; i++) {
                u16 color = source[i];
                s32 r = color & 0x1f;
                s32 g = (color >> 5) & 0x1f;
                s32 b = (color >> 10) & 0x1f;

                if (r <= 9) r = 10;
                if (g <= 15) g = 16;
                if (b <= 15) b = 16;
                if (r > 28) r = 28;
                if (g > 24) g = 24;
                if (b > 26) b = 26;

                r = Func_08091294(r);
                g = Func_08091294(g + 2);
                b = Func_08091294(b + 2);

                dest[i * 3 + 0] = Table_0809e92e[b];
                dest[i * 3 + 1] = Table_0809e92e[g];
                dest[i * 3 + 2] = Table_0809e92e[r];
            }
            break;

        case 4:
            for (i = 0; i < count; i++) {
                u16 color = source[i];
                s32 r = color & 0x1f;
                s32 g = (color >> 5) & 0x1f;
                s32 b = (color >> 10) & 0x1f;
                s32 avg = Func_08091294(Func_080022ec(r + g + b, 3));
                s32 rOut = Func_08091294(avg + (r >> 1));
                s32 gOut = Func_08091294(avg + (g >> 1));
                s32 bOut = Func_08091294(avg + (b >> 1));

                dest[i * 3 + 0] = Table_0809e9ae[bOut];
                dest[i * 3 + 1] = Table_0809e9ae[gOut];
                dest[i * 3 + 2] = Table_0809e9ae[rOut];
            }
            break;

        case 5:
            for (i = 0; i < count; i++) {
                u16 color = source[i];
                s32 r = color & 0x1f;
                s32 g = (color >> 5) & 0x1f;
                s32 b = (color >> 10) & 0x1f;
                s32 rOut = Func_08091294(r + (g >> 3) + (b >> 3));
                s32 gOut = g - Func_080022ec(g, 3);
                s32 bOut = b - Func_080022ec(b, 3);

                dest[i * 3 + 0] = Table_0809e92e[bOut];
                dest[i * 3 + 1] = Table_0809e92e[gOut];
                dest[i * 3 + 2] = Table_0809e96e[rOut];
            }
            break;

        case 6:
            for (i = 0; i < count; i++) {
                TintTriple(source[i], dest + i * 3);
            }
            break;
        }
        return;
    }

    if ((selector & 0x4000) != 0) {
        s32 tintR = selector & 0x1f;
        s32 tintG = (selector >> 5) & 0x1f;
        s32 tintB = (selector >> 10) & 0x1f;

        for (i = 0; i < count; i++) {
            s32 luma = (s32)BroadcastBlend(source[i]);
            s32 factor = Func_080022ec(luma, 96);

            dest[i * 3 + 0] = (u16)Func_080912a8(tintB * factor);
            dest[i * 3 + 1] = (u16)Func_080912a8(tintG * factor);
            dest[i * 3 + 2] = (u16)Func_080912a8(tintR * factor);
        }
    } else if ((selector & 0x8000) != 0) {
        s32 tintR = selector & 0x1f;
        s32 tintG = (selector >> 5) & 0x1f;
        s32 tintB = (selector >> 10) & 0x1f;
        s32 tintSum = tintR + tintG + tintB;

        for (i = 0; i < count; i++) {
            u16 color = source[i];
            s32 r = color & 0x1f;
            s32 g = (color >> 5) & 0x1f;
            s32 b = (color >> 10) & 0x1f;
            s32 ratio = Func_080022ec((r + g + b) << 4, tintSum);
            s32 rOut = Func_03000118((((tintR * ratio) >> 4) << 16),
                                      (tintR << 16) >> 4) >>
                       16;
            s32 gOut = Func_03000118((((tintG * ratio) >> 4) << 16),
                                      (tintG << 16) >> 4) >>
                       16;
            s32 bOut = Func_03000118((((tintB * ratio) >> 4) << 16),
                                      (tintB << 16) >> 4) >>
                       16;

            rOut = Func_08091294(rOut);
            gOut = Func_08091294(gOut);
            bOut = Func_08091294(bOut);

            dest[i * 3 + 0] = Table_0809e92e[bOut];
            dest[i * 3 + 1] = Table_0809e92e[gOut];
            dest[i * 3 + 2] = Table_0809e92e[rOut];
        }
    } else if ((selector & 0x10000) != 0) {
        for (i = 0; i < count; i++) {
            u16 color = source[i];

            dest[i * 3 + 0] = (u16)(color & 0x7c00);
            dest[i * 3 + 1] = (u16)((color & 0x3e0) << 5);
            dest[i * 3 + 2] = (u16)((color & 0x1f) << 10);
        }
    } else {
        u32 *dma = (u32 *)0x040000d4;

        if (mode == 2) {
            selector += 1344;
        }
        dma[0] = (u32)selector;
        dma[1] = (u32)dest;
        dma[2] = (u32)(((count * 3) >> 1) | 0x84000000);
    }
}
