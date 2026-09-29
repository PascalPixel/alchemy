#include "TYPES.H"

extern u8 Data_0200ad3c[];
extern u8 Data_0200ad74[];
extern u8 Data_0200ade4[];

extern u8 Data_03001ebc[];
#define SCENE_WORK (*(u8 **)Data_03001ebc)
static __inline__ void bump_step(s32 amount)
{
    u16 *step = (u16 *)(*(u8 **)Data_03001ebc + 0x1d8);

    *step = *step + amount;
}
#define SCENE_FIELD_1C8 (*(s32 *)(SCENE_WORK + 0x1c8))

void Scene_CallPairWith10();
void State_ForwardMaskedHalfwordWith10();
s32 GameFlag_IsSet();
void Data_0200abc4();
void Battle_WaitMode0();
void Battle_Reset();
void Battle_SchedShoulder();
s32 UiWork_WaitThenFinalizeCapacity();
s32 Func_02003548();
void Func_0200351e();
void Motion_EnableActCb();
void Motion_MarkActiveAndSetActionCallback();
void Motion_ResetPosMode2();
void Motion_SetPosReset();
void Motion_SetHPosTerrain();
void Object_SetModeById();
void Motion_CallWaitAnim();
void Motion_SetVarCb();
void Motion_SetVarCbObj();
void SceneWork_SetStepValue();
void Func_020036ac();
void Func_020034c4();
void Motion_ArmCb();
void BattleFx_SpawnLinked();
void Func_020034ac();
void Motion_SetSpeedLim();
void Motion_CamBounds();
void Object_CommitPositionThenWaitIfModeZero();
u8 *RuntimeBlock_GetOffset1e0Pointer();
void Func_02003a98();
void Func_020039be();
void Motion_SetSpeed();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Runs only while flag 0x962 is set: resets the battle runtime, stages
 * presentation actors 17 to 20, places party actors 0 to 3 (taking optional
 * party record positions), then plays one of three closing sequences chosen by
 * two queries. The later two sequences first advance the scene step counter;
 * every sequence stores 40 in scene work field 0x1c8. */
