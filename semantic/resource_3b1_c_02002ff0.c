#include "types.h"

/*
 * resource_3b1 owner at 0x02002ff0, 884 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools-rs/overlay-driver resource_3b1), so the loader enters
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

extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 callback, s32 arg1);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a098(s32 id, const void *table);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a248(s32 arg0);

extern void Func_02003364(void);
extern void Func_0200351c(void);
extern void Func_0200366c(void);
extern void Func_02003d10(void);
extern void Func_02003dec(void);
extern void Func_02003e34(void);
extern void Func_02003eec(void);
extern void Func_02003f94(void);
extern void Func_020040e8(void);
extern void Func_0200413c(void);
extern void Func_02004198(void);
extern void Func_02004218(void);
extern void Func_020048e8(s32 id, s32 arg1, s32 arg2);
extern void Func_02005068(void);
extern void Func_02005684(void);
extern void Func_020056dc(void);
extern void Func_02005780(void);
extern void Func_020057ec(void);
extern void Func_02005ca4(void);
extern void Func_02005d10(void);
extern void Func_02006110(void);

s32 Func_02002ff0(void)
{
    s32 which;
    s32 sharedModeRecord;

    Func_080000c0(1);
    Func_080770c8(324);
    *(s32 *)(Data_03001ebc + 448) = 521;

    switch (Data_02000240[225]) {
    case 1:
    case 2:
    case 11:
        if (Func_080770c0(0x93e) == 0 && Func_080770c0(0x928) != 0) {
            which = 9;
        } else if (Func_080770c0(0x911) != 0) {
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
        if (Func_080770c0(0x93e) == 0 && Func_080770c0(0x911) != 0) {
            which = 13;
        }
        break;
    default:
        break;
    }
    if (which != 0) {
        Func_0808a158(which, 2);
    }
    which = 0;

    switch (Data_02000240[225]) {
    case 1:
    case 2:
        Func_0200351c();
        break;
    case 4:
        Func_0200366c();
        break;
    case 5:
        Func_02003364();
        break;
    case 10:
        if (Func_080770c0(0x928) != 0) {
            Func_02003dec();
        } else {
            Func_02003d10();
        }
        break;
    case 11:
        if (Func_080770c0(0x928) != 0) {
            Func_02003eec();
        } else {
            Func_02003e34();
        }
        break;
    case 12:
        Func_02003f94();
        break;
    case 13:
        Func_020040e8();
        break;
    case 14:
        Func_0200413c();
        break;
    case 15:
        if (Func_080770c0(0x109) != 0) {
            Func_0808a018();
            Func_020048e8(25, 1, 0);
            Func_020048e8(22, 0, 0);
            Func_0808a098(36, (const void *)0x0200e8e4);
            Func_0808a098(37, (const void *)0x0200e8e4);
            Func_0808a098(38, (const void *)0x0200e8e4);
            Func_0808a158(36, 3);
            Func_0808a158(37, 3);
            sharedModeRecord = 38;
            goto finish_mode3;
        } else {
            Func_02004198();
        }
        break;
    case 16:
        Func_02004218();
        break;
    case 17:
        if (Func_080770c0(0x109) != 0) {
            Func_0808a018();
            Func_020048e8(25, 2, 0);
            Func_020048e8(22, 0, 0);
            Func_0808a098(36, (const void *)0x0200e8e4);
            Func_0808a098(37, (const void *)0x0200e8e4);
            Func_0808a020();
        } else {
            Func_02005068();
        }
        break;
    case 18:
        Func_02005684();
        break;
    case 19:
        if (Func_080770c0(0x109) != 0) {
            Func_0808a018();
            Func_020048e8(25, 3, 0);
            Func_020048e8(22, 0, 0);
            Func_0808a098(36, (const void *)0x0200e8e4);
            Func_0808a098(37, (const void *)0x0200e8e4);
            Func_0808a158(36, 3);
            sharedModeRecord = 37;
finish_mode3:
            Func_0808a158(sharedModeRecord, 3);
            Func_0808a020();
        } else {
            Func_020056dc();
        }
        break;
    case 20:
        Func_02005780();
        break;
    case 21:
        if (Func_080770c0(0x109) != 0) {
            if (Func_080770c0(0x302) != 0) {
                *(s32 *)0x0200ff84 = 0;
                Func_080000d0(0x0200dc49, 3200);
                Func_0808a100(9, 5);
            }
        } else {
            Func_020057ec();
        }
        break;
    case 22:
        Func_02005ca4();
        break;
    case 23:
        if (Func_080770c0(0x109) == 0) {
            Func_02005d10();
        }
        break;
    case 24:
        Func_02006110();
        break;
    case 30:
        Func_020048e8(20, 0x926, 0x92b);
        Func_020048e8(21, 0, 0);
        Func_080770c8(0x902);
        Func_0808a248(1);
        break;
    default:
        break;
    }
    return 0;
}
