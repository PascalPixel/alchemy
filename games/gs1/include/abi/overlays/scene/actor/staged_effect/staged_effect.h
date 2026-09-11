#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_EFFECT_STAGED_EFFECT
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_EFFECT_STAGED_EFFECT

/* Binding layer — not production source. Address / far-call ABI only. */

u16 Func_0200317c(s32 deltaZ, s32 deltaX);
void Func_020032f2(s32 id, s32 angle, s32 frames);
s32 Func_02004054();
void Func_0200250c(void *, s32);
void Func_02002524(void *, s32);
void Func_020025e4(void *, s32);
void Func_02004026();
void Func_02004038();
void Func_02004824(u8 *);
void Func_02003362(s32 cue);
void Func_0200338c();
void Func_020033ec();
void Func_0200336c();
void Func_020033a8();
void Func_02000e6a();
s32 Func_020030ea();
void Func_0200340a();
void Func_0200342a();
void Func_02003452();
void Func_02003458();
s32 Func_02003464();
void Func_02003470();
void Func_0200348e();
void Func_020034c2();
void Func_020034d2();
void Func_02003568();
void Func_020011e8();
s32 Func_0200345a();
s32 Func_02003466();
s32 Func_0200346c();
s32 Func_02003474();
s32 Func_020034cc();
s32 Func_020034d2_a();
s32 Func_020034e4();
void Func_020034f0();
void Func_020034fe();
void Func_02003512();
void Func_02003560();
void Func_0200357c();
void Func_02003586();
void Func_02003592();
void Func_0200359c();
void Func_020035da();
void Func_020035ea();
void Func_020035ee();
void Func_020035f0();
void Func_020035fe();
void Func_02003604();
void Func_0200366e();
void Func_020036c6();
void Func_020036d6();
void Func_020036dc();
s32 Func_0200365e();
void Func_02003648();
void Func_0200368c();
void Func_02003696();
s32 Func_020036d2();
void Func_020036ee();
s32 Func_020036fa();
void Func_020036fc();
void Func_02003728();
s32 Func_02003748();
void Func_02003772();
s32 Func_0200377a();
s32 Func_02003782();
void Func_020037a8();
void Func_020037aa();
void Func_020037c2();
void Func_020037e4();
void Func_0200383a();
void Func_0200384e();
void Func_02003884();
void Func_02003884_a();
void Func_020038b2();
void Func_020038cc();
s32 Func_02003e30();
void Func_02003e44();
void Func_02003e60();
s32 Func_02003e76();
s32 Func_02003e80();
s32 Func_02003e88();
void Func_02003ea6();
void Func_02003ebe();
void Func_02003ece();
void Func_02003ed8();
void Func_02003edc();
void Func_02003f00();
void Func_02003f08();
void Func_02003f1c();
void Func_02003f20();
void Func_02003f28();
void Func_02003f2c();
void Func_02003f3e();
void Func_02003f46();
void Func_02003f9c();
void Func_02003fb0();
void Func_02001e82(int page);
void Func_02001e8e(int page);
void Func_02001e9a(int page);
void Func_020036ee_a();
void Func_02003622();
void Func_0200365a();
void Func_02002e58(void);
void Func_02002d50(void);
void Func_020031d4(void);
void Func_02002eac(void);
s32 Func_02002908(struct StagedActor *arg0, s32 *arg1);
void Func_020028d8(struct StagedActor *arg0, s32 arg1);
void Func_02002890(s32 arg0);
void Func_02002abe(s32 arg0);
void Func_0200291e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200292e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200293c(struct StagedActor *arg0);
void Func_02002ae0(void);
void Func_02002944(struct StagedActor *arg0, s32 arg1);
s32 Func_02002a8a(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_02002cc4(u8 *, s32 *);
void Func_020033ba(void);
void Func_020034f8(s32);
void Func_02003428(s32, s32);
void Func_0200341c(s32, s32, s32);
void Func_0200342a_a(s32);
void Func_02003462(s32, s32);
s32 Func_02003400(s32);
void Func_020033a6(s32, s32);
void Func_020033f8(s32);
void Func_02003504(void);
void Func_02003510(void);
void Func_020034d6(s32);
void Func_0200341a(void);
void Func_020044ca(s32 arg0);
void Func_02004456(s32 *object, s32 arg1);
void Func_02004456_a(s32 *object, s32 arg1);
void Func_02000ed0(s32 a)
{
    u8 *v = Func_020033c4(0);

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ s32 Value1_0200116c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value1_02001208(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3_02001208(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call1_0200196c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_0200196c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define AcquireOverlayObject Func_020024a6
#define Actor_Apply Func_0200317c
#define Actor_Apply2 Func_02002cc4
#define Actor_Apply3 Func_02003428
#define Actor_Apply4 Func_02003462
#define Actor_Apply5 Func_020033a6
#define Actor_Apply6 Func_02004456
#define Actor_Apply7 Func_02004456_a
#define Actor_Check10 Func_020030ea
#define Actor_Check11 Func_02003464
#define Actor_Check12 Func_0200345a
#define Actor_Check13 Func_02003466
#define Actor_Check14 Func_0200346c
#define Actor_Check15 Func_02003748
#define Actor_Check16 Func_02003782
#define Actor_Check17 Func_02003e76
#define Actor_Check18 Func_02003e88
#define Actor_Check19 Func_02003400
#define Actor_Check9 Func_02004054
#define Actor_Do10 Func_0200342a_a
#define Actor_Do11 Func_020033f8
#define Actor_Do12 Func_020034d6
#define Actor_Do13 Func_020044ca
#define Actor_Do4 Func_02004824
#define Actor_Do5 Func_02003362
#define Actor_Do6 Func_02001e82
#define Actor_Do7 Func_02001e8e
#define Actor_Do8 Func_02001e9a
#define Actor_Do9 Func_020034f8
#define Actor_EntOp Func_02002a8a
#define Actor_FaceLeaderWhileGrounded Func_02000d58
#define Actor_Place7 Func_020032f2
#define Actor_Place8 Func_02000da6
#define Actor_Place9 Func_0200341c
#define Actor_Run10 Func_02004038
#define Actor_Run11 Func_0200338c
#define Actor_Run12 Func_020033ec
#define Actor_Run13 Func_0200336c
#define Actor_Run14 Func_020033a8
#define Actor_Run15 Func_02000e6a
#define Actor_Run16 Func_0200342a
#define Actor_Run17 Func_02003568
#define Actor_Run18 Func_020034d2
#define Actor_Run19 Func_0200340a
#define Actor_Run20 Func_02003458
#define Actor_Run21 Func_020034c2
#define Actor_Run22 Func_02003470
#define Actor_Run23 Func_020034f0
#define Actor_Run24 Func_020035f0
#define Actor_Run25 Func_020034fe
#define Actor_Run26 Func_02003604
#define Actor_Run27 Func_02003512
#define Actor_Run28 Func_0200366e
#define Actor_Run29 Func_020011e8
#define Actor_Run30 Func_02003586
#define Actor_Run31 Func_020036d6
#define Actor_Run32 Func_0200359c
#define Actor_Run33 Func_020036dc
#define Actor_Run34 Func_020035ea
#define Actor_Run35 Func_020035fe
#define Actor_Run36 Func_02003648
#define Actor_Run37 Func_0200368c
#define Actor_Run38 Func_02003696
#define Actor_Run39 Func_020036fc
#define Actor_Run40 Func_020037aa
#define Actor_Run41 Func_020036ee
#define Actor_Run42 Func_0200383a
#define Actor_Run43 Func_0200384e
#define Actor_Run44 Func_02003884
#define Actor_Run45 Func_02003884_a
#define Actor_Run46 Func_020037e4
#define Actor_Run47 Func_02003728
#define Actor_Run48 Func_020038b2
#define Actor_Run49 Func_020038cc
#define Actor_Run50 Func_020037a8
#define Actor_Run51 Func_02003e60
#define Actor_Run52 Func_02003f9c
#define Actor_Run53 Func_02003fb0
#define Actor_Run54 Func_02003ea6
#define Actor_Run55 Func_02003f28
#define Actor_Run56 Func_02003f20
#define Actor_Run57 Func_02003ebe
#define Actor_Run58 Func_02003f00
#define Actor_Run59 Func_02003ece
#define Actor_Run6 Func_0200324c
#define Actor_Run60 Func_02003f46
#define Actor_Run61 Func_02003f2c
#define Actor_Run62 Func_02003f3e
#define Actor_Run63 Func_02003f08
#define Actor_Run64 Func_020036ee_a
#define Actor_Run65 Func_02003622
#define Actor_Run66 Func_0200365a
#define Actor_Run67 Func_02002e58
#define Actor_Run68 Func_02002d50
#define Actor_Run69 Func_020031d4
#define Actor_Run7 Func_02004020
#define Actor_Run70 Func_02002eac
#define Actor_Run71 Func_0200090a
#define Actor_Run72 Func_020033c4
#define Actor_Run73 Func_0200058c
#define Actor_Run74 Func_02000758
#define Actor_Run75 Func_020033ba
#define Actor_Run76 Func_02003504
#define Actor_Run77 Func_02003510
#define Actor_Run78 Func_0200341a
#define Actor_Run79 Func_02001fe8
#define Actor_Run8 Func_020047ec
#define Actor_Run9 Func_02004026
#define AdvanceStagedActorPair Func_020003a8
#define CanStartStagedActorMove Func_02002908
#define CreateOverlayObject Func_020024fe
#define EffectDescriptorTable Data_0200a814
#define Effect_AdvanceAnchoredRiseFrame Func_02001c20
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define FindNextStagedActor Func_0200073e
#define FinishStagedActorEffect Func_02002ae0
#define FinishStagedActorMove Func_0200293c
#define GetPartyEffect Func_0200264a
#define GetStagedActor Func_020028a4
#define OvObj_AdvanceScaleCounter Func_02001c88
#define OvObj_CreateConfigured Func_02000048
#define OvObj_CreateConfiguredObject Func_020000a0
#define OvObj_RampWords24And28Over16Frames Func_02001c60
#define OvObj_ReleasePublishedAttachment Func_020023b8
#define OvObj_SpawnKind24AtActor Func_02001bc8
#define RunOverlayObjectCommand0 Func_0200250c
#define RunOverlayObjectCommand1 Func_02002524
#define RunOverlayObjectCommand14 Func_020025e4
#define ScaleEffectDeltaFromAccumulated Func_02002658
#define ScaleEffectDeltaFromOrigin Func_02002670
#define ScaleEffectVerticalDelta Func_0200267e
#define SceneAudio_PlayCue118AndReturnZero Func_02000d48
#define SceneData_GetTablea928 Func_02000e90
#define SceneData_GetTableabf8 Func_02000e9c
#define SceneData_GetTableac58 Func_02000ea4
#define SceneData_GetTablead60 Func_02000fd0
#define SceneData_ReturnZero Func_02000e98
#define Scene_CallHelper Func_02000e6c
#define Scene_CallHelper2 Func_02000e84
#define Scene_Forward Func_02000e60
#define Scene_RunActor8StepWithTableA820 Func_02001150
#define Scene_RunIndexedStep17 Func_02000fac
#define Scene_RunIndexedStep18 Func_02000fb8
#define Scene_RunIndexedStep19 Func_02000fc4
#define Scene_RunIndexedStep63 Func_02001144
#define Scene_RunScene39b Func_0200196c
#define Scene_RunSingleStep Func_02000e78
#define Scene_RunStepWithValue1632 Func_02000eac
#define Scene_RunSupplementalSequenceTwo Func_02000fd8
#define SelectStagedActorSlot Func_02002890
#define SetEffectCallbackArgument Func_02002704
#define SetEffectCallbackMode Func_020026f4
#define SetEffectDescriptor Func_020025f4
#define SetEffectMode Func_02002776
#define SetEffectVariant Func_020025da
#define SetOverlayObjectMode Func_02002568
#define SetOverlayObjectSlot Func_02002640
#define SetStagedActorMode Func_020028d8
#define SetStagedActorTransition Func_02002944
#define SpawnConfiguredEffect Func_0200013c
#define SpawnEffect Func_020025d0
#define StagedActorStepTable Data_0200a700
#define StartLeadStagedActorMove Func_0200292e
#define StartNextStagedActorMove Func_0200291e
#define StartStagedActorEffect Func_02002abe
#define Value Func_02008cc1
#define gIw Data_03001edc
#define gOv Data_0200a740
#define gOv2 Data_0200a758
#define gWork Data_03001ebc
#define Actor_Place(a0, a1, a2) Call3(Func_02003452, a0, a1, a2)
#define Actor_Place2(a0, a1, a2) Call3(Func_0200348e, a0, a1, a2)
#define Actor_Check(a0) Value1(Func_020034d2_a, a0)
#define Actor_Check2(a0) Value1(Func_020034e4, a0)
#define Actor_Run(a0, a1) Call2(Func_020035da, a0, a1)
#define Actor_Run2(a0, a1) Call2(Func_020035ee, a0, a1)
#define Actor_Run3() Value0(Func_02003474)
#define Actor_Do(a0) Call1(Func_02003592, a0)
#define Actor_Check3(a0, a1) Value2(Func_020034cc, a0, a1)
#define Actor_SetRect(a0, a1, a2, a3, a4, a5) Call6(Func_02003560, a0, a1, a2, a3, a4, a5)
#define Actor_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_0200357c, a0, a1, a2, a3, a4, a5)
#define Actor_Run4(a0, a1) Call2(Func_020036c6, a0, a1)
#define Actor_Check4(a0) Value1_0200116c(Func_0200365e, a0)
#define Actor_Check5(a0) Value1_02001208(Func_020036fa, a0)
#define Actor_Check6(a0) Value1_02001208(Func_020036d2, a0)
#define Actor_Run5(a0, a1, a2, a3) Call4(Func_020037c2, a0, a1, a2, a3)
#define Actor_Place3(a0, a1, a2) Value3(Func_0200377a, a0, a1, a2)
#define Actor_Place4(a0, a1, a2) Call3_02001208(Func_02003772, a0, a1, a2)
#define Actor_Check7(a0) Value1(Func_02003e30, a0)
#define Actor_Do2(a0) Call1_0200196c(Func_02003e44, a0)
#define Actor_Check8(a0) Value1(Func_02003e80, a0)
#define Actor_Place5(a0, a1, a2) Call3_0200196c(Func_02003ed8, a0, a1, a2)
#define Actor_Place6(a0, a1, a2) Call3_0200196c(Func_02003f1c, a0, a1, a2)
#define Actor_Do3(a0) Call1_0200196c(Func_02003edc, a0)

#endif
