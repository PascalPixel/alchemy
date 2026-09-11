#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_FIELD_EXTENDED_SEQUENCE_EXTENDED_SEQUENCE
#define ALCHEMY_ABI_OVERLAYS_SCENE_FIELD_EXTENDED_SEQUENCE_EXTENDED_SEQUENCE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02002a02(s32, s32);
s32 Func_02002a0a(struct Obj *);
void Func_02002b6e(struct Obj *, s32);
void Func_02002b78(struct Obj *, s32);
void Func_02002ae2(struct Obj *);
u16 Func_02002af2(s32, s32);
s32 Func_02002c10(s32);
void Func_0200029e(s32);
void Func_02002c70(struct Rec *);
void Func_02002e10(s32);
void Func_02002d58(s32);
void Func_02002d78(s32, s32);
void Func_02002c9c(void);
void Func_02002e3a(s32, s32, s32);
s32 Func_02002d10();
void Func_02002d34();
void Func_02002d88();
void Func_02002d94();
void Func_02002e54();
void Func_02002fbc(void);
s32 Func_0200311e();
void Func_020030dc();
s32 Func_0200312a();
void Func_020030e8();
s32 Func_02003136();
void Func_020030f4();
s32 Func_02003142();
void Func_02003100();
void Func_0200316a();
void Func_02003172();
void Func_02003172_a();
void Func_0200317a();
void Func_02003182();
s32 Func_02003138();
s32 Func_0200317c();
void Func_0200313c();
void Func_0200314c();
void Func_0200315e();
s32 Func_02003190();
void Func_02001078(void);
s32 Func_020031ae();
void Func_020031f8();
void Func_020031fe();
void Func_02003204();
void Func_0200320a();
void Func_02003210();
void Func_02003216();
void Func_02003216_a();
void Func_0200321c();
void Func_02003222();
void Func_02003228();
void Func_02003444(s32, s32);
void Func_02003454(s32, s32);
void Func_02003470(void);
void Func_0200344e(s32);
void Func_0200326c(s32);
void Func_02003462(s32);
void Func_0200347e(void);
void Func_0200348a(void);
void Func_020050c4(s32, s32);
void Func_02004fe2(s32);
void Func_020050ec(s32, s32, s32);
void Func_02004ffa(s32);
void Func_02004fd6(struct Obj *);
void Func_020051ee(s32);
void Func_02005036(struct Obj *, s32);
void Func_0200506c(struct Obj *, s32, s32, s32);
void Func_02005054(struct Obj *, u8 *);
void Func_020051ac(s32, s32);
void Func_020051b4(s32, s32);
s32 Func_02005334(s32);
void Func_020053da(s32, s32, s32);
void Func_0200533e(s32, s32, s32, s32, s32, s32);
void Func_02005322(void);
void Func_020052e0(s32);
void Func_02005404(s32, s32, s32);
s32 Func_02005372(s32);
void Func_0200536a(s32, s32, s32, s32, s32, s32);
void Func_0200536a_b(void);
void Func_02005386(s32, s32, s32, s32, s32, s32);
void Func_02005328(s32);
void Func_02002a34();
s32 Func_02002a1c();
void Func_02002a0c();
void Func_02002b4c();
void Func_020029e4();
void Func_02002ab4();
void Func_0200299c();
void Func_02002b74();
void Func_02002b7c();
void Func_02002a64();
void Func_02002a9c();
void Func_02002a6c();
void Func_02002a74();
void Func_02002b24();
void Func_02002ae4();
void Func_02002a2c();
void Func_02002ad4();
void Func_02002b04();
void Func_020025a8();
void Func_02002ac4();
void Func_02002adc();
void Func_02002abc();
void Func_020008ac();
void Func_020008d8();
void Func_02002b34();
void Func_02002aec();
void Func_02002b3c();
void Func_020025c0();
s32 Func_02002b0c();
s32 Func_02002a4c();
void Func_02002bcc();
void Func_02002b64();
void Func_02002b6c();
s32 Func_020029a4();
void Func_02002aa4();
s32 Func_020029ac();
void Func_02002b2c();
void Func_02002a7c();
void Func_02002a84();
void Func_02002acc();
void Func_02002b14();
void Func_02002a8c();
void Func_02002aac();
s32 Func_02002a24();
void Func_02002bb4();
void Func_02002a3c();

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

