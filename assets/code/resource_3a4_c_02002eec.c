#include "types.h"

/*
 * resource_3a4 owner at 0x02002eec, 36 bytes: a per-frame tick that
 * counts to sixty and fires one presentation call, then rewinds itself.
 * A PUBLISHED CALLBACK -- its address is written into a record as a
 * plain word rather than reached by any `bl`, which is why the whole-
 * image call-graph walk could not see it and why it survived the old
 * closure standard.  Sweep B of tools/overlay_published.ts finds it.
 *
 * Complete owner: `push {r5, lr}` at 0x02002eec through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02002f04-
 * 0x02002f08, one alignment halfword at 0x02002f0a, and the one-word
 * literal pool 0x02002f0c-0x02002f0f; the next owner begins at
 * 0x02002f10.  measureSpan reports 30 because it stops at the code, so
 * the pool-is-not-a-terminator trap costs 6 bytes even on a row this
 * small.  No arguments, void.
 *
 * The single call resolves to Func_080f9010 via targetOffset.  The
 * listing's own arrow reads 0x02003d20, which is a real address inside
 * this image and therefore a plausible-looking lie -- the stored
 * displacement is an absolute image offset minus 2, not pc-relative.
 * This row is a clean small demonstration of why that arrow can never
 * be trusted.
 *
 * Uncertainty: the counter's limit of 60 reads as one second of frames
 * but nothing here proves a frame rate.  183 is the literal argument.
 * Data_0200d1b0 is in-image writable data at image offset 0x51b0,
 * spelled by its linked address per this overlay's existing files.
 */

extern s32 Data_0200d1b0;   /* image offset 0x51b0 */

extern void Func_02006c1e();


void Func_02002eec(void)
{
    Data_0200d1b0 = Data_0200d1b0 + 1;
    if (Data_0200d1b0 == 60) {
        Func_02006c1e(183);
        Data_0200d1b0 = 0;
    }
}
