#include "types.h"
#include "scene.h"

#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define RECORD_S32(rec, off) (*(s32 *)((rec) + (off)))

extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 Value_00000923;
extern u8 Value_00000922;
extern u8 Value_00000924;
extern s32 gOv6[];
extern s32 gOv7[];
extern u16 gCell[];
extern u8 Value_0000006f;

u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();

u8 *Effect_unk11_4(void);

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSevenActorEnsemble:
 * Seven actor setup and motion sequences, two scene-workspace state writes,
 * the closing map-state publication, and the final selector branch. */

/*
 * Scripted per-object step for resource_3b0.  It turns the object to one of two
 * fixed headings when a sampled value picks them out, then reports finished.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* A record's four-byte field at the given byte offset. Used below for the
 * seven fields that get the same reset pattern twice, once per record. */

s32 Effect_unk12_4();            /* Sampled value. */

/*
 * The shift pair is a windowed extraction of bits 10..15 of the sample, giving
 * a value in 0..63; a plain right shift would let larger values reach the
 * comparisons.  The headings 0xd000 and 0xb000 are built from an immediate and
 * a shift, and the local in each arm is what forces that.  The halfword at
 * object + 6 is the facing angle.  What Effect_unk12_4 samples is not
 * established here.
 */
s32 Actor_SetFacingFromSample(u8 *object)
{
    extern u8 gWork[];

    u32 sample = (u32)(Effect_unk12_4() << 6) >> 16;   /* bits 10..15 */

    if (sample == 6) {
        s32 value = 0xd000;

        *(u16 *)(object + 6) = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        *(u16 *)(object + 6) = value;
    }

    return 1;
}

void OvObj_Add160ToFields18And1c(u8 *o)
{
    extern u8 gWork[];

    if (*(s32 *)(o + 24) < 0x10000) {
        *(s32 *)(o + 24) += 160;
        *(s32 *)(o + 28) += 160;
    }
}

s32 OvObj_ShrinkScaleThenStop(u8 *o)
{
    extern u8 gWork[];

    u8 *t = *(u8 **)(o + 0x50);

    t[9] |= 12;
    *(s32 *)(o + 48) = 0x20000;
    *(s32 *)(o + 52) = 0x10000;
    if (*(s32 *)(o + 24) > 0x1000) {
        *(s32 *)(o + 24) += 0xFFFFFC00;
        *(s32 *)(o + 28) += 0xFFFFFC00;
    } else {
        *(s32 *)(o + 8) = 0;
        *(s32 *)(o + 12) = 0;
        *(s32 *)(o + 16) = 0;
        *(s32 *)(o + 36) = 0;
        *(s32 *)(o + 40) = 0;
        *(s32 *)(o + 44) = 0;
    }
    return 1;
}

u8 *SceneData_GetTable95c0(void)
{
    extern u8 gWork[];

    return gOv;
}

u8 *SceneData_GetTable9680(void)
{
    extern u8 gWork[];

    return gOv2;
}

u8 *SceneData_GetTable96a0(void)
{
    extern u8 gWork[];

    return gOv3;
}

u8 *SceneData_GetTable96C4(void)
{
    extern u8 gWork[];

    return gOv4;
}

u8 *SceneData_GetTable988c(void)
{
    extern u8 gWork[];

    return gOv5;
}

void Scene_CallHelper(void)
{
    extern u8 gWork[];

    Effect_unk13_4();
}

void Scene_RunActor232SceneWhenFlag923Or922(void)
{
    extern u8 gWork[];

    if (Effect_Check((s32)&Value_00000923) != 0 || Effect_unk2((s32)&Value_00000922) != 0) {
        Effect_unk14_4();
        Effect_Apply(0xE8, 3);
        Effect_Apply2(0xE8, 0);
        Effect_unk5_2((s32)&Value_00000924);
        Effect_unk15_4();
    }
}

