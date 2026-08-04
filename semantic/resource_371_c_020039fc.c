/*
 * resource_371 owner at 0x020039fc, 1,296 bytes: 0x020039fc-0x02003f0b.
 *
 * NO INVENTORY ROW.  Found by sweeping the two-byte gaps in
 * `metrics/gs1-en-executable.json` for `push {..,lr}` prologues; there is no
 * row for it in `out/decomp/overlays.json`, no exact sibling and no semantic
 * source, so the span below is DERIVED.
 *
 * How the span was established.  A control-flow walk from the prologue at
 * 0x020039fc, following every branch, reaches its last instruction at
 * 0x02003f0a and closes on the single matching high-register unwind
 * `pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 / mov sl,r6 / mov fp,r7 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02003efc-0x02003f0b.  There is
 * exactly one epilogue, no `bl` resolves inside the body (so no long tail
 * branch), and there is no `mov pc, rN` dispatch, so nothing is hidden behind
 * a jump table.  Extent 0x020039fc-0x02003f0c = 1,296 bytes, of which 1,154
 * are instructions the walk reaches.
 *
 * The end is NOT marked by alignment or a pool: 0x02003f0c is immediately the
 * literal pool this owner's own last `ldr` reads (0x00000101 at 0x02003f0c),
 * and the next `push {r4, lr}` follows it directly.  The boundary therefore
 * rests on the epilogue, not on a gap.
 *
 * POOL MAP - six interior literal pools, 142 bytes, none reachable as an
 * instruction, each jumped over by an unconditional `b.n`:
 *   0x02003b32-0x02003b77  (70 bytes)  jumped by `b.n` at 0x02003b30
 *   0x02003bb2-0x02003bbf  (14 bytes)  jumped by `b.n` at 0x02003bb0
 *   0x02003bfa-0x02003c07  (14 bytes)  jumped by `b.n` at 0x02003bf8
 *   0x02003cfc-0x02003d07  (12 bytes)  jumped by `b.n` at 0x02003cfa
 *   0x02003e80-0x02003e8b  (12 bytes)  jumped by `b.n` at 0x02003e7e
 *   0x02003ed8-0x02003eeb  (20 bytes)  jumped by `b.n` at 0x02003ed6
 * Every pc-relative `ldr` in the body lands in one of those six or in the
 * trailing pool at 0x02003f0c, which is outside the registered span.
 *
 * LINK BASE, proved from this owner's own pool.  Two pool words are ODD and
 * resolve under `pool_word - 0x8000` onto real prologues in this image:
 * 0x0200b8fd -> 0x020038fc (`push {lr}`) and 0x0200b95d -> 0x0200395c
 * (`push {r5, r6, lr}`).  Both are handed to Func_080000d0 as the task to
 * install, which is the established task-install witness.  Three further pool
 * words are EVEN and land inside the image as data: 0x0200c4ac -> 0x020044ac
 * (the 32-byte-stride palette table indexed by this owner's argument),
 * 0x0200c7a6 -> 0x020047a6 and 0x0200c4ec -> 0x020044ec (two compressed
 * sources).  Odd = Thumb entry, even = data, with no exceptions here.
 *
 * Two other absolute addresses are NOT in-image, by the same two-sided band
 * test: 0x02002090 and 0x02000240 are BELOW the 0x02008000 link band, so they
 * are RAM globals - 0x02002090 is the same deferred-write queue
 * `resource_36f_c_020002e8.c` documents, at the same address with the same
 * 12-byte slot layout.  0x0200e7a0 is above the band but resolves to 0x000067a0,
 * four bytes PAST the assembled image end (0x0000679c), so it is not in-image
 * either; it is treated here as an absolute u16 scratch global.
 *
 * Call accounting: 36 `bl` sites over 16 distinct targets, all import veneers -
 * no intra-overlay call and no `call_via` slot.  Resolved with
 * `bun tools/overlay_call_targets.ts resource_371 39fc 3f0c`, never from
 * `overlay_show.ts`'s printed annotations.  Per-target multiset:
 * Func_0808a010 x17, Func_080000d0 x2, Func_080001a8 x2, Func_0808a370 x2,
 * Func_080f9010 x2, and one each of Func_080000c0, Func_08000170,
 * Func_08000178, Func_08009230, Func_08009238, Func_080770c8, Func_080770d0,
 * Func_0808a018, Func_0808a080, Func_0808a360, Func_0808a368.
 *
 * WHAT IT IS.  A scripted animation played out of a scratch buffer.  It takes
 * one argument, used once as `0x020044ac + (argument << 5)` - the source of the
 * palette DMA - so the argument selects one of the 32-byte palettes in that
 * in-image table.  The owner allocates a 16 KiB buffer, decompresses two
 * sources into it 4 KiB apart, queues the palette and the first VRAM page,
 * installs the task at 0x020038fc, and then walks the frame counter at
 * 0x0200e7a0 up and down while queueing successive 0x140-word pages out of the
 * buffer, each separated by a short `Func_0808a010` wait.  It ends by
 * installing the second task at 0x0200395c, playing cue 141, holding 45
 * frames, releasing the buffer and setting story flag 0x101.  Story flag 0x109
 * is cleared on entry.
 *
 * THE TWELVE ENQUEUES ARE ONE SHAPE, WRITTEN ONCE.  Each is the critical
 * section `ldrh/strh r5,[r5]` around a bounded 32-slot append; `strh r5,[r5]`
 * with r5 = 0x04000208 stores the register's own ADDRESS, whose low halfword
 * 0x0208 has bit 0 clear, so it is an IME clear and not a data store.  Only
 * the three stored words differ between sites, so they are the helper's three
 * arguments.  `resource_36f_c_020002e8.c` documents the identical queue with a
 * constant third word; here the third word is a DMA control (0x84000140,
 * 0x84000400, 0x80000010) and the first two are source and destination, so the
 * queue is carrying deferred DMA rather than deferred register writes.
 *
 * fp, r9 and sl exist only to hold 0x0b00, 0x0a00 and 0x0900 across the middle
 * of the routine so the descending half can reuse them; that is why the owner
 * saves high registers at all.  They are written as named locals rather than
 * repeated constants, because folding them back into literals would hide that
 * the ascending and descending halves are the SAME three values.
 *
 * The epilogue's final `pop {r0}` takes the return address, so the owner is
 * `void`.
 *
 * UNCERTAINTIES:
 *  - Func_0808a360, Func_0808a368 and Func_0808a370 are called with no
 *    argument register written; spelled with no arguments.
 *  - Func_08009230 / Func_08009238 bracket the sequence and take no arguments.
 *  - The record fetched by Func_0808a080 has its byte at +84 cleared.  The
 *    sibling owner `resource_38d_c_020006e0.c` clears +85 on its records; this
 *    one is +84 and the difference is in the assembly (`adds r0, #84`).
 *  - The buffer offsets are written as the assembly builds them
 *    (`movs rN,#C / lsls rN,rN,#5 or #6`); nothing here proves a page size.
 */

