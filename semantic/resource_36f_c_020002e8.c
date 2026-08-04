#include "types.h"

/*
 * resource_36f owner at 0x020002e8, 364 bytes: the overlay's scene ROOT — set
 * the background up, start the sprite task, fade the screen in, hold, then fade
 * the brightness back out and hand control on.
 *
 * It is the root by the call graph, not by assumption.  Nothing inside the
 * strict rows calls it; its one caller is the tracked byte-exact sibling
 * `assets/code/resource_36f_c_02000054.c`, whose `bl` at 0x02000080 prints as
 * `Func_0200036a` and resolves through the +2 rule to 0x036a - 0x080 - 2 =
 * 0x02e8 — this function.  (That source was written with the printed, wrong
 * names, which HANDOVER records as the normal state of tracked overlay C; the
 * mapping is consistent and inverts.)  So this scene runs when the mode
 * halfword `Data_02000240[225]` is 10.
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` at 0x020002e8; single
 * epilogue `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000434, so the popped
 * word is the return address and the owner is **void**.  Code runs
 * 0x020002e8..0x0200031b, hops its FIRST interior pool 0x0200031c..0x0200032f
 * with the `b.n 0x02000330` at 0x0200031a, continues 0x02000330..0x0200043a and
 * is followed by its second pool 0x0200043c..0x02000453; 0x02000454 is the next
 * prologue.  364 bytes, matching the inventory row.  Two interior pools, both
 * found by a control-flow walk from the prologue, not by reading through.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_36f
 * 02e8` (an overlay `bl` stores target offset - 2):
 *   0x020002ea -> 0x0454 prologue -> Func_02000454   background setup
 *   0x020002f0 -> veneer 0x05a8   -> Func_0808a010(30)
 *   0x020002fc -> 0x01c0 prologue -> Func_020001c0   portrait upload
 *   0x02000306 -> veneer 0x0540   -> Func_080000d0   install the sprite task
 *   0x020003d2 -> veneer 0x05a8   -> Func_0808a010(120)
 *   0x02000408 -> veneer 0x0538   -> Func_080000c0(3)
 *   0x02000426 -> veneer 0x05c0   -> Func_0808a360
 *   0x0200042a -> veneer 0x05c8   -> Func_0808a370
 * Eight sites, seven distinct targets (Func_0808a010 twice).
 *
 * The task pointer is the link-base witness: `Func_080000d0(0x02008239, 3200)`,
 * and 0x02008239 is file offset 0x0238 plus the Thumb bit — the sprite task
 * `Func_02000238` in this same overlay.
 *
 * DISPLAY-REGISTER QUEUE.  Five times over, this owner runs the same shape:
 *
 *     ldrh r3,[r5] / adds r1,r3,#0 / strh r5,[r5]      IME saved, then cleared
 *     ldrh r2,[r7] / cmp r2,#31 / bgt <skip>           queue full?
 *     r3 = r7 + r2*12 + 4 ; r7[0] = r2 + 1             claim slot r2
 *     stmia r3!,{value} ; stmia r3!,{address} ; str 0x20000
 *     strh r1,[r5]                                     IME restored
 *
 * with r5 = 0x04000208 (IME) and r7 = 0x02002090 (a RAM global — BELOW the
 * 0x02008000 link band, so not in-image; HANDOVER's two-sided band test).  The
 * `strh r5,[r5]` is the cheap-IME-clear idiom: the *address* 0x04000208 is
 * stored to itself, and its low halfword 0x0208 has bit 0 clear, which is all
 * IME reads.  So each block is one critical-section enqueue of a deferred
 * {value, register, 0x20000} write onto a 32-slot, 12-byte-per-slot queue.  The
 * four unrolled ones set DISPCNT = 0x1540, BLDCNT = 0x2fce, BLDY = 16 and
 * BLDALPHA = 0x1010; the loop then walks BLDY from 16 down to 0, one step per
 * `Func_080000c0(3)`, which is the fade-in.
 *
 * The 17 fade steps are a real counted loop with a back edge and a compare, not
 * a run of sites — unlike the script-table shape HANDOVER warns about — so it is
 * spelled as a loop and the multiset carries one Func_080000c0 site, which is
 * what `overlay_multiset_check.ts` confirms.
 *
 * Uncertainties: 0x0200868c is cleared here and incremented once per frame by
 * Func_02000238, so it is that task's frame counter; the +448 and +456 s32
 * slots off the 0x03001ebc workspace pointer are written with 0, then 1, then
 * 60 (the second write happening only after the two Func_0808a36x bookkeeping
 * calls), which reads as "phase id 0, request 1, then hold 60" but the slot
 * identities are not otherwise witnessed here.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a010();           /* wait this many frames */
void Func_080000c0();           /* advance the task scheduler */
void Func_080000d0();           /* install a per-frame task */
void Func_0808a360();           /* scene bookkeeping, no arguments */
void Func_0808a370();           /* scene bookkeeping, no arguments */

void Func_020001c0(void);
void Func_02000454(void);
void Func_02000238(void);

/* In-image: the sprite task's frame counter. */
extern u16 Data_0200868c[];

struct DisplayWrite_020002e8 {
    u32 value;
    u32 address;
    u32 flags;
};

struct DisplayQueue_020002e8 {
    u16 count;
    u16 pad;
    struct DisplayWrite_020002e8 entries[32];
};

/* Deferred display-register writes, drained by the vblank handler. */
#define DISPLAY_QUEUE_020002e8 ((struct DisplayQueue_020002e8 *)0x02002090)

static void Queue_020002e8(u32 value, u32 address)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u16 saved = *interruptMaster;
    s32 slot;

    /* `strh r5,[r5]`: storing the register's own address clears bit 0. */
    *interruptMaster = 0x0208;

    slot = DISPLAY_QUEUE_020002e8->count;
    if (slot <= 31) {
        DISPLAY_QUEUE_020002e8->count = (u16)(slot + 1);
        DISPLAY_QUEUE_020002e8->entries[slot].value = value;
        DISPLAY_QUEUE_020002e8->entries[slot].address = address;
        DISPLAY_QUEUE_020002e8->entries[slot].flags = 0x20000;
    }

    *interruptMaster = saved;
}

void Func_020002e8(void)
{
    u8 *workspace;
    s32 step;

    Func_02000454();
    Func_0808a010(30);

    Data_0200868c[0] = 0;
    Func_020001c0();

    /* 0x02008239 == Func_02000238 + the Thumb bit. */
    Func_080000d0((s32)Func_02000238 + 1, 3200);   /* 200 << 4 */

    Queue_020002e8(0x1540, 0x04000000);            /* DISPCNT   */
    Queue_020002e8(0x2fce, 0x04000050);            /* BLDCNT    */
    Queue_020002e8(0x0010, 0x04000054);            /* BLDY      */
    Queue_020002e8(0x1010, 0x04000052);            /* BLDALPHA  */

    Func_0808a010(120);

    for (step = 0; step <= 16; step++) {
        Queue_020002e8((u32)(16 - step), 0x04000054);
        Func_080000c0(3);
    }

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 0;
    *(s32 *)(workspace + 456) = 1;

    Func_0808a360();
    Func_0808a370();

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 60;
}
