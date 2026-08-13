#include "types.h"

/*
 * resource_39e owner at 0x02003e58, 644 bytes: THE OVERLAY'S ENTRY
 * DRIVER, and the reason this overlay's closure was false -- the
 * function the loader actually calls was owned by nobody while the
 * rows it leads to were drafted.
 *
 * It is the target of this overlay's header veneer (the word at image
 * offset 4; `overlay overlay_driver resource_39e`), so nothing
 * inside the image calls it.
 *
 * Top split is on the s16 scene id at 0x02000240+448:
 *
 * - scene 61 (0x3d), the overlay's own scene: write 521 to the
 *   workspace's +448 s32, then branch on the sub-selector s16 at
 *   0x02000240+450 --
 *     1: flag 0x88f set -> Func_0808a100(8, 6) and done; clear ->
 *        Func_0808a100(8, 5), then run the drafted Func_020027ec only
 *        if 0xf14 is set and 0x893 and 0x109 are both clear;
 *     2 or 4 (shared arm): raise flag 0x12f, then stage record 19
 *        (clear +85, +12 and +60 = 0xc0000, +24 = 0xcccc,
 *        +28 = 0x8000, and 0x8000 into +30 of the record its +80
 *        points at).  Flag 0x895 clear takes the placement path --
 *        with 0x89a set and 0x89b clear it places actors 18 and 16 and
 *        hangs the callback constant 0x02008325 on records
 *        18/13/14/15/16 at +108; 0x895 set instead sets bit 3 of +89
 *        and pops panel (14, 11, 1, 1, 14, 10).  Both meet at the same
 *        tail: Func_02000048(0x1300000, 0x180000, 0xe00000, 223) --
 *        the exact-C owner -- then Func_0808a100 on 10 and 11;
 *     3: raise flag 0x12f; with 0x895 clear run Func_02002ad0 (the
 *        now-exact 5000-byte row in this overlay's former gap), otherwise,
 *        if 0x8b2 is clear, park actors 8 and 9 at the origin.
 *
 * - any other scene: Func_0808a5e0(170), set bit 4 of record 9's +89,
 *   then two independent panel pops -- one when the sub-selector is 3
 *   with 0xf14 set and 0x894 clear, one when 0x892 is set (which also
 *   places and halts actor 9).
 *
 * Complete owner: `push {r5, lr}` at 0x02003e58 with `sub sp, #8`
 * through `movs r0, #0 / add sp, #8 / pop {r5} / pop {r1} / bx r1` at
 * 0x02004092-0x0200409a, then the sixteen-word literal pool
 * 0x0200409c-0x020040db; next owner (resource_39e_c_020040dc.c,
 * already drafted) exactly at 0x020040dc.  Returns a literal 0.
 *
 * The +448 / +450 split in the 0x02000240 global is the same one
 * resource_3a4's driver uses: +448 selects the scene, +450
 * sub-selects within it.  Both are recorded by offset.
 *
 * Uncertainty: the flag ids are literal; the record fields (+12, +24,
 * +28, +60, +85, +89, +108, and +30 of the record at +80) are
 * transcribed by offset with roles open; 0x02008325 is an in-image
 * code pointer (image offset 0x324 | 1) installed as a callback, not
 * called here.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;
extern u8 Value_0000003d;

/*
 * The overlay's BL sites call local relocation veneers, not the logical
 * 0x080xxxxx imports printed by overlay_show.  These aliases are named from
 * the raw Thumb targets in the released overlay.  A suffix distinguishes two
 * signatures that share one relocation address.
 */
extern s32 Func_0200824c();
extern void Func_020082f0();
extern void Func_020082fa();
extern s32 Func_02008268();
extern s32 Func_02008274();
extern s32 Func_02008280();
extern void Func_020066ba();
extern void Func_020082aa();
extern s32 Func_020082a0();
extern u8 *Func_020082f4();
extern s32 Func_020082ce();
extern void Func_02008370();
extern s32 Func_020082e6();
extern void Func_02008388();
extern u8 *Func_02008346();
extern u8 *Func_02008350();
extern u8 *Func_02008358();
extern u8 *Func_02008360();
extern u8 *Func_02008368();
extern u8 *Func_02008372();
extern void Func_0200834c();
extern void *Func_0200400a();
extern void Func_0200841e();
extern void Func_02008426();
extern void Func_020083aa();
extern s32 Func_020083a0();
extern void Func_02006abc();
extern s32 Func_020083b0();
extern void Func_0200844e();
extern void Func_02008458();
extern void Func_02008578();
extern u8 *Func_0200841e_b();
extern s32 Func_020083f4();
extern s32 Func_020083fe();
extern void Func_020083f6();
extern s32 Func_0200841c();
extern void Func_020084be();
extern void Func_02008540();
extern void Func_0200842c();