#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_080000c0();           /* wait n frames */
void Func_080000d0();           /* install a per-frame task */
u8 *Func_08000170();            /* allocate a scratch buffer */
void Func_08000178();           /* release a scratch buffer */
void Func_080001a8();           /* decompress source into destination */
void Func_08009230();           /* sequence bracket: open */
void Func_08009238();           /* sequence bracket: close */
void Func_080770c8();           /* set a story flag */
void Func_080770d0();           /* clear a story flag */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
u8 *Func_0808a080();            /* record by slot index */
void Func_0808a360();           /* scene bookkeeping, no arguments */
void Func_0808a368();           /* scene bookkeeping, no arguments */
void Func_0808a370();           /* scene bookkeeping, no arguments */
void Func_080f9010();           /* play a sound cue */

/* In-image, under the 0x02008000 link base. */
extern const u8 Data_020044ac[];        /* 0x0200c4ac: 32-byte palettes */
extern const u8 Data_020044ec[];        /* 0x0200c4ec */
extern const u8 Data_020047a6[];        /* 0x0200c7a6 */

/* The tasks installed here, both named by an odd pool word. */
void Func_020038fc(void);
void Func_0200395c(void);

struct DeferredWrite_020039fc {
    u32 source;
    u32 destination;
    u32 control;
};

struct DeferredQueue_020039fc {
    u16 count;
    u16 pad;
    struct DeferredWrite_020039fc entries[32];
};

/* The same 32-slot deferred queue resource_36f_c_020002e8.c documents. */
#define DEFERRED_QUEUE_020039fc ((struct DeferredQueue_020039fc *)0x02002090)

