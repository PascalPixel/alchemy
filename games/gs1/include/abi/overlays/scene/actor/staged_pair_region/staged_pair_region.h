#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PAIR_REGION_STAGED_PAIR_REGION
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PAIR_REGION_STAGED_PAIR_REGION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0200293e(s32 dz, s32 dx);
void Func_020028fa(s32 cnt);
void Func_02003670();
void Func_02003900(s32 actor, s32 state);
void Func_02003908(s32 actor, s32 state);
void Func_020033f2(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
s32 Func_02003418(s32 flagId);
void Func_020030d2(s32 width, s32 top, s32 bottom);
void Func_02003416(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
void Func_020030ee(s32 width, s32 top, s32 bottom);
void Func_02003434(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
void Func_020008ec(StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/staged_pair_region/overlay_object.c */


void *Func_02001c9e(s32, s32, s32, s32);
void Func_02001cfc(void *, s32);
void Func_02001d14(void *, s32);
void Func_02001d94(void *, s32);
void Func_020034b8(s32 cue);
void Func_020034a6(s32 val);
void Func_02002d4e();
void Func_02002d60();
void Func_02002d74();
s32 Func_02002d90();
s32 Func_02002da8();
void Func_02002dac();
void Func_02002dd0();
void Func_02002dde();
void Func_02002dfc();
s32 Func_02002e3c_a();
s32 Func_02002e3c_b();
void Func_02002e94();
s32 Func_02002e34();
void Func_02002e48();
void Func_02002e64();
void Func_02002e7a();
void Func_02002e9e();
void Func_02002ecc();
s32 Func_02002edc();
void Func_02002ede();
void Func_02002ef0_a();
void Func_02002ef0_b();
void Func_02002ef6();
void Func_02002ef8_a();
void Func_02002ef8_b();
void Func_02002ef8_c();
void Func_02002f04();
void Func_02002f0c();
void Func_02002f10_a();
void Func_02002f10_b();
void Func_02002f5a();
void Func_02002f76();
s32 Func_02002eec();
s32 Func_02002ef6_a();
void Func_02002f08();
void Func_02002f0e();
void Func_02002f2a();
void Func_02002f3a();
void Func_02002f68();
void Func_02002f78();
void Func_02002f8e();
void Func_02002f94();
void Func_02002f9c();
s32 Func_02002fa2_a();
s32 Func_02002fa2_b();
void Func_02002faa();
void Func_02002fac();
void Func_02002ff4();
s32 Func_02002f96();
s32 Func_02002fe8();
s32 Func_02002ff0();
s32 Func_02002ffa();
s32 Func_02003002();
void Func_02003030();
s32 Func_02001ac2();
void Func_02001ad2();
void Func_0200223c();
void Func_0200315c();
void Func_0200315e();
void Func_02003176();
void Func_02003186();
void Func_02003188();
void Func_020031c0_a();
void Func_020031c0_b();
s32 Func_020031c6();
void Func_020031c8();
s32 Func_020031d2();
void Func_02003200();
void Func_02003206();
s32 Func_0200320e();
void Func_02003214();
void Func_02003222();
void Func_02003224();
void Func_0200322c();
s32 Func_0200323e();
void Func_02003244();
void Func_02003246_a();
void Func_02003246_b();
void Func_02003248();
void Func_0200324c();
void Func_0200326e();
void Func_02003270();
void Func_02003298();
void Func_0200329a();
void Func_020032a0();
void Func_02003312();
void Func_02003616();
void Func_0200309c();
void Func_020030a6();
void Func_02003568();
void Func_020034f6();
void Func_0200304e();
void Func_02003640();
void Func_02003656(s32 soundId);
void Func_020030de(s32 actorId, s32 cellX, s32 cellZ);
void Func_020030ea(s32 actorId, s32 cellX, s32 cellZ);
void Func_020035b4(s32 flagId);
void Func_0200353a(s32 frames);
void Func_02003092(void);
void Func_02003684(s32 soundId);
void Func_02002a44();
void Func_02002a60();
void Func_02002d78();
void Func_02001478();
void Func_02002d88();
void Func_02002050();
void Func_02003028(void);
void Func_02003080(s32 actor, s32 mode);
void Func_0200303c(void);
s32 Func_020020f8(struct StagedActor *arg0, s32 *arg1);
void Func_020020d0(struct StagedActor *arg0, s32 arg1);
void Func_020020c0(s32 arg0);
void Func_02002206(s32 arg0);
void Func_0200210e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200211e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200212c(struct StagedActor *arg0);
void Func_02002228(void);
void Func_0200213c(struct StagedActor *arg0, s32 arg1);
s32 Func_0200227a(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_020024b4(u8 *, s32 *);
void Func_020034b4(void);
void Func_02003496(Obj *, s32, s32, s32);
void Func_0200347e(Obj *, s32);
void Func_020034c2(Obj *, s32, s32, s32);
void Func_020034c0(Obj *, s32);
void Func_020034ee(Obj *);
void Func_0200354a(void);
void Func_02002a16(s32, s32, s32);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_020011a0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1_02001258(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001258(s32 (*f)(), s32 a0)
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

#define ActorDraw_AdvanceActorElevenStates Func_02001bfc
#define Actor_Apply Func_0200293e
#define Actor_Apply10 Func_020034c0
#define Actor_Apply2 Func_02003900
#define Actor_Apply3 Func_02003908
#define Actor_Apply4 Func_02001cfc
#define Actor_Apply5 Func_02001d94
#define Actor_Apply6 Func_02001d14
#define Actor_Apply7 Func_02003080
#define Actor_Apply8 Func_020024b4
#define Actor_Apply9 Func_0200347e
#define Actor_Check11 Func_02003418
#define Actor_Check12 Func_02002e3c_b
#define Actor_Check13 Func_02002edc
#define Actor_Check14 Func_02002fa2_a
#define Actor_Check15 Func_020031c6
#define Actor_Check16 Func_020031d2
#define Actor_Check17 Func_0200320e
#define Actor_Check18 Func_0200323e
#define Actor_Do10 Func_02003656
#define Actor_Do11 Func_020035b4
#define Actor_Do12 Func_0200353a
#define Actor_Do13 Func_02003684
#define Actor_Do14 Func_020034ee
#define Actor_Do7 Func_020028fa
#define Actor_Do8 Func_020034b8
#define Actor_Do9 Func_020034a6
#define Actor_EntOp Func_0200227a
#define Actor_FaceActorZero Func_02000cf8
#define Actor_Far Data_02009ea0
#define Actor_Far10 Func_020026b0
#define Actor_Far11 Func_0200265e
#define Actor_Far12 Func_020026c8
#define Actor_Far13 Func_020026a6
#define Actor_Far14 Func_02002698
#define Actor_Far15 Func_02002638
#define Actor_Far16 Func_02002642
#define Actor_Far17 Func_02002770_a
#define Actor_Far18 Func_02002674
#define Actor_Far19 Func_02002712
#define Actor_Far2 Data_02009e48
#define Actor_Far20 Func_0200272a
#define Actor_Far21 Func_020026fc
#define Actor_Far22 Func_02002740
#define Actor_Far23 Func_02002758
#define Actor_Far24 Func_02002770_b
#define Actor_Far25 Func_02002736
#define Actor_Far26 Func_020027e8
#define Actor_Far27 Func_020027ee
#define Actor_Far28 Func_020026dc
#define Actor_Far29 Func_02002766
#define Actor_Far3 Func_02000eca
#define Actor_Far30 Func_020027c2
#define Actor_Far31 Func_020028c4
#define Actor_Far4 Func_02001036
#define Actor_Far5 Func_02001048
#define Actor_Far6 Func_02001094
#define Actor_Far7 Func_020025cc
#define Actor_Far8 Func_020025d8
#define Actor_Far9 Func_02002670
#define Actor_Place14 Func_020030d2
#define Actor_Place15 Func_020030ee
#define Actor_Place16 Func_020030de
#define Actor_Place17 Func_020030ea
#define Actor_Place18 Func_02000da6
#define Actor_Place19 Func_02002a16
#define Actor_PlaceAtTile Func_02001938
#define Actor_Run10 Func_02002dd0
#define Actor_Run11 Func_02002d60
#define Actor_Run12 Func_02002d4e
#define Actor_Run13 Func_02002e94
#define Actor_Run14 Func_02002d74
#define Actor_Run15 Func_02002dde
#define Actor_Run16 Func_02002dac
#define Actor_Run17 Func_02002dfc
#define Actor_Run18 Func_02002e64
#define Actor_Run19 Func_02002ef8_b
#define Actor_Run20 Func_02002f0c
#define Actor_Run21 Func_02002e7a
#define Actor_Run22 Func_02002ef8_c
#define Actor_Run23 Func_02002e9e
#define Actor_Run24 Func_02002f76
#define Actor_Run25 Func_02002ede
#define Actor_Run26 Func_02002ef0_b
#define Actor_Run27 Func_02002f04
#define Actor_Run28 Func_02002f2a
#define Actor_Run29 Func_02002f94
#define Actor_Run30 Func_02002f3a
#define Actor_Run31 Func_02002f78
#define Actor_Run32 Func_02003030
#define Actor_Run33 Func_0200315e
#define Actor_Run34 Func_0200315c
#define Actor_Run35 Func_020031c8
#define Actor_Run36 Func_02003176
#define Actor_Run37 Func_020031c0_a
#define Actor_Run38 Func_02003186
#define Actor_Run39 Func_0200324c
#define Actor_Run40 Func_02003200
#define Actor_Run41 Func_02003206
#define Actor_Run42 Func_02003214
#define Actor_Run43 Func_0200322c
#define Actor_Run44 Func_02003222
#define Actor_Run45 Func_020032a0
#define Actor_Run46 Func_02003248
#define Actor_Run47 Func_02003246_b
#define Actor_Run48 Func_02003224
#define Actor_Run49 Func_0200223c
#define Actor_Run5 Func_020029bc
#define Actor_Run50 Func_02003270
#define Actor_Run51 Func_02003298
#define Actor_Run52 Func_02003312
#define Actor_Run53 Func_0200326e
#define Actor_Run54 Func_02003616
#define Actor_Run55 Func_0200309c
#define Actor_Run56 Func_020030a6
#define Actor_Run57 Func_02003568
#define Actor_Run58 Func_020034f6
#define Actor_Run59 Func_0200304e
#define Actor_Run6 Func_02003602
#define Actor_Run60 Func_02003640
#define Actor_Run61 Func_02003092
#define Actor_Run62 Func_02002a44
#define Actor_Run63 Func_02002a60
#define Actor_Run64 Func_02002d78
#define Actor_Run65 Func_02001478
#define Actor_Run66 Func_02002d88
#define Actor_Run67 Func_02002050
#define Actor_Run68 Func_02003028
#define Actor_Run69 Func_0200303c
#define Actor_Run7 Func_02003670
#define Actor_Run70 Func_0200090a
#define Actor_Run71 Func_020034b6_a
#define Actor_Run72 Func_020034b6_b
#define Actor_Run73 Func_020034be
#define Actor_Run74 Func_0200058c
#define Actor_Run75 Func_02000758
#define Actor_Run76 Func_020017d8
#define Actor_Run77 Func_020034b4
#define Actor_Run78 Func_0200354a
#define Actor_Run8 Func_02001c9e
#define Actor_Run9 Func_020010e0
#define Actor_SetMode Func_02003496
#define Actor_SetMode2 Func_020034c2
#define Actor_SetRect3 Func_020033f2
#define Actor_SetRect4 Func_02003416
#define Actor_SetRect5 Func_02003434
#define Actor_WaitActorDescent Func_02000cd0
#define AdvanceStagedActorPair Func_020003a8
#define CanStartStagedActorMove Func_020020f8
#define CreateOverlayObject Func_02001cf6
#define DefaultOverlayData Data_0200a414
#define DialogueLayout_ConfigureRowsByFlag301 Func_02001770
#define EffectDescriptorTable Data_02009f60
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define FindNextStagedActor Func_0200073e
#define FinishStagedActorEffect Func_02002228
#define FinishStagedActorMove Func_0200212c
#define GetPartyEffect Func_02001e1a
#define GetStagedActor Func_02002074
#define OvObj_CreateConfigured Func_02000048
#define OvObj_CreateConfiguredB Func_020000a0
#define PrimaryOverlayData Data_0200a420
#define RefreshStagedActor Func_020026e0
#define RuntimeSelectorTable Data_02000240
#define ScaleEffectDeltaFromAccumulated Func_02001e90
#define ScaleEffectDeltaFromOrigin Func_02001ea8
#define ScaleEffectVerticalDelta Func_02001eb6
#define SceneAudio_PlayCue123AndDispatchWork364 Func_02001748
#define SceneDataA234 Data_0200a234
#define SceneDataA2C4 Data_0200a2c4
#define SceneDataA39C Data_0200a39c
#define SceneData_GetTablea1dc Func_02000d78
#define SceneData_ReturnZero Func_02000d74
#define SceneData_SelectByRuntimeSelector Func_02000d20
#define SceneData_SelectSecondaryByRuntimeSelector Func_0200169c
#define Scene_ConfigureRegionAtRow15 Func_02000dcc
#define Scene_ConfigureRegionAtRow17 Func_02000de8
#define Scene_RunActor11Transition301 Func_020018f4
#define Scene_RunActor8ZeroStep Func_02001378
#define Scene_RunInitBracketThenSequence Func_020010c8
#define Scene_RunScene3a6 Func_020014ac
#define Scene_RunScene3a6SequenceA Func_020011a0
#define Scene_RunScene3a6SequenceB Func_02001258
#define Scene_RunScene3a6SequenceC Func_02001300
#define Scene_RunShiftAndSetFlag301 Func_020018b4
#define SecondaryOverlayData Data_0200a450
#define SelectSceneDataByState Func_02000d80
#define SelectStagedActorSlot Func_020020c0
#define SetEffectCallbackArgument Func_02001efc
#define SetEffectCallbackMode Func_02001eec
#define SetEffectDescriptor Func_02001dec
#define SetEffectMode Func_02001f26
#define SetEffectVariant Func_02001dd2
#define SetOverlayObjectMode Func_02001d58
#define SetOverlayObjectSlot Func_02001df0
#define SetStagedActorMode Func_020020d0
#define SetStagedActorTransition Func_0200213c
#define SpawnConfiguredEffect Func_0200013c
#define SpawnEffect Func_02001dc8
#define StagedActorStepTable Data_02009e48
#define StartLeadStagedActorMove Func_0200211e
#define StartNextStagedActorMove Func_0200210e
#define StartStagedActorEffect Func_02002206
#define State_SetValues8_3_4 Func_02000cc0
#define TertiaryOverlayData Data_0200a624
#define Value Func_020082a9
#define Value4 Func_0200858d
#define gOv Data_02009e88
#define gWork Data_03001ebc
#define Actor_Check(a0) Value1(Func_02002da8, a0)
#define Actor_Check2(a0, a1) Value2(Func_02002d90, a0, a1)
#define Actor_Place(a0, a1, a2) Value3(Func_02002e3c_a, a0, a1, a2)
#define Actor_Check3(a0) Value1_020011a0(Func_02002e34, a0)
#define Actor_Do(a0) Call1(Func_02002e48, a0)
#define Actor_Run(a0, a1) Call2(Func_02002ef8_a, a0, a1)
#define Actor_Place2(a0, a1, a2) Call3(Func_02002ef6, a0, a1, a2)
#define Actor_Run2(a0, a1) Call2(Func_02002f10_a, a0, a1)
#define Actor_Place3(a0, a1, a2) Call3(Func_02002ecc, a0, a1, a2)
#define Actor_Place4(a0, a1, a2) Call3(Func_02002ef0_a, a0, a1, a2)
#define Actor_Place5(a0, a1, a2) Call3(Func_02002f10_b, a0, a1, a2)
#define Actor_Place6(a0, a1, a2) Call3(Func_02002f5a, a0, a1, a2)
#define Actor_Check4(a0) Value1_02001258(Func_02002eec, a0)
#define Actor_Check5(a0) Value1_02001258(Func_02002ef6_a, a0)
#define Actor_Do2(a0) Call1_02001258(Func_02002f08, a0)
#define Actor_Do3(a0) Call1_02001258(Func_02002f0e, a0)
#define Actor_Run3(a0, a1) Call2(Func_02002fac, a0, a1)
#define Actor_Place7(a0, a1, a2) Call3(Func_02002f68, a0, a1, a2)
#define Actor_Place8(a0, a1, a2) Call3(Func_02002f8e, a0, a1, a2)
#define Actor_Place9(a0, a1, a2) Call3(Func_02002f9c, a0, a1, a2)
#define Actor_Place10(a0, a1, a2) Call3(Func_02002faa, a0, a1, a2)
#define Actor_Place11(a0, a1, a2) Call3(Func_02002ff4, a0, a1, a2)
#define Actor_Run4() Call0((void (*)())Func_02002fa2_b)
#define Actor_Check6(a0) Value1(Func_02002f96, a0)
#define Actor_Check7(a0) Value1(Func_02002fe8, a0)
#define Actor_Check8(a0) Value1(Func_02002ff0, a0)
#define Actor_Check9(a0) Value1(Func_02002ffa, a0)
#define Actor_Check10(a0) Value1(Func_02003002, a0)
#define Actor_Place12(a0, a1, a2) Call3(Func_02003188, a0, a1, a2)
#define Actor_Do4(a0) Call1(Func_02003246_a, a0)
#define Actor_Place13(a0, a1, a2) Call3(Func_020031c0_b, a0, a1, a2)
#define Actor_Do5(a0) Call1(Func_0200329a, a0)
#define Actor_SetRect(a0, a1, a2, a3, a4, a5) Value6(Func_02001ac2, a0, a1, a2, a3, a4, a5)
#define Actor_SetRect2(a0, a1, a2, a3, a4, a5) Call6(Func_02001ad2, a0, a1, a2, a3, a4, a5)
#define Actor_Do6(a0) Call1(Func_02003244, a0)

#endif
