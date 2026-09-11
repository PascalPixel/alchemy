/*
 * Overlay resource_386: actor placement, per-actor dialogue lines and the
 * scene initialiser that installs the per-frame task.
 */

#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/placement.h"

#define WORKSPACE (*(u8 **)0x03001ebc)

#include "resource_386_state.h"

extern u8 gWork[];
extern s16 gCell[];

u8 *Talk_Run2(s32);

u8 *Talk_Run3(s32);

u8 *Talk_Run4(s32);

u8 *Talk_Run5(s32);

u8  *Talk_Run6();
u8  *Talk_Run7();
u8  *Talk_Run8();

u8 *Talk_Run9(s32);
u8 *Talk_Run10(s32, s32);

/* Each Func_ name is a loader-relocated call word, not a runtime address. */

void State_CheckPositionWindow(void)
{
    s32 v1;
    s32 v0;

    v0 = ((struct Resource386FirstView *)Resource386_GetFirstView(0))->sample_08;
    v1 = (s32)((struct Resource386SecondView *)Resource386_GetSecondView(0))->sample_10 >> 0x14;
    if (((u32)((v0 >> 0x14) - 0x22) <= 1U) && (v1 > 0x28) && (v1 <= 0x2A)) {
        Resource386_OnWindowMatch(0x250);
        return;
    }
    Resource386_OnWindowMiss(0x250);
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020086dc;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020087cc;
}

u8 *SceneData_InitAndGetTable87f4(void)
{
    u8 *slot = (u8 *)0x020087F4;

    Talk_Do8(slot);
    return slot;
}

void Actor_RunActorStep(s32 arg0)
{
    Talk_Do9(arg0);
    Talk_Apply(arg0, 1);
    Talk_Apply2(arg0, 0);
    Talk_Run11();
}

void Dialogue_RunActor9Line(void)
{
    Talk_Do10(0x1CC9);
    Talk_Place3(9, 0, 2);
    Talk_Do11(9);
}

void Dialogue_RunActor11Line(void)
{
    Talk_Do12(0x1CCD);
    Talk_Place4(11, 0, 2);
    Talk_Do13(11);
}

void Dialogue_RunActor12Line(void)
{
    Talk_Do14(0x1CD0);
    Talk_Place5(12, 0, 2);
    Talk_Do15(12);
}

void Scene_RunActor16Sequence(void)
{
    u32 i;
    s32 record;

    Talk_Run12();
    Talk_Do(0x1cd4);
    Talk_Run13(16, 0, 2);
    Talk_Run14(16, 1);
    Talk_Run15(16, 0, 20);
    Talk_Run16(16, 4);
    Talk_Run17(20);
    Talk_Run18(16, 0, 20);
    Talk_Place(16, 0x102, 60);
    Talk_Run19(16, 0, 30);
    Talk_Check(16, 0);
    if (Talk_Check2(0, 0) != 0) {
        bump_step(1);
    }
    Talk_Run20(16, 0, 20);
    Talk_Do2(0x300);
    Talk_Do3(0x868);
    Talk_Run21();
}

void Dialogue_RunActor16Line(void)
{
    void Talk_Run22(s32, s32, s32);

    Talk_Do16(0x1CDA);
    Talk_Place6(16, 0, 2);
    Talk_Do17(16);
}

void Dialogue_RunActor23Line(void)
{
    void Talk_Run22(s32, s32, s32);

    Talk_Do18(0x1CEE);
    Talk_Run22(23, 0, 2);
    Talk_Do19(23);
}

void Scene_RunActor18FlaggedSequence(void)
{
    void Talk_Run23();
    void Talk_Run24();
    void Talk_Run25();

    u32 i;
    s32 record;

    Talk_Run26();
    Talk_Run24(18, 0, 0);
    if (Talk_Check3(0x85b) == 0) {
        Talk_Do4(0x137c);
        Talk_Run27(18, 0);
    } else {
        Talk_Do5(0x1385);
        Talk_Run28(18, 0);
    }
    if (Talk_Check4(0, 0) == 0) {
        Talk_Run23(20);
        Talk_Run29(18, 0);
        Talk_Run30(20);
        Talk_Run31(18, 2);
        Talk_Run32(20);
        if (Talk_Run() == 0) {
            Talk_Run33(18, 4);
            Talk_Run34(20);
            Talk_Do6(0x1384);
            Talk_Run35(18, 0);
            goto L_020002d4;
        }
        Talk_Run36(231, 3);
        Talk_Run37(231, 0);
        Talk_Do7(0x85b);
    } else {
        bump_step(1);
        Talk_Run38(20);
        Talk_Run39(18, 3);
        Talk_Run40(20);
        Talk_Run25(18, 0);
    }
    L_020002d4:;
    Talk_Place2(18, 0x4000, 0);
    Talk_Run41();
}

