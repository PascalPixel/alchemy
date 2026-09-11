#include "types.h"
#include "scene.h"

extern u8 Value_000000aa;
extern u8 Value_000000ab;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 Value_000000a9;
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];

u8 *Talk_unk4_4(s32);

u8 *Talk_unk5_4(s32);

u8 *Talk_unk6_4(s32);

u8 *Talk_unk7_4(s32);

/*
 * The eight-byte owner at 0x02000084 includes its one pool word, which holds
 * the returned table address 0x02008b48.
 */

/*
 * Select a table from the scene id.  The 88-byte owner at 0x0200008c includes
 * its seven-word literal pool at 0x020000c4-0x020000e3.  Element 224 of the
 * cross-overlay block gCell is the signed scene id this overlay keys
 * on.  The compared constants are spelled as the addresses of Value_000000aa
 * and its neighbours: that is how a small integer is pooled here, and the
 * names are not references to symbols.
 */

/* Old-style declarations: overlay import arities vary per call site. */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/*
 * Engine calls: each pseudo symbol is the per-site call word the overlay image
 * holds -- one word can serve two sites with different targets -- and the
 * macro names the engine function the site reaches through the overlay veneer
 * and the main-image veneer island, keeping the site's own calling form.
 * Unbound names are provisional.
 */

/*
 * Runs a fixed sequence of setup calls, mostly in mirrored pairs for entities
 * 8 and 9, followed by two six-argument calls whose second and last arguments
 * match (entity 6/27 and entity 9/26).
 */

                        /* Test a story flag; used in a condition. */

                        /* Open a scripted scene. */

                        /* Close a scripted scene. */

                        /* Dialogue prompt; the result selects the branch. */

                        /* Scene-presentation request. */

                        /* Show a dialogue line by id. */

                        /* Dialogue-line variant with a mode word. */

                        /* Wait for the slot's action to finish. */

s32 Talk_unk8_4();    /* Raw encoded call destination. */

void Scene_SetActor13Value1A(void)
{
    Talk_Apply(0xD, 0x1A);
}

s32 SceneData_GetPrimaryTable(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

s32 SceneData_ReturnZero(void)
{
    extern s16 gCell[];

    return 0;
}

u8 *SceneData_GetTable8B48(void)
{
    return (u8 *)0x02008b48;
}

s32 SceneData_GetSecondaryTable(void)
{
    extern s16 gCell[];

    s16 scene = gCell[224];

    if (scene == (s32)&Value_000000aa) {
        return (s32)gOv4;
    }
    if (scene == (s32)&Value_000000a9) {
        if (Talk_unk8_4(0x96f) != 0) {
            return (s32)gOv5;
        }
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

s32 SceneData_GetTertiaryTable(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)gOv8;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)gOv9;
    }
    return (s32)gOv10;
}

s32 Talk_unk9_4(void)
{
    extern u8 gCell[];
    extern u8 gWork[];

    s16 *q;
    s16 *p;
    s16 *r;
    s32 v;

    if (Talk_Check(0x89f) != 0) {
        s32 a = (s32)gVal;

        q = (s16 *)gCell;
        q[226] = a;
        {
            s16 *t = q + 227;
            s32 shown = 10;

            *t = shown;
        }
    }
    p = (s16 *)gCell;
    v = p[224];
    if (v == (s32)gVal2) {
        if (Talk_unk2(0x897) != 0) {
            Talk_unk10_4(10, 0, 0);
        }
        if (p[225] == 3) {
            if (Talk_unk3(0x8fb) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 1;

                    *t = shown;
                }
            }
            if (Talk_unk4(0x8fc) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 5;

                    *t = shown;
                }
            }
            Talk_Do(0x12f);
        }
        r = (s16 *)gCell;
        if (r[225] == 1) {
            Talk_unk2_2(0x8fb);
            if (Talk_unk5(0x96f) == 0) {
                Talk_SetRect(6, 0, 2, 1, 8, 27);
            }
        }
        if (r[225] != 5) {
            goto L_0200024c;
        }
        Talk_unk3_2(0x8fc);
    } else {
        if (v == (s32)gVal3) {
            Talk_unk11_4(8, 4);
            Talk_unk12_4(9, 4);
            Talk_unk13_4(10, 3);
            Talk_unk14_4(11, 4);
            Talk_unk15_4(12, 3);
            *(volatile s32 *)(Talk_unk9(15) + 28) = 0x19999;
            Talk_unk2_5(108, 38, 1, 1, 102, 56);
        }
    }
    L_0200024c:;
    return 0;
}

