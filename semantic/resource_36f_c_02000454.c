#include "types.h"

/*
 * resource_36f background setup at 0x02000454, 228 bytes.
 *
 * NEAR-TWIN of the already-admitted `semantic/overlays/resource_370_c_02000054.c`
 * — the same routine shared between the two overlays, step for step, constant
 * for constant, with exactly one difference: resource_370 closes with
 * `record = Func_0808a080(...); record[85] = 0;` and this copy does not (it has
 * three call sites against resource_370's four).  Every other value agrees:
 * BG1CNT = 0x0681, the halfword at 0x03001ada, resource id 26, the three DMA3
 * control words 0x84000070 / 0x84002580 / 0x84000004, the 0x02010000 scratch,
 * the 30-wide / 20-tall screen fill from tile 416 at 0x06003000 with 511 in the
 * two off-screen columns, the four scroll shadow words at 0x03001ad0, and the
 * 0x1400 written at +20 of the 0x03001e70 workspace.  That agreement is the
 * correctness proof for this file; it was diffed before it was written, per
 * HANDOVER's twin rule.
 *
 * Complete owner.  Prologue `push {r5, r6, lr}` at 0x02000454; the body runs to
 * the `b.n 0x020004c0` at 0x02000496, which hops the literal pool at
 * 0x02000498..0x020004bf carrying r0/r1/r2/r3 across it into the `stmia` at
 * 0x020004c0 — the bare-forward-branch-over-a-pool shape, and the reason the
 * inventory reports a phantom second row at 0x020004be.  Execution continues to
 * the single epilogue `pop {r5, r6} / pop {r0} / bx r0` at 0x0200051a, followed
 * by the second pool at 0x02000520..0x02000537; 0x02000538 is the first import
 * veneer.  228 bytes, matching the inventory row.  The popped word is the
 * return address, so the owner is **void**.  r4 is scratch and is not saved,
 * which is what the reference does.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_36f
 * 0454` (an overlay `bl` stores target offset - 2):
 *   0x0200045a -> veneer 0x0588 -> Func_080002a8
 *   0x0200046c -> veneer 0x0578 -> Func_08000290
 *   0x0200048a -> veneer 0x0558 -> Func_080001a8
 * The same three imports in the same order as the resource_370 twin.
 *
 * The `subs r3,#12` after each `stmia r3!,{r0,r1,r2}` rewinds the
 * auto-incremented register and is dead (HANDOVER's DMA3 idiom).
 *
 * Uncertainty carried over from the twin: the tile ids are advanced through a
 * 16-bit sign extension in the original, but the range used (416..1015) never
 * reaches it, so the extension is not observable.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_080002a8();
void *Func_08000290();
void Func_080001a8();

static void Dma3_02000454(const void *source, void *destination, u32 control)
{
    *(volatile u32 *)0x040000d4 = (u32)source;
    *(volatile u32 *)0x040000d8 = (u32)destination;
    *(volatile u32 *)0x040000dc = control;
}

void Func_02000454(void)
{
    u8 *resource;
    u16 *screen;
    u32 *scroll;
    s32 tile;
    u32 row;
    u32 column;
    u32 i;

    Func_080002a8(0);

    *(volatile u16 *)0x0400000c = 0x0681;   /* BG1CNT */
    *(u16 *)0x03001ada = 0;

    resource = (u8 *)Func_08000290(26);

    /* 112 words of palette straight from the resource block. */
    Dma3_02000454(resource, (void *)0x05000000, 0x84000070);

    Func_080001a8(resource + 448, (void *)0x02010000);

    /* 9,600 words of decoded tiles into character memory. */
    Dma3_02000454((const void *)0x02010000, (void *)0x06006800, 0x84002580);

    screen = (u16 *)0x06003000;
    tile = 416;
    for (row = 0; row <= 19; row++) {
        for (column = 0; column <= 29; column++) {
            tile = (s16)(tile + 1);
            *screen++ = (u16)tile;
        }
        /* The two off-screen columns of every row get the blank id. */
        *screen++ = 511;
        *screen++ = 511;
    }

    for (i = 0; i <= 3; i++) {
        ((u16 *)scroll)[1] = 0;
        ((u16 *)scroll)[0] = 0;
        scroll++;
    }
    scroll = (u32 *)0x03001ad0;

    Dma3_02000454((const void *)0x03001ad0, (void *)0x04000010, 0x84000004);

    *(u16 *)((u8 *)*(void **)0x03001e70 + 20) = 0x1400;
}
