#include "types.h"

/*
 * resource_39c owner at 0x02003d20, 3304 bytes: THE OVERLAY'S ENTRY
 * DRIVER, and the last of the ninety-six.  Identified from the header
 * veneer at image offset 4 (bun tools/overlay_driver.ts resource_39c),
 * so the loader enters here and no bl inside the image reaches it,
 * which is why it sat in no structural inventory row.
 *
 * Head, unconditional: raise flag 0x111, write 11 and 57 into the
 * loader block at indices 289 and 288, 516 into the workspace's +448
 * s32, REG_BLDCNT = 0x3f40 and REG_BLDALPHA = 0x1010,
 * Func_02004fcc(21, Data_02001000), Func_0808a408(0), then flag 0x875
 * either installs callback 0x02008d59 with 3200 or runs Func_02000dfc.
 * This is resource_39b's head to the halfword apart from the two
 * pointers, and the two overlays are siblings.
 *
 * The body is FOUR CHAINED SCENE BLOCKS, not one dispatcher.  Each is
 * guarded by an equality test on the scene id at Data_02000240[224] --
 * 0x36, 0x37, 0x38, 0x39 in file order -- and the first three then
 * switch on the sub-selector at Data_02000240[225] biased by one,
 * through jump tables at image offsets 0x3db8 (15 entries, 6 arms),
 * 0x3f88 (9 entries, 4 arms) and 0x4224 (16 entries, 8 arms).  The
 * fourth block is an if/else chain over sub-selectors 10, 11, 12 and
 * 15 with no table.  A scene that does not match a block's id branches
 * to the head of the NEXT block, and a sub-selector out of a block's
 * range does the same, so the blocks are chained rather than nested and
 * three arm addresses -- 0x3f56, 0x41f4, 0x466c -- are simultaneously a
 * block's default arm and the next block's entry.  Since the scene id
 * can satisfy only one of the four tests, transcribing the chain as
 * four sequential guarded blocks is faithful.
 *
 * ARMS ARE NOT INDEPENDENT BLOCKS, four times over in this row:
 * scene 0x36 sub-selector 15 (0x3e1e) falls THROUGH into sub-selector
 * 4 (0x3e2a); scene 0x38 sub-selector 3/13 (0x437c) falls through into
 * 1/2/12 (0x4380); scene 0x38 sub-selector 16 (0x447c) falls through
 * into 14 (0x44b6), which is why that arm re-reads Data_02000240[225]
 * twice and gates its own work on it still being 14; and scene 0x39
 * sub-selector 12 (0x47ee) falls through into 11 (0x4800).  Two more
 * pairs share a close: scene 0x38's arms 0x4264 and 0x434a converge on
 * one Func_080091c0 at 0x2004372, and scene 0x39's paths converge at
 * 0x020048bc and again at 0x020049d2.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8 spill at
 * 0x02003d20-0x02003d24 with `sub sp, #8` at 0x02003d28, through the
 * unspill and `bx r1` at 0x020049ea-0x020049f6, one alignment halfword
 * at 0x020049f8 and a four-word trailing pool at 0x020049fa-0x02004a07;
 * the next function begins at 0x02004a08.  Span 3304 bytes.
 * measureSpan reports 3288 because it stops at the first pool boundary,
 * and FOUR literal pools sit INSIDE the function -- 0x0200409c-0x20040ef
 * between the scene-0x37 arms, 0x02004454-0x0200447b between the
 * scene-0x38 arms, 0x02004864-0x020048a3 inside the scene-0x39 chain,
 * and only the last is trailing -- the resource_3a0 trap four times in
 * one row.
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: every bl resolved as an ABSOLUTE image offset (stored
 * displacement + 2) with tools/overlay_call_targets.ts, never read off
 * the listing, and every jump-table word as `word - 0x8000` in the same
 * space, both tables re-dumped from the assembled image rather than
 * carried over from the earlier map.  tools/m2c_guard.ts finds no
 * call-through-register hazard, flags the three `mov pc, rN` dispatches
 * at 0x02003db4, 0x02003f84 and 0x02004222, and reports full call
 * coverage against this draft.
 *
 * REG_BLDCNT (0x04000050) and REG_BLDALPHA (0x04000052) are public GBA
 * hardware, spelled out by address per this project's convention.
 *
 * Uncertainty: flag ids, record ids and modes are literal.  Record
 * fields +12, +16, +24, +34, +60, +68, +72, +85 and +89 are recorded by
 * offset with roles open; +12 is written both absolutely and by
 * accumulation and is copied into +60 in three places, which reads like
 * a saved previous value but is not proven here.  Func_0808a228 is
 * called for effect AND for its return value, which is used as a record
 * pointer whose byte at +85 is then cleared -- the shape is read off
 * the one call site only.  Loader indices 288 and 289 are written but
 * not read here.  The scene-0x38 sub-selector 4/5/6 loop walks four
 * record ids 15..18 against flag pairs 0x330/0x331 upward while
 * stepping one Func_0808a0f0 coordinate by 0x00400000 per iteration;
 * the coordinates are transcribed as the shifted literals the reference
 * materialises rather than converted to any assumed fixed-point unit.
 * 0x02008d59, 0x0200a2c5, 0x0200a649 and 0x0200adcd are in-image code
 * pointers (image offsets 0xd58, 0x22c4, 0x2648 and 0x2dcc with the
 * Thumb bit) installed as callbacks and never called here -- exactly
 * the data-installed pointers neither a BL sweep nor the unindexed
 * check can see.
 */