void Scene_RunScene3b0(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_unk16_4();
    Effect_Run(-1, -1, -1, 0);
    Effect_unk17_4(1);
    *(u8 *)(Effect_unk3() + 85) = 0;
    Effect_Place(0xa40000, 0x400000, 0x1410000);
    Effect_unk18_4();
    Effect_unk19_4(1);
    Effect_unk20_4(0, 0, 0);
    Effect_unk21_4();
    Effect_unk22_4();
}

void Scene_RunScene3b0(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_unk23_4();
    Effect_unk2_3(0, 0xa40000, 0x1410000);
    Effect_unk24_4(0, 15);
    record = Effect_unk4(0);
    Effect_unk25_3(record, 0);
    Effect_unk26_3(1);
    Effect_unk27_3();
    Effect_unk28_3(1);
    Effect_unk29_3();
    Effect_unk30_2();
}

void Scene_RunScene3b0(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x202;
    Effect_unk31_2();
    Effect_unk32_2();
    Effect_unk33_2(20);
    Effect_unk3_3(8, 0x10000, 0x8000);
    Effect_unk4_3(8, 164, 0x141);
    Effect_unk5_3(8, 0xd000, 40);
    Effect_unk6_3(8, 0xb000, 40);
    Effect_unk7_3(8, 0xd000, 40);
    Effect_unk8_3(8, 0x3000, 10);
    Effect_unk9_3(8, 164, 0x14e);
    Effect_unk34_2(8, 4, 40);
    Effect_unk35_2(8, 2);
    Effect_Do(0x1e3a);
    Effect_unk36_2(8, 0, 20);
    Effect_unk37_2();
    Effect_unk38_2();
    Effect_unk39_2(10);
}

/* Runs the record-8/record-9 pair through two near-identical setup-then-move
 * cycles (position waypoints, a movement flag reset, then animation/sound
 * calls), followed by a shorter closing cycle for record 8 alone. */
