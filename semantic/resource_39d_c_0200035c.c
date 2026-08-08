#include "types.h"

/*
 * resource_39d owner at 0x0200035c, 528 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay-driver resource_39d), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Raises flag 0x111 and writes 516 to the workspace's +448 s32
 * unconditionally, then does nothing further unless the scene id is
 * 58.  For that scene:
 * - raise flag 324 and install callback 0x0200b4bd with 3200;
 * - put sixteen records into mode 1, unrolled in the order
 *   0, 1, 2, 3, 5, 20, 21, 22, 23, 24, 8, 9, 10, 11, 12, 13 -- kept
 *   in that order, which is not sorted;
 * - then a real loop over ids 14..19: mode 1, +85 = 4, OR 2 into +35,
 *   and +12 = -0x328000;
 * - if flag 0x109 is set, ask Func_020008cc for an id and, if both it
 *   and its record are non-null, clear that record's +85;
 * - seat records 9, 10, 11, 12 and 13 with Func_080091e0(record, 0),
 *   then set +24 of records 12 and 13 to -0x10000;
 * - finally branch on the sub-selector: 1 runs Func_0200056c unless
 *   flag 0x109 is set; 2 (with flag 0x251 clear) writes 0x4000000 to
 *   +368 of the camera block, refreshes, draws strip
 *   (4, 70, 4, 74, 5, 4), parks record 9 and runs Func_02001af0
 *   unless flag 0x109 is set; 5 raises flag 0x251.
 *
 * Returns 0 on every path.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200035c with `sub sp, #8`
 * through `movs r0, #0 / add sp, #8 / pop {r5, r6} / pop {r1} /
 * bx r1` at 0x02000538-0x02000540, then the ten-word literal pool
 * 0x02000544-0x0200056b; the next function's `push {lr}` follows at
 * 0x0200056c -- and that function is Func_0200056c, which this driver
 * itself calls.  Returns a literal 0.
 *
 * Cross-checked per the 2026-08-01 ruling: tools/m2c_guard.ts clears
 * this row of the call-through-register hazard and reports full call
 * coverage against this draft.
 *
 * Uncertainty: flag ids and the scene/sub-selector values are
 * literal; record fields +12, +24, +35, +85 and the camera block's
 * +368 are recorded by offset with roles open; 0x0200b4bd is an
 * in-image code pointer (image offset 0x34bc | 1) installed as a
 * callback, not called here -- one of the data-installed pointers
 * that neither a BL sweep nor the unindexed check can see.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001e70;
extern u8 *Data_03001ebc;

extern s32 Func_020008cc(void);
extern void Func_0200056c(void);
extern void Func_02001af0(void);

extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 callback, s32 arg1);
extern void Func_08009128(void);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a1e0(s32 id, s32 mode);

s32 Func_0200035c(void)
{
    u8 *record;
    s32 id;

    Func_080770c8(0x111);
    *(s32 *)(Data_03001ebc + 448) = 516;
    if (Data_02000240[224] != 58) {
        return 0;
    }
    Func_080770c8(324);
    Func_080000d0(0x0200b4bd, 3200);
    Func_0808a1e0(0, 1);
    Func_0808a1e0(1, 1);
    Func_0808a1e0(2, 1);
    Func_0808a1e0(3, 1);
    Func_0808a1e0(5, 1);
    Func_0808a1e0(20, 1);
    Func_0808a1e0(21, 1);
    Func_0808a1e0(22, 1);
    Func_0808a1e0(23, 1);
    Func_0808a1e0(24, 1);
    Func_0808a1e0(8, 1);
    Func_0808a1e0(9, 1);
    Func_0808a1e0(10, 1);
    Func_0808a1e0(11, 1);
    Func_0808a1e0(12, 1);
    Func_0808a1e0(13, 1);
    for (id = 14; id <= 19; id++) {
        Func_0808a1e0(id, 1);
        Func_0808a080(id)[85] = 4;
        Func_0808a080(id)[35] |= 2;
        *(s32 *)(Func_0808a080(id) + 12) = 0xffcd8000;
    }
    if (Func_080770c0(0x109) != 0) {
        id = Func_020008cc();
        if (id != 0) {
            record = Func_0808a080(id);
            if (record != 0) {
                record[85] = 0;
            }
        }
    }
    Func_080091e0(Func_0808a080(9), 0);
    Func_080091e0(Func_0808a080(10), 0);
    Func_080091e0(Func_0808a080(11), 0);
    Func_080091e0(Func_0808a080(12), 0);
    Func_080091e0(Func_0808a080(13), 0);
    *(s32 *)(Func_0808a080(12) + 24) = 0xffff0000;
    *(s32 *)(Func_0808a080(13) + 24) = 0xffff0000;
    if (Data_02000240[225] == 1) {
        *(s32 *)(Data_03001e70 + 368) = 0x4000000;
        if (Func_080770c0(0x109) == 0) {
            Func_0200056c();
        }
    } else if (Data_02000240[225] == 2) {
        if (Func_080770c0(0x251) != 0) {
            return 0;
        }
        Func_08009128();
        Func_080000c0(1);
        Func_08009180(4, 70, 4, 74, 5, 4);
        Func_0808a0f0(9, 0, 0);
        if (Func_080770c0(0x109) == 0) {
            Func_02001af0();
        }
    } else if (Data_02000240[225] == 5) {
        Func_080770c8(0x251);
    }
    return 0;
}
