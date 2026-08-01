typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_383 owner at 0x02002758, 1096 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_383), so the loader enters
 * here and no bl inside the image reaches it, which is why it sat in
 * no structural inventory row.
 *
 * Writes 521 to the workspace's +448 s32, then dispatches on the
 * SUB-SELECTOR at Data_02000240[225] through a 13-entry jump table at
 * image offset 0x2784 biased by five, so the live range is 5..17 and
 * everything else returns 0 at once.  The scene id is never read.
 * Nine distinct arms: 6, 8 and 9 are empty, 7 and 11 share one arm,
 * and 13 and 14 share another.  Every arm converges on the epilogue at
 * 0x02002b92, which is why the row is 1096 bytes and not the 1092 a
 * table-blind span reports.
 *
 * The arms are scripted scene steps gated on story flags:
 * - 5: clear record 8's +85, +12 and +20;
 * - 7, 11: flag 0x855 places record 18, mode 1, halts it, runs
 *   Func_02004698(231, 0x2380000, 0x100000, 0x2a00000) and installs
 *   callback 0x0200a71d with 3200; otherwise flag 0x853 places record
 *   18 at the origin;
 * - 10: flag 0x850 places and halts record 2, flag 0x856 returns it to
 *   the origin, and flag 0x855 places record 16, mode 1, and halts it;
 * - 12: flags 0x109, 0x852 set, 0x853 clear and 0x300 set draw panel
 *   (14, 45, 3, 1, 14, 44) and install callback 0x0200aba1; otherwise
 *   flag 0x856 places and halts record 25, and flag 0x852 draws the
 *   same panel and then either draws (14, 50, 3, 1, 14, 44) when flag
 *   0x853 is clear or sets records 24 and 25 to +100 = 5 and 4 and
 *   installs 0x0200aba1;
 * - 13, 14: five Func_08009180 cues, set bit 2 of record 26's +89,
 *   seat it, and on flag 0x859 place it and draw panel
 *   (101, 24, 3, 4, 41, 24);
 * - 15: two cues, Func_0808a018, then flag 0x855 puts records 15, 16
 *   and 17 in mode 2; otherwise place record 8 and either (flag 0x854)
 *   run Func_02002db4 and close, or run the mode-7 branch through
 *   Func_020045cc, Func_080f9010(17), text 0x12c3 and Func_020045f4;
 * - 16: two cues, place record 8, Func_02002bf0, Func_0808a248(16);
 * - 17: two cues, then flag 0x109 chooses between placing record 8
 *   with Func_02002fd4 and Func_020036f8, and the same three-record
 *   mode-2 close as arm 15.
 * Returns 0 on every path.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002758 with `sub sp, #8`
 * at 0x02002774, through the epilogue at 0x02002b92-0x02002b9a and one
 * trailing pool word at 0x02002b9c; the next function begins at
 * 0x02002ba0.  The rest of the literal pool is INSIDE the function, at
 * 0x02002af4-0x02002b2b, jumped over between the arms at 0x02002ab2
 * and 0x02002b2c -- the resource_3a0 trap once more.
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: tools/m2c_guard.ts clears this row of the call-through-
 * register hazard, flags the jump table, and reports full call
 * coverage against this draft.  Every bl was resolved as an ABSOLUTE
 * image offset (stored displacement + 2) and every table word as
 * `word - 0x8000` in the same space.
 *
 * Note on the compiled form, kept out of the C: arms 7/11 and 12 share
 * the callback-installer tail at 0x020028de, which supplies the common
 * `3200`; each arm loads its own pointer and branches there.  The draft
 * writes both installs out because that is what each path performs.
 *
 * Uncertainty: flag ids, record ids and modes are literal; record
 * fields +12, +20, +85, +89, +100 are recorded by offset with roles
 * open, as are the argument positions of the Func_080091xx cue family
 * beyond "six arguments, last two on the stack".  0x0200a71d and
 * 0x0200aba1 are in-image code pointers (image offsets 0x271c | 1 and
 * 0x2ba0 | 1) installed as callbacks and never called here -- and
 * 0x0200aba1 is the very function this driver's span ends at, which is
 * a useful reminder that the next row after a driver is often the
 * thing the driver installed rather than the thing it calls.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern s32 Func_080770c0(s32 flagId);
extern void Func_080000d0(s32 callback, s32 arg1);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0f0(s32 id, s32 x, s32 z);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a248(s32 arg0);

