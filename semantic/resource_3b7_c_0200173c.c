/*
 * resource_3b7 owner at 0x0200173c, 280 bytes (0x0200173c-0x02001853).
 *
 * The scene runner that Func_020016a8 sets up for: it resets the shared state
 * block, then spins one frame at a time until the completion flag is set,
 * firing two scheduled events on frame 16 and frame 50, and finally returns the
 * result word.
 *
 * Code runs 0x0200173c-0x0200182d.  The trailing 38 bytes,
 * 0x0200182e-0x02001853, are an alignment halfword plus a literal pool of nine
 * words - 0x0200a070, 0x0200a0c0, 0x0200a134, 0x0000ffff, 0x0200a130,
 * 0x02000434, 0x00014ccc, 0xfffe0000, 0x0200a138.  They lie past the `bx r1`
 * return and are read only by `ldr rN, [pc, ...]`, so they are data.
 *
 * That pool is a textbook case of the documented inverse trap: because
 * 0x00014ccc decodes as `ldr r4, [pc, #816]`, `overlay_show.ts` lists a tenth
 * "pool word referenced" at 0x02001b7c which this owner never touches, and it
 * renders 0x0200183c and 0x0200184e as NEON instructions.  The pool map here is
 * taken from the owner's own control flow - everything after the interworking
 * return - not from that listing.
 *
 * LINK BASE - 0x02008000, established in
 * semantic/overlays/resource_3b7_c_020016a8.c on three witnesses.  The image is
 * 0x206a bytes, so the five 0x0200aXXX words all land ABOVE it, in the
 * overlay's scratch EWRAM, and the layout they imply is self-consistent with
 * the initialiser:
 *   0x0200a070 (+0x2070)  the 0x50-byte state block Func_020016a8 also resets
 *   0x0200a0c0 (+0x20c0)  the mode word, written from this owner's argument
 *   0x0200a0d0 (+0x20d0)  Func_020016a8's four 24-byte actor records, which
 *                         end exactly at 0x0200a130
 *   0x0200a130 (+0x2130)  the frame counter
 *   0x0200a134 (+0x2134)  the completion flag
 *   0x0200a138 (+0x2138)  the result word
 * The two remaining words are not in the in-image band and so are unshifted:
 * 0x0000ffff is a constant, and 0x02000434 is a RAM pointer cell - note that it
 * is exactly Data_02000240 + 500, the same slot handle Func_02000104 loads in
 * this overlay, which cross-checks the workspace layout the byte-exact siblings
 * `assets/code/resource_3b7_c_02000044.c` and `..._020001a8.c` establish.
 *
 * Call targets were resolved with `tools/lib/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x02001770 -> veneer 0x020019c4 -> Func_080f9010
 *   0x0200177e -> veneer 0x02001954 -> Func_0808a100
 *   0x020017b6 -> veneer 0x0200193c -> Func_0808a080
 *   0x020017bc -> veneer 0x0200188c -> Func_08009080
 *   0x020017c2 -> veneer 0x0200193c -> Func_0808a080
 *   0x020017c8 -> veneer 0x0200188c -> Func_08009080
 *   0x020017d0 -> prologue 0x02000e44 -> Func_02000e44
 *   0x020017d8 -> prologue 0x02000e44 -> Func_02000e44
 *   0x020017e0 -> prologue 0x02000e44 -> Func_02000e44
 *   0x020017e8 -> veneer 0x0200193c -> Func_0808a080
 *   0x020017ee -> veneer 0x0200188c -> Func_08009080
 *   0x020017f4 -> veneer 0x0200193c -> Func_0808a080
 *   0x020017fa -> veneer 0x0200188c -> Func_08009080
 *   0x02001802 -> prologue 0x02000e44 -> Func_02000e44
 *   0x0200180a -> prologue 0x02000e44 -> Func_02000e44
 *   0x02001812 -> prologue 0x02000e44 -> Func_02000e44
 *   0x02001818 -> veneer 0x02001864 -> Func_080000c0
 * Completeness, as a multiset: Func_02000e44 x6, Func_0808a080 x4,
 * Func_08009080 x4, and one each of Func_080f9010, Func_0808a100,
 * Func_080000c0 - 17 sites over 6 distinct targets, exactly the tool's
 * `sites=17 distinct_targets=6` and the inventory row's calls=17.
 *
 * Func_02000e44 is tracked byte-exact C in this very overlay
 * (`assets/code/resource_3b7_c_02000e44.c`): it looks a slot record up and,
 * when it exists, writes its second argument to the record's +0x54 byte.  So
 * the six calls here are six per-slot tag writes, and the Func_0808a080 /
 * Func_08009080 pairs immediately above them are the same lookup-and-retag
 * shape Func_020016a8 uses.
 *
 * The epilogue is `pop {r5, r6, r7} / pop {r1} / bx r1`; the popped register is
 * r1, not r0, so r0 survives and is the result.  The owner returns s32.
 *
 * The loop entry `b.n 0x02001766` skips the counter increment on the first
 * pass, which is why the increment is written at the bottom of the C loop.
 *
 * Uncertainties:
 *  - The frame counter is reloaded from memory after Func_080f9010, so the C
 *    reloads it too rather than reusing the value; whether the callee can
 *    really change it is unverified, but the assembly does not assume it
 *    cannot.
 *  - The state-block field names are descriptive only; the +4/+8/+12 word
 *    triple matches the 16.16 position layout used elsewhere in this project
 *    but is read off the stores alone here.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

#include "types.h"

extern unsigned char Data_0200a070[];   /* scratch EWRAM: the 0x50-byte state block */
extern s32 Data_0200a0c0;               /* scratch EWRAM: the mode word */
extern s32 Data_0200a130;               /* scratch EWRAM: the frame counter */
extern s32 Data_0200a134;               /* scratch EWRAM: the completion flag */
extern s32 Data_0200a138;               /* scratch EWRAM: the result word */
extern s32 Data_02000434;               /* RAM: Data_02000240 + 500, a slot handle */

