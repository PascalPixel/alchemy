/*
 * Overlay resource_386: actor placement, per-actor dialogue lines and the
 * scene initialiser that installs the per-frame task.
 */

#include "types.h"
#include "scene.h"

#define WORKSPACE (*(u8 **)0x03001ebc)

#include "resource_386_state.h"

extern u8 gWork[];
extern s16 gCell[];

u8 *Talk_unk2_4(s32);

u8 *Talk_unk3_4(s32);

u8 *Talk_unk4_4(s32);

u8 *Talk_unk5_4(s32);

u8  *Talk_unk6_4();
u8  *Talk_unk7_4();
u8  *Talk_unk8_4();

u8 *Talk_unk9_4(s32);
u8 *Talk_unk10_4(s32, s32);

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

    Talk_unk8_2(slot);
    return slot;
}

void Actor_RunActorStep(s32 arg0)
{
    Talk_unk9_2(arg0);
    Talk_Apply(arg0, 1);
    Talk_Apply2(arg0, 0);
    Talk_unk11_4();
}

void Dialogue_RunActor9Line(void)
{
    Talk_unk10_2(0x1CC9);
    Talk_unk3_3(9, 0, 2);
    Talk_unk11_2(9);
}

void Dialogue_RunActor11Line(void)
{
    Talk_unk12_2(0x1CCD);
    Talk_unk4_3(11, 0, 2);
    Talk_unk13_2(11);
}

void Dialogue_RunActor12Line(void)
{
    Talk_unk14_2(0x1CD0);
    Talk_unk5_3(12, 0, 2);
    Talk_unk15_2(12);
}

void Scene_RunActor16Sequence(void)
{
    u32 i;
    s32 record;

    Talk_unk12_4();
    Talk_Do(0x1cd4);
    Talk_unk13_4(16, 0, 2);
    Talk_unk14_4(16, 1);
    Talk_unk15_4(16, 0, 20);
    Talk_unk16_4(16, 4);
    Talk_unk17_4(20);
    Talk_unk18_4(16, 0, 20);
    Talk_Place(16, 0x102, 60);
    Talk_unk19_4(16, 0, 30);
    Talk_Check(16, 0);
    if (Talk_unk2(0, 0) != 0) {
        bump_step(1);
    }
    Talk_unk20_4(16, 0, 20);
    Talk_unk2_2(0x300);
    Talk_unk3_2(0x868);
    Talk_unk21_4();
}

void Dialogue_RunActor16Line(void)
{
    void Talk_unk22_4(s32, s32, s32);

    Talk_unk16_2(0x1CDA);
    Talk_unk6_3(16, 0, 2);
    Talk_unk17_2(16);
}

void Dialogue_RunActor23Line(void)
{
    void Talk_unk22_4(s32, s32, s32);

    Talk_unk18_2(0x1CEE);
    Talk_unk22_4(23, 0, 2);
    Talk_unk19_2(23);
}

void Scene_RunActor18FlaggedSequence(void)
{
    void Talk_unk23_4();
    void Talk_unk24_4();
    void Talk_unk25_4();

    u32 i;
    s32 record;

    Talk_unk26_4();
    Talk_unk24_4(18, 0, 0);
    if (Talk_unk3(0x85b) == 0) {
        Talk_unk4_2(0x137c);
        Talk_unk27_4(18, 0);
    } else {
        Talk_unk5_2(0x1385);
        Talk_unk28_4(18, 0);
    }
    if (Talk_unk4(0, 0) == 0) {
        Talk_unk23_4(20);
        Talk_unk29_4(18, 0);
        Talk_unk30_4(20);
        Talk_unk31_4(18, 2);
        Talk_unk32_4(20);
        if (Talk_Run() == 0) {
            Talk_unk33_4(18, 4);
            Talk_unk34_4(20);
            Talk_unk6_2(0x1384);
            Talk_unk35_4(18, 0);
            goto L_020002d4;
        }
        Talk_unk36_4(231, 3);
        Talk_unk37_4(231, 0);
        Talk_unk7_2(0x85b);
    } else {
        bump_step(1);
        Talk_unk38_4(20);
        Talk_unk39_4(18, 3);
        Talk_unk40_4(20);
        Talk_unk25_4(18, 0);
    }
    L_020002d4:;
    Talk_unk2_3(18, 0x4000, 0);
    Talk_unk41_4();
}

