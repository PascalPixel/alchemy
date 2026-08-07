#include "types.h"

/*
 * resource_3c9 owner at 0x02004bec, 2716 bytes: the second of the
 * "branchy pair" (with 0x02003e9c) and in truth the overlay's second
 * scripted-scene giant -- the queue's "1000+B" estimate was well short.
 * Exactly two conditional branches, one idiom instance around
 * Func_0808a070(0, 0), the same skip-beat gate 0x02003e9c resolved,
 * here with increment 3 per arm instead of 1:
 *
 *   taken = 0;
 *   if (Func_0808a070(0, 0) == 0) { ...one arm...; taken = 1; }
 *   else { skipBeatCounter += 3; ...other arm...; }
 *   if (taken) skipBeatCounter += 3;
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill and an
 * 8-byte frame at 0x02004bec-0x02004bfe through `add sp,#8 / pop {r3,
 * r5, r6, r7} / mov r8,r3 / mov r9,r5 / mov sl,r6 / mov fp,r7 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02005654-0x2005664,
 * trailing pool 0x2005666-0x2005687; immediately after sits the
 * pushless leaf Func_02005688 (declared in resource_3c9_c_020059f0.c /
 * _020048d8.c, still not itself drafted), so this region ends exactly
 * at 0x02005688. Three inline pool clusters reached only by
 * unconditional forward branches. No incoming arguments read, `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/lib/overlay_call_targets.ts resource_3c9 4bec 5688's
 * +2 rule). Called from this overlay's own resource_3c9_c_02003924.c.
 *
 * SHARED IDIOMS, cited: dialogue-by-id Func_0808a170 (0x282e here),
 * the Func_02005688 position-delta blocks (this overlay's 0x48d8 and
 * 0x423c drafts), the id-record pass-through, and the closing
 * additive/subtractive workspace idiom -- HERE in its canonical 513
 * form (displacement 448 = 224<<1, value 513 = 448+65, exactly as
 * first named in resource_377_c_02000a0c.c) plus a second write of 16
 * to workspace+456. The tail then reaches the WINDOW ENGINE global
 * 0x03001e8c (the documented Data_03001e8c text/window state): clears the two u16s at
 * +0x12f4/+0x12f6 and calls the main-image window routine
 * Func_08015210(0x284f, 0, 0) between Func_08009208/Func_08009210 --
 * the only row in this overlay so far that touches it. The compiled
 * form derives 0x03001e8c by `subs #48` from the 0x03001ebc pool
 * word; written here as the two distinct globals they are.
 *
 * Local callees: the pre-owner helper pair Func_02000894/Func_020008a8
 * (as in 0x8b4/0x3e9c, mixed id/packed-constant first argument kept as
 * compiled -- 0x2015 twice through Func_0808a188 here).
 *
 * Uncertainty: callee roles, text/fade ids, and record field roles are
 * recorded by shape only; the five odd 0x0200xxxx pool pointers
 * (0x0200e074/e324/e360/e3c0/e39c through Func_0808a098/0b0 and
 * 0x02008401 into record+0x6c) are recorded as read.
 */

extern u8 *Data_03001ebc;
extern u8 *Data_03001e8c;

extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a0(s32 id);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a0b0(s32 id, s32 arg1);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_08009180(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_08009208(void);
extern void Func_08009210(void);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 arg0);
extern void Func_02000894(s32 arg0);
extern void Func_020008a8(s32 arg0, s32 arg1);
extern void Func_02005688(void *record);

