#include "types.h"

/*
 * resource_3a0 owner at 0x02001148, 488 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_3a0), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Shape: write 256 to the workspace's +448 s32, Func_0808a5e0(169),
 * and raise flag 0x12f when the sub-selector is greater than 9.
 * Then:
 * - flag 0x895: halt record 13, place record 14 at (0x920000,
 *   0x1380000) and halt it, and with flag 0x89a also park record 17;
 * - flag 0x8b0: park record 17 (again -- an independent gate on the
 *   same record, kept as compiled);
 * - a three-iteration loop over records 23, 24, 25: rewrite the low
 *   nibble of +9 in the record its +80 points at (clear bits 0, 2 and
 *   3, set bit 2), clear +85, set +89 to 8, seat it, Func_0808a160
 *   with 15, and rewrite +35 to (old & 0xfe) | 2;
 * - flag 0x202: place record 14 as above and halt it;
 * - flag 0x201: record 20 into mode 5, then draw panel (3, 17, 1, 1,
 *   x, z) where x and z are that record's +8 and +16 shifted right by
 *   20 -- an arithmetic shift, so negatives round toward minus
 *   infinity -- and install callback 0x02008325 with 3200;
 * - unconditionally: record 18 into mode 2 with callback 0x02008501
 *   at its +108; record 19 cleared at +85 with +12 and +60 set to
 *   0x100000, +24 = 0x8ccc, +28 = 0x6666 and 0x8000 into +30 of the
 *   record at its +80; record 21 seated, +85 cleared, +12 zeroed and
 *   +60 set to 0x80000000.
 *
 * Returns 0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8 spill at
 * 0x02001148-0x0200114c with `sub sp, #8` through the unspill and
 * `bx r1` at 0x02001324-0x0200132c; the overlay's import-veneer table
 * begins at 0x02001330.  NOTE the layout: the twelve-word literal
 * pool at 0x020012f4-0x02001323 sits INSIDE the function, before the
 * epilogue, which is reached by the `b 0x02001324` at 0x020012f2 --
 * so the pool is jumped over rather than trailing the body.
 *
 * Cross-checked per the 2026-08-01 ruling: tools/m2c_guard.ts clears
 * this row of the call-through-register hazard and reports full call
 * coverage against this draft.
 *
 * Uncertainty: flag ids are literal; record fields +9 (of the +80
 * record), +12, +24, +28, +35, +60, +85, +89, +108 are recorded by
 * offset with roles open; 0x02008325 and 0x02008501 are in-image code
 * pointers (image offsets 0x324 | 1 and 0x500 | 1) installed as
 * callbacks, not called here -- data-installed pointers of the kind
 * neither a BL sweep nor the unindexed check can see.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_080000d0(s32 callback, s32 arg1);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a160(void *record, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a5e0(s32 arg0);

s32 Func_02001148(void)
{
    u8 *record;
    u8 *inner;
    s32 index;

    *(s32 *)(Data_03001ebc + 448) = 256;
    Func_0808a5e0(169);
    if (Data_02000240[225] > 9) {
        Func_080770d0(0x12f);
    }
    if (Func_080770c0(0x895) != 0) {
        Func_0808a1b8(13, 0x8000, 0);
        Func_0808a0f0(14, 0x920000, 0x1380000);
        Func_0808a1b8(14, 0, 0);
        if (Func_080770c0(0x89a) != 0) {
            Func_0808a0f0(17, 0, 0);
        }
    }
    if (Func_080770c0(0x8b0) != 0) {
        Func_0808a0f0(17, 0, 0);
    }
    for (index = 0; index <= 2; index++) {
        record = Func_0808a080(index + 23);
        inner = *(u8 **)(record + 80);
        inner[9] = (inner[9] & ~13) | 4;
        record[85] = 0;
        record[89] = 8;
        Func_080091e0(record, 0);
        Func_0808a160(record, 15);
        record[35] = (record[35] & 0xfe) | 2;
    }
    if (Func_080770c0(0x202) != 0) {
        Func_0808a0f0(14, 0x920000, 0x1380000);
        Func_0808a1b8(14, 0, 0);
    }
    if (Func_080770c0(0x201) != 0) {
        Func_0808a100(20, 5);
        Func_080091c0(3, 17, 1, 1,
            *(s32 *)(Func_0808a080(20) + 8) >> 20,
            *(s32 *)(Func_0808a080(20) + 16) >> 20);
        Func_080000d0(0x02008325, 3200);
    }
    Func_0808a158(18, 2);
    *(s32 *)(Func_0808a080(18) + 108) = 0x02008501;
    record = Func_0808a080(19);
    record[85] = 0;
    *(s32 *)(record + 12) = 0x100000;
    *(s32 *)(record + 60) = 0x100000;
    *(s32 *)(record + 24) = 0x8ccc;
    *(s32 *)(record + 28) = 0x6666;
    *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
    Func_080091e0(Func_0808a080(21), 0);
    Func_0808a080(21)[85] = 0;
    *(s32 *)(Func_0808a080(21) + 12) = 0;
    *(s32 *)(Func_0808a080(21) + 60) = 0x80000000;
    return 0;
}