void Actor_RunActor16StepWithFlag91(void)
{
    u8 *Talk_Run42(s32);
    u8 *Talk_Run43(s32);

    u8 *slot;
    u8 clear = 0;

    Talk_Run44();
    Talk_Apply3(16, 1);
    Talk_Run45();
    slot = Talk_Run42(16) + 91;
    *slot = 1;
    Talk_Run46();
    slot = Talk_Run43(16) + 91;
    *slot = clear;
    Talk_Apply4(16, 2);
}

void Scene_RunActor18ConditionalCue(void)
{
    void Talk_Run47(s32);
    void Talk_Run48(s32, s32);

    Talk_Run49();

    if (Talk_Check5() == 0) {
        Talk_Run48(18, 4);
        Talk_Run47(20);
        Talk_Do20(0x1384);
        Talk_Apply5(18, 0);
    } else {
        Talk_Apply6(0xE7, 3);
        Talk_Apply7(0xE7, 0);
    }

    Talk_Run50();
}

void Scene_RunActor19StepByPlace(void)
{
    void Talk_Run51(void);

    u32 place;

    place = *(u16 *)(Talk_Run2(0) + 6);
    Talk_Run51();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply8(4, 19);
    } else {
        Talk_Do21(0x1CE2);
        Talk_Apply9(19, 0);
    }

    Talk_Run52();
}

void Scene_RunActor20StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Talk_Run3(0) + 6);
    Talk_Run53();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply10(5, 20);
    } else {
        Talk_Do22(0x1CE4);
        Talk_Apply11(20, 0);
    }

    Talk_Run54();
}

void Scene_RunActor21StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Talk_Run4(0) + 6);
    Talk_Run55();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply12(6, 21);
    } else {
        Talk_Do23(0x1CE6);
        Talk_Apply13(21, 0);
    }

    Talk_Run56();
}

void Scene_RunActor22StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Talk_Run5(0) + 6);
    Talk_Run57();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply14(1, 22);
    } else {
        Talk_Do24(0x1CEC);
        Talk_Apply15(22, 0);
    }

    Talk_Run58();
}

void Dialogue_RunActor18FlaggedLine(void)
{
    s32 Talk_Run59(s32);

    Talk_Run60();
    if (Talk_Run59(0x85B) == 0) {
        Talk_Do25(0x1382);
    } else {
        Talk_Do26(0x1CF4);
    }
    Talk_Apply16(18, 0);
    Talk_Run61();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetEffectTable(void)
{
    s32 Talk_Run59(s32);

    return (u8 *)0x0200898c;
}

/*
 * Overlay entry point: selects the scene from the global block and runs the
 * matching setup. It returns a constant zero status.
 *
 * gCell is the cross-overlay RAM global block rather than an in-image
 * address, and the signed halfword read out of it selects the scene.
 */
s32 Scene_InitSceneStateByStep(void)
{
    void Talk_Run62();

    s32 scene;
    s32 zero;

    *(s32 *)(WORKSPACE + 448) = 521;
    scene = gCell[225];

    if (scene == 5) {
        s32 fifth = 4;
        s32 sixth = 3;

        /*
         * The fifth and sixth arguments go on the stack. The two locals
         * are what put them there, so they must stay locals.
         */
        Talk_SetRect(0, 120, 8, 67, fifth, sixth);
        zero = 0;
        Talk_Run6(8)[0x55] = zero;
        *(s32 *)(Talk_Run7(8) + 12) = zero;
        *(s32 *)(Talk_Run8(8) + 20) = zero;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        Talk_Run62(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /*
         * 0x02008031 is State_CheckPositionWindow plus the Thumb bit, a task callback
         * rather than data; 200 << 4 is the period.
         */
        Talk_Run63(0x02008031, 0xc80);
    }

    return 0;
}

/*
 * Prepare object 22 for display. Call sites set three further registers that
 * this function does not read.
 */
void OvObj_InitObject22(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Talk_Run9(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Talk_Run10(17, 0x608);
        Talk_Do27(a);
        v += 0x400;
        Talk_Place7(q[28], 0x80, v);
        Talk_Do28(17);
    }
}