extern s16 Data_02000240[];
extern u8 Data_02001000[];
extern u8 *Data_03001ebc;

extern void Func_080000c0(s32 arg0);
extern void Func_080000d0(s32 callback, s32 arg1);
extern void Func_08009128(void);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_08009198(void);
extern void Func_080091a0(void);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(u8 *record, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080091f8(void);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a158(s32 arg0, s32 arg1);
extern void Func_0808a1b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 *Func_0808a228(void);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a338(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a408(s32 arg0);
extern void Func_0808a5e0(s32 arg0);
extern void Func_080f9010(s32 arg0);

extern void Func_02000030(u8 *record, s32 arg1);
extern void Func_02000ba4(s32 id);
extern void Func_02000dfc(void);
extern void Func_020017a8(s32 arg0);
extern void Func_02001db4(s32 arg0);
extern void Func_02002030(s32 arg0);
extern void Func_02003788(void);
extern void Func_02004c68(void);
extern void Func_02004fcc(s32 arg0, u8 *arg1);

s32 Func_02003d20(void)
{
    volatile u16 *bldcnt = (volatile u16 *)0x04000050;
    u8 *record;
    u32 index;
    s32 flagId;
    s32 coordinate;
    s32 cleared;
    s32 sharedMessageArg0;
    s32 sharedMessageArg1;
    s32 sharedMessageArg4;
    s32 sharedMessageArg5;

    Func_080770c8(0x111);
    Data_02000240[288] = 57;
    Data_02000240[289] = 11;
    bldcnt[0] = 0x3f40;
    *(s32 *)(Data_03001ebc + 448) = 516;
    Func_02004fcc(21, Data_02001000);
    Func_0808a408(0);
    bldcnt[1] = 0x1010;
    if (Func_080770c0(0x875) != 0) {
        Func_080000d0(0x02008d59, 3200);
    } else {
        Func_02000dfc();
    }

    /* ---- scene 0x36, table at image offset 0x3db8, 15 entries ---- */
    if (Data_02000240[224] == 0x36) {
        switch (Data_02000240[225]) {
        case 1:
        case 2:
            break;
        case 3:
            if (Func_080770c0(0x875) != 0) {
                Func_080091c0(66, 5, 27, 23, 2, 5);
            }

            break;
        case 5:
        case 6:
            Func_0808a5e0(170);

        case 15:
            Func_02000030(Func_0808a080(0), 1);
            /* falls through into the sub-selector 4 arm */
        case 4:
            if (Func_080770c0(0x876) != 0) {
                Func_080000c0(1);
                Func_0808a0f0(9, 0x02a00000, 0x00800000);
                Func_0808a0f0(10, 0x02c00000, 0x00c00000);
                Func_0808a0f0(11, 0x02880000, 0x00f00000);
            } else if (Func_080770c0(0x109) == 0) {
                Func_080770d0(0x310);
                Func_080770d0(0x311);
                Func_080770d0(0x312);
                Func_080770d0(0x313);
            }
            Func_02000030(Func_0808a080(9), 1);
            Func_02000030(Func_0808a080(10), 1);
            Func_02000030(Func_0808a080(11), 1);
            Func_02000ba4(9);
            Func_02000ba4(10);
            Func_02000ba4(11);
            Func_02000030(Func_0808a080(12), 1);
            if (Func_080770c0(0x310) == 0) {
                break;
            }
            Func_08009180(119, 9, 109, 11, 1, 1);
            if (Func_080770c0(0x311) != 0) {
                Func_08009180(118, 9, 104, 13, 1, 1);
            }
            Func_08009128();
            Func_080000c0(1);
            break;

        case 9:
        case 10:
            if (Func_080770c0(0x873) == 0) {
                Func_0808a0f0(3, 0x02b80000, 0x02780000);
                Func_0808a1b8(3, 0, 0);
            } else {
                Func_0808a0f0(8, 0x03080000, 0x02780000);
                Func_080091c0(110, 39, 5, 1, 46, 39);
            }
            break;

        default:
            break;
        }
    }

    /* ---- scene 0x37, table at image offset 0x3f88, 9 entries ---- */
    if (Data_02000240[224] == 0x37) {
        switch (Data_02000240[225]) {
        case 1:
        case 2:
            Func_02000030(Func_0808a080(8), 1);
            Func_02000030(Func_0808a080(9), 1);
            if (Func_080770c0(0x302) == 0) {
                break;
            }
            Func_080000c0(1);
            Func_080f9010(211);
            Func_0808a0f0(8, 0x00b80000, 0x02100000);
            Func_080091c0(11, 31, 1, 4, 9, 31);
            Func_080091c0(7, 30, 1, 4, 11, 31);
            Func_08009180(74, 58, 70, 32, 1, 2);
            Func_08009180(74, 59, 70, 34, 1, 1);
            Func_08009180(76, 60, 74, 38, 3, 1);
            Func_08009180(77, 60, 76, 38, 2, 1);
            Func_08009180(75, 58, 86, 41, 1, 3);
            Func_08009180(75, 59, 86, 43, 1, 2);
            Func_08009180(76, 59, 80, 49, 2, 1);
            Func_08009180(77, 59, 82, 49, 2, 1);
            break;

        case 3:
        case 4:
            Func_08009128();
            Func_080000c0(1);
            if (Func_080770c0(0x109) != 0 && Func_080770c0(0x256) != 0) {
                Func_08009180(5, 2, 5, 11, 1, 1);
                Func_08009180(9, 1, 9, 7, 1, 2);
            }
            if (Func_080770c0(0x874) == 0) {
                break;
            }
            Func_0808a0f0(11, 0x00580000, 0x00d80000);
            *(s32 *)(Func_0808a080(11) + 12) += 0xfffe0000;
            record = Func_0808a080(11);
            *(s32 *)(record + 60) = *(s32 *)(Func_0808a080(11) + 12);
            Func_08009180(9, 1, 9, 7, 1, 2);
            Func_08009180(5, 2, 5, 11, 1, 1);
            Func_080091c0(9, 5, 1, 1, 9, 10);
            break;

        case 7:
        case 8:
        case 9:
            Func_02000030(Func_0808a080(10), 1);
            if (Func_080770c0(0x306) == 0) {
                break;
            }
            Func_020017a8(0);
            Func_080091c0(42, 41, 4, 1, 42, 39);
            Func_080091c0(42, 40, 4, 1, 42, 41);
            Func_0808a0f0(10, 0x02c00000, 0x02800000);
            break;

        default:
            break;
        }
    }

    /* ---- scene 0x38, table at image offset 0x4224, 16 entries ---- */
    if (Data_02000240[224] == 0x38) {
        switch (Data_02000240[225]) {
        case 4:
        case 5:
        case 6:
            Func_02000030(Func_0808a080(15), 1);
            Func_02000030(Func_0808a080(16), 1);
            Func_02000030(Func_0808a080(17), 1);
            Func_02000030(Func_0808a080(18), 1);
            Func_02000030(Func_0808a080(19), 1);
            Func_02000030(Func_0808a080(0), 1);
            coordinate = 0x02780000;
            flagId = 0x330;
            for (index = 0; index <= 3; index++) {
                if (Func_080770c0(flagId) != 0) {
                    Func_0808a0f0(index + 15, coordinate, 0x00580000);
                } else if (Func_080770c0(flagId + 1) != 0) {
                    Func_0808a0f0(index + 15, coordinate + 0x00200000, 0x00580000);
                }
                coordinate += 0x00400000;
                flagId += 2;
            }
            sharedMessageArg0 = 53;
            sharedMessageArg1 = 10;
            sharedMessageArg4 = 58;
            sharedMessageArg5 = 7;
            if (Func_080770c0(0x338) != 0) {
                Func_0808a0f0(19, 0x03980000, 0x00580000);
                goto scene38_shared_message;
            } else if (Func_080770c0(0x339) != 0) {
                Func_0808a0f0(19, 0x03b80000, 0x00580000);
                goto scene38_shared_message;
            }
            break;

        case 10:
        case 11:
            if (Func_080770c0(0x348) == 0) {
                break;
            }
            Func_0808a0f0(20, 0x01c80000, 0x01480000);
            sharedMessageArg0 = 29;
            sharedMessageArg1 = 20;
            sharedMessageArg4 = 31;
            sharedMessageArg5 = 20;
scene38_shared_message:
            Func_080091c0(
                sharedMessageArg0,
                sharedMessageArg1,
                1,
                1,
                sharedMessageArg4,
                sharedMessageArg5);
            break;

        case 3:
        case 13:
            Func_02003788();
            /* falls through into the sub-selector 1/2/12 arm */
        case 1:
        case 2:
        case 12:
            Func_0808a5e0(170);
            break;

        case 7:
        case 8:
        case 9:
            if (Func_080770c0(0x109) != 0 && Func_080770c0(0x256) != 0) {
                *(s32 *)(Func_0808a080(0) + 12) = 0xfffe0000;
                record = Func_0808a080(0);
                *(s32 *)(record + 60) = *(s32 *)(Func_0808a080(0) + 12);
                Func_08009180(6, 29, 10, 23, 1, 1);
                Func_08009180(10, 28, 10, 18, 1, 2);
            }
            if (Func_080770c0(0x878) == 0) {
                break;
            }
            Func_0808a0f0(8, 0x00a80000, 0x01780000);
            *(s32 *)(Func_0808a080(8) + 12) += 0xfffe0000;
            record = Func_0808a080(8);
            *(s32 *)(record + 60) = *(s32 *)(Func_0808a080(8) + 12);
            Func_08009180(6, 29, 10, 23, 1, 1);
            Func_08009180(10, 28, 10, 18, 1, 2);
            Func_080091c0(10, 16, 1, 1, 10, 19);
            Func_08009128();
            break;

        case 16:
            Func_080000c0(1);
            Func_0808a0f0(10, 0x03300000, 0x02600000);
            Func_0808a0f0(11, 0x03080000, 0x02400000);
            Func_02000030(Func_0808a080(0), 1);
            Func_02002030(0);
            Func_02001db4(1);
            /* falls through into the sub-selector 14 arm */
        case 14:
            if (Data_02000240[225] == 14) {
                Func_080f9010(211);
            }
            Func_02000030(Func_0808a080(9), 1);
            Func_0808a1e0(10, 2);
            Func_0808a080(10)[34] = 2;
            Func_02000030(Func_0808a080(11), 1);
            Func_02000030(Func_0808a080(12), 1);
            Func_02000030(Func_0808a080(13), 1);
            Func_02000030(Func_0808a080(14), 1);
            Func_02000ba4(10);
            Func_02000ba4(11);
            if (Data_02000240[225] != 14) {
                break;
            }
            if (Func_080770c0(0x109) == 0) {
                Func_080770d0(0x318);
                Func_080770d0(0x319);
                Func_080770d0(0x31a);
                Func_080770d0(0x31b);
                break;
            }
            Func_02002030(0);
            if (Func_080770c0(0x319) != 0) {
                Func_02001db4(2);
                if ((*(s32 *)(Func_0808a080(9) + 16) >> 20) == 44) {
                    Func_080000d0(0x0200a2c5, 3200);
                }
                break;
            }
            if (Func_080770c0(0x31a) != 0) {
                Func_02001db4(1);
                break;
            }
            if (Func_080770c0(0x31b) != 0) {
                break;
            }
            Func_02001db4(0);
            break;

        case 15:
            Func_0808a018();
            Func_02000030(Func_0808a080(8), 1);
            Func_0808a158(0, 15);
            Func_080091e0(Func_0808a080(0), 0);
            *(s32 *)(Func_0808a080(8) + 12) = 0x00900000;
            Func_0808a080(8)[85] = 0;
            *(s32 *)(Func_0808a080(8) + 68) = 0;
            *(s32 *)(Func_0808a080(8) + 72) = 0x4ccc;
            *(s32 *)(Data_03001ebc + 448) = 256;
            Func_0808a360();
            Func_0808a370();
            Func_0808a080(8)[85] = 3;
            Func_080f9010(189);
            Func_0808a010(32);
            Func_080f9010(188);
            Func_02000030(Func_0808a080(8), 2);
            Func_080091f0(0x00030000, 0x00030000, 0x00010000);
            Func_080091f0(-1, -1, 0xe666);
            Func_080091f8();
            Func_0808a010(60);
            Func_0808a248(16);
            Func_0808a020();
            break;

        default:
            break;
        }
    }

    /* ---- scene 0x39, an if/else chain, no table ---- */
    if (Data_02000240[224] == 0x39) {
        switch (Data_02000240[225]) {
        case 10:
            Func_080000d0(0x0200adcd, 3200);
            if (Func_080770c0(0x109) != 0) {
                break;
            }
            Func_02003788();
            Func_0808a5e0(170);
            Func_0808a338(0x00010000, 0);
            Func_0808a330(0x00010003, 1);
            Func_0808a348(30);
            Func_0808a370();
            Func_0808a100(0, 1);
            Func_0808a010(30);
            Func_08015210(0x1633, 0, 0);
            Func_0808a330(0x00010000, 0);
            Func_0808a348(30);
            break;

        case 15:
            Func_0808a0f0(3, 0, 0);
            Func_080091a0();
            bldcnt[0] = 0;
            *(s32 *)(Func_0808a080(9) + 24) = 0xffff0000;
            Func_0808a1e0(14, 1);
            Func_0808a1e0(15, 1);
            Func_0808a1e0(16, 1);
            cleared = Func_080770c0(0x109);
            if (cleared != 0) {
                Func_080091c0(104, 34, 5, 4, 40, 34);
                Func_08009180(45, 91, 40, 91, 5, 4);
                break;
            }
            Func_0808a0f0(10, 0x03380000, 0x02580000);
            Func_080091c0(116, 36, 3, 4, 52, 36);
            Func_0808a080(10)[85] = cleared;
            Func_02000030(Func_0808a080(10), 1);
            Func_0808a018();
            Func_0808a210(-1, -1, -1, 0);
            Func_0808a228()[85] = cleared;
            Func_0808a1e0(0, 1);
            Func_0808a1e0(13, 1);
            Func_02004c68();
            Func_08009198();
            Func_0808a020();
            break;

        case 12:
            Func_0808a010(1);
            Func_02000030(Func_0808a080(0), 1);
            /* falls through into the sub-selector 11 arm */
        case 11:
            Func_080091c0(104, 34, 5, 4, 40, 34);
            Func_08009180(45, 91, 40, 91, 5, 4);
            if (Func_080770c0(0x881) == 0) {
                Func_08009180(30, 45, 50, 45, 5, 6);
                Func_08009180(50, 105, 50, 109, 5, 3);
                Func_08009128();
                Func_080000c0(1);
            } else {
                Func_0808a1e0(14, 1);
                Func_0808a1e0(15, 1);
                Func_0808a1e0(16, 1);
            }
            *(s32 *)(Func_0808a080(9) + 24) = 0xffff0000;
            if (Func_080770c0(0x82b) == 0) {
                Func_0808a0f0(3, 0, 0);
                Func_0808a0f0(10, 0x03380000, 0x02580000);
                Func_0808a1e0(10, 1);
                Func_080091c0(116, 37, 3, 3, 52, 37);
                Func_08009180(126, 35, 116, 35, 1, 2);
                Func_080000d0(0x0200a649, 3200);
                break;
            }
            if (Func_080770c0(0x871) == 0) {
                if (Func_080770c0(0x870) == 0) {
                    Func_0808a1b8(3, 0, 0);
                    Func_0808a100(3, 16);
                } else {
                    Func_0808a0f0(3, 0x03480000, 0x02780000);
                    Func_0808a1b8(3, 0xc000, 0);
                }
                Func_02000030(Func_0808a080(3), 1);
                Func_08009180(126, 35, 116, 35, 1, 2);
                Func_080000d0(0x0200a649, 3200);
            } else {
                Func_0808a0f0(3, 0, 0);
                Func_0808a0f0(10, 0x03380000, 0x02580000);
                Func_080091c0(116, 36, 3, 4, 52, 36);
                Func_0808a080(10)[89] = 254;
                Func_0808a010(1);
            }
            Func_0808a080(10)[85] = 0;
            Func_02000030(Func_0808a080(10), 1);
            break;

        default:
            break;
        }
    }
    return 0;
}