void Actor_RunActor16StepWithFlag91(void)
{
    u8 *Talk_unk42_4(s32);
    u8 *Talk_unk43_4(s32);

    u8 *slot;
    u8 clear = 0;

    Talk_unk44_4();
    Talk_Apply3(16, 1);
    Talk_unk45_4();
    slot = Talk_unk42_4(16) + 91;
    *slot = 1;
    Talk_unk46_4();
    slot = Talk_unk43_4(16) + 91;
    *slot = clear;
    Talk_Apply4(16, 2);
}

void Scene_RunActor18ConditionalCue(void)
{
    void Talk_unk47_4(s32);
    void Talk_unk48_4(s32, s32);

    Talk_unk49_4();

    if (Talk_unk5() == 0) {
        Talk_unk48_4(18, 4);
        Talk_unk47_4(20);
        Talk_unk20_2(0x1384);
        Talk_Apply5(18, 0);
    } else {
        Talk_Apply6(0xE7, 3);
        Talk_Apply7(0xE7, 0);
    }

    Talk_unk50_4();
}

void Scene_RunActor19StepByPlace(void)
{
    void Talk_unk51_4(void);

    u32 place;

    place = *(u16 *)(Talk_unk2_4(0) + 6);
    Talk_unk51_4();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply8(4, 19);
    } else {
        Talk_unk21_2(0x1CE2);
        Talk_Apply9(19, 0);
    }

    Talk_unk52_4();
}

void Scene_RunActor20StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Talk_unk3_4(0) + 6);
    Talk_unk53_4();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply10(5, 20);
    } else {
        Talk_unk22_2(0x1CE4);
        Talk_Apply11(20, 0);
    }

    Talk_unk54_4();
}

void Scene_RunActor21StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Talk_unk4_4(0) + 6);
    Talk_unk55_4();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply12(6, 21);
    } else {
        Talk_unk23_2(0x1CE6);
        Talk_Apply13(21, 0);
    }

    Talk_unk56_4();
}

void Scene_RunActor22StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Talk_unk5_4(0) + 6);
    Talk_unk57_4();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Talk_Apply14(1, 22);
    } else {
        Talk_unk24_2(0x1CEC);
        Talk_Apply15(22, 0);
    }

    Talk_unk58_4();
}

void Dialogue_RunActor18FlaggedLine(void)
{
    s32 Talk_unk59_4(s32);

    Talk_unk60_4();
    if (Talk_unk59_4(0x85B) == 0) {
        Talk_unk25_2(0x1382);
    } else {
        Talk_unk26_2(0x1CF4);
    }
    Talk_Apply16(18, 0);
    Talk_unk61_4();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetEffectTable(void)
{
    s32 Talk_unk59_4(s32);

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
    void Talk_unk62_4();

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
        Talk_unk6_4(8)[0x55] = zero;
        *(s32 *)(Talk_unk7_4(8) + 12) = zero;
        *(s32 *)(Talk_unk8_4(8) + 20) = zero;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        Talk_unk62_4(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /*
         * 0x02008031 is State_CheckPositionWindow plus the Thumb bit, a task callback
         * rather than data; 200 << 4 is the period.
         */
        Talk_unk63_4(0x02008031, 0xc80);
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
    o = Talk_unk9_4(22);
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
        v = Talk_unk10_4(17, 0x608);
        Talk_unk27_2(a);
        v += 0x400;
        Talk_unk7_3(q[28], 0x80, v);
        Talk_unk28_2(17);
    }
}
