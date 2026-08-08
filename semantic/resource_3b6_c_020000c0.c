#include "types.h"

/*
 * Resource 3b6 scene entry / camera placement at 0x020000c0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020000c0 and the matching
 * `pop {r5, r6, r7} / pop {r0} / bx r0` interworking return at 0x0200012e.
 * The return address is popped into r0, so the owner is `void`, and it takes
 * no argument (r0 is written before it is read).  A two-word literal pool
 * occupies 0x02000134-0x0200013b.
 *
 * All seven `bl` sites resolve with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- — an overlay
 * `bl` stores `target_offset - 2` — and are named for the main-image address
 * in each veneer entry's trailing word.  Seven sites, matching the row's
 * advertised call count exactly.
 *
 * Data addressing.  This overlay is linked at 0x02008000 (witness: the
 * byte-exact sibling at 0x02000044 returns 0x02009040 / 0x02008e00, and file
 * offsets 0x1040 / 0x0e00 hold two parallel tables of identical record shape).
 * So the pool word 0x02009dcc is an in-image table at file offset 0x1dcc, not
 * a RAM global.  Its records are eight bytes: a pointer, then two halfwords.
 *
 * 0x03001ebc is a pointer CELL, not the workspace: `ldr r3,[pc]` loads the
 * cell address and `ldr r6,[r3]` loads the workspace pointer out of it.  This
 * matches the spelling in byte-exact assets/code/resource_3b6_c_0200009c.c.
 *
 * Uncertainties, recorded rather than guessed: the meaning of the byte at
 * record + 0x55 that the loop clears, the argument 158 to 0x080f9010, and the
 * scene index bias of 14.
 */

extern u8 *Data_03001ebc;

/* Old-style declarations: interfaces vary by call site across this overlay. */
u8 *Func_02000ac6();
void Func_02000b78();
void Func_02000ab2();
void Func_02000b10();
u8 *Func_02000b0e();
void Func_02000b3c();
void Func_02000b8c();
                       /* fetch a record by slot; 0 when the slot is empty */

                     

/* One eight-byte record of the in-image table at file offset 0x1dcc. */
struct Placement_020000c0 {
    void *data;  /* loaded with `ldr r0,[base, index]` at offset 0 */
    u16 x;       /* halfword at offset 4 */
    u16 y;       /* halfword at offset 6 */
};

void Func_020000c0(void)
{
    u8 *workspace;
    u8 *record;
    s16 *scene;
    s32 slot;
    s32 index;
    struct Placement_020000c0 *table;

    workspace = Data_03001ebc;

    /* Slots 8..65 inclusive; the compare is `cmp r5,#65 / bls`, so unsigned. */
    for (slot = 8; (u32)slot <= 65; slot++) {
        record = Func_02000ac6(slot);
        if (record != 0) {
            record[0x55] = 0;
        }
    }

    /* 182 << 1 = 364 = 0x16c.  The halfword there is read signed (ldrsh). */
    scene = (s16 *)(workspace + 0x16c);
    index = (s32)scene[0] - 14;

    Func_02000b78(158);

    /* Return value of 0x080f9010 is discarded: r0 is reloaded from the pool. */
    table = (struct Placement_020000c0 *)0x02009dcc;
    Func_02000ab2(table[index].data, table[index].x, table[index].y);

    /* r1 = 128 << 8 = 0x8000, r2 = 128 << 7 = 0x4000. */
    Func_02000b10(0, 0x8000, 0x4000);

    /* Slot 0's record is used without a null test here, unlike the loop above;
     * that asymmetry is in the assembly, not an omission. */
    record = Func_02000b0e(0);
    record[0x55] = 0;

    Func_02000b3c(0, 2);
    Func_02000b8c((s32)scene[0]);
}
