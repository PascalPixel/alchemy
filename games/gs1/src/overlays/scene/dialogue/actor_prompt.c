#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/actor_prompt.h"


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

u8 *Talk_Run4(s32);

u8 *Talk_Run5(s32);

u8 *Talk_Run6(s32);

u8 *Talk_Run7(s32);

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

s32 Talk_Run8();    /* Raw encoded call destination. */

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
        if (Talk_Run8(0x96f) != 0) {
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

s32 Talk_Run9(void)
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
        if (Talk_Check2(0x897) != 0) {
            Talk_Run10(10, 0, 0);
        }
        if (p[225] == 3) {
            if (Talk_Check3(0x8fb) != 0) {
                p[288] = v;
                {
                    s16 *t = p + 289;
                    s32 shown = 1;

                    *t = shown;
                }
            }
            if (Talk_Check4(0x8fc) != 0) {
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
            Talk_Do2(0x8fb);
            if (Talk_Check5(0x96f) == 0) {
                Talk_SetRect(6, 0, 2, 1, 8, 27);
            }
        }
        if (r[225] != 5) {
            goto L_0200024c;
        }
        Talk_Do3(0x8fc);
    } else {
        if (v == (s32)gVal3) {
            Talk_Run11(8, 4);
            Talk_Run12(9, 4);
            Talk_Run13(10, 3);
            Talk_Run14(11, 4);
            Talk_Run15(12, 3);
            *(volatile s32 *)(Talk_Check9(15) + 28) = 0x19999;
            Talk_SetRect2(108, 38, 1, 1, 102, 56);
        }
    }
    L_0200024c:;
    return 0;
}

void Talk_Run16(void)
{
    extern u8 gCell[];
    extern u8 gWork[];

    s32 arg;
    s32 v5;

    arg = *(s16 *)(*(u8 **)gWork + 0x16c);
    *(u8 *)(Talk_Check10(0) + 85) = 0;
    v5 = 2;
    Talk_Run17(158);
    Talk_SetRect3(66, 36, 71, 8, v5, v5);
    Talk_Run18(4);
    Talk_SetRect4(68, 36, 71, 8, v5, v5);
    Talk_Run19(4);
    Talk_Place(0, 3, -16);
    Talk_Run20(arg);
}

void Dialogue_ShowMessages8fbAnd8fc(void)
{
    extern u8 *gWork;

    s16 token = *(s16 *)(gWork + 364);

    Talk_Do6(123);
    Talk_Do7(0x8FB);
    Talk_Do8(0x8FC);
    Talk_Do9(token);
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
    Talk_SetRect5(6, 27, 1, 1, 7, 27);
    Talk_SetRect6(9, 26, 2, 1, 7, 26);
    Battle_SchedShoulder_1();
}