extern void Func_02002bf0();
extern void Func_02002db4();
extern void Func_02002fd4();
extern void Func_020036f8();
extern void Func_020045cc();
extern void Func_020045f4(s32 arg0, s32 arg1);
extern void Func_02004698(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 Func_02002758(void)
{
    *(s32 *)(Data_03001ebc + 448) = 521;

    switch (Data_02000240[225]) {
    case 5:
        Func_0808a080(8)[85] = 0;
        *(s32 *)(Func_0808a080(8) + 12) = 0;
        *(s32 *)(Func_0808a080(8) + 20) = 0;
        break;

    case 7:
    case 11:
        if (Func_080770c0(0x855) != 0) {
            Func_0808a0f0(18, 0x2380000, 0x2880000);
            Func_0808a098(18, 1);
            Func_0808a1b8(18, 0x4000, 0);
            Func_02004698(231, 0x2380000, 0x100000, 0x2a00000);
            Func_080000d0(0x0200a71d, 3200);
        } else if (Func_080770c0(0x853) != 0) {
            Func_0808a0f0(18, 0, 0);
        }
        break;

    case 10:
        if (Func_080770c0(0x850) != 0) {
            Func_0808a0f0(2, 0x1780000, 0x1780000);
            Func_0808a1b8(2, 0x4000, 0);
        }
        if (Func_080770c0(0x856) != 0) {
            Func_0808a0f0(2, 0, 0);
        }
        if (Func_080770c0(0x855) != 0) {
            Func_0808a0f0(16, 0x2180000, 0x1d00000);
            Func_0808a098(16, 1);
            Func_0808a1b8(16, 0x5000, 0);
        }
        break;

    case 12:
        if (Func_080770c0(0x109) != 0 && Func_080770c0(0x852) != 0 &&
            Func_080770c0(0x853) == 0 && Func_080770c0(0x300) != 0) {
            Func_080091c0(14, 45, 3, 1, 14, 44);
            Func_080000d0(0x0200aba1, 3200);
            break;
        }
        if (Func_080770c0(0x856) != 0) {
            Func_0808a0f0(25, 0x780000, 0x2b80000);
            Func_0808a1b8(25, 0x8000, 0);
        }
        if (Func_080770c0(0x852) == 0) {
            break;
        }
        Func_080091c0(14, 45, 3, 1, 14, 44);
        if (Func_080770c0(0x853) == 0) {
            Func_080091c0(14, 50, 3, 1, 14, 44);
            break;
        }
        *(u16 *)(Func_0808a080(24) + 100) = 5;
        *(u16 *)(Func_0808a080(25) + 100) = 4;
        Func_080000d0(0x0200aba1, 3200);
        break;

    case 13:
    case 14:
        Func_0808a010(2);
        Func_08009180(54, 2, 35, 20, 2, 10);
        Func_08009180(54, 2, 95, 20, 2, 10);
        Func_08009180(54, 2, 35, 80, 2, 10);
        Func_08009180(54, 2, 46, 21, 4, 8);
        Func_08009180(54, 2, 46, 81, 4, 8);
        Func_0808a080(26)[89] |= 4;
        Func_080091e0(Func_0808a080(26), 0);
        if (Func_080770c0(0x859) != 0) {
            Func_0808a0f0(26, 0x2a40000, 0x019b0000);
            Func_080091c0(101, 24, 3, 4, 41, 24);
        }
        break;

    case 15:
        Func_0808a010(2);
        Func_08009180(54, 2, 44, 21, 2, 8);
        Func_08009180(54, 2, 44, 81, 2, 8);
        Func_0808a018();
        if (Func_080770c0(0x855) != 0) {
            Func_0808a100(15, 2);
            Func_0808a100(16, 2);
            Func_0808a010(1);
            Func_0808a100(17, 2);
            break;
        }
        Func_0808a0f0(8, 0x3380000, 0x1c80000);
        if (Func_080770c0(0x854) != 0) {
            Func_02002db4();
            Func_0808a020();
            break;
        }
        Func_0808a100(8, 7);
        Func_020045cc();
        Func_080f9010(17);
        Func_0808a170(0x12c3);
        Func_020045f4(8, 10);
        Func_0808a020();
        break;

    case 16:
        Func_080091b8(54, 2, 2, 8, 44, 21);
        Func_080091b8(54, 2, 2, 8, 44, 81);
        Func_0808a0f0(8, 0x3380000, 0x1c80000);
        Func_02002bf0();
        Func_0808a248(16);
        break;

    case 17:
        Func_080091b8(54, 2, 2, 8, 44, 21);
        Func_080091b8(54, 2, 2, 8, 44, 81);
        if (Func_080770c0(0x109) == 0) {
            Func_0808a0f0(8, 0x3380000, 0x1c80000);
            Func_02002fd4();
            Func_020036f8();
        } else {
            Func_0808a100(15, 2);
            Func_0808a100(16, 2);
            Func_0808a010(1);
            Func_0808a100(17, 2);
        }
        break;

    default:
        break;
    }
    return 0;
}
