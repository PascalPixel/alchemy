#include "types.h"

/*
 * resource_39b owner at 0x020012e0, 1104 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay/overlay_driver.ts resource_39b), so the loader enters
 * here and no bl inside the image reaches it, which is why it sat in
 * no structural inventory row.
 *
 * Head, unconditional: REG_BLDCNT = 0x3f40 and REG_BLDALPHA = 0x1010,
 * Func_02001a3c(21, Data_02001000), raise flag 0x111, write 11 and 57
 * into the loader block at indices 289 and 288, Func_0808a408(0), then
 * flag 0x875 either installs callback 0x02008d99 with 3200 or runs
 * Func_02000e3c.  Finally 516 into the workspace's +448 s32.
 *
 * It then dispatches on the SUB-SELECTOR at Data_02000240[225] through
 * a 31-entry jump table at image offset 0x1368 biased by one, so the
 * live range is 1..31.  Ten distinct arms; the scene id is never read.
 * Two of them fall THROUGH rather than branch (arm 1 into arm 2, arms
 * 18..20 into arm 17), and arms 3..6 and 25 share the Func_0808a020
 * close at 0x0200168e, so the arms are not independent blocks.  Every
 * path converges on the epilogue at 0x02001718, which is why the row is
 * 1104 bytes and not the 1098 a table-blind span reports.
 *
 * - 1: flag 0x872 clear runs Func_0808a248(20), then falls into 2;
 * - 2: records 12, 13, 14 take 0xffff0000 at +24; Func_080000c0(1);
 * - 3..6: install callback 0x02008d99, and on flag 0x875 run a cue,
 *   Func_08009128, Func_080000c0(1) and a panel.  At sub-selector 6
 *   only, and only once (guarded and then set by flag 0x251), the long
 *   cinematic: reset the camera, seat record 0 with +12 = 0x820000,
 *   +72 = 0x8000, +68 = 0, +85 stepped 0 -> 3, then SEVENTEEN calls to
 *   Func_0200013c sweeping a 12-bit angle through Func_08000120 and
 *   Func_08000118 with the results scaled by 3/4 and 1/2, then a
 *   camera/fade sequence, and on flag 0x875 clear a further
 *   Func_08015210(0x1632, 0, 0) sequence;
 * - 7..12: flag 0x875 draws two panels;
 * - 13..16, 21..24, 26..29: nothing;
 * - 17: Func_0808a5e0(170).  18..20 run Func_02001208 first and fall
 *   into it;
 * - 25: record 0 into mode 15, seated, camera reset, 256 into the
 *   workspace's +448, Func_0808a010(120), Func_0808a248(50);
 * - 30: flag 0x109 clear runs Func_02001730, set draws panel
 *   (0, 0, 3, 3, 7, 9);
 * - 31: flag 0x109 clear runs Func_0200196c.
 * Returns 0 on every path.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8/r9/sl spill at
 * 0x020012e0-0x020012e8 with `sub sp, #68` at 0x020012f0, through the
 * unspill and `bx r1` at 0x0200171a-0x02001728, one alignment halfword,
 * and one trailing pool word at 0x0200172c; the next function begins at
 * 0x02001730, and this driver itself calls it (arm 30).  The rest of
 * the literal pool is INSIDE the function at 0x020016a4-0x020016f3,
 * jumped over between the arms at 0x02001694 and 0x020016f4 -- the
 * resource_3a0 trap once more.
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: tools/m2c_guard.ts clears this row of the call-through-
 * register hazard, flags the jump table, and reports full call coverage
 * against this draft.  Every bl was resolved as an ABSOLUTE image
 * offset (stored displacement + 2) and every table word as
 * `word - 0x8000` in the same space.
 *
 * REG_BLDCNT (0x04000050) and REG_BLDALPHA (0x04000052) are public GBA
 * hardware, spelled out by address per this project's convention.
 *
 * Uncertainty: flag ids, record ids and modes are literal; record
 * fields +8, +12, +16, +24, +68, +72, +85 are recorded by offset with
 * roles open.  Loader indices 288 and 289 are written but not read
 * here.  The sweep loop's first component (the Func_08000120 result
 * scaled by 3/4) is computed and stored into a stack slot that is
 * never passed to Func_0200013c; that is what the compiled code does
 * and it is transcribed rather than tidied away.  Func_0200013c's
 * seven-argument shape is read off the call site only.  0x02008d99 is
 * an in-image code pointer (image offset 0xd98 | 1) installed as a
 * callback and never called here -- a data-installed pointer of the
 * kind neither a BL sweep nor the unindexed check can see.
 */