void Scene_RunBranchingActorPresentation(void)
{
    s32 record;
    s32 value;
    u8 *slot;

    if (Value1(GameFlag_IsSet, 0x962) == 0) {
        return;
    }

    Battle_Reset();
    Motion_SetVarCbObj(17, 1);
    Call3(Motion_ArmCb, 17, 0x3000, 20);
    value = 192;
    Motion_ArmCb(17, 0, 60);
    Call3(BattleFx_SpawnLinked, 17, 0x100, 40);
    State_ForwardMaskedHalfwordWith10(17, (value << 6));
    Call1(SceneWork_SetStepValue, 0x2267);
    Motion_SetVarCb(17, 2);
    Scene_CallPairWith10(17);
    Motion_ArmCb(18, (value << 6), 0);
    Motion_ArmCb(19, (value << 6), 0);
    Value3(Motion_ArmCb, 20, (value << 6), 0);
    slot = RuntimeBlock_GetOffset1e0Pointer();
    *(u8 *)(slot + 85) = 0;
    Call2(Motion_SetSpeedLim, 0x19999, 0x3333);
    Call4(Motion_CamBounds, 0x1000000, -1, 0xac0000, 1);
    Object_CommitPositionThenWaitIfModeZero();
    Battle_WaitMode0(20);
    Call3(BattleFx_SpawnLinked, 18, 0x102, 40);
    Scene_CallPairWith10(18);
    Motion_CallWaitAnim(17, 3);
    Scene_CallPairWith10(17);
    Call2(Func_020034ac, 19, 0x102);
    Battle_WaitMode0(40);
    State_ForwardMaskedHalfwordWith10(19, 0);
    Scene_CallPairWith10(19);
    Call3(BattleFx_SpawnLinked, 20, 0x103, 40);
    State_ForwardMaskedHalfwordWith10(20, 0);
    Motion_SetVarCb(20, 2);
    Func_020034c4(20, 0, 20);
    Motion_SetVarCbObj(17, 1);
    Battle_WaitMode0(10);
    Call3(Motion_ArmCb, 17, 0xb000, 20);
    Func_020034c4(17, 0, 20);
    Motion_SetVarCbObj(18, 1);
    Motion_CallWaitAnim(18, 4);
    Battle_WaitMode0(20);
    State_ForwardMaskedHalfwordWith10(17, 0x8000);
    Func_020034c4(17, 0, 20);
    Motion_SetVarCbObj(19, 1);
    Battle_WaitMode0(40);
    Motion_CallWaitAnim(19, 4);
    Func_020034c4(17, 0, 20);
    Motion_SetVarCbObj(20, 2);
    Battle_WaitMode0(40);
    Motion_CallWaitAnim(20, 3);
    Battle_WaitMode0(20);
    Object_SetModeById(17, 3);
    Object_SetModeById(18, 3);
    Object_SetModeById(19, 3);
    Motion_CallWaitAnim(20, 3);
    Motion_ArmCb(17, (value << 6), 0);
    Motion_ArmCb(18, (value << 6), 0);
    Motion_ArmCb(19, (value << 6), 0);
    State_ForwardMaskedHalfwordWith10(20, (value << 6));
    Call3(Func_0200351e, 17, 0x9999, 0x4ccc);
    Call3(Motion_SetPosReset, 17, 0x102, 172);
    Call3(Func_0200351e, 0, 0xcccc, 0x6666);
    Call3(Motion_SetPosReset, 0, 0x106, 188);
    Call3(Motion_ArmCb, 0, 0xc000, 0);
    record = Value1(Func_02003548, 0);
    if (record != 0) {
        Motion_SetHPosTerrain(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003548, 0);
    if (record != 0) {
        Motion_SetHPosTerrain(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003548, 0);
    if (record != 0) {
        Motion_SetHPosTerrain(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_0200351e, 1, 0xcccc, 0x6666);
    Call3(Func_0200351e, 2, 0xcccc, 0x6666);
    Call3(Func_0200351e, 3, 0xcccc, 0x6666);
    Motion_ResetPosMode2(1, 246, 200);
    Call3(Motion_ResetPosMode2, 2, 0x106, 200);
    Call3(Motion_SetPosReset, 3, 0x116, 200);
    Object_SetModeById(2, 1);
    Object_SetModeById(1, 1);
    Call3(Motion_ArmCb, 1, 0xc000, 0);
    Call3(Motion_ArmCb, 2, 0xc000, 0);
    Call3(Motion_ArmCb, 3, 0xc000, 40);
    Motion_SetVarCbObj(17, 1);
    Scene_CallPairWith10(17);
    Motion_SetVarCb(0, 2);
    Motion_SetVarCb(1, 2);
    Motion_SetVarCb(2, 2);
    Motion_SetVarCbObj(3, 2);
    Motion_CallWaitAnim(17, 3);
    Value2(Func_020036ac, 17, 0);
    Call3(Motion_ArmCb, 1, 0xe000, 0);
    Call3(Motion_ArmCb, 3, 0xa000, 0);

    if (Value2(UiWork_WaitThenFinalizeCapacity, 0, 0) == 0) {
        Motion_CallWaitAnim(17, 3);
        Scene_CallPairWith10(17);
        Motion_EnableActCb(1, (s32)Data_0200ad3c);
        Value2(Motion_EnableActCb, 2, (s32)Data_0200ad3c);
        Value2(Motion_MarkActiveAndSetActionCallback, 3, (s32)Data_0200ad3c);
        Call2(Motion_SetSpeedLim, 0x6666, 0xccc);
        Call4(Motion_CamBounds, 0x1000000, -1, 0x640000, 1);
        Call3(Func_0200351e, 17, 0x10000, 0x8000);
        Motion_EnableActCb(17, (s32)Data_0200ade4);
        Battle_WaitMode0(10);
        Call3(Func_0200351e, 0, 0x10000, 0x8000);
        Value2(Motion_EnableActCb, 0, (s32)Data_0200ade4);
        Battle_WaitMode0(80);
        SCENE_FIELD_1C8 = 40;
        Func_020039be();
        Motion_SetSpeed();
    } else {
        bump_step(1);
        Call3(BattleFx_SpawnLinked, 17, 0x102, 40);
        Scene_CallPairWith10(17);
        Call3(BattleFx_SpawnLinked, 18, 0x102, 40);
        Scene_CallPairWith10(18);
        Call3(BattleFx_SpawnLinked, 19, 0x102, 40);
        Scene_CallPairWith10(19);
        Call3(BattleFx_SpawnLinked, 20, 0x102, 40);
        Scene_CallPairWith10(19);
        Motion_CallWaitAnim(17, 4);
        Scene_CallPairWith10(17);
        Motion_SetVarCbObj(2, 1);
        Battle_WaitMode0(20);
        Scene_CallPairWith10(2);
        Call3(BattleFx_SpawnLinked, 3, 0x102, 40);
        Scene_CallPairWith10(3);
        Call3(Motion_ArmCb, 17, 0xb000, 0);
        Motion_ArmCb(19, 0x8000, 0);
        Motion_ArmCb(20, 0, 60);
        Motion_ArmCb(17, (value << 6), 0);
        Motion_ArmCb(19, (value << 6), 0);
        Motion_ArmCb(20, (value << 6), 20);
        Object_SetModeById(17, 3);
        Object_SetModeById(18, 3);
        Object_SetModeById(19, 3);
        Motion_CallWaitAnim(20, 3);
        Call3(BattleFx_SpawnLinked, 3, 0x100, 60);
        Call2(State_ForwardMaskedHalfwordWith10, 3, 0xa000);
        Scene_CallPairWith10(3);
        Object_SetModeById(1, 4);
        Battle_WaitMode0(20);
        Value2(Func_020036ac, 1, 0);

        record = Value2(UiWork_WaitThenFinalizeCapacity, 0, 0);
        if (record == 0) {
            Motion_CallWaitAnim(17, 3);
            Scene_CallPairWith10(17);
            Motion_EnableActCb(1, (s32)Data_0200ad3c);
            Value2(Motion_EnableActCb, 2, (s32)Data_0200ad3c);
            Value2(Motion_MarkActiveAndSetActionCallback, 3, (s32)Data_0200ad3c);
            Call2(Motion_SetSpeedLim, 0x6666, 0xccc);
            Call4(Motion_CamBounds, 0x1000000, -1, 0x640000, 1);
            Call3(Func_0200351e, 17, 0x10000, 0x8000);
            Motion_EnableActCb(17, (s32)Data_0200ade4);
            Battle_WaitMode0(10);
            Call3(Func_0200351e, 0, 0x10000, 0x8000);
            Value2(Motion_EnableActCb, 0, (s32)Data_0200ade4);
            Battle_WaitMode0(80);
            SCENE_FIELD_1C8 = 40;
            Func_020039be();
            Motion_SetSpeed();
        } else {
            bump_step(1);
            Battle_WaitMode0(20);
            Motion_SetVarCb(1, 2);
            Func_020034c4(1, 0, 20);
            Call3(BattleFx_SpawnLinked, 2, 0x102, 60);
            Scene_CallPairWith10(2);
            State_ForwardMaskedHalfwordWith10(3, 0x8000);
            Object_SetModeById(3, 3);
            Scene_CallPairWith10(3);
            Motion_EnableActCb(2, (s32)Data_0200ad74);
            Value2(Motion_MarkActiveAndSetActionCallback, 3, (s32)Data_0200ad74);
            Battle_WaitMode0(20);
            Value2(Motion_MarkActiveAndSetActionCallback, 0, (s32)Data_0200ad74);
            Call3(Motion_SetPosReset, 1, 0x106, 188);
            Value2(State_ForwardMaskedHalfwordWith10, 1, 0xc000);
            Motion_CallWaitAnim(1, 3);
            Scene_CallPairWith10(1);
            Call2(Motion_SetSpeedLim, 0x6666, 0xccc);
            Call4(Motion_CamBounds, 0x1000000, -1, 0x640000, 1);
            Call3(Func_0200351e, 17, 0x10000, 0x8000);
            Motion_EnableActCb(17, (s32)Data_0200ade4);
            Battle_WaitMode0(10);
            Call3(Func_0200351e, 1, 0x10000, 0x8000);
            Value2(Motion_EnableActCb, 1, (s32)Data_0200ade4);
            Battle_WaitMode0(80);
            SCENE_FIELD_1C8 = 40;
            Func_020039be();
            Motion_SetSpeed();
        }
    }

    Func_02003a98(2);
    Call1(Data_0200abc4, 0x93f);
    Battle_SchedShoulder();
}
