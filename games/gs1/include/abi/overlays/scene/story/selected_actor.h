#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_SELECTED_ACTOR
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_SELECTED_ACTOR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02001db4(void *, s32);
void Func_02001dcc(void *, s32);
void Func_02001e5c(void *, s32);
void Func_02002152();
void Func_02001fc0();
s32 Func_020021e6();
s32 Func_020021f0();
void Func_02002200();
void Func_02002202();
void Func_02002208();
void Func_02002238();
void Func_020022c4();
void Func_02002306();
void Func_02002a04();
void Func_02002a54();
void Func_02002a5e();
s32 Func_0200291c();
s32 Func_020029d4();
void Func_020029e6();
void Func_02002a02();
void Func_02002a26();
void Func_02002a8a();
void Func_02002a9a();
void Func_02002512();
void Func_02003160();
s32 Func_02003208();
s32 Func_02003216();
void Func_02003216_a();
void Func_0200322e();
s32 Func_02003270();
s32 Func_02003278();
s32 Func_02003280();
s32 Func_0200328c();
void Func_0200328e();
void Func_020032a6();
s32 Func_020032c4();
void Func_020032ce();
s32 Func_020032dc();
void Func_020032f6();
void Func_020032f8();
void Func_020032fc();
void Func_0200330e();
void Func_02003324();
s32 Func_0200332c();
void Func_02003334();
void Func_02003338();
void Func_0200333a();
void Func_02003348();
void Func_02003366();
void Func_0200337e();
void Func_02003380();
void Func_02003398();
void Func_0200339a();
void Func_02003412();
void Func_020035fe();
void Func_02003620();
void Func_02003636();
void Func_02003648();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call3_02001874(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define CalculatePlanarDistance Func_02000e00
#define CreateOverlayObject Func_02001dae
#define EffectDescriptorTable Data_0200a120
#define Effect_SpawnConfiguredEffect Func_0200013c
#define GetPartyEffect Func_02001ed2
#define OvObj_CreateConfiguredObject Func_020000a0
#define OvObj_PrepareObject Func_02000048
#define OvObj_WaitUntilField12BelowLimit Func_02000324
#define ScaleEffectDeltaFromAccumulated Func_02001f00
#define ScaleEffectDeltaFromOrigin Func_02001f18
#define ScaleEffectVerticalDelta Func_02001f26
#define SceneData_GetTableA500 Func_020004dc
#define SceneData_GetTablea36c Func_02000414
#define SceneData_ReturnZero Func_02000410
#define SceneData_SelectTableByScene59To5c Func_020003ac
#define Scene_ApplyActor13Values3And3 Func_02000314
#define Scene_RunFlags8B2And8B3Steps Func_02000498
#define Scene_RunScene3a5 Func_02001874
#define SetEffectCallbackArgument Func_02001fb4
#define SetEffectCallbackMode Func_02001fa4
#define SetEffectDescriptor Func_02001ea4
#define SetEffectMode Func_02001fee
#define SetEffectVariant Func_02001e8a
#define SetOverlayObjectMode Func_02001e10
#define SetOverlayObjectSlot Func_02001eb8
#define SpawnEffect Func_02001e80
#define State_SetHalfwordB030 Func_02001c78
#define Story_Apply Func_02001db4
#define Story_Apply2 Func_02001e5c
#define Story_Apply3 Func_02001dcc
#define Story_Check10 Func_020032c4
#define Story_Check11 Func_020032dc
#define Story_Check12 Func_0200332c
#define Story_Check8 Func_02003280
#define Story_Check9 Func_0200328c
#define Story_Run10 Func_02002a04
#define Story_Run11 Func_02002a54
#define Story_Run12 Func_02002a5e
#define Story_Run13 Func_020029e6
#define Story_Run14 Func_02002a02
#define Story_Run15 Func_02002a8a
#define Story_Run16 Func_02002a9a
#define Story_Run17 Func_02002a26
#define Story_Run18 Func_02002512
#define Story_Run19 Func_02003160
#define Story_Run20 Func_020032ce
#define Story_Run21 Func_020032f8
#define Story_Run22 Func_0200328e
#define Story_Run23 Func_020032f6
#define Story_Run24 Func_020032a6
#define Story_Run25 Func_02003334
#define Story_Run26 Func_0200333a
#define Story_Run27 Func_02003348
#define Story_Run28 Func_0200330e
#define Story_Run29 Func_02003324
#define Story_Run3 Func_02001d56
#define Story_Run30 Func_02003412
#define Story_Run31 Func_0200339a
#define Story_Run32 Func_02003338_a
#define Story_Run33 Func_02003380
#define Story_Run34 Func_02003398
#define Story_Run35 Func_0200328e_a
#define Story_Run36 Func_02003636
#define Story_Run37 Func_02003620
#define Story_Run38 Func_02003648
#define Story_Run4 Func_02002152
#define Story_Run5 Func_02001fc0
#define Story_Run6 Func_02002200
#define Story_Run7 Func_02002306
#define Story_Run8 Func_020022c4
#define Story_Run9 Func_02002238
#define gCell Data_02000240
#define gOv Data_0200a174
#define gOv2 Data_0200a1d4
#define gOv3 Data_0200a234
#define gOv4 Data_0200a2dc
#define gOv5 Data_0200a12c
#define gVal Data_0000005b
#define gWork Data_03001ebc
#define Story_Check(a0) Value1(Func_020021e6, a0)
#define Story_Check2(a0) Value1(Func_020021f0, a0)
#define Story_Do(a0) Call1(Func_02002202, a0)
#define Story_Do2(a0) Call1(Func_02002208, a0)
#define Story_Run() Value0(Func_0200291c)
#define Story_Check3(a0) Value1(Func_020029d4, a0)
#define Story_Check4(a0) Value1(Func_02003208, a0)
#define Story_Check5(a0) Value1(Func_02003216, a0)
#define Story_Do3(a0) Call1(Func_0200322e, a0)
#define Story_Check6(a0) Value1(Func_02003270, a0)
#define Story_Check7(a0) Value1(Func_02003278, a0)
#define Story_Run2(a0, a1) Call2(Func_02003338, a0, a1)
#define Story_Place(a0, a1, a2) Call3(Func_02003216_a, a0, a1, a2)
#define Story_Place2(a0, a1, a2) Call3(Func_020032fc, a0, a1, a2)
#define Story_Place3(a0, a1, a2) Call3(Func_02003366, a0, a1, a2)
#define Story_Place4(a0, a1, a2) Call3(Func_0200337e, a0, a1, a2)
#define Story_Place5(a0, a1, a2) Call3_02001874(Func_020035fe, a0, a1, a2)

#endif
