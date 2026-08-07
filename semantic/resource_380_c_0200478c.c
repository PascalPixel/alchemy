#include "types.h"

/*
 * resource_380 owner at 0x0200478c, 200 bytes: build a tracked-object
 * roster.  Allocates a 404-byte array and zero-fills it with an
 * inline DMA3 transfer (0x040000d4 loaded and
 * written src/dst/control 0x85000065 in one stmia -- a 101-word fill
 * from a fixed source, the on-stack zero), clamps the requested count
 * to 10, then per entry: resolves object id (base+i), stores it in a
 * 40-byte slot, clears record byte +38 and object byte +85, pushes the
 * result of Func_0808a080(base+i) into Func_080091e8(.., 1), and seeds
 * slot +28/+32 from two resident tables (0x0200d140, negated
 * 0x0200d168) with slot byte +36 = 3.  Writes the final count at array
 * +400 and installs the resident watcher 0x0200c49d for 3200 ticks
 * (the same install idiom as the main image's Func_080041d8).
 *
 * Complete owner: full high-register prologue at 0x0200478c through
 * `pop {r0} / bx r0` at 0x0200483e, then the five-word literal pool
 * 0x02004840-0x02004853 (0x040000d4, 0x85000065, 0x0200d140,
 * 0x0200d168, 0x0200c49d); next owner's prologue at 0x02004854.
 *
 * CORRECTION (name sweep): the parenthetical claim that "this overlay
 * executes at 0x02000000 and the resident module sits at 0x02008000+"
 * is STRUCK -- there is no resident module; that reading came from a
 * naive pc-relative misdecode of the bl targets, which resolve under
 * the +2 rule (tools/overlay_call_targets.ts) through this overlay's
 * own import-veneer table into main-ROM code.  The pool pointers fit
 * the standard 0x02008000 link base.
 *
 * Uncertainty: callee roles beyond call shape remain open; the three
 * 0x0200xxxx pool constants are in-image data/code pointers, recorded
 * as raw constants.
 */

extern void Func_080000d0(u32 callback, s32 duration);
extern s32 Func_08000148(s32 arg0, s32 size);
extern void Func_080091e8(s32 arg0, s32 arg1);
extern s32 Func_0808a080(s32 id);

void Func_0200478c(s32 base, u32 count)
{
    s32 permuted_6;
    u8 *array = (u8 *)Func_08000148(33, 404);
    s32 zero = 0;
    u8 *slot = array;
    volatile u32 *dma = (volatile u32 *)0x040000d4;
    u32 index;

    dma[2] = 0x85000065;
    dma[0] = (u32)&zero;
    dma[1] = (u32)array;

    if (count > 10)
        count = 10;
    for (index = 0; index < count; index++) {
        s32 object = Func_0808a080(base + index);

        *(s32 *)slot = object;
        *(u8 *)(*(s32 *)(object + 80) + 38) = 0;
        *(u8 *)(object + 85) = 0;
        Func_080091e8(Func_0808a080(base + index), 1);
        permuted_6 = *(s32 *)(0x0200d140 + index * 4);
        *(s32 *)(slot + 32) = -*(s32 *)(0x0200d168 + index * 4);
        slot[36] = 3;
        *(s32 *)(slot + 28) = permuted_6;
        slot += 40;
    }
    *(unsigned short *)(array + 400) = count;
    Func_080000d0(0x0200c49d, 3200);
}