void Scene_RunScene3c3SequenceA(void)
{
    extern u8 gWork[];
    void Talk_Run21();

    u32 i;
    s32 record;
    s32 v5;

    Talk_Run22();
    Talk_Place2(0, 0x19999, 0xcccc);
    Talk_Place3(0, 120, 0x1b6);
    Talk_Place4(0, 0xc000, 0);
    record = Talk_Check6(0);
    if (record != 0) {
        Talk_Run23(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Talk_Run24(1);
    Talk_Place5(11, 0x19999, 0xcccc);
    Talk_Place6(11, 108, 0x1af);
    Talk_Place7(11, 0xd000, 10);
    Talk_Place8(11, 0x100, 20);
    Talk_Place9(11, 0xd000, 20);
    Talk_Run25(11, 0, 40);
    Talk_Place10(11, 0xd000, 40);
    Talk_Run26(11, 0, 20);
    Talk_Run27(11, 2);
    Talk_Do4(0x2654);
    Talk_Run28(11, 0, 40);
    Talk_Place11(8, 0x100, 0);
    Talk_Run29(8, 2);
    Talk_Run30(8, 0, 10);
    Talk_Place12(11, 132, 0x1a4);
    Talk_Place13(11, 0xd000, 0);
    Talk_Place14(0, 0xe000, 0);
    Talk_Place15(11, 138, 0x1a0);
    Talk_Place16(11, 0xb000, 10);
    Talk_Run31(11, 2);
    Talk_Run32(11, 0, 40);
    ((void (*)())Talk_Check11)(8, 2);
    Talk_Run33(8, 0, 40);
    Talk_Place17(9, 0x100, 20);
    Talk_Run34(9, 2);
    Talk_Run35(9, 0, 20);
    Talk_Place18(0, 0xc000, 0);
    Talk_Place19(11, 144, 0x1a4);
    Talk_Run36(20);
    Talk_Run37(9, 2);
    Talk_Run38(9, 0, 20);
    Talk_Run(9, 0x102);
    Talk_Run39(9, 3);
    Talk_Run40(9, 0, 20);
    Talk_Place20(11, 0x5000, 20);
    Talk_Run41(11, 0);
    if (Talk_Check7(0x9b0) != 0) {
        Talk_Place21(11, 0xd000, 40);
        Talk_Run2(11, 0x102);
        Talk_Run42(40);
        Talk_Run43(11, 0, 10);
    } else {
        bump_step(1);
    }
    Talk_Place22(11, 0x5000, 10);
    Talk_Run44(11, 0, 40);
    Talk_Place23(11, 0x100, 40);
    Talk_Place24(11, 0xb000, 10);
    Talk_Run45(11, 0, 10);
    Talk_Place25(11, 138, 0x1a0);
    Talk_Place26(11, 0xb000, 20);
    Talk_Run46(8, 2);
    Talk_Run47(8, 0, 10);
    Talk_Run3(11, 0x102);
    Talk_Run48(11, 1);
    Talk_Run49(20);
    ((void (*)())Talk_Check12)(11, 0, 20);
    Talk_Run50(9, 2);
    Talk_Run51(20);
    Talk_Place27(0, 0xe000, 10);
    Talk_Run52(9, 1);
    Talk_Run53(20);
    Talk_Run54(9, 0, 10);
    Talk_Run55(8, 2);
    Talk_Run56(8, 0, 20);
    Talk_Run57(11, 1);
    Talk_Run58(20);
    Talk_Run59(11, 0, 20);
    Talk_Place28(11, 0x5000, 10);
    Talk_Run60(11, 0, 10);
    Talk_Run61(11, 2);
    record = Talk_Check8(0);
    if (record != 0) {
        Talk_Run62(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run21(11);
    v5 = 7;
    Talk_Run63(11, 0, 0);
    Talk_SetRect7(6, 27, 1, 1, v5, 27);
    Talk_SetRect8(9, 26, 2, 1, v5, 26);
    Talk_Do5(0x89f);
    Talk_Run64();
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
    s32 Talk_Run21();
    s32 Talk_Run65();

    u8 *work;

    Talk_Run66();

    if (Talk_Run65(0x89f) != 0) {
        Talk_Run67(0x2668);
        goto close;
    }

    Talk_Run68(0x264e);
    {
        s32 mode = 0;
        s32 no = 9;

        Talk_Check13(no, mode);
    }

    if (Talk_Check14(0, 0) != 0) {
        goto skip;
    }

    Talk_Run69(9, 0);
    Talk_Run70(9, 4);

close:
    Talk_Run71(9, 0);
    goto done;

skip:
    /* Skip-beat counter, two beats' worth. */
    work = gWork;
    *(u16 *)(work + 472) += 2;
    Talk_Run72(9, 0);

done:
    Talk_Run73();
}

void Scene_RunActorTenRepeatedMotion(void)
{
    extern u8 *gWork;
    s32 Talk_Run21();

    unsigned int beat;

    Talk_Run74();

    Talk_Do10(0x266d);
    Talk_Place29(10, 0, 10);

    beat = 0;
    do {
        Talk_Apply2(10, 0);
        Talk_Apply3(Talk_Run4(10), 1);
        Talk_Do11(4);

        Talk_Apply4(10, 15);
        Talk_Apply5(Talk_Run5(10), 0);
        beat++;
        Talk_Do12(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Talk_Apply6(10, 0);
        Talk_Apply7(Talk_Run6(10), 1);
        Talk_Do13(2);

        Talk_Apply8(10, 15);
        Talk_Apply9(Talk_Run7(10), 0);
        beat++;
        Talk_Do14(2);
    } while (beat <= 11);

    Talk_Place30(10, 0, 0);

    Talk_Do15(0x897);

    Talk_Run75();
}
