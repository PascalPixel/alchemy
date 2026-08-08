#include "types.h"

/*
 * Resource 370 background setup at 0x02000054.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000054 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x0200012e.  The return address is
 * popped into r0, so the owner is `void` (HANDOVER section 0, epilogue rule).
 * No argument register is read before being written, so it takes none.
 *
 * Two literal pools sit inside the reviewed span and are skipped by the
 * control-flow walk from the prologue: 0x02000098-0x020000bf, hopped by the
 * lone `b.n 0x020000c0` at 0x02000096 (the "bare forward branch over a pool"
 * shape from HANDOVER section 0 -- register state set at 0x0200008e-0x02000094
 * is carried ACROSS it into the `stmia` at 0x020000c0), and
 * 0x02000134-0x02000153 after the epilogue.
 *
 * Four call sites, matching the inventory's calls=4, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_370 --json`:
 *   0x0200005a -> veneer 0x1364 -> Func_080002a8
 *   0x0200006c -> veneer 0x135c -> Func_08000290
 *   0x0200008a -> veneer 0x134c -> Func_080001a8
 *   0x02000124 -> veneer 0x149c -> Func_0808a080
 * Func_0808a080 is the scene-record accessor established elsewhere in the
 * project; its argument here is the word at Data_02000240 + 500 and its result
 * is a record whose byte at +85 is cleared.
 *
 * What the body does, in order:
 *  1. Func_080002a8(0), then BG1CNT (0x0400000c) = 0x0681 and the halfword at
 *     0x03001ada cleared.
 *  2. Func_08000290(26) returns a resource block.  Its first 112 words are
 *     DMA'd straight to palette RAM at 0x05000000, and the part starting 448
 *     bytes in is handed with 0x02010000 to Func_080001a8 -- which, given the
 *     next step copies 9,600 words out of 0x02010000, reads as a decompressor
 *     writing into that EWRAM scratch buffer.
 *  3. That scratch buffer is DMA'd to character memory at 0x06006800.
 *  4. The 32x20 screen block at 0x06003000 is filled with tile ids running
 *     from 416 upward, 30 per row, the two rightmost columns of every row
 *     getting the blank id 511.
 *  5. The four scroll shadow words at 0x03001ad0 are cleared and DMA'd to
 *     BG0HOFS..BG3VOFS (0x04000010).
 *  6. The halfword at +20 of the workspace pointed to by 0x03001e70 is set to
 *     0x1400.
 *
 * Uncertainties.  The DMA control words are written as-is: 0x84000070 and
 * 0x84000004 are 32-bit immediate transfers of 112 and 4 words, and
 * 0x84002580 is 9,600 words.  The identity of the +85 byte in the scene record
 * and of the +20 halfword in the 0x03001e70 workspace is not determined here.
 * Tile ids are advanced through a 16-bit sign extension in the original; the
 * range used (416..1015) never reaches it, so the extension is not observable.
 */

/* Overlay imports (via the veneer table).  Old-style declarations: overlay
 * imports vary their argument count between call sites, and these two are used
 * for their return values. */
void Func_020013c0();
void *Func_020013ca();
void Func_020013d8();
u8 *Func_020015c2();

                     

static void Dma3_02000054(const void *source, void *destination, u32 control)
{
    /* The original writes DMA3SAD/DAD/CNT with one `stmia r3!, {r0, r1, r2}`
     * from 0x040000d4 and then rewinds r3 for the next use. */
    *(volatile u32 *)0x040000d4 = (u32)source;
    *(volatile u32 *)0x040000d8 = (u32)destination;
    *(volatile u32 *)0x040000dc = control;
}

void Func_02000054(void)
{
    u8 *resource;
    u16 *screen;
    u32 *scroll;
    u8 *record;
    s32 tile;
    u32 row;
    u32 column;
    u32 i;

    Func_020013c0(0);

    *(volatile u16 *)0x0400000c = 0x0681;   /* BG1CNT */
    *(u16 *)0x03001ada = 0;

    resource = (u8 *)Func_020013ca(26);

    /* 112 words of palette straight from the resource block. */
    Dma3_02000054(resource, (void *)0x05000000, 0x84000070);

    Func_020013d8(resource + 448, (void *)0x02010000);

    /* 9,600 words of decoded tiles into character memory. */
    Dma3_02000054((const void *)0x02010000, (void *)0x06006800, 0x84002580);

    screen = (u16 *)0x06003000;
    for (row = 0; row <= 19; row++) {
        for (column = 0; column <= 29; column++) {
            tile = (s16)(tile + 1);
            *screen++ = (u16)tile;
        }
        /* The two off-screen columns of every row get the blank id. */
        *screen++ = 511;
        *screen++ = 511;
    }
    tile = 416;

    scroll = (u32 *)0x03001ad0;
    for (i = 0; i <= 3; i++) {
        ((u16 *)scroll)[1] = 0;
        ((u16 *)scroll)[0] = 0;
        scroll++;
    }

    Dma3_02000054((const void *)0x03001ad0, (void *)0x04000010, 0x84000004);

    *(u16 *)((u8 *)*(void **)0x03001e70 + 20) = 0x1400;

    record = Func_020015c2(*(s32 *)((u8 *)0x02000240 + 500));
    record[85] = 0;
}
