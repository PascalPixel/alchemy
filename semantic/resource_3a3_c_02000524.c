#include "types.h"

/*
 * Resource 3a3 scene-variant entry sequence at 0x02000524 (384 bytes,
 * 0x02000524 .. 0x020006a3, of which 0x0200069a .. 0x020006a3 is alignment
 * plus the literal pool).
 *
 * Complete owner: `push {r5, r6, r7, lr}` / `push {r5, r6, r7}` (the saved
 * r8, r9, sl) / `sub sp,#8` at 0x02000524, and the matching
 * `add sp,#8 / pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x0200068a.  The final popped
 * register is r0, so the owner returns nothing.
 *
 * All 24 call sites are placed (row reports calls=24): Func_0808a018 x1,
 * Func_0808a080 x2, Func_080f9010 x2, Func_08009180 x5, Func_0808a010 x3,
 * Func_08009178 x1, Func_0808a090 x1, Func_0808a100 x1, Func_0808a0d8 x2,
 * Func_0808a1e0 x1, Func_0808a0e0 x1, Func_0808a248 x1, Func_0808a368 x1,
 * Func_0808a370 x1, Func_0808a020 x1.
 *
 * `0x03001ebc` is a pointer CELL holding the overlay workspace; it is loaded
 * twice (r9 at entry and again at 0x0200060c) and both loads dereference it.
 *
 * The variant index is `(s16)(workspace_halfword_at_364 - 2)`.  It selects an
 * 8-byte record from the in-image table at 0x02009e70 -- even and in the
 * 0x0200_9xxx band, so under the proven 0x02008000 link base that is image
 * data at file offset 0x1e70, not a RAM address.  The record's word at +0 is
 * passed straight to Func_08009178; its two halfwords at +4 and +6 are signed
 * and are the coordinates threaded through every Func_08009180 call.
 *
 * Func_08009180 is the six-argument renderer ABI: four register arguments plus
 * two stack words, both of which are 2 on the sl==1 path and 1/2 on the sl==3
 * path.
 *
 * Uncertainty: the variant index is used unchecked as a table subscript, so a
 * workspace halfword below 2 would index behind the table.  Nothing in this
 * owner bounds it; presumably its writer does.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0808a018();       /* opens a scripted sequence */
u8 *Func_0808a080();        /* scene/actor record accessor */
void Func_080f9010();
void Func_08009180();       /* six-argument renderer entry */
void Func_0808a010();       /* waits the given number of frames */
void Func_08009178();
void Func_0808a090();
void Func_0808a100();
void Func_0808a0d8();
void Func_0808a1e0();
void Func_0808a0e0();
void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
void Func_0808a020();       /* closes the scripted sequence */

extern u8 *Data_03001ebc;   /* pointer cell holding the overlay workspace */

struct Variant02009e70 {
    s32 f00;
    s16 f04;
    s16 f06;
};

extern struct Variant02009e70 Data_02009e70[];

void Func_02000524(void)
{
    u8 *workspace;
    u8 *record;
    s32 id;
    s32 variant;
    s32 x;
    s32 z;

    s32 permuted_14;
    workspace = Data_03001ebc;

    Func_0808a018();

    /* Clear the +0x55 byte of every existing record in the id range 8..65. */
    for (id = 8; id <= 65; id++) {
        record = Func_0808a080(id);
        if (record != 0) {
            record[0x55] = 0;
        }
    }

    permuted_14 = (s16)(*(u16 *)(workspace + 364) - 2);
    z = Data_02009e70[variant].f06;
    variant  = permuted_14;
    x = Data_02009e70[variant].f04;

    if (variant == 1) {
        Func_080f9010(188);
        Func_08009180(42, 33, x, z, 2, 2);
        Func_08009180(42, 35, x + 2, z, 2, 2);
        Func_0808a010(4);
        Func_08009180(40, 33, x, z, 2, 2);
        Func_08009180(40, 35, x + 2, z, 2, 2);
        Func_0808a010(4);
    } else {
        Func_080f9010(158);
        if (variant == 3) {
            Func_08009180(33, 42, 8, 17, 1, 2);
        }
        Func_08009178(Data_02009e70[variant].f00, x, z);
    }

    Func_0808a090(0, 0x8000, 0x4000);           /* 128<<8, 128<<7 */

    /* The workspace pointer is reloaded here; this is a 32-bit store. */
    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;            /* `movs r2,#128 / lsls #1` */

    record = Func_0808a080(0);
    record[0x55] = 0;
    Func_0808a100(0, 2);

    if (variant == 6) {
        Func_0808a0d8(0, 2, 0);
        goto placement_done;
    }
    if (variant != 1) {
        Func_0808a0d8(0, 2, -4);
        goto placement_done;
    }
    Func_0808a1e0(0, 2);
    Func_0808a0e0(0, 0, -4);

placement_done:
    Func_0808a010(10);

    Func_0808a248((s32)*(s16 *)(workspace + 364));

    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
}
