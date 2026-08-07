#include "types.h"

/*
 * Resource 3bc, owner at 0x0200004c.  316 strict bytes: 288 bytes of code
 * (0x004c-0x016b) and a seven-word literal pool at 0x016c-0x0187.
 *
 * This is the per-frame task the overlay installs: the byte-exact sibling
 * assets/code/resource_3bc_c_020001b4.c passes 0x0200804D to the task
 * installer, and 0x0200804D = Func_0200004c + the Thumb bit under the proven
 * 0x02008000 link base.  Its two state words Data_0200d480 / Data_0200d484
 * (file offsets 0x5480 / 0x5484) are exactly the pair that Func_02000188
 * clears before installing it and Func_020001c8 spins on afterwards, so all
 * four owners are one handshake.
 *
 * Six call sites resolved with tools/lib/overlay_call_targets.ts, all import
 * veneers, matching the row's calls=6 exactly:
 *   0808a080 x1, 0808a100 x3, 080091c0 x2.
 * The C below contains those six call expressions and no others; the two
 * calls inside the first loop are single sites reached once per iteration,
 * not duplicated arms.
 *
 * Pool word 0x0200cc20 is an in-image *data* address (even, per the parity
 * rule) at file offset 0x4c20: a table of signed bytes read with `ldrsb`,
 * indexed `phase * 6 + (row - 18)`.
 *
 * `asrs r3,#20` on the 16.16 Z word at +0x10 is the tile-grid idiom, matching
 * assets/code/resource_3bc_c_02000658.c.  The workspace store target is
 * `*0x03001ebc + 386`, computed as `movs r3,#193 / lsls #1` — the same
 * workspace+386 halfword the adjacent exact sources use.
 *
 * The X-band test in the second loop is written exactly as encoded:
 * `(u32)(object->x - (row << 21) + 0x0031FFFF) <= 0x0013FFFE`.  Both magic
 * words come straight from the pool; no attempt is made to restate the band
 * in tile units, since the shift is by 21 rather than the usual 20 and the
 * intent is not established.
 *
 * Epilogue `add sp,#8 / pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5,r6,r7} / pop {r0} / bx r0` — r0 is the popped return address, so
 * the owner is void, which is correct for an installed task.
 *
 * Uncertainty: Data_0200d484 is compared with `bls`, i.e. unsigned, so it is
 * declared u32 here; Func_020001c8 reads the same word against 1, which does
 * not distinguish the signedness.
 */

typedef struct Object {
    u8 filler00[8];
    s32 x;              /* 16.16 */
    u8 filler0C[4];
    s32 z;              /* 16.16 */
} Object;

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;   /* pointer cell holding the scene workspace */
extern s8 Data_0200cc20[];  /* in-image byte table at file offset 0x4c20 */
extern u32 Data_0200d480;
extern u32 Data_0200d484;

Object *Func_02004a80();
void Func_02004b18();
void Func_02004b22();
void Func_02004a04();
void Func_02004a1a();
void Func_02004b68();

                     

void Func_0200004c(void)
{
    u8 *workspace;
    s16 *table;
    Object *object;
    s32 tileRow;
    s32 row;
    s32 column;
    s32 slot;
    s32 value;

    workspace = Data_03001ebc;
    table = Data_02000240;
    object = Func_02004a80(*(s32 *)&table[250]);
    tileRow = object->z >> 20;

    if (Data_0200d484 == 0) {
        Data_0200d480 = (Data_0200d480 + 1) & 3;

        column = 33;
        for (row = 18; row <= 22; row++) {
            slot = Data_0200d480;
            value = Data_0200cc20[slot * 6 + (row - 18)];

            Func_02004b18(row, value);
            Func_02004b22(row + 5, value + 8);
            Func_02004a04(32, 11, 1, 2, column, 11);
            if (value != 7) {
                Func_02004a1a(74, 12, 1, 1, column, 11);
            }
            column += 2;
        }

        slot = Data_0200d480;
        Func_02004b68(28, Data_0200cc20[slot * 6 + 5]);
    } else {
        s16 *target;
        s8 *entry;

        target = (s16 *)(workspace + 386);
        slot = Data_0200d480;
        entry = &Data_0200cc20[slot * 6];

        for (row = 18; row <= 22; row++) {
            if ((u32)(object->x - (row << 21) + 0x0031FFFF) <= 0x0013FFFE) {
                if (tileRow == 11 && value == 4) {
                    *target = (s16)value;
                }
                if (tileRow == 12 && value == 5) {
                    *target = (s16)value;
                }
            }
            entry++;
            value = *entry;
        }
    }

    if (Data_0200d484 > 17) {
        Data_0200d484 = 0;
    }
    Data_0200d484++;
}