void Scene_RunActorNinePresentationCycles(void)
{
    extern u8 gWork[];

    u8 *Scene_GetRecord_1();
    u8 *Scene_GetRecord_2();

    u32 i;
    u8 *rec9;
    u8 *record;

    Battle_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1(0);
    Effect_unk40_2(record, 0);
    Effect_unk41_2(1);
    Object_NotifyLastActiveOfEvent_1(0x200976c);
    Effect_unk42(1);
    Effect_unk43(9);
    Effect_unk44(10);
    Effect_unk45(11);
    Effect_unk46(12);
    Effect_unk47(13);
    Effect_unk48(14);
    Effect_unk49(15);
    Motion_EnableActCb_1(8, 0x200939c);
    SCENE_PHASE = 0x203;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Battle_WaitMode0_2(120);
    rec9 = Scene_GetRecord_2(9);
    Motion_EnableReset_1(9);
    /* Reset record 9's waypoint/velocity fields: three fields to the
     * minimum s32, then four fields to zero. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Battle_WaitMode0_3(20);
    Motion_SetSpeed_2(9, 0x80000, 0x40000);
    Object_SetPosition_1(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_1(rec9);
    Object_SetPosition_2(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_2(rec9);
    Object_SetPosition_3(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_3(rec9);
    Object_SetPosition_4(rec9, 0x900000, 0, 0xa90000);
    Motion_EnableReset_2(8);
    Effect_unk50(1);
    Motion_ArmCb_1(8, 0x8000, 0);
    BattleFx_SpawnLinked_1(8, 0x103, 60);
    Motion_SetSpeed_3(9, 0x20000, 0x10000);
    Effect_unk51(9);
    Battle_WaitMode0_4(20);
    Motion_EnableActCb_2(8, 0x200939c);
    Battle_WaitMode0_5(120);
    Motion_EnableReset_3(9);
    /* Same reset pattern on record 9 for the second cycle. */
    RECORD_S32(rec9, 56) = -0x80000000;
    RECORD_S32(rec9, 60) = -0x80000000;
    RECORD_S32(rec9, 64) = -0x80000000;
    RECORD_S32(rec9, 36) = 0;
    RECORD_S32(rec9, 40) = 0;
    RECORD_S32(rec9, 44) = 0;
    RECORD_S32(rec9, 76) = 0;
    Battle_WaitMode0_6(20);
    Motion_SetSpeed_4(9, 0x80000, 0x40000);
    Object_SetPosition_5(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition_4(rec9);
    Motion_SetSpeed_5(9, 0x50000, 0x28000);
    Object_SetPosition_6(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_5(rec9);
    Object_SetPosition_7(rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition_6(rec9);
    Object_SetPosition_8(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition_7(rec9);
    Object_SetPosition_9(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition_8(rec9);
    Object_SetPosition_10(rec9, 0x900000, 0, 0xa90000);
    Motion_EnableReset_4(8);
    Effect_unk52(1);
    Motion_ArmCb_2(8, 0x8000, 0);
    BattleFx_SpawnLinked_2(8, 0x103, 60);
    Motion_SetSpeed_6(9, 0x20000, 0x10000);
    Effect_unk53(9);
    Motion_Launch_1(8, 4, 20);
    Motion_Launch_2(8, 6, 40);
    Audio_PlayCue_1(29);
    GameFlag_Set_1(0x8f0);
    SceneWork_SetStepValue_1(0x1e49);
    Battle_RunThenWaitIfModeZero_1(16, 0, 20);
    BattleFx_SpawnLinked_3(8, 0x100, 0);
    Motion_SetPosReset_1(8, 164, 0x158);
    Battle_WaitMode0_7(40);
    Motion_SetVarCbObj_1(8, 2);
    Effect_unk54();
    Motion_SetSpeed_7();
    Effect_unk55(12);
    Battle_SchedShoulder_1();
}

void OvObj_InitWithRandomFields(s32 a)
{
    extern u8 gWork[];

    u8 *obj;
    u32 x;

    obj = Effect_unk11_4();
    Effect_Apply3(a, 1);
    obj[0x55] = 0;
    *(u16 *)(obj + 0x64) = Effect_unk56() >> 15;
    *(u16 *)(obj + 0x66) = Effect_unk57() >> 15;
    x = Effect_unk58();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    *(s32 *)(obj + 0xc) = x;
    x = Effect_unk59();
    *(s32 *)(obj + 0x4c) = ((x * 3 << 13) >> 16) - 0x3000;
    *(s32 *)(obj + 0x18) = 0x14000;
    *(s32 *)(obj + 0x1c) = 0x14000;
    Effect_Apply4(a, gOv6);
}

/* Drives ids 8 through 18 through position, scale, and flag updates in
 * sequence, stepping the shared scene phase at the start and end. */
void Scene_RunPrimarySequence(void)
{
    extern u8 gWork[];

    s32 Scene_GetRecord_1();
    s32 Scene_GetRecord_2();

    u32 i;
    s32 rec;
    s32 id0_state;

    BattleRuntime_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    id0_state = Scene_GetRecord_1(0);
    Effect_unk60(id0_state, 0);
    Effect_unk61(1);
    Object_NotifyLastActiveOfEvent_1(0x200976c);
    Effect_unk62(1);
    Object_NotifyLastActiveOfEvent_2(0x2009844);
    Effect_unk63(1);
    Effect_unk64(9);
    Effect_unk65(10);
    Effect_unk66(11);
    Effect_unk67(12);
    Effect_unk68(13);
    Effect_unk69(14);
    Effect_unk70(15);
    ObjectMotion_EnableActionAndSetCallback_1(8, 0x200939c);
    SCENE_PHASE = 0x203;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1();
    BattleRuntime_WaitIfModeZero_2(0x12c);
    Audio_PlayCue_1(147);
    BattleRuntime_WaitIfModeZero_3(100);
    ObjectMotion_EnableActionAndResetMotion_1(9);
    ObjectMotion_EnableActionAndResetMotion_2(10);
    ObjectMotion_EnableActionAndResetMotion_3(11);
    ObjectMotion_EnableActionAndResetMotion_4(12);
    Motion_EnableReset_5(13);
    Motion_EnableReset_6(14);
    Motion_EnableReset_7(15);
    ObjectMotion_SetSpeedParameters_2(9, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_3(10, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_4(11, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_5(12, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_6(13, 0x30000, 0x18000);
    ObjectMotion_SetSpeedParameters_7(14, 0x30000, 0x18000);
    Motion_SetSpeed_8(15, 0x30000, 0x18000);
    Motion_ResetAndSetPosition_1(9, 0, 100);
    Motion_ResetAndSetPosition_2(10, 60, 100);
    Motion_ResetAndSetPosition_3(11, 120, 100);
    Motion_ResetAndSetPosition_4(12, 180, 100);
    Motion_ResetAndSetPosition_5(13, 240, 100);
    Motion_ResetAndSetPosition_6(14, 0x140, 100);
    Motion_ResetAndSetPosition_7(15, 0x17c, 100);
    BattleRuntime_WaitIfModeZero_4(40);
    BattleEffect_SpawnLinkedResourceObject_1(8, 0x101, 0);
    BattleRuntime_WaitIfModeZero_5(20);
    Motion_SetHPosTerrain_1(9, 0, 0);
    Motion_SetHPosTerrain_2(10, 0, 0);
    Motion_SetHPosTerrain_3(11, 0, 0);
    Motion_SetHPosTerrain_4(12, 0, 0);
    Motion_SetHPosTerrain_5(13, 0, 0);
    Motion_SetHPosTerrain_6(14, 0, 0);
    Motion_SetHPosTerrain_7(15, 0, 0);
    BattleRuntime_WaitIfModeZero_6(100);
    rec = Scene_GetRecord_2(18);
    *(s32 *)(rec + 24) = 0x1999;
    *(s32 *)(rec + 28) = 0x1999;
    Motion_SetHPosTerrain_8(18, 0xac0000, 0x1540000);
    Motion_EnableReset_8(8);
    Effect_unk71(1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(8, 1);
    ObjectMotion_ArmCallback_1(8, 0x3000, 0);
    Audio_PlayCue_2(29);
    GameFlag_Set_1(0x8f0);
    for (i = 0; i < 32; i++) {
        *(s32 *)(rec + 24) += 0xccc;
        *(s32 *)(rec + 28) += 0xccc;
        Effect_unk72(1);
    }
    BattleEffect_SpawnLinkedResourceObject_2(8, 0x101, 60);
    Motion_SetVarCbObj_2(8, 2);
    ObjectMotion_SetPositionAndReset_1(8, 168, 0x154);
    Motion_SetPosReset_2(8, 200, 0x154);
    ObjectMotion_ArmCallback_2(8, 0x8000, 0);
    rec = Scene_GetRecord_3(17);
    *(s32 *)(rec + 24) = 0x12666;
    *(s32 *)(rec + 28) = 0x12666;
    *(s32 *)(rec + 8) = 0xac0000;
    *(s32 *)(rec + 12) = 0xa00000;
    *(s32 *)(rec + 16) = 0x1540000;
    {
        /* Clear the flag word at +6. */
        s32 shown = 0;

        *(u16 *)(rec + 6) = shown;
    }
    *(s32 *)(rec + 68) = 0x6666;
    *(s32 *)(rec + 72) = 0x30000;
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_Launch_1(8, 6, 20);
    Audio_PlayCue_3(147);
    Battle_WaitMode0_8(20);
    ObjectMotion_EnableActionAndSetCallback_2(8, 0x20093ac);
    Battle_WaitMode0_9(80);
    Motion_SetActionVariant_1(17, 1);
    Motion_SetSpeed_9(17, 0x10000, 0x8000);
    *(s32 *)(rec + 68) = 0x1999;
    *(s32 *)(rec + 72) = 0xb333;
    Audio_PlayCue_4(153);
    *(s32 *)(rec + 40) = 0x80000;
    Motion_ResetAndSetPosition_8(17, 132, 0x168);
    Motion_ResetAndSetPosition_9(18, 132, 0x168);
    Battle_WaitMode0_10(40);
    Motion_SetHPosTerrain_9(17, 0, 0);
    rec = Scene_GetRecord_4(8);
    *(s32 *)(rec + 24) = 0x10000;
    *(s32 *)(rec + 28) = 0x10000;
    {
        /* Set the flag word at +6. */
        s32 shown = 0x5000;

        *(u16 *)(rec + 6) = shown;
    }
    Battle_WaitMode0_11(40);
    SCENE_PHASE = 0x202;
    Effect_unk73();
    Motion_SetSpeed_10();
    Effect_unk74(13);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void Scene_RunSevenActorEnsemble(void)
{
    extern u8 *gWork;

    s32 ensemble;
    s32 selector;

    Effect_unk75();
    Effect_unk76(0, 15);
    ensemble = Effect_unk5(0);
    Effect_unk77(ensemble, 0);
    Effect_unk2_2(33593196);
    Effect_unk78(1);
    Effect_unk79(9);
    Effect_unk80(10);
    Effect_unk81(11);
    Effect_unk82(12);
    Effect_unk83(13);
    Effect_unk84(14);
    Effect_unk85(15);
    Effect_unk2_4(8, 33592220);
    *(s32 *)(gWork + 448) = 515;
    Effect_unk86();
    Effect_unk87();
    Effect_unk3_2(400);
    Effect_unk88(9);
    Effect_unk89(10);
    Effect_unk90(11);
    Effect_unk91(12);
    Effect_unk92(13);
    Effect_unk93(14);
    Effect_unk94(15);
    Effect_unk10_3(9, 196608, 98304);
    Effect_unk11_3(10, 196608, 98304);
    Effect_unk12_3(11, 196608, 98304);
    Effect_unk13_3(12, 196608, 98304);
    Effect_unk14_3(13, 196608, 98304);
    Effect_unk15_3(14, 196608, 98304);
    Effect_unk16_3(15, 196608, 98304);
    Effect_unk3_4(9, 33592400);
    Effect_unk4_4(10, 33592448);
    Effect_unk5_4(11, 33592496);
    Effect_unk6_4(12, 33592544);
    Effect_unk7_4(13, 33592592);
    Effect_unk8_4(14, 33592640);
    Effect_unk9_4(15, 33592688);
    Effect_unk95(40);
    Effect_unk96(8, 3);
    Effect_unk10_4(8, 258);
    Effect_unk97(120);
    Effect_unk98(8, 1);
    Effect_unk17_3(8, 256, 60);
    Effect_unk18_3(8, 65536, 32768);
    Effect_unk19_3(8, 164, 344);
    Effect_unk99(8, 4, 10);
    Effect_unk100(8, 6, 20);
    Effect_unk4_2(7908);
    Effect_unk101(8, 0, 20);
    *(s32 *)(gWork + 448) = 514;
    Effect_unk102();
    Effect_unk103();
    gCell[226] = (s32)&Value_0000006f;
    gCell[227] = 2;
    selector = Effect_unk6();
    if (selector == 11) {
        Effect_unk104(15);
    } else {
        Effect_unk105(14);
    }
    Effect_unk106();
}

s32 SceneData_GetDifferenceOfPairSums(void)
{
    extern u8 gWork[];

    s32 a;
    s32 b;

    a = Effect_unk7(0);
    a += Effect_unk8(2);
    b = Effect_unk9(1);
    b += Effect_unk10(3);
    return a - b;
}

s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
    extern u8 gWork[];

    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++) {
        if (Effect_unk11(base + i) != 0)
            return gOv7[i];
    }
    return 0;
}
