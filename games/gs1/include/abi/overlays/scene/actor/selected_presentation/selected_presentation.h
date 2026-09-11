#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_SELECTED_PRESENTATION_SELECTED_PRESENTATION
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_SELECTED_PRESENTATION_SELECTED_PRESENTATION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020009c2(int actor);
void Func_020009ce(int actor);
void Func_020009da(int actor);
void Func_020009e6(int actor);
void Func_020009f2(int actor);
void Func_02000d96(int actor);
void Func_02000da2(int actor);
void Func_02000dae(int actor);
void Func_02000dba(int actor);
void Func_02000dc6(int actor);
s32 Func_02001d2e(s32);
void Func_02001d40(s32);
void Func_02001d9e(s32, const void *);
void Func_02001dc6(s32, s32, s32);
void Func_020012bc(void *, s32);
void Func_020012d4(void *, s32);
void Func_0200137c(void *, s32);
s32 Func_020016d4();
s32 Func_020016da();
s32 Func_0200170c();
void Func_02001714();
void Func_02001714_a();
void Func_0200171c();
s32 Func_0200173a();
void Func_02001762();
void Func_0200176c();
void Func_0200177c();
void Func_02001784();
void Func_0200178a();
void Func_0200178e();
void Func_020017aa();
void Func_020017ba();
void Func_020017c4();
void Func_02001840();
s32 Func_02001a8c();
s32 Func_02001a98();
s32 Func_02001aa6();
void Func_02001aba();
void Func_02001ae6();
void Func_02001b50();
void Func_02001b5a();
void Func_02001b6c();
void Func_02001b74();
void Func_02001b76();
void Func_02001b7c();
void Func_02001ba0();
void Func_02001bb4();
void Func_02001bbe();
void Func_02001be0();
void Func_02001be4();
void Func_02001be4_a();
void Func_02001bee();
void Func_02001bee_a();
void Func_02001c00();
void Func_02001c06();
void Func_02001c10();
void Func_02001c16();
void Func_02001c24();
void Func_02001c24_a();
void Func_02001c30();
void Func_02001c32();
void Func_02001c32_a();
void Func_02001c44();
void Func_02001c44_a();
void Func_02001c54();
void Func_02001c54_a();
void Func_02001c54_b();
void Func_02001c54_c();
void Func_02001c62();
void Func_02001c74();
void Func_02001c74_a();
void Func_02001fa2();
s32 Func_02001fa8();
void Func_02001fbe();
s32 Func_02001fca();
s32 Func_02001fd2();
s32 Func_02001fd6();
void Func_02002026();
void Func_020020d4();
s32 Func_02002042();
void Func_0200204a();
s32 Func_0200206e();
void Func_02002070();
s32 Func_0200207a();
void Func_020020ce();
void Func_02002180();
void Func_0200218c();
s32 Func_02001686(s32 state);
s32 Func_0200168c(s32 flag);
void Func_020016bc(s32 state, s32 value);
void Func_02001660();

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

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1_02000838(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value1_02000d24(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value2_02000dc8(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

#define Actor Func_02000400
#define Actor_Apply Func_02001d9e
#define Actor_Apply2 Func_020012bc
#define Actor_Apply3 Func_0200137c
#define Actor_Apply4 Func_020012d4
#define Actor_Apply5 Func_020016bc
#define Actor_Check10 Func_0200206e
#define Actor_Check11 Func_0200207a
#define Actor_Check12 Func_02001686
#define Actor_Check13 Func_0200168c
#define Actor_Check7 Func_02001d2e
#define Actor_Check8 Func_02001fca
#define Actor_Check9 Func_02001fd6
#define Actor_Do10 Func_02000dba
#define Actor_Do11 Func_02000dc6
#define Actor_Do12 Func_02001d40
#define Actor_Do2 Func_020009c2
#define Actor_Do3 Func_020009ce
#define Actor_Do4 Func_020009da
#define Actor_Do5 Func_020009e6
#define Actor_Do6 Func_020009f2
#define Actor_Do7 Func_02000d96
#define Actor_Do8 Func_02000da2
#define Actor_Do9 Func_02000dae
#define Actor_Place3 Func_02001dc6
#define Actor_Run10 Func_02001840
#define Actor_Run11 Func_0200171c
#define Actor_Run12 Func_020017ba
#define Actor_Run13 Func_020017aa
#define Actor_Run14 Func_02000d24
#define Actor_Run15 Func_02002026
#define Actor_Run16 Func_020020d4
#define Actor_Run17 Func_02000dc8
#define Actor_Run18 Func_0200218c
#define Actor_Run19 Func_0200169a
#define Actor_Run20 Func_02000400
#define Actor_Run21 Func_02001660
#define Actor_Run4 Func_0200127e
#define Actor_Run5 Func_02001714
#define Actor_Run6 Func_02001714_a
#define Actor_Run7 Func_0200177c
#define Actor_Run8 Func_0200176c
#define Actor_Run9 Func_0200178a
#define CreateOverlayObject Func_020012d6
#define DefaultOverlayData Data_020095f8
#define EffectDescriptorTable Data_02009424
#define EncounterPalette_Pulse Func_02000ce4
#define GetPartyEffect Func_020013ea
#define OvObj_CreateAndInitialize Func_02000048
#define OvObj_CreateConfigured Func_020000a0
#define PlaceActorTwelveWhenFlagClear Func_02000adc
#define PrimaryOverlayData Data_02009610
#define RuntimeSelectorTable Data_02000240
#define ScaleEffectDeltaFromAccumulated Func_02001468
#define ScaleEffectDeltaFromOrigin Func_02001480
#define ScaleEffectVerticalDelta Func_0200148e
#define SceneData_GetTable95c0 Func_020003a4
#define SceneData_ReturnZero Func_020003a0
#define SceneData_SelectDataByRuntimeSelector Func_0200034c
#define SceneData_SelectOverlayDataByRuntimeSelector Func_020003ac
#define SceneData_SelectTable99C4Or97B4ByStateA6 Func_02000e5c
#define SceneWork Func_02000400
#define Scene_RunActor10Step Func_02000570
#define Scene_RunActor11Step Func_0200057c
#define Scene_RunActor12Step Func_02000588
#define Scene_RunActor8Step Func_02000558
#define Scene_RunActor9Step Func_02000564
#define Scene_RunActorThirteenRestoration Func_02000838
#define Scene_RunLateActor10Step Func_02000814
#define Scene_RunLateActor11Step Func_02000820
#define Scene_RunLateActor12Step Func_0200082c
#define Scene_RunLateActor8Step Func_020007fc
#define Scene_RunLateActor9Step Func_02000808
#define Scene_RunMiddleAuxiliarySequence Func_02000464
#define SecondaryOverlayData Data_020096b8
#define Selection Func_02000400
#define SetEffectCallbackArgument Func_020014dc
#define SetEffectCallbackMode Func_020014cc
#define SetEffectDescriptor Func_020013cc
#define SetEffectMode Func_0200150e
#define SetEffectVariant Func_020013b2
#define SetOverlayObjectMode Func_02001318
#define SetOverlayObjectSlot Func_020013d8
#define SpawnConfiguredEffect Func_0200013c
#define SpawnEffect Func_020013a8
#define State_SendRequest15With45 Func_02000314
#define State_SetStateByte52 Func_02000b14
#define TertiaryOverlayData Data_02009790
#define gCell Data_02000240_t
#define gIw Data_03001e40
#define gOv Data_020099c4
#define gOv2 Data_020097b4
#define gOv3 Data_02009a00
#define gVal Data_000000a5
#define gWork Data_03001ebc
#define GameFlag_IsSet_1(a0) Value1(Func_02001a8c, a0)
#define GameFlag_IsSet_2(a0) Value1(Func_02001a98, a0)
#define GameFlag_IsSet_3(a0) Value1(Func_02001aa6, a0)
#define GameFlag_Set_1(a0) Call1(Func_02001aba, a0)
#define Battle_Reset_1(args...) Func_02001ae6(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02001b6c, a0)
#define Scene_GetRecord_1(args...) Func_02001b02(args)
#define Motion_SetHPosTerrain_1(args...) Func_02001b50(args)
#define Motion_SetAngleToward_1(a0, a1, a2) Call3(Func_02001b7c, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02001b5a, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02001bbe, a0, a1, a2)
#define Motion_SetPosReset_2(a0, a1, a2) Call3(Func_02001b74, a0, a1, a2)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_02001be0, a0, a1, a2)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_02001be4, a0, a1, a2)
#define Motion_CallWaitAnim_1(args...) Func_02001bb4(args)
#define BattleEv_RunWait_1(args...) Func_02001be4_a(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_02001c06, a0, a1, a2)
#define BattleFx_SpawnLinked_3(a0, a1, a2) Call3(Func_02001c10, a0, a1, a2)
#define BattleEv_RunWait_2(args...) Func_02001c00(args)
#define Battle_WaitMode0_1(args...) Func_02001b76(args)
#define Motion_SetVarCbObj_1(args...) Func_02001bee(args)
#define BattleEv_RunWait_3(args...) Func_02001c16(args)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_02001c32, a0, a1, a2)
#define BattleEventRuntime_ProcessAction_1(args...) Func_02001c32_a(args)
#define Battle_WaitMode0_2(args...) Func_02001ba0(args)
#define BattleFx_SpawnLinked_4(a0, a1, a2) Call3(Func_02001c54, a0, a1, a2)
#define BattleEv_RunWait_4(args...) Func_02001c44(args)
#define Motion_CallWaitAnim_2(args...) Func_02001c24(args)
#define BattleEv_RunWait_5(args...) Func_02001c54_a(args)
#define Motion_SetSpeed_1(a0, a1, a2) Call3(Func_02001bee_a, a0, a1, a2)
#define Motion_SetPosReset_3(a0, a1, a2) Call3(Func_02001c24_a, a0, a1, a2)
#define BattleEv_RunWait_6(args...) Func_02001c74(args)
#define Motion_CallWaitAnim_3(args...) Func_02001c54_b(args)
#define Object_SetModeById_1(args...) Func_02001c54_c(args)
#define Scene_GetRecord_2(args...) Func_02001c12(args)
#define Motion_ResetAndSetPosition_1(args...) Func_02001c44_a(args)
#define Motion_CommitPos_1(args...) Func_02001c62(args)
#define Motion_SetHPosTerrain_2(args...) Func_02001c74_a(args)
#define Battle_SchedShoulder_1(args...) Func_02001c30(args)
#define Actor_Check(a0) Value1(Func_0200170c, a0)
#define Actor_Check2(a0) Value1(Func_020016da, a0)
#define Actor_Run(a0, a1) Call2(Func_020017c4, a0, a1)
#define Actor_Check3(a0) Value1(Func_0200173a, a0)
#define Actor_Check4(a0, a1) Value2(Func_020016d4, a0, a1)
#define Actor_Do(a0) Call1(Func_02001762, a0)
#define Actor_Run2(a0, a1) Call2(Func_02001784, a0, a1)
#define Actor_Run3(a0, a1) Call2(Func_0200178e, a0, a1)
#define Actor_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02001fa2, a0, a1, a2, a3, a4, a5)
#define Actor_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_02001fbe, a0, a1, a2, a3, a4, a5)
#define Actor_SetRect3(a0, a1, a2, a3, a4, a5) Value6(Func_02001fd2, a0, a1, a2, a3, a4, a5)
#define Actor_Check5(a0) Value1_02000d24(Func_02001fa8, a0)
#define Actor_Place(a0, a1, a2) Call3(Func_020020ce, a0, a1, a2)
#define Actor_SetRect4(a0, a1, a2, a3, a4, a5) Call6(Func_0200204a, a0, a1, a2, a3, a4, a5)
#define Actor_Place2(a0, a1, a2) Call3(Func_02002180, a0, a1, a2)
#define Actor_SetRect5(a0, a1, a2, a3, a4, a5) Call6(Func_02002070, a0, a1, a2, a3, a4, a5)
#define Actor_Check6(a0, a1) Value2_02000dc8(Func_02002042, a0, a1)

#endif
