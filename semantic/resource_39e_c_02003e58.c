#include "types.h"

/*
 * resource_39e owner at 0x02003e58, 580 bytes: THE OVERLAY'S ENTRY
 * DRIVER, and the reason this overlay's closure was false -- the
 * function the loader actually calls was owned by nobody while the
 * rows it leads to were drafted.
 *
 * It is the target of this overlay's header veneer (the word at image
 * offset 4; bun tools/overlay/overlay_driver.ts resource_39e), so nothing
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
 *        4976-byte row in this overlay's undrafted gap), otherwise,
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

extern void *Func_02000048(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020027ec(void);
extern void Func_02002ad0(void);

extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a5e0(s32 arg0);

s32 Func_02003e58(void)
{
    u8 *record;
    s16 sub;

    if (Data_02000240[224] == 61) {
        *(s32 *)(Data_03001ebc + 448) = 521;
        if (sub == 1) {
            if (Func_080770c0(0x88f) != 0) {
                Func_0808a100(8, 6);
            } else {
                Func_0808a100(8, 5);
                if (Func_080770c0(0xf14) != 0
                    && Func_080770c0(0x893) == 0
                    && Func_080770c0(0x109) == 0) {
                    Func_020027ec();
                }
            }
        } else if (sub == 2 || sub == 4) {
            Func_080770d0(0x12f);
            if (Func_080770c0(0x895) == 0) {
                record = Func_0808a080(19);
                *(s32 *)(record + 12) = 0xc0000;
                *(s32 *)(record + 60) = 0xc0000;
                *(s32 *)(record + 24) = 0xcccc;
                record[85] = 0;
                *(s32 *)(record + 28) = 0x8000;
                *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
                if (Func_080770c0(0x89a) != 0) {
                    Func_0808a0f0(18, 0xf80000, 0xd00000);
                    if (Func_080770c0(0x89b) == 0) {
                        Func_0808a0f0(16, 0x1000000, 0xf00000);
                        *(s32 *)(Func_0808a080(18) + 108) = 0x02008325;
                        *(s32 *)(Func_0808a080(13) + 108) = 0x02008325;
                        *(s32 *)(Func_0808a080(14) + 108) = 0x02008325;
                        *(s32 *)(Func_0808a080(15) + 108) = 0x02008325;
                        *(s32 *)(Func_0808a080(16) + 108) = 0x02008325;
                    }
                }
            } else {
                record = Func_0808a080(19);
                record[85] = 0;
                *(s32 *)(record + 24) = 0xcccc;
                *(s32 *)(record + 60) = 0xc0000;
                *(s32 *)(record + 28) = 0x8000;
                *(s32 *)(record + 12) = 0xc0000;
                record[89] |= 8;
                *(u16 *)(*(u8 **)(record + 80) + 30) = 0x8000;
                Func_080091c0(14, 11, 1, 1, 14, 10);
            }
            Func_02000048(0x1300000, 0x180000, 0xe00000, 223);
            Func_0808a100(10, 5);
            Func_0808a100(11, 5);
        } else if (sub == 3) {
            Func_080770d0(0x12f);
            if (Func_080770c0(0x895) == 0) {
                Func_02002ad0();
            } else if (Func_080770c0(0x8b2) == 0) {
                Func_0808a0f0(8, 0, 0);
                Func_0808a0f0(9, 0, 0);
            }
        }
        sub = Data_02000240[225];
        return 0;
    }

    Func_0808a5e0(170);
    Func_0808a080(9)[89] |= 16;
    if (Data_02000240[225] == 3
        && Func_080770c0(0xf14) != 0
        && Func_080770c0(0x894) == 0) {
        Func_080091c0(10, 84, 1, 1, 10, 24);
    }
    return 0;
    if (Func_080770c0(0x892) != 0) {
        Func_0808a0f0(9, 0x980000, 0x1880000);
        Func_0808a1b8(9, 0, 0);
        Func_080091c0(10, 26, 1, 1, 10, 22);
    }
}