void Talk_unk16_4(void)
{
    extern u8 gCell[];
    extern u8 gWork[];

    s32 arg;
    s32 v5;

    arg = *(s16 *)(*(u8 **)gWork + 0x16c);
    *(u8 *)(Talk_unk10(0) + 85) = 0;
    v5 = 2;
    Talk_unk17_4(158);
    Talk_unk3_5(66, 36, 71, 8, v5, v5);
    Talk_unk18_4(4);
    Talk_unk4_5(68, 36, 71, 8, v5, v5);
    Talk_unk19_4(4);
    Talk_Place(0, 3, -16);
    Talk_unk20_4(arg);
}

void Dialogue_ShowMessages8fbAnd8fc(void)
{
    extern u8 *gWork;

    s16 token = *(s16 *)(gWork + 364);

    Talk_unk6_2(123);
    Talk_unk7_2(0x8FB);
    Talk_unk8_2(0x8FC);
    Talk_unk9_2(token);
}

void Scene_RunPrimarySequence(void)
{
    Battle_Reset_1();
    Motion_SetSpeed_1(8, 65536, 32768);
    Motion_SetSpeed_2(9, 65536, 32768);
    Motion_ResetPosMode2_1(8, 136, 384);
    Motion_SetPosReset_1(9, 152, 384);
    Motion_ArmCb_1(8, 16384, 0);
    Motion_ArmCb_2(9, 16384, 0);
    Object_SetModeById_1(8, 1);
    Talk_unk5_5(6, 27, 1, 1, 7, 27);
    Talk_unk6_5(9, 26, 2, 1, 7, 26);
    Battle_SchedShoulder_1();
}

