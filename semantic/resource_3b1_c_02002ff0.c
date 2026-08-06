#include "types.h"

/*
 * resource_3b1 owner at 0x02002ff0, 884 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_3b1), so the loader enters
 * here and no bl inside the image reaches it, which is why it sat in
 * no structural inventory row.
 *
 * TWO jump tables, both switching on the SAME value: the sub-selector
 * at Data_02000240[225], biased by one.  The first, at image offset
 * 0x3028, has 24 entries collapsing to four arms whose only job is to
 * choose an argument for Func_0808a158.  The second, at 0x30f0, has
 * 30 entries over twenty distinct arms, most of them a single call
 * into this overlay's own body.  Both tables' arms run to the shared
 * epilogue at 0x02003320, which is why the row is one function of 884
 * bytes and not the 824 a table-blind span reports.
 *
 * Head: Func_080000c0(1), Func_080770c8(324), and 521 into the
 * workspace's +448 s32.
 *
 * First switch (sub-selector 1..24; anything else falls straight to
 * the second):
 * - 1, 2, 11: flag 0x93e clear and flag 0x928 set selects 9; else
 *   flag 0x911 set selects 12; otherwise nothing;
 * - 4, 12, 15..21, 23, 24: selects 19 unconditionally;
 * - 5: flag 0x93e clear and flag 0x911 set selects 13;
 * - 3, 6..10, 13, 14, 22: nothing.
 * The selection is passed as Func_0808a158(id, 2).
 *
 * Second switch (sub-selector 1..30) is a scene-step dispatcher: each
 * arm runs one of this overlay's own step functions, several of them
 * chosen by flag 0x928.  Steps 15, 17, 19 and 21 share a shape: flag
 * 0x109 gates a "recap" variant that runs Func_0808a018, seats records
 * 25 and 22 through Func_020048e8, points records 36..38 at the table
 * at 0x0200e8e4 and puts them in mode 3, against an ordinary variant
 * that is a single call.  Step 21's gated branch instead clears the
 * word at 0x0200ff84, installs callback 0x0200dc49 with 3200 and puts
 * record 9 in mode 5.  Step 30 seats records 20 and 21, raises flag
 * 0x902 and calls Func_0808a248(1).  Returns 0 on every path.
 *
 * Note on the compiled form, kept out of the C: the step-15 arm does
 * not emit its own last call.  It loads 38 and branches to 0x02003298,
 * the tail of the step-19 arm, which supplies the shared `3` and the
 * bl.  The draft writes both calls out because that is what each path
 * performs.
 *
 * Complete owner: `push {r5, lr}` at 0x02002ff0 through the epilogue
 * at 0x02003320-0x02003326, then the fifteen-word literal pool at
 * 0x02003328-0x02003363; the next function begins at 0x02003364, and
 * this driver itself calls it (step 5).
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: tools/m2c_guard.ts clears this row of the call-through-
 * register hazard, flags both jump tables, and reports full call
 * coverage against this draft.  Every bl was resolved as an ABSOLUTE
 * image offset (stored displacement + 2) and every table word as
 * `word - 0x8000` in the same space; the objdump listing's pc-relative
 * arrows are wrong throughout.
 *
 * Uncertainty: flag ids and record ids are literal; the sub-selector
 * values are step numbers only in the sense that they index this
 * dispatcher.  0x0200e8e4 is an in-image data table handed to
 * Func_0808a098 and not read here.  0x0200dc49 is an in-image code
 * pointer (image offset 0x5c48 | 1) installed as a callback and never
 * called here -- a data-installed pointer of the kind neither a BL
 * sweep nor the unindexed check can see.  0x0200ff84 is the first word
 * PAST the assembled image (size 0x7f84), so it is overlay-local
 * scratch beyond the loaded bytes; it is written as its literal
 * address because nothing in the tree names it yet.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;


































extern void Func_02009416(s32 arg0);
extern void Func_020094a6(s32 flagId);
extern s32 Func_0200952c(s32 flagId);
extern s32 Func_02009536(s32 flagId);
extern s32 Func_02009546(s32 flagId);
extern s32 Func_0200955c(s32 flagId);
extern s32 Func_02009566(s32 flagId);
extern void Func_0200966a(s32 id, s32 arg1);
extern void Func_02006686(void);
extern void Func_020067dc(void);
extern void Func_020064da(void);
extern s32 Func_0200961e(s32 flagId);
extern void Func_02006f72(void);
extern void Func_02006e9c(void);
extern s32 Func_02009634(s32 flagId);
extern void Func_02007088(void);
extern void Func_02006fd6(void);
extern void Func_0200713c(void);
extern void Func_02007296(void);
extern void Func_020072f0(void);
extern s32 Func_0200965c(s32 flagId);
extern void Func_0200968c(void);
extern void Func_02007ab6(s32 id, s32 arg1, s32 arg2);
extern void Func_02007ac0(s32 id, s32 arg1, s32 arg2);
extern void Func_020096e2(s32 id, const void *table);
extern void Func_020096ea(s32 id, const void *table);
extern void Func_020096f2(s32 id, const void *table);
extern void Func_02009792(s32 id, s32 arg1);
extern void Func_0200979a(s32 id, s32 arg1);
extern void Func_020073a2(void);
extern void Func_02007428(void);
extern s32 Func_020096b8(s32 flagId);
extern void Func_020096e8(void);
extern void Func_02007b12(s32 id, s32 arg1, s32 arg2);
extern void Func_02007b1c(s32 id, s32 arg1, s32 arg2);
extern void Func_0200973e(s32 id, const void *table);
extern void Func_02009746(s32 id, const void *table);
extern void Func_0200971a(void);
extern void Func_020082b8(void);
extern void Func_020088da(void);
extern s32 Func_020096fe(s32 flagId);
extern void Func_0200972e(void);
extern void Func_02007b58(s32 id, s32 arg1, s32 arg2);
extern void Func_02007b62(s32 id, s32 arg1, s32 arg2);
extern void Func_02009784(s32 id, const void *table);
extern void Func_0200978c(s32 id, const void *table);
extern void Func_0200982c(s32 id, s32 arg1);
extern void Func_02009834(s32 id, s32 arg1);
extern void Func_02009770(void);
extern void Func_02008982(void);
extern void Func_02008a2c(void);
extern s32 Func_02009754(s32 flagId);
extern s32 Func_0200975e(s32 flagId);
extern void Func_020096fa(s32 callback, s32 arg1);
extern void Func_0200982a(s32 id, s32 arg1);
extern void Func_02008acc(void);
extern void Func_02008f8a(void);
extern s32 Func_0200978e(s32 flagId);
extern void Func_02009006(void);
extern void Func_0200940c(void);
extern void Func_02007bf0(s32 id, s32 arg1, s32 arg2);
extern void Func_02007bfa(s32 id, s32 arg1, s32 arg2);
extern void Func_020097c0(s32 flagId);
extern void Func_02009936(s32 arg0);
s32 Func_02002ff0(void)
{
    s32 which;
    s32 sharedModeRecord;

    Func_02009416(1);
    Func_020094a6(324);
    *(s32 *)(Data_03001ebc + 448) = 521;

    switch (Data_02000240[225]) {
    case 1:
    case 2:
    case 11:
        if (Func_0200952c(0x93e) == 0 && Func_02009536(0x928) != 0) {
            which = 9;
        } else if (Func_02009546(0x911) != 0) {
            which = 12;
        }
        break;
    case 4:
    case 12:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 23:
    case 24:
        which = 19;
        break;
    case 5:
        if (Func_0200955c(0x93e) == 0 && Func_02009566(0x911) != 0) {
            which = 13;
        }
        break;
    default:
        break;
    }
    if (which != 0) {
        Func_0200966a(which, 2);
    }
    which = 0;

    switch (Data_02000240[225]) {
    case 1:
    case 2:
        Func_02006686();
        break;
    case 4:
        Func_020067dc();
        break;
    case 5:
        Func_020064da();
        break;
    case 10:
        if (Func_0200961e(0x928) != 0) {
            Func_02006f72();
        } else {
            Func_02006e9c();
        }
        break;
    case 11:
        if (Func_02009634(0x928) != 0) {
            Func_02007088();
        } else {
            Func_02006fd6();
        }
        break;
    case 12:
        Func_0200713c();
        break;
    case 13:
        Func_02007296();
        break;
    case 14:
        Func_020072f0();
        break;
    case 15:
        if (Func_0200965c(0x109) != 0) {
            Func_0200968c();
            Func_02007ab6(25, 1, 0);
            Func_02007ac0(22, 0, 0);
            Func_020096e2(36, (const void *)0x0200e8e4);
            Func_020096ea(37, (const void *)0x0200e8e4);
            Func_020096f2(38, (const void *)0x0200e8e4);
            Func_02009792(36, 3);
            Func_0200979a(37, 3);
            sharedModeRecord = 38;
            goto finish_mode3;
        } else {
            Func_020073a2();
        }
        break;
    case 16:
        Func_02007428();
        break;
    case 17:
        if (Func_020096b8(0x109) != 0) {
            Func_020096e8();
            Func_02007b12(25, 2, 0);
            Func_02007b1c(22, 0, 0);
            Func_0200973e(36, (const void *)0x0200e8e4);
            Func_02009746(37, (const void *)0x0200e8e4);
            Func_0200971a();
        } else {
            Func_020082b8();
        }
        break;
    case 18:
        Func_020088da();
        break;
    case 19:
        if (Func_020096fe(0x109) != 0) {
            Func_0200972e();
            Func_02007b58(25, 3, 0);
            Func_02007b62(22, 0, 0);
            Func_02009784(36, (const void *)0x0200e8e4);
            Func_0200978c(37, (const void *)0x0200e8e4);
            Func_0200982c(36, 3);
            sharedModeRecord = 37;
finish_mode3:
            Func_02009834(sharedModeRecord, 3);
            Func_02009770();
        } else {
            Func_02008982();
        }
        break;
    case 20:
        Func_02008a2c();
        break;
    case 21:
        if (Func_02009754(0x109) != 0) {
            if (Func_0200975e(0x302) != 0) {
                *(s32 *)0x0200ff84 = 0;
                Func_020096fa(0x0200dc49, 3200);
                Func_0200982a(9, 5);
            }
        } else {
            Func_02008acc();
        }
        break;
    case 22:
        Func_02008f8a();
        break;
    case 23:
        if (Func_0200978e(0x109) == 0) {
            Func_02009006();
        }
        break;
    case 24:
        Func_0200940c();
        break;
    case 30:
        Func_02007bf0(20, 0x926, 0x92b);
        Func_02007bfa(21, 0, 0);
        Func_020097c0(0x902);
        Func_02009936(1);
        break;
    default:
        break;
    }
    return 0;
}
