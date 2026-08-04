#include "types.h"

/*
 * Core-drive row 0x080f3078, 1796 bytes. Colour/palette blend family --
 * A prior working note mapped this one first and flagged it as the twin of
 * 0x08090a5c. This "canonical" twin was drafted first, second-reading
 * every arithmetic body against the addressed disassembly rather than
 * trusting the earlier summary.
 *
 * Signature (best read): Func_080f3078(s32 selector, u16 *source,
 * u16 *dest, s32 mode). `mode` (0/1/2) only adjusts the bucket count
 * (sp+36, default 512, 256 for modes 1/2) and, for mode 2, offsets both
 * source and dest by a fixed table stride before the real work starts.
 * `selector` then branches three ways:
 *
 *   - selector == 0x8000 is a sentinel: re-read the real seed colour
 *     through *source before continuing (both this row and its twin
 *     share this convention; the twin instead falls back to a fixed
 *     palette-RAM address here -- documented in its own park note).
 *   - selector < 0x8000: run the "broadcast" mode directly (repack the
 *     colour into a different 5-bit layout, run it through the IWRAM
 *     divide helper by 7, and write the single result into all three
 *     output halfwords per pixel -- effectively a per-pixel luminance
 *     broadcast).
 *   - 0x8000 <= selector < 0x100000: selector - 0x10001, clamped to
 *     [0,6], selects one of seven per-channel blend bodies through a
 *     genuine `mov pc, r3` jump table at 0x080f3138; any other value in
 *     this range (i.e. selector not exactly one of the seven sentinels)
 *     falls to a plain per-channel repack with no blend arithmetic at
 *     all (0x080f34f8).
 *   - selector >= 0x100000: `mode` (still live as the fourth original
 *     argument, here read as a bit-flag word) selects one of two
 *     "multiply a fixed tint colour by each pixel's own luminance"
 *     blends (bits 0x4000/0x8000), a third plain repack identical in
 *     shape to the 0x100000-window default (bit 0x10000), or, if none
 *     of the three flags match, queues a DMA transfer instead of
 *     computing anything per-pixel (source/dest/count-derived control
 *     word written to the fixed 0x040000d4 DMA3 register triad, the
 *     same construction 0x0800c62c already established for this ROM).
 *
 * The seven jump-table bodies (cases 1-6, case 2 and case 6 byte-for-byte
 * identical down to the same three lookup tables) share three constant
 * lookup tables (0x080f39ee, 0x080f3a2e, 0x080f3a6e, sizes not
 * established -- indexed 0-28 across every case, declared as raw u16
 * arrays rather than guessed-length types) and two already-exact clamp
 * helpers (src/080f3898.c: clamp to [0,31]; src/080f38ac.c: clamp to a
 * max of 31744/0x7c00). `Func_080022ec` (exact, src/08003adc.c/
 * src/08003d28.c) is the same IWRAM signed-divide veneer some call sites
 * reach directly and others reach through the runtime's own
 * `_call_via_r2/r3/r7` thunks (0x080072e4-0x08007320, already documented
 * as ordinary indirect calls by semantic/main/0800ebec.c's header
 * comment) -- represented uniformly here as Func_080022ec calls since
 * both paths reach the identical divide.
 *
 * Not yet checked byte-exact.
 */

extern const u16 Table_080f39ee[];
extern const u16 Table_080f3a2e[];
extern const u16 Table_080f3a6e[];

extern s32 Func_080022ec(s32 value, s32 divisor);
extern s32 Func_080f3898(s32 value); /* clamp [0,31] */
extern s32 Func_080f38ac(s32 value); /* clamp to max 31744 */

/* IWRAM-relocated fixed-point multiply, opaque -- same helper already
 * treated as opaque for 0x0800c62c, 0x0808b3ec and 0x0808bec0. Reached
 * here through the runtime's own indirect-call convention. */
extern s32 Func_03000118(s32 value, s32 multiplier);

static u16 BroadcastBlend(u16 color)
{
    /* Repacks the BGR555 colour into a different 5-5-5 bit layout before
       dividing by 7 -- transcribed as the three literal masked terms
       (0x080f3168-0x080f3184) rather than simplified, since the exact
       resulting bit layout isn't independently confirmed. */
    s32 term1 = ((s32)color << 11) & 0xf800;
    s32 term2 = ((s32)color << 7) & 0x1f000;
    s32 term3 = color & 0x7c00;

    return (u16)Func_080022ec(term1 + term2 + term3, 7);
}

/* Case 2 and case 6 are byte-for-byte the same body (same formula, same
   three tables) -- factored into one helper rather than duplicated. */
static void TintTriple(u16 color, u16 *dest)
{
    s32 r = color & 0x1f;
    s32 g = (color >> 5) & 0x1f;
    s32 b = (color >> 10) & 0x1f;
    s32 rOut = Func_080f3898(r - (r >> 1) + 6);
    s32 gOut = Func_080f3898(g - Func_080022ec(g, 3) + 4);
    s32 bOut = Func_080f3898(b - 6);

    dest[0] = Table_080f3a6e[bOut];
    dest[1] = Table_080f3a2e[gOut];
    dest[2] = Table_080f39ee[rOut];
}

