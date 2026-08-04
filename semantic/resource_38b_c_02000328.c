#include "types.h"

/*
 * Resource 38b room-entry setup at 0x02000328 (220 bytes, 15 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000328 and the interworking
 * return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020003f2.  The popped r0
 * is the return address, so the owner is `void`.  0x020003f8-0x02000403 is
 * the literal pool (three pointer words), branched past and not code.
 *
 * All 15 call sites resolve to import veneers via
 * tools/overlay_call_targets.ts; 13 distinct callees, with Func_080f9010 and
 * Func_0808a1e0 each reached from two arms of a test.  Site count 15 matches
 * the inventory's `calls` field exactly.
 *
 * FREE LAYOUT WITNESS for the two tables, needing no disassembly.  Under the
 * proven 0x02008000 link base the pool words 0x02009cf0 and 0x02009d0c are
 * file offsets 0x1cf0 and 0x1d0c, and 0x1d0c - 0x1cf0 = 28 = 7 entries of 4
 * bytes.  The next table in the image, 0x02009d28 (used by 0x02000cb4),
 * sits 28 bytes after 0x02009d0c as well.  So both tables hold exactly 7
 * four-byte entries, indexed 0..6 — which agrees with the selector's own
 * range and with the `== 6` and `== 4` tests below.
 *
 * The selector is `(s16)(*(u16 *)(workspace + 364) - 3)`: a u16 room id
 * loaded from the workspace, biased by 3, sign-extended through a
 * `lsls #16 / asrs #16` pair.
 *
 * The write to workspace + 448 is the subtractive displacement/value shape:
 * `movs r2,#224 / lsls r2,#1` gives 448, used to form the address, and only
 * then `subs r2,#192` gives the stored value 256.  Offset 448, value 256 —
 * the same field 0x02000890 writes.
 */

/* The overlay's scene workspace pointer CELL — one dereference, not two. */
extern u8 *Data_03001ebc;

/* Seven four-byte entries at file offset 0x1cf0: the argument Func_08009178
 * receives first.  Overlay image data is writable EWRAM, so not const. */
extern s32 Data_02009cf0[];

/* Seven (s16, s16) coordinate pairs at file offset 0x1d0c. */
extern s16 Data_02009d0c[];

/* Imports, all through the veneer table at 0x02001018.  Old-style
 * declarations: arities vary per site across this overlay. */
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a010();
extern void Func_0808a090();
extern void Func_0808a100();
extern void Func_0808a0e0();
extern void Func_0808a0d8();
extern void Func_0808a1e0();
extern void Func_0808a248();
extern void Func_08009178();
extern void Func_080f9010();
extern u8 *Func_0808a080();

void Func_02000328(void)
{
    u8 *workspace;
    u8 *record;
    s32 slot;
    s32 room;

    workspace = Data_03001ebc;
    Func_0808a018();

    /* Clear the +0x55 byte on every actor slot from 8 to 65 inclusive.  The
     * counter is bumped before the `cmp r5,#65 / bls` test, so the body runs
     * for slot = 8 .. 65 — 58 iterations. */
    for (slot = 8; slot <= 65; slot++) {
        record = Func_0808a080(slot);
        if (record != 0) {
            *(u8 *)(record + 0x55) = 0;
        }
    }

    /* 182 << 1 = 364. */
    room = (s16)(*(u16 *)(workspace + 364) - 3);

    /* Two distinct sites of one callee — kept as two calls, not merged. */
    if (room == 6) {
        Func_080f9010(188);
    } else {
        Func_080f9010(158);
    }

    Func_08009178(Data_02009cf0[room],
                  (s32)Data_02009d0c[room * 2],
                  (s32)Data_02009d0c[room * 2 + 1]);

    /* 128 << 8 = 0x8000, 128 << 7 = 0x4000. */
    Func_0808a090(0, 0x8000, 0x4000);

    record = Func_0808a080(0);
    *(u8 *)(record + 0x55) = 0;

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;

    if (room == 6) {
        Func_0808a100(0, 2);
        Func_0808a0e0(0, 0, -4);
    } else {
        Func_0808a0d8(0, 3, -16);
    }

    if (room == 4) {
        Func_0808a1e0(0, 3);
    } else {
        Func_0808a1e0(0, 2);
    }

    Func_0808a010(16);
    Func_0808a248(room + 3);
    Func_0808a020();
}