void Func_080f9010();
void Func_0808a100();
s32 Func_0808a080();
void Func_08009080();
void Func_02000e44();
void Func_080000c0();

s32 Func_0200173c(s32 mode)
{
    unsigned char *state = Data_0200a070;
    s32 frame;

    *(s32 *)(state + 8) = 0;
    *(s32 *)(state + 20) = 0;
    *(s32 *)(state + 32) = 0;
    *(s32 *)(state + 44) = 0;

    Data_0200a0c0 = mode;
    Data_0200a134 = 0;

    *(u16 *)(state + 2) = 0xffff;

    Data_0200a130 = 0;

    for (;;) {
        frame = Data_0200a130;

        if (frame == 50) {
            Func_080f9010(300);         /* 150 << 1 */
            frame = Data_0200a130;
        }

        if (frame == 16) {
            Func_0808a100(Data_02000434, 29);

            *(s32 *)(state + 64) = 0x14ccc;
            *(s32 *)(state + 72) = (s32)0xfffe0000; /* -2.0 in 16.16 */
            *(s32 *)(state + 12) = 0x980000;        /* 152 << 16 */
            *(s32 *)(state + 68) = 0x40000;         /* 128 << 11, i.e. 4.0 */
            *(s32 *)(state + 4) = 0x780000;         /* 240 << 15, i.e. 120.0 */
            *(u16 *)(state + 2) = 0;
            *(s32 *)(state + 8) = 0x100000;         /* 128 << 13, i.e. 16.0 */
            *(s32 *)(state + 76) = 300;             /* 150 << 1 */

            if (Data_0200a0c0 == 1) {
                /* r0 carries each lookup's record straight into the retag. */
                Func_08009080(Func_0808a080(16), 3);
                Func_08009080(Func_0808a080(17), 0);
                Func_02000e44(15, 1);
                Func_02000e44(14, 1);
                Func_02000e44(13, 1);
            } else {
                Func_08009080(Func_0808a080(11), 3);
                Func_08009080(Func_0808a080(12), 0);
                Func_02000e44(10, 1);
                Func_02000e44(9, 1);
                Func_02000e44(8, 1);
            }
        }

        Func_080000c0(1);

        if (Data_0200a134 == 1) {
            break;
        }

        Data_0200a130 = Data_0200a130 + 1;
    }

    return Data_0200a138;
}