void Func_080f3078(s32 selector, u16 *source, u16 *dest, s32 mode)
{
    s32 count = 512;
    s32 i;

    if (mode == 1) {
        count = 256;
    } else if (mode == 2) {
        dest += 768; /* 0xc0 << 3 halfwords */
        count = 256;
        source += 256; /* 0x80 << 1 halfwords */
    }

    if (selector == 0x8000) {
        selector = *source;
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
            /* 0x080f34f8: plain repack, no blend arithmetic -- each
               channel isolated in place, G and R additionally shifted
               up into the same bit range B already occupies. */
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

                dest[i * 3 + 0] = Table_080f3a2e[idxLow];
                dest[i * 3 + 1] = Table_080f3a2e[idxLow];
                dest[i * 3 + 2] = Table_080f3a2e[idxHigh];
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

                r = Func_080f3898(r);
                g = Func_080f3898(g + 2);
                b = Func_080f3898(b + 2);

                dest[i * 3 + 0] = Table_080f39ee[b];
                dest[i * 3 + 1] = Table_080f39ee[g];
                dest[i * 3 + 2] = Table_080f39ee[r];
            }
            break;

        case 4:
            for (i = 0; i < count; i++) {
                u16 color = source[i];
                s32 r = color & 0x1f;
                s32 g = (color >> 5) & 0x1f;
                s32 b = (color >> 10) & 0x1f;
                s32 avg = Func_080f3898(Func_080022ec(r + g + b, 3));
                s32 rOut = Func_080f3898(avg + (r >> 1));
                s32 gOut = Func_080f3898(avg + (g >> 1));
                s32 bOut = Func_080f3898(avg + (b >> 1));

                dest[i * 3 + 0] = Table_080f3a6e[bOut];
                dest[i * 3 + 1] = Table_080f3a6e[gOut];
                dest[i * 3 + 2] = Table_080f3a6e[rOut];
            }
            break;

        case 5:
            for (i = 0; i < count; i++) {
                u16 color = source[i];
                s32 r = color & 0x1f;
                s32 g = (color >> 5) & 0x1f;
                s32 b = (color >> 10) & 0x1f;
                s32 rOut = Func_080f3898(r + (g >> 3) + (b >> 3));
                s32 gOut = g - Func_080022ec(g, 3);
                s32 bOut = b - Func_080022ec(b, 3);

                dest[i * 3 + 0] = Table_080f39ee[bOut];
                dest[i * 3 + 1] = Table_080f39ee[gOut];
                dest[i * 3 + 2] = Table_080f3a2e[rOut];
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

    /* selector >= 0x100000: bits 14/15/16 of selector itself select the
       path here (not `mode`) -- the low 15 bits simultaneously carry a
       packed tint colour for the two blend branches below. Confirmed
       against the disassembly: `ands r3,r0` masks r0 (selector), not a
       separately-tracked mode/flags register. */
    /* Corrected after a second read against the addressed disassembly:
       bits 0x4000 and 0x8000 are genuinely DIFFERENT bodies, not two
       branches into the same code as first assumed (and initially merged
       into one `||` before catching this) -- 0x4000 is a flat
       luminance-scaled tint, 0x8000 is a per-channel ratio between the
       pixel's own channel sum and the tint's, squared back through the
       IWRAM multiply. Caught by tracing the twin (0x08090a5c) and
       finding its own 0x8000 body used the multiply helper where this
       row's did not -- which meant this row's own body needed
       rechecking, not just the twin's. */
    if ((selector & 0x4000) != 0) {
        s32 tintR = selector & 0x1f;
        s32 tintG = (selector >> 5) & 0x1f;
        s32 tintB = (selector >> 10) & 0x1f;

        for (i = 0; i < count; i++) {
            s32 luma = (s32)BroadcastBlend(source[i]);
            s32 factor = Func_080022ec(luma, 96);

            dest[i * 3 + 0] = (u16)Func_080f38ac(tintB * factor);
            dest[i * 3 + 1] = (u16)Func_080f38ac(tintG * factor);
            dest[i * 3 + 2] = (u16)Func_080f38ac(tintR * factor);
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

            rOut = Func_080f3898(rOut);
            gOut = Func_080f3898(gOut);
            bOut = Func_080f3898(bOut);

            dest[i * 3 + 0] = Table_080f39ee[bOut];
            dest[i * 3 + 1] = Table_080f39ee[gOut];
            dest[i * 3 + 2] = Table_080f39ee[rOut];
        }
    } else if ((selector & 0x10000) != 0) {
        for (i = 0; i < count; i++) {
            u16 color = source[i];

            dest[i * 3 + 0] = (u16)(color & 0x7c00);
            dest[i * 3 + 1] = (u16)((color & 0x3e0) << 5);
            dest[i * 3 + 2] = (u16)((color & 0x1f) << 10);
        }
    } else {
        /* No blend flag matched: queue a DMA transfer instead of
           computing per-pixel. Same source/dest/control-triad
           construction already established for 0x0800c62c. */
        u32 *dma = (u32 *)0x040000d4;

        if (mode == 2) {
            selector += 1536; /* 0xc0 << 3, same stride mode 2 applied
                                * to source/dest above */
        }
        dma[0] = (u32)selector;
        dma[1] = (u32)dest;
        dma[2] = (u32)(((count * 3) >> 1) | 0x84000000);
    }
}
