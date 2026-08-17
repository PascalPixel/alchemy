/*
 * resource_3c4 @ 0x02000e20 (168 bytes: 94 code + a 44-byte jump table +
 * four pool words).
 *
 * Decides whether the record the service returns is high enough to be
 * published into the workspace slot at +24 of the block whose pointer lives at
 * 0x03001ee0.  The threshold depends on the scene selector pair held at
 * Data_02000240[224] and [225] — the same pair the byte-exact neighbours
 * `resource_3c4_c_02001270.c` and `resource_3c4_c_020012b0.c` read; both
 * halfwords are read with `ldrsh`, so they are signed.
 *
 * The dispatch at 0x02000e56 is `mov pc,r3` through a word table whose base
 * pool word is 0x02008e58.  This overlay is linked at 0x02008000 while the
 * inventory prints a 0x02000000 base, so the table is at file offset 0xe58 and
 * its eleven entries resolve to the three constant arms and the default.  The
 * index is `[225] - 3` tested with `bhi`, i.e. unsigned, so any [225] outside
 * 3..13 takes the default of 0.
 *
 * The height test uses `asrs #19`, one bit finer than the 12.20 grid used
 * elsewhere in this overlay, and `bgt`, so it is signed.
 *
 * `pop {r5, r6} ; pop {r0} ; bx r0` return: void.

 * BYTE-EXACT BUT NOT ADOPTABLE. This source compiles to the ROM's 168 bytes
 * exactly: `overlay score semantic/resource_3c4_c_02000e20.c` reports
 * differing_halfwords=0, class=exact. It cannot be adopted, and the reason is
 * in the audit rather than in the source.
 *
 * The owner contains a switch, and its jump table -- fourteen `.4byte` entries
 * at 0x02000e58 reached by `mov pc, r3` -- is classified as data, so
 * `metrics/gs1-en-executable.json` emits no interval for those 56 bytes and
 * leaves a hole inside the owner. `overlay adopt` requires the span to sit in
 * audited executable intervals, and `full_c_progress` enforces the same rule
 * for byte accounting. Relaxing only the first is wrong: the 56 bytes are not
 * in the audited denominator, so counting them in the numerator would overstate
 * the share. Both have to move together, with the audit classifying a jump
 * table reached from inside a function as part of that function.
 *
 * This is not one owner's problem. Across the 96 overlays there are 300 such
 * holes bounded by code on both sides, 21,256 bytes, in 46 overlays.
 */
#include "types.h"

extern s16 Data_02000240[];
extern void **Data_03001ee0;   /* workspace block pointer in IWRAM */

typedef struct Record_02000e20 {
    s32 pad0;
    s32 pad4;
    s32 pad8;
    s32 pad12;
    s32 height;            /* +16 */
} Record_02000e20;

typedef struct Workspace_02000e20 {
    s32 pad0[6];
    Record_02000e20 *published;   /* +24 */
} Workspace_02000e20;

/* このオーバーレイは同じ本体関数へ二つのベニアを持つ。
 * This overlay holds TWO veneers to the same main-image routine: resource_3c4's
 * 0x0200034c reaches it through 0x0200310c, this owner through 0x02003f32.
 * Naming the callee by the main-image symbol is therefore ambiguous and
 * resolves to the other veneer, so it is named by the entry actually called. */
Record_02000e20 *Func_02003f32();  /* Func_0808a080 */

extern u8 Value_000000ac;
void Func_02000e20(void)
{
    Record_02000e20 *record = Func_02003f32(0);
    Workspace_02000e20 *workspace = (Workspace_02000e20 *)Data_03001ee0;
    s32 threshold = 0;

    if (Data_02000240[224] == ((s32) &Value_000000ac)) {
        switch (Data_02000240[225]) {
        case 3:
        case 4:
            threshold = 94;
            break;
        case 8:
        case 9:
            threshold = 74;
            break;
        case 12:
        case 13:
            threshold = 118;
            break;
        default:
            /* 5..7, 10, 11 and anything outside 3..13 keep 0. */
            break;
        }
    } else if (Data_02000240[225] == 12) {
        threshold = 93;
    }

    /* 判定は逆向き。参照は零書き込みへ落ち、レコード書き込みへ分岐する。
     * The reference falls through to the zero store and branches to the record
     * store, so the test is `<=` with the arms this way round. */
    if ((record->height >> 19) <= threshold) {
        workspace->published = 0;
    } else {
        workspace->published = record;
    }
}