static void Queue_020039fc(u32 source, u32 destination, u32 control)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u16 saved = *interruptMaster;
    s32 slot;

    /* `strh r5,[r5]`: storing the register's own address clears bit 0. */
    *interruptMaster = 0x0208;

    slot = DEFERRED_QUEUE_020039fc->count;
    if (slot <= 31) {
        DEFERRED_QUEUE_020039fc->count = (u16)(slot + 1);
        DEFERRED_QUEUE_020039fc->entries[slot].source = source;
        DEFERRED_QUEUE_020039fc->entries[slot].destination = destination;
        DEFERRED_QUEUE_020039fc->entries[slot].control = control;
    }

    *interruptMaster = saved;
}

void Func_020039fc(s32 palette)
{
    volatile u16 *frame = (volatile u16 *)0x0200e7a0;
    u8 *buffer;
    u8 *record;
    u8 *workspace;
    u16 highB;                  /* fp */
    u16 highA;                  /* r9 */
    u16 highLow;                /* sl */

    buffer = Func_08000170(128 << 7);           /* 16384 */
    Func_080000c0(1);
    Func_080770d0(0x109);
    Func_08009230();

    Func_080001a8(Data_020047a6, buffer);
    Func_080001a8(Data_020044ec, buffer + (128 << 5));   /* 4096 */

    Queue_020039fc((u32)Data_020044ac + (palette << 5), 0x050001c0, 0x80000010);
    Queue_020039fc((u32)buffer, 0x06001000, 0x84000400);

    Func_080000d0((s32)Func_020038fc + 1, 200 << 4);     /* 3200 */
    Func_0808a018();

    Queue_020039fc((u32)(buffer + (234 << 6)), 0x06002000, 0x84000140);

    record = Func_0808a080(*(s32 *)(0x02000240 + (250 << 1)));   /* +500 */
    record[84] = 0;

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + (228 << 1)) = 16;       /* +456 */

    Func_0808a360();
    Func_0808a370();
    Func_080f9010(246);

    *frame = 0x0e00;
    Queue_020039fc((u32)(buffer + (210 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(2);

    *frame = 0x0d00;
    Queue_020039fc((u32)(buffer + (186 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(2);

    *frame = 0x0c00;
    Queue_020039fc((u32)(buffer + (162 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(2);

    highB = (u16)(176 << 4);                     /* 0x0b00 */
    *frame = highB;
    Queue_020039fc((u32)(buffer + (138 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(2);

    highA = (u16)(160 << 4);                     /* 0x0a00 */
    *frame = highA;
    Queue_020039fc((u32)(buffer + (228 << 5)), 0x06002000, 0x84000140);
    Func_0808a010(2);

    highLow = (u16)(144 << 4);                   /* 0x0900 */
    *frame = highLow;
    Queue_020039fc((u32)(buffer + (180 << 5)), 0x06002000, 0x84000140);
    Func_0808a010(2);

    *frame = 0x0800;
    Queue_020039fc((u32)(buffer + (132 << 5)), 0x06002000, 0x84000140);
    Func_0808a010(140);

    /* The descending half re-shows the same pages; the first two steps hold
     * the frame counter where the ascending half left it. */
    Queue_020039fc((u32)(buffer + (180 << 5)), 0x06002000, 0x84000140);
    Func_0808a010(4);

    Queue_020039fc((u32)(buffer + (228 << 5)), 0x06002000, 0x84000140);
    Func_0808a010(4);

    Queue_020039fc((u32)(buffer + (138 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(4);

    *frame = highLow;
    Queue_020039fc((u32)(buffer + (162 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(4);

    *frame = highA;
    Queue_020039fc((u32)(buffer + (186 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(4);

    *frame = highB;
    Queue_020039fc((u32)(buffer + (210 << 6)), 0x06002000, 0x84000140);
    Func_0808a010(4);

    *frame = 0x0c00;
    Queue_020039fc((u32)(buffer + (234 << 6)), 0x06002000, 0x84000140);

    Func_08009238();
    Func_080000d0((s32)Func_0200395c + 1, 200 << 4);     /* 3200 */
    Func_080f9010(141);

    *frame = 0x0d00;
    Func_0808a010(4);
    *frame = 0x0e00;
    Func_0808a010(4);
    *frame = 0x0f00;
    Func_0808a010(4);
    *frame = 0x1000;
    Func_0808a010(45);

    Func_0808a368();
    Func_0808a370();

    Func_08000178(buffer);
    Func_080770c8(0x101);
}