#define Actor_SetActors19To22HeightByFrameParity Func_02000800
#define Actor_TurnTowardTarget Func_02000114
#define Effect_AdvanceAngleUntilIdle Func_020025d8
#define Effect_AdvanceCounterAndSwitchMode Func_02000050
#define Effect_SetModeByFrameBit1 Func_020026a4
#define Effect_SpawnObject26EveryEightFrames Func_02002608
#define Effect_UpdateFallingObject Func_020000cc
#define Ent Func_02000800
#define Field_Apply Func_02002a02
#define Field_Apply10 Func_02005054
#define Field_Apply11 Func_020051ac
#define Field_Apply12 Func_020051b4
#define Field_Apply2 Func_02002b6e
#define Field_Apply3 Func_02002b78
#define Field_Apply4 Func_02002af2
#define Field_Apply5 Func_02002d78
#define Field_Apply6 Func_02003444
#define Field_Apply7 Func_02003454
#define Field_Apply8 Func_020050c4
#define Field_Apply9 Func_02005036
#define Field_Check10 Func_0200311e
#define Field_Check11 Func_0200312a
#define Field_Check12 Func_02003136
#define Field_Check13 Func_02003142
#define Field_Check14 Func_02003138
#define Field_Check15 Func_0200317c
#define Field_Check16 Func_02003190
#define Field_Check17 Func_020031ae
#define Field_Check18 Func_02005334
#define Field_Check19 Func_02005372
#define Field_Check7 Func_02002a0a
#define Field_Check8 Func_02002c10
#define Field_Check9 Func_02002d10
#define Field_Do10 Func_02003462
#define Field_Do11 Func_02004fe2
#define Field_Do12 Func_02004ffa
#define Field_Do13 Func_02004fd6
#define Field_Do14 Func_020051ee
#define Field_Do15 Func_020052e0
#define Field_Do16 Func_02005328
#define Field_Do3 Func_02002ae2
#define Field_Do4 Func_0200029e
#define Field_Do5 Func_02002c70
#define Field_Do6 Func_02002e10
#define Field_Do7 Func_02002d58
#define Field_Do8 Func_0200344e
#define Field_Do9 Func_0200326c
#define Field_Place10 Func_020050ec
#define Field_Place11 Func_020053da
#define Field_Place12 Func_02005404
#define Field_Place9 Func_02002e3a
#define Field_Run10 Func_020032aa
#define Field_Run11 Func_020032d2
#define Field_Run12 Func_02005006
#define Field_Run13 Func_02002c9c
#define Field_Run14 Func_02002d94
#define Field_Run15 Func_02002e54
#define Field_Run16 Func_02002fbc
#define Field_Run17 Func_020030dc
#define Field_Run18 Func_020030e8
#define Field_Run19 Func_020030f4
#define Field_Run20 Func_02003100
#define Field_Run21 Func_0200316a
#define Field_Run22 Func_02003172
#define Field_Run23 Func_0200317a
#define Field_Run24 Func_02003182
#define Field_Run25 Func_0200313c
#define Field_Run26 Func_0200314c
#define Field_Run27 Func_0200315e
#define Field_Run28 Func_02003172_a
#define Field_Run29 Func_02001078
#define Field_Run30 Func_020031f8
#define Field_Run31 Func_020031fe
#define Field_Run32 Func_02003204
#define Field_Run33 Func_0200320a
#define Field_Run34 Func_02003210
#define Field_Run35 Func_02003216
#define Field_Run36 Func_0200321c
#define Field_Run37 Func_02003222
#define Field_Run38 Func_02003228
#define Field_Run39 Func_02003216_a
#define Field_Run40 Func_02003470
#define Field_Run41 Func_0200347e
#define Field_Run42 Func_0200348a
#define Field_Run43 Func_02005322
#define Field_Run44 Func_0200536a_b
#define Field_Run45 Func_02002a54
#define Field_Run46 Func_02002a34
#define Field_Run47 Func_02002a0c
#define Field_Run48 Func_020029e4
#define Field_Run49 Func_0200299c
#define Field_Run50 Func_02002b74
#define Field_Run51 Func_02002b7c
#define Field_Run52 Func_02002a9c
#define Field_Run53 Func_02002ab4
#define Field_Run54 Func_02002a6c
#define Field_Run55 Func_02002a74
#define Field_Run56 Func_02002ae4
#define Field_Run57 Func_02002a2c
#define Field_Run58 Func_02002ad4
#define Field_Run59 Func_02002ac4
#define Field_Run60 Func_02002adc
#define Field_Run61 Func_02002abc
#define Field_Run62 Func_02002b24
#define Field_Run63 Func_02002aec
#define Field_Run64 Func_02002bcc
#define Field_Run65 Func_02002b6c
#define Field_Run66 Func_02002a64
#define Field_Run67 Func_02002b2c
#define Field_Run68 Func_02002a7c
#define Field_Run69 Func_02002a84
#define Field_Run7 Func_02002c8a
#define Field_Run70 Func_02002acc
#define Field_Run71 Func_02002b14
#define Field_Run72 Func_02002a8c
#define Field_Run73 Func_02002aac
#define Field_Run74 Func_02002bb4
#define Field_Run75 Func_02002a3c
#define Field_Run8 Func_0200325a
#define Field_Run9 Func_02003282
#define Field_SetMode Func_0200506c
#define Field_SetRect Func_0200533e
#define Field_SetRect2 Func_0200536a
#define Field_SetRect3 Func_02005386
#define Obj Func_020025d8
#define Obj2 Func_02002608
#define SceneData_GetTableB010ForSelector26 Func_020001ac
#define SceneData_GetTableB040 Func_020001d4
#define SceneData_InitRecordTable Func_020000a0
#define SceneData_SelectTableAe60BySelector Func_0200016c
#define SceneData_SelectTableB080BySelector Func_020001dc
#define SceneData_SelectTableB3b0BySelector Func_02000284
#define Scene_ConfigureActor0ThenRun Func_020002b4
#define Scene_RunActor16MessageBranch Func_02000230
#define Scene_RunActor27Step Func_02000274
#define Scene_RunExtendedActorSequence Func_020008ec
#define Scene_RunFlag845And847Branches Func_02002910
#define Scene_RunPairedStepA Func_020025a8
#define Scene_RunPairedStepB Func_020025c0
#define Scene_RunSequenceA Func_020008d8
#define Scene_SetupEntryBySelector Func_02000694
#define Scene_StartEffect141Sequence Func_020008ac
#define State_FlushPendingWordB698 Func_02000030
#define gCell Data_02000240
#define gIw Data_03001e40
#define gOv Data_0200af80
#define gOv10 Data_0200add8
#define gOv11 Data_0200b2d8
#define gOv12 Data_0200b6a0
#define gOv13 Data_0200b5d8
#define gOv14 Data_0200b69c
#define gOv15 Data_0200b68c
#define gOv16 Data_0200b690
#define gOv17 Data_0200b694
#define gOv18 Data_0200b698
#define gOv19 Data_02008801
#define gOv2 Data_0200afc8
#define gOv20 Data_0200a609
#define gOv21 Data_0200a7ad
#define gOv22 Data_0200abd4
#define gOv23 Data_0200ac08
#define gOv24 Data_0200ac3c
#define gOv25 Data_0200ac70
#define gOv26 Data_0200acfc
#define gOv27 Data_0200ad20
#define gOv28 Data_0200ad7c
#define gOv3 Data_0200ae60
#define gOv4 Data_0200b010
#define gOv5 Data_0200b098
#define gOv6 Data_0200b368
#define gOv7 Data_0200b080
#define gOv8 Data_0200b590
#define gOv9 Data_0200b3b0
#define gWork Data_03001ebc
#define Field_Place(a0, a1, a2) Call3(Func_02002d34, a0, a1, a2)
#define Field_Place2(a0, a1, a2) Call3(Func_02002d88, a0, a1, a2)
#define Field_Check(a0) Value1(Func_02002a1c, a0)
#define Field_Run(a0, a1, a2, a3) Call4(Func_02002b4c, a0, a1, a2, a3)
#define Field_Place3(a0, a1, a2) Call3(Func_02002ab4, a0, a1, a2)
#define Field_Place4(a0, a1, a2) Call3(Func_02002a64, a0, a1, a2)
#define Field_Run2(a0, a1) Call2(Func_02002a6c, a0, a1)
#define Field_Place5(a0, a1, a2) Call3(Func_02002b24, a0, a1, a2)
#define Field_Do(a0) Call1(Func_02002b04, a0)
#define Field_Place6(a0, a1, a2) Call3(Func_02002a9c, a0, a1, a2)
#define Field_Place7(a0, a1, a2) Call3(Func_02002b34, a0, a1, a2)
#define Field_Run3(a0, a1) Call2(Func_02002b3c, a0, a1)
#define Field_Check2(a0, a1) Value2(Func_02002b0c, a0, a1)
#define Field_Check3(a0, a1) Value2(Func_02002a4c, a0, a1)
#define Field_Run4(a0, a1) Call2(Func_02002b64, a0, a1)
#define Field_Check4(a0, a1) Value2(Func_020029a4, a0, a1)
#define Field_Place8(a0, a1, a2) Call3(Func_02002aa4, a0, a1, a2)
#define Field_Check5(a0) Value1(Func_020029ac, a0)
#define Field_Run5(a0, a1) Call2(Func_02002ad4, a0, a1)
#define Field_Do2(a0) Call1(Func_02002a2c, a0)
#define Field_Run6(a0, a1) Call2(Func_02002adc, a0, a1)
#define Field_Check6(a0) Value1(Func_02002a24, a0)

#endif
