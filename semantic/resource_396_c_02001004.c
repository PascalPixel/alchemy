#include "types.h"

/*
 * resource_396 owner at 0x02001004, 544 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay/overlay_driver.ts resource_396), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * This is the row that proved m2c unsafe as a seed, and it is drafted
 * from the bytes alone: tools/m2c_guard.ts REFUSES it, because the two
 * `mov ip, pc / bx r4` sites at 0x020010ac and 0x020010c4 are calls
 * through a register, and m2c reads the first as a return and drops
 * everything after it -- 8 of the span's 21 callees survived in its
 * output, behind a clean closing brace.  My own first size estimate
 * made the same mistake and called this row ~172 bytes.
 *
 * Shape:
 * - stage records 8 and 9 (clear +85, set +12 to -0xa0000), install
 *   0x02001000 at 0x0200add0, run Func_02001224, set record 9's mode
 *   15 and run Func_02001424(0);
 * - unless the sub-selector is 19, install callback 0x02009245 with
 *   3200; if flag 0x844 is set, park records 9 and 8; if flag 0x109
 *   is set, run Func_020017ec;
 * - advance two 16.16 accumulators on the camera block at
 *   Data_03001e70: each takes a field (+236, +240), biases it
 *   (+0xa00000, +0x880000), scales it through the established IWRAM
 *   16.16 multiply Func_03000118 by 0x1999, and ADDS the result into
 *   +268 and +272; then +276 and +280 are set to 0xe666 outright;
 * - raise flags 0x201, 0x20d, 0x20f and 0x213, refresh, run
 *   Func_02000f58(0), and write 514 to the workspace's +448 s32;
 * - finally read the sub-selector and the s32 record id at
 *   Data_02000240+500, and branch: selectors 50/40/30/20 share one
 *   long arm (bracket open, mode 27, seat, 0x101, a four-argument
 *   Func_0808a210(-1, -1, -1, 0), then the record's +85 = 2,
 *   +12 = 0x640000, +20 = -0xa00000, +72 = 0x8000, a
 *   Func_080f9010(204), Func_0808a238(45, selector - 10) and a run of
 *   holds); selector 10 runs Func_020018b8 unless flag 0x109 is set;
 *   selector 19 runs Func_02000f58(1).
 *
 * Returns 0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8 spill at
 * 0x02001004-0x02001008 through the matching unspill and `bx r1` at
 * 0x020011ce-0x020011d8, alignment halfword, then the eighteen-word
 * literal pool 0x020011dc-0x02001223; the next function -- a leaf
 * that does not open with `push`, and which this driver calls as
 * Func_02001224 -- follows at 0x02001224.
 *
 * Uncertainty: flag ids, selector values and the callback constants
 * are literal; the camera block's +236/+240/+268/+272/+276/+280 and
 * the record's +34/+85/+12/+20/+72 are recorded by offset with roles
 * open; the s32 at Data_02000240+500 is used only as an id argument
 * here, so what it indexes is not established.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001e70;
extern u8 *Data_03001ebc;

extern void Func_02000f58(s32 arg0);
extern void Func_02001224(void);
extern void Func_02001424(s32 arg0);
extern void Func_020017ec(void);
extern void Func_020018b8(void);

/* IWRAM 16.16 multiply, established; reached at 0x03000118 through the
 * `mov ip, pc / bx r4` idiom rather than by bl. */
typedef s32 (*Multiply16_16)(s32 value, s32 scale);

extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 callback, s32 arg1);
extern void Func_080091e0(void *record, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a1e0(s32 id, s32 mode);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a238(s32 arg0, s32 arg1);
extern void Func_0808a360(void);
extern void Func_080f9010(s32 arg0);

s32 Func_02001004(void)
{
    u8 *record;
    u8 *camera;
    Multiply16_16 multiply16_16 = (Multiply16_16)0x03000118;
    s32 id;
    s16 selector;

    record = Func_0808a080(8);
    *(s32 *)0x0200add0 = 0x02001000;
    Func_02001224();
    *(s32 *)(record + 12) = 0xfff60000;
    record[85] = 0;
    record[85] = 0;
    *(s32 *)(record + 12) = 0xfff60000;
    record = Func_0808a080(9);
    Func_0808a158(9, 15);
    Func_02001424(0);
    if (Data_02000240[225] != 19) {
        Func_080000d0(0x02009245, 3200);
    }
    if (Func_080770c0(0x844) != 0) {
        Func_0808a0f0(9, 0, 0);
        Func_0808a0f0(8, 0, 0);
    }
    if (Func_080770c0(0x109) != 0) {
        Func_020017ec();
    }

    *(s32 *)(camera + 276) = 0xe666;
    camera = Data_03001e70;
    *(s32 *)(camera + 268) += multiply16_16(*(s32 *)(camera + 236) + 0xa00000, 0x1999);
    *(s32 *)(camera + 280) = 0xe666;
    *(s32 *)(camera + 272) += multiply16_16(*(s32 *)(camera + 240) + 0x880000, 0x1999);
    Func_080770c8(0x201);
    Func_080770c8(0x20d);
    Func_080770c8(0x20f);
    Func_080770c8(0x213);
    Func_080000c0(1);
    Func_02000f58(0);
    *(s32 *)(Data_03001ebc + 448) = 514;

    id = *(s32 *)((u8 *)Data_02000240 + 500);
    selector = Data_02000240[225];
    record = Func_0808a080(id);
    if (selector == 50 || selector == 40 || selector == 30 || selector == 20) {
        Func_0808a360();
        Func_0808a100(id, 27);
        Func_080091e0(Func_0808a080(id), 0);
        Func_0808a1f0(id, 0x101);
        Func_0808a210(-1, -1, -1, 0);
        record[85] = 2;
        *(s32 *)(record + 12) = 0x640000;
        *(s32 *)(record + 20) = 0xff600000;
        *(s32 *)(record + 72) = 0x8000;
        Func_080f9010(204);
        Func_0808a238(45, selector - 10);
        Func_0808a010(20);
        record[34] = 2;
        Func_0808a1e0(id, 3);
        Func_0808a010(2);
        Func_0808a1f0(id, 256);
        Func_0808a010(8);
    } else if (selector == 10) {
        if (Func_080770c0(0x109) == 0) {
            Func_020018b8();
        }
    } else if (selector == 19) {
        Func_02000f58(1);
    }
    return 0;
}