s32 Func_02003e58(void)
{
    u8 *record;
    if (Data_02000240[224] == (s32)&Value_0000003d) {
        *(s32 *)(Data_03001ebc + 448) = 521;
        if (Data_02000240[225] == 1) {
            if (Func_0200824c(0x88f) != 0) {
                Func_020082f0(8, 6);
            } else {
                Func_020082fa(8, 5);
                if (Func_02008268(0xf14) != 0
                    && Func_02008274(0x893) == 0
                    && Func_02008280(0x109) == 0) {
                    Func_020066ba();
                }
            }
        } else if (Data_02000240[225] == 2 || Data_02000240[225] == 4) {
            Func_020082aa(0x12f);
            if (Func_020082a0(0x895) == 0) {
                record = Func_020082f4(19);
                record[85] = 0;
                *(s32 *)(record + 12) = 0xc0000;
                *(s32 *)(record + 60) = 0xc0000;
                *(s32 *)(record + 24) = 0xcccc;
                *(s32 *)(record + 28) = 0x8000;
                *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
                if (Func_020082ce(0x89a) != 0) {
                    Func_02008370(18, 0xf80000, 0xd00000);
                    if (Func_020082e6(0x89b) == 0) {
                        Func_02008388(16, 0x1000000, 0xf00000);
                        *(s32 *)(Func_02008346(18) + 108) = 0x02008325;
                        *(s32 *)(Func_02008350(13) + 108) = 0x02008325;
                        *(s32 *)(Func_02008358(14) + 108) = 0x02008325;
                        *(s32 *)(Func_02008360(15) + 108) = 0x02008325;
                        *(s32 *)(Func_02008368(16) + 108) = 0x02008325;
                    }
                }
            } else {
                record = Func_02008372(19);
                record[85] = 0;
                *(s32 *)(record + 12) = 0xc0000;
                *(s32 *)(record + 60) = 0xc0000;
                *(s32 *)(record + 24) = 0xcccc;
                *(s32 *)(record + 28) = 0x8000;
                {
                    u8 *flag = record + 89;
                    u8 value = *flag;
                    *flag = value | 8;
                }
                *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
                {
                    s32 stack_arg0 = 14;
                    s32 stack_arg1 = 10;
                    Func_0200834c(14, 11, 1, 1, stack_arg0, stack_arg1);
                }
            }
            Func_0200400a(0x1300000, 0x180000, 0xe00000, 223);
            Func_0200841e(10, 5);
            Func_02008426(11, 5);
        } else if (Data_02000240[225] == 3) {
            Func_020083aa(0x12f);
            if (Func_020083a0(0x895) == 0) {
                Func_02006abc();
            } else if (Func_020083b0(0x8b2) == 0) {
                Func_0200844e(8, 0, 0);
                Func_02008458(9, 0, 0);
            }
        }
        return 0;
    }

    Func_02008578(170);
    Func_0200841e_b(9)[89] |= 16;
    if (Data_02000240[225] == 3
        && Func_020083f4(0xf14) != 0
        && Func_020083fe(0x894) == 0) {
        {
            s32 stack_arg0 = 10;
            s32 stack_arg1 = 24;
            Func_020083f6(10, 84, 1, 1, stack_arg0, stack_arg1);
        }
    }
    if (Func_0200841c(0x892) != 0) {
        {
            s32 actor = 9;
            Func_020084be(actor, 0x980000, 0x1880000);
        }
        Func_02008540(9, 0, 0);
        {
            s32 stack_arg0 = 10;
            s32 stack_arg1 = 22;
            Func_0200842c(10, 26, 1, 1, stack_arg0, stack_arg1);
        }
    }
    return 0;
}