extern s16 Data_02000240[];
extern u8 Data_02001000[];
extern u8 *Data_03001ebc;

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 callback, s32 arg1);
extern s32 Func_08000118(s32 angle);
extern s32 Func_08000120(s32 angle);
extern void Func_08009128(void);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080091f8(void);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a1f0(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a338(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a408(s32 arg0);
extern void Func_0808a5e0(s32 arg0);

extern void Func_0200013c(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                          s32 arg5, s32 *arg6);
extern void Func_02000e3c(void);
extern void Func_02001208(void);
extern void Func_02001730(void);
extern void Func_0200196c(void);
extern void Func_02001a3c(s32 arg0, u8 *arg1);

s32 Func_020012e0(void)
{
    volatile u16 *bldcnt = (volatile u16 *)0x04000050;
    u8 *record;
    s32 sweep[3];
    s32 pair[2];
    s32 index;
    s32 angle;

    bldcnt[0] = 0x3f40;
    Data_02000240[289] = 11;
    Func_02001a3c(21, Data_02001000);
    Func_080770c8(0x111);
    bldcnt[1] = 0x1010;
    Data_02000240[288] = 57;
    Func_0808a408(0);
    if (Func_080770c0(0x875) != 0) {
        Func_080000d0(0x02008d99, 3200);
    } else {
        Func_02000e3c();
    }
    *(s32 *)(Data_03001ebc + 448) = 516;

    switch (Data_02000240[225]) {
        if (Func_080770c0(0x872) == 0) {
            Func_0808a248(20);
        }
    case 1:
        /* falls through into the sub-selector 2 arm */
    case 2:
        *(s32 *)(Func_0808a080(12) + 24) = 0xffff0000;
        *(s32 *)(Func_0808a080(13) + 24) = 0xffff0000;
        break;
        *(s32 *)(Func_0808a080(14) + 24) = 0xffff0000;
        Func_080000c0(1);

    case 7:
    case 8:
        if (Func_080770c0(0x875) != 0) {
            Func_080091c0(84, 5, 10, 7, 20, 5);
            Func_080091c0(101, 5, 12, 7, 37, 5);
        }
    case 12:
    case 11:
    case 9:
    case 10:
        break;

    case 3:
    case 4:
    case 5:
    case 6:
        Func_080000d0(0x02008d99, 3200);
        if (Func_080770c0(0x875) != 0) {
            Func_08009180(37, 98, 10, 97, 5, 3);
            Func_08009128();
            Func_080000c0(1);
            Func_080091c0(70, 32, 13, 7, 6, 32);
        }
        if (Data_02000240[225] != 6) {
            break;
        }
        if (Func_080770c0(0x251) != 0) {
            break;
        }
        Func_080770c8(0x251);
        Func_0808a018();
        Func_0808a210(-1, -1, -1, 0);
        Func_08009128();
        Func_080000c0(1);
        *(s32 *)(Func_0808a080(0) + 12) = 0x820000;
        *(s32 *)(Func_0808a080(0) + 72) = 0x8000;
        *(s32 *)(Func_0808a080(0) + 68) = 0;
        Func_0808a080(0)[85] = 0;
        Func_0808a360();
        Func_0808a370();
        Func_0808a010(30);
        Func_0808a080(0)[85] = 3;
        Func_080f9010(204);
        Func_0808a010(24);

        record = Func_0808a080(0);
        pair[1] = 7;
        for (index = 0; index <= 16; index++) {
            angle = index << 12;
            sweep[0] = Func_08000120(angle);
            sweep[1] = 0;
            sweep[2] = Func_08000118(angle);
            sweep[0] = sweep[0] - sweep[0] / 4;
            sweep[2] = sweep[2] - sweep[2] / 2;
            Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                *(s32 *)(record + 16), sweep[1], sweep[2], 0x00010001, pair);
        }

        Func_080f9010(188);
        Func_0808a1f0(0, 0x101);
        Func_0808a100(0, 22);
        Func_080091f0(0x50000, 0x50000, 0x10000);
        Func_080091f0(-1, -1, 0xe666);
        Func_080091f8();
        Func_0808a1f0(0, 256);
        *(s32 *)(Func_0808a080(0) + 72) = 0x10000;
        *(s32 *)(Func_0808a080(0) + 68) = 0x4000;
        if (Func_080770c0(0x875) == 0) {
            Func_0808a338(0x10000, 0);
            Func_0808a330(0x00010003, 1);
            Func_0808a348(30);
            Func_0808a370();
            Func_0808a100(0, 1);
            Func_0808a010(30);
            Func_08015210(0x1632, 0, 0);
            Func_0808a330(0x10000, 0);
            Func_0808a348(30);
        }
        goto scene_close;

    case 18:
    case 19:
    case 20:
        Func_02001208();
        /* falls through into the sub-selector 17 arm */
    case 17:
        Func_0808a5e0(170);
        break;

    case 25:
        Func_0808a158(0, 15);
        Func_080091e0(Func_0808a080(0), 0);
        Func_0808a018();
        Func_08009128();
        Func_080000c0(1);
        *(s32 *)(Data_03001ebc + 448) = 256;
        Func_0808a360();
        Func_0808a370();
        Func_0808a010(120);
        Func_0808a248(50);

scene_close:
        Func_0808a020();
        break;

    case 30:
        if (Func_080770c0(0x109) == 0) {
            Func_02001730();
        } else {
            Func_080091c0(0, 0, 3, 3, 7, 9);
        }
        break;

    case 31:
        if (Func_080770c0(0x109) == 0) {
            Func_0200196c();
        }
        break;

    default:
        break;
    }
    return 0;
}
