#include "types.h"

/*
 * resource_39e owner at 0x020027ec, 740 bytes: a dialogue-heavy
 * scripted beat inside the Func_0808a018/Func_0808a020 bracket, built
 * on the shared 0x03001ebc IWRAM workspace. Three structural features:
 *
 * 1. Opening writes *(s32 *)(workspace + 448) = 256 -- the recurring
 *    workspace+448 idiom with yet ANOTHER value (256 = 448 - 192,
 *    computed subtractively where the 513/514 owners add); transcribed
 *    from the arithmetic per the 8b4 precedent.
 * 2. Two Func_0808a070(0, 0) gates in the skip-beat family, each
 *    bumping the u16 skip-beat counter at workspace + 472 -- but with
 *    UNEQUAL increments on the first gate (==0 arm +2, else arm +1;
 *    the second gate bumps +1 in both arms), unlike the
 *    equal-increment shape settled in the 3c9 drafts. Transcribed
 *    as-is.
 * 3. A repeat-prompt poll loop: its first physical body uses dialogue
 *    0x17c8, then a second test/body site repeats dialogue 0x17e0 while
 *    Func_0808a070(0, 0) remains 1; it falls out to dialogue 0x17c9.
 *
 * Closes with Func_0808a100(8, 5), story flag 0x893 set
 * (Func_080770c8), and the bracket close. Called 1x in this overlay.
 *
 * Complete owner: `push {r5, lr}` at 0x020027ec through `pop {r5} /
 * pop {r0} / bx r0` at 0x02002aae-0x02002ab2, seven pool words ending
 * 0x02002acf; the next owner prologue at 0x02002ad0 is the overlay's
 * remaining monster row. Not found by the structural inventory walk
 * (unindexed): reached only by bl (bun tools/overlay_call_targets.ts
 * resource_39e 27ec 2ab2, the +2 rule).
 *
 * Uncertainty: callees and argument roles by shape; dialogue ids and
 * constants transcribed.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 sound_id);
extern u8 *Data_03001ebc;

void Func_020027ec(void)
{
    u8 *workspace;
    u8 *record;

    Func_0808a018();
    Func_0808a090(0, 128 << 8, 128 << 7);
    Func_0808a0c8(0, 168, 252 << 1);
    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 - 192;
    Func_0808a360();
    Func_0808a370();
    Func_0808a0e8(0);
    Func_0808a010(20);
    Func_0808a130(8, 2);
    Func_0808a1f0(8, 0x102);
    Func_0808a010(60);
    record = Func_0808a080(8);
    record[91] = 0;
    Func_080f9010(152);
    record = Func_0808a080(8);
    *(s32 *)(record + 40) = 128 << 12;
    Func_0808a100(8, 1);
    Func_0808a170(0x17be);
    Func_0808a188(8, 0, 20);
    Func_0808a188(8, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(0, 0x101, 60);
    Func_0808a178(8, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a110(8, 3);
        Func_0808a010(20);
        Func_0808a188(8, 0, 20);
        *(u16 *)(workspace + 472) += 2;
    } else {
        Func_0808a010(10);
        Func_0808a138(8, 2);
        Func_0808a010(20);
        *(u16 *)(workspace + 472) += 1;
        Func_0808a188(8, 0, 20);
        Func_0808a110(8, 3);
        Func_0808a010(20);
        Func_0808a188(8, 0, 20);
    }
    Func_0808a188(8, 0, 20);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a1e8(0, 0x101, 60);
    Func_0808a178(8, 0);
    if (Func_0808a070(0, 0) == 1) {
        Func_0808a010(10);
        Func_0808a1e8(8, 0x102, 60);
        Func_0808a170(0x17c8);
        Func_0808a178(8, 0);
repeatPrompt:
        if (Func_0808a070(0, 0) == 1) {
            Func_0808a010(10);
            Func_0808a1e8(8, 0x102, 60);
            Func_0808a170(0x17e0);
            Func_0808a178(8, 0);
            goto repeatPrompt;
        }
    }
    Func_0808a170(0x17c9);
    Func_0808a010(10);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a178(8, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a110(0, 3);
        Func_0808a010(20);
        Func_0808a188(8, 0, 20);
        *(u16 *)(workspace + 472) += 1;
    } else {
        Func_0808a010(10);
        Func_0808a138(8, 2);
        *(u16 *)(workspace + 472) += 1;
        Func_0808a188(8, 0, 20);
    }
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a100(8, 5);
    Func_080770c8(0x893);
    Func_0808a020();
}