void Scene_RunScene3c3SequenceA(void)
{
    extern u8 gWork[];
    void Talk_unk21_4();

    u32 i;
    s32 record;
    s32 v5;

    Talk_unk22_4();
    Talk_unk2_3(0, 0x19999, 0xcccc);
    Talk_unk3_3(0, 120, 0x1b6);
    Talk_unk4_3(0, 0xc000, 0);
    record = Talk_unk6(0);
    if (record != 0) {
        Talk_unk23_4(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Talk_unk24_4(1);
    Talk_unk5_3(11, 0x19999, 0xcccc);
    Talk_unk6_3(11, 108, 0x1af);
    Talk_unk7_3(11, 0xd000, 10);
    Talk_unk8_3(11, 0x100, 20);
    Talk_unk9_3(11, 0xd000, 20);
    Talk_unk25_4(11, 0, 40);
    Talk_unk10_3(11, 0xd000, 40);
    Talk_unk26_4(11, 0, 20);
    Talk_unk27_4(11, 2);
    Talk_unk4_2(0x2654);
    Talk_unk28_4(11, 0, 40);
    Talk_unk11_3(8, 0x100, 0);
    Talk_unk29_4(8, 2);
    Talk_unk30_4(8, 0, 10);
    Talk_unk12_3(11, 132, 0x1a4);
    Talk_unk13_3(11, 0xd000, 0);
    Talk_unk14_3(0, 0xe000, 0);
    Talk_unk15_3(11, 138, 0x1a0);
    Talk_unk16_3(11, 0xb000, 10);
    Talk_unk31_4(11, 2);
    Talk_unk32_4(11, 0, 40);
    ((void (*)())Talk_unk11)(8, 2);
    Talk_unk33_4(8, 0, 40);
    Talk_unk17_3(9, 0x100, 20);
    Talk_unk34_4(9, 2);
    Talk_unk35_4(9, 0, 20);
    Talk_unk18_3(0, 0xc000, 0);
    Talk_unk19_3(11, 144, 0x1a4);
    Talk_unk36_4(20);
    Talk_unk37_4(9, 2);
    Talk_unk38_4(9, 0, 20);
    Talk_Run(9, 0x102);
    Talk_unk39_4(9, 3);
    Talk_unk40_4(9, 0, 20);
    Talk_unk20_3(11, 0x5000, 20);
    Talk_unk41_4(11, 0);
    if (Talk_unk7(0x9b0) != 0) {
        Talk_unk21_3(11, 0xd000, 40);
        Talk_unk2_4(11, 0x102);
        Talk_unk42_4(40);
        Talk_unk43_4(11, 0, 10);
    } else {
        bump_step(1);
    }
    Talk_unk22_3(11, 0x5000, 10);
    Talk_unk44_4(11, 0, 40);
    Talk_unk23_3(11, 0x100, 40);
    Talk_unk24_3(11, 0xb000, 10);
    Talk_unk45_4(11, 0, 10);
    Talk_unk25_3(11, 138, 0x1a0);
    Talk_unk26_3(11, 0xb000, 20);
    Talk_unk46_4(8, 2);
    Talk_unk47_4(8, 0, 10);
    Talk_unk3_4(11, 0x102);
    Talk_unk48_4(11, 1);
    Talk_unk49_4(20);
    ((void (*)())Talk_unk12)(11, 0, 20);
    Talk_unk50_4(9, 2);
    Talk_unk51_4(20);
    Talk_unk27_3(0, 0xe000, 10);
    Talk_unk52_4(9, 1);
    Talk_unk53_4(20);
    Talk_unk54_4(9, 0, 10);
    Talk_unk55_4(8, 2);
    Talk_unk56_4(8, 0, 20);
    Talk_unk57_4(11, 1);
    Talk_unk58_4(20);
    Talk_unk59_4(11, 0, 20);
    Talk_unk28_3(11, 0x5000, 10);
    Talk_unk60_4(11, 0, 10);
    Talk_unk61_4(11, 2);
    record = Talk_unk8(0);
    if (record != 0) {
        Talk_unk62_4(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk21_4(11);
    v5 = 7;
    Talk_unk63_4(11, 0, 0);
    Talk_unk7_5(6, 27, 1, 1, v5, 27);
    Talk_unk8_5(9, 26, 2, 1, v5, 26);
    Talk_unk5_2(0x89f);
    Talk_unk64_4();
}

/*
 * Dialogue bracket at 0x02000730.  The 124-byte owner includes its four-word
 * literal pool at 0x0200079c-0x020007ab.  The tail is shared through a label
 * rather than copied into each arm: copying it would add a fourth call site
 * where there are three.  The skip-beat counter sits at byte offset 472 off
 * the gWork pointer cell, which costs one dereference.  The guard is
 * tested against zero at this site; the polarity is read per call site.
 */
void Scene_RunActorNinePromptDialogue(void)
{
    extern u8 *gWork;
    s32 Talk_unk21_4();
    s32 Talk_unk65_4();

    u8 *work;

    Talk_unk66_4();

    if (Talk_unk65_4(0x89f) != 0) {
        Talk_unk67_4(0x2668);
        goto close;
    }

    Talk_unk68_4(0x264e);
    {
        s32 mode = 0;
        s32 no = 9;

        Talk_unk13(no, mode);
    }

    if (Talk_unk14(0, 0) != 0) {
        goto skip;
    }

    Talk_unk69_4(9, 0);
    Talk_unk70_4(9, 4);

close:
    Talk_unk71_4(9, 0);
    goto done;

skip:
    /* Skip-beat counter, two beats' worth. */
    work = gWork;
    *(u16 *)(work + 472) += 2;
    Talk_unk72_4(9, 0);

done:
    Talk_unk73_4();
}

void Scene_RunActorTenRepeatedMotion(void)
{
    extern u8 *gWork;
    s32 Talk_unk21_4();

    unsigned int beat;

    Talk_unk74_4();

    Talk_unk10_2(0x266d);
    Talk_unk29_3(10, 0, 10);

    beat = 0;
    do {
        Talk_Apply2(10, 0);
        Talk_Apply3(Talk_unk4_4(10), 1);
        Talk_unk11_2(4);

        Talk_Apply4(10, 15);
        Talk_Apply5(Talk_unk5_4(10), 0);
        beat++;
        Talk_unk12_2(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Talk_Apply6(10, 0);
        Talk_Apply7(Talk_unk6_4(10), 1);
        Talk_unk13_2(2);

        Talk_Apply8(10, 15);
        Talk_Apply9(Talk_unk7_4(10), 0);
        beat++;
        Talk_unk14_2(2);
    } while (beat <= 11);

    Talk_unk30_3(10, 0, 0);

    Talk_unk15_2(0x897);

    Talk_unk75_4();
}