void Func_02004bec(void)
{
    u8 *record;
    u8 *record21;
    u8 *windowState;
    s32 taken;

    Func_0808a170(0x282e);
    Func_0808a1f0(21, 129 << 1);
    Func_0808a010(20);
    Func_0808a188(0x2015, 0, 20);
    Func_0808a138(6, 2);
    Func_0808a188(6, 0, 20);
    Func_0808a100(6, 6);
    Func_0808a010(10);
    Func_0808a130(21, 2);
    Func_0808a188(0x2015, 0, 40);
    Func_0808a100(6, 7);
    Func_0808a188(6, 0, 20);
    Func_080f9010(17);
    Func_0808a138(6, 2);
    Func_0808a098(6, 0x0200e324);
    Func_0808a010(20);
    Func_0808a1f0(21, 129 << 1);
    Func_0808a130(21, 3);
    Func_0808a0a0(6);
    Func_0808a010(160);
    Func_0808a138(21, 1);
    Func_0808a010(20);
    record = Func_0808a080(21);
    *(u16 *)(record + 6) = 160 << 7;
    Func_0808a100(21, 0);
    Func_0808a010(80);
    Func_0808a110(21, 4);
    Func_0808a010(40);
    Func_0808a188(21, 0, 40);
    Func_0808a100(21, 5);
    Func_0808a010(10);
    Func_080091e0(Func_0808a080(21), 0);
    Func_0808a100(21, 0);
    Func_0808a128(21, 6, 0);
    Func_0808a090(21, 192 << 10, 192 << 9);
    record = Func_0808a080(21);
    record[0x5a] &= ~1;

    record21 = Func_0808a080(21);
    *(u32 *)(record21 + 0x6c) = 0x02008401;
    *(u16 *)(record21 + 6) = 128 << 8;
    Func_0808a0c0(21, 184, 237);
    Func_0808a0b0(21, 0x0200e360);
    Func_0808a010(120);
    Func_02000894(0);
    Func_080f9010(72);
    Func_0808a208(128 << 11, 128 << 8);
    Func_0808a210(171 << 17, 128 << 14, 212 << 16, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a138(0, 2);
    Func_0808a010(40);
    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a100(0, 1);
    Func_0808a128(0, 6, 60);
    record = Func_0808a080(0);
    *(u16 *)(record + 0x64) = 10;
    Func_0808a098(0, 0x0200e074);
    Func_0808a1e8(1, 128 << 1, 80);
    Func_0808a188(1, 0, 40);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_02000894(2);
    Func_0808a1e8(3, 129 << 1, 40);
    Func_02000894(3);
    Func_080091e0(Func_0808a080(1), 1);
    Func_0808a100(1, 1);
    Func_0808a128(1, 6, 60);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_080091e0(Func_0808a080(2), 1);
    Func_0808a100(2, 1);
    Func_0808a128(2, 6, 40);
    Func_080091e0(Func_0808a080(3), 1);
    Func_0808a100(3, 1);
    Func_0808a128(3, 6, 60);
    Func_0808a1b8(1, 192 << 7, 60);
    Func_0808a100(1, 4);
    Func_02000894(1);
    Func_0808a1b8(2, 192 << 7, 20);
    Func_0808a100(2, 4);
    Func_02000894(2);
    Func_020008a8(3, 192 << 7);
    Func_0808a100(3, 4);
    Func_0808a188(3, 0, 80);
    Func_020008a8(1, 128 << 7);
    Func_0808a188(1, 0, 40);
    Func_0808a1b8(2, 192 << 8, 20);
    Func_0808a188(2, 0, 40);
    Func_0808a130(1, 1);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a1b8(1, 192 << 7, 0);
    Func_0808a1b8(2, 192 << 7, 20);
    Func_0808a138(3, 1);
    Func_0808a1b8(3, 224 << 8, 20);
    Func_02000894(3);
    Func_0808a1e8(1, 128 << 1, 20);
    Func_020008a8(1, 128 << 7);
    Func_02000894(1);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a0d0(1, 0x14d, 194);
    Func_0808a0d0(1, 0x14d, 206);
    Func_0808a1b8(1, 0, 20);
    Func_0808a138(1, 1);
    Func_0808a188(1, 0, 80);
    Func_0808a0a8(0);
    Func_080000c0(1);
    *(s32 *)(record21 + 0x18) = 128 << 9;
    *(s32 *)(record21 + 0x1c) = 128 << 9;
    Func_0808a010(40);
    Func_0808a1e8(0, 128 << 1, 80);
    Func_020008a8(1, 128 << 6);
    Func_0808a128(1, 4, 0);
    Func_02000894(1);
    Func_020008a8(1, 224 << 8);
    Func_0808a178(1, 0);

    taken = 0;
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a110(1, 3);
        Func_02000894(1);
        Func_0808a138(2, 1);
        Func_02000894(2);
        Func_0808a100(3, 4);
        Func_02000894(3);
        taken = 1;
    } else {
        *(u16 *)(Data_03001ebc + 472) += 3; /* skip-beat counter */
        Func_0808a010(60);
        Func_02000894(1);
        Func_0808a138(2, 1);
        Func_02000894(2);
        Func_0808a100(3, 4);
        Func_02000894(3);
    }
    if (taken != 0) {
        *(u16 *)(Data_03001ebc + 472) += 3;
    }

    Func_020008a8(0, 128 << 7);
    Func_0808a1f0(0, 129 << 1);
    Func_0808a010(80);
    Func_080f9010(17);
    Func_0808a1e8(1, 128 << 1, 20);
    Func_0808a1b8(1, 160 << 8, 20);
    Func_0808a1b8(1, 224 << 8, 40);
    Func_0808a1b8(1, 128 << 8, 20);
    Func_0808a1e8(2, 0x101, 40);
    Func_02000894(2);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(1, 128 << 6, 20);
    Func_0808a100(1, 4);
    Func_02000894(1);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 128 << 8, 40);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 40);
    Func_0808a1f0(3, 129 << 1);
    Func_0808a010(60);
    Func_02000894(3);
    Func_080f9010(141);
    Func_080091f0(192 << 11, 192 << 11, 128 << 9);
    Func_0808a010(40);
    Func_080f9010(145);

    Func_08009180(110, 105, 74, 4, 18, 23);
    Func_08009180(92, 86, 83, 4, 8, 23);
    Func_08009180(75, 28, 75, 4, 8, 23);
    Func_08009180(92, 86, 11, 72, 16, 20);
    Func_08009180(19, 92, 19, 68, 8, 21);

    record = Func_0808a080(0);
    *(s32 *)(record + 0x10) += 0xffe00000;
    *(u16 *)(record + 0x66) = 0;
    Func_02005688(record);
    record = Func_0808a080(1);
    *(s32 *)(record + 8) += 0xfffc0000;
    *(s32 *)(record + 0x10) += 0xffe00000;
    *(u16 *)(record + 0x66) = 0;
    Func_02005688(record);
    record = Func_0808a080(2);
    *(s32 *)(record + 8) += 0xfffc0000;
    *(s32 *)(record + 0x10) += 0xffe00000;
    *(u16 *)(record + 0x66) = 0;
    Func_02005688(record);
    record = Func_0808a080(3);
    *(s32 *)(record + 8) += 0xfffc0000;
    *(s32 *)(record + 0x10) += 0xffee0000;
    *(u16 *)(record + 0x66) = 0;
    Func_02005688(record);

    record = Func_0808a080(23);
    *(s32 *)(record + 0xc) = 224 << 14;
    Func_0808a210(169 << 17, 128 << 14, 180 << 16, 0);
    Func_08009128();
    Func_080000c0(1);

    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 0);
    Func_0808a128(0, 6, 0);
    Func_0808a128(1, 6, 0);
    Func_0808a128(2, 6, 0);
    Func_0808a128(3, 6, 0);
    Func_0808a098(0, 0x0200e3c0);
    Func_0808a098(1, 0x0200e3c0);
    Func_0808a098(2, 0x0200e3c0);
    Func_0808a098(3, 0x0200e3c0);

    Func_080091f0(192 << 10, 192 << 10, 128 << 9);
    Func_0808a010(80);
    Func_080091f0(192 << 11, 192 << 11, 128 << 9);
    Func_0808a010(40);
    Func_080f9010(145);
    Func_080091c0(110, 106, 18, 14, 10, 5);
    Func_08009180(110, 105, 74, 4, 18, 23);
    Func_08009180(92, 86, 11, 68, 16, 20);

    record = Func_0808a080(0);
    *(s32 *)(record + 8) += 0xfff00000;
    Func_02005688(record);
    record = Func_0808a080(1);
    *(s32 *)(record + 8) += 0xfff00000;
    Func_02005688(record);
    record = Func_0808a080(2);
    *(s32 *)(record + 8) += 0xfff00000;
    Func_02005688(record);
    record = Func_0808a080(3);
    *(s32 *)(record + 8) += 0xfff00000;
    Func_02005688(record);
    record = Func_0808a080(8);
    *(s32 *)(record + 8) += 128 << 13;
    Func_02005688(record);
    record = Func_0808a080(9);
    *(s32 *)(record + 8) += 128 << 13;
    Func_02005688(record);
    record = Func_0808a080(10);
    *(s32 *)(record + 8) += 0xfff00000;
    Func_02005688(record);
    record = Func_0808a080(11);
    *(s32 *)(record + 8) += 0xfff00000;
    Func_02005688(record);

    Func_0808a210(161 << 17, 128 << 14, 180 << 16, 0);
    Func_08009128();
    Func_080000c0(1);

    Func_0808a1f0(0, 129 << 1);
    Func_0808a1f0(1, 129 << 1);
    Func_0808a1f0(2, 129 << 1);
    Func_0808a1f0(3, 129 << 1);
    Func_0808a128(0, 6, 0);
    Func_0808a128(1, 6, 0);
    Func_0808a128(2, 6, 0);
    Func_0808a128(3, 6, 0);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(80);
    Func_080f9010(0x121);
    Func_0808a010(40);
    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a0a8(2);
    Func_0808a0a8(3);
    Func_0808a010(120);

    Func_0808a1e8(2, 0x105, 120);
    Func_0808a138(2, 1);
    Func_0808a1b8(2, 160 << 8, 20);
    Func_02000894(2);
    Func_0808a110(1, 3);
    Func_0808a1b8(1, 128 << 6, 0);
    Func_02000894(1);
    Func_0808a1b8(3, 224 << 8, 20);
    Func_0808a100(3, 4);
    Func_02000894(3);
    Func_0808a128(2, 2, 20);
    Func_02000894(2);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_020008a8(1, 0);
    Func_02000894(1);
    Func_0808a1b8(0, 128 << 8, 20);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a090(1, 128 << 9, 128 << 8);
    Func_0808a090(2, 128 << 9, 128 << 8);
    Func_0808a090(3, 128 << 9, 128 << 8);
    Func_0808a098(1, 0x0200e39c);
    Func_0808a098(2, 0x0200e39c);
    Func_0808a098(3, 0x0200e39c);
    Func_0808a010(60);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a0d0(0, 136 << 1, 216);
    Func_0808a0c8(0, 136 << 1, 254);
    Func_0808a010(80);

    /* closing workspace writes: the canonical 513-at-448 idiom, plus 16
     * at +456 */
    *(u32 *)(Data_03001ebc + 448) = 513;
    *(u32 *)(Data_03001ebc + 456) = 16;
    Func_0808a368();
    Func_0808a370();
    Func_0808a010(80);

    Func_08009208();
    windowState = Data_03001e8c;
    *(u16 *)(windowState + 0x12f4) = 0;
    *(u16 *)(windowState + 0x12f6) = 0;
    Func_08015210(0x284f, 0, 0);
    Func_08009210();
    Func_0808a010(80);
}
