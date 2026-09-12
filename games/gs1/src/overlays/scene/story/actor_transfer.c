#include "types.h"

#define StoryProgress_TriggerEvent0808 Func_02000030
#define StoryProgress_TriggerEvent0809 Func_0200008c
#define StoryProgress_TriggerEvent080A Func_020000d4
#define StoryActor_AdvanceTimer Func_0200011c
#define StoryActor_ConfigureSpawnedObject Func_0200013c
#define StoryActor_Initialize Func_02000250
#define StoryActor_ApplyFlaggedMode Func_020002a4
#define StoryActor_ResetPosition Func_02000314
#define StoryActor_ClearActiveFlag Func_02000350
#define SceneData_GetTableD27C Func_02000290
#define SceneData_ReturnZero Func_02000298
#define SceneData_GetTableDA2C Func_0200029c
#define SceneData_GetTableE3F4 Func_02000590
#define StoryScene_SelectPlacementTable Func_0200037c
#define StoryScene_SetBranchValueFromX Func_020004a0
#define StoryScene_SetBranchValueFromZ Func_020004f0
#define StoryScene_ShowRewardDialogue Func_020028a8
#define StoryScene_StartTransition Func_02003130
#define StoryScene_ComputeOpposingSlotDelta Func_0200341c
#define SceneState_SetValues130_6_47 Func_02000540
#define SceneState_ApplyValues150And46And11 Func_02000550
#define SceneState_ApplyValues116And56And21 Func_02000560
#define SceneState_ApplyValues151And25And54 Func_02000570
#define SceneState_ApplyFlag85aBranch Func_020006b4
#define SceneState_RunSlotStepOnOddFrames Func_02003f88
#define FieldScene_RunStep7D3B1E Func_02000580
#define FieldScene_RunStep74 Func_020006dc
#define FieldScene_RunScene371_0200155c Func_0200155c
#define FieldScene_RunScene371_02001680 Func_02001680
#define FieldScene_RunScene371_020017a4 Func_020017a4
#define FieldScene_RunScene371_020017fc Func_020017fc
#define FieldScene_RunScene371_02001888 Func_02001888
#define FieldScene_RunScene371_02001938 Func_02001938
#define FieldScene_RunScene371_020019e8 Func_020019e8
#define FieldScene_RunScene371_02001a98 Func_02001a98
#define BattleRuntime_Reset_1(args...) Func_020075d0_b(args)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Func_020076fa, a0, a1, a2, a3)
#define Object_SetModeById_1(args...) Func_02007678(args)
#define ObjectMotion_SetHorizontalPositionWithTerrain_1(a0, a1, a2) Call3(Func_0200767c, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_02007622(args)
#define RatioHistory_RecordAndScheduleCallback_1(a0, a1) Call2(Func_020077c6, a0, a1)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(args...) Func_020076a0(args)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02007730(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Func_020077a0(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_0200767a, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_1(a0, a1, a2) Call3(Func_020076ae, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_2(a0, a1, a2) Call3(Func_020076ba, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Func_0200769c, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_3(a0, a1, a2) Call3(Func_020076d0, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1, a2) Call3(Func_020076b2, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_4(a0, a1, a2) Call3(Func_020076e6, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_4(a0, a1, a2) Call3(Func_020076c8, a0, a1, a2)
#define ObjectMotion_SetPositionAndCommit_5(a0, a1, a2) Call3(Func_020076fc, a0, a1, a2)
#define Object_SetModeById_2(args...) Func_02007734(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Func_020076aa(args)
#define ObjectMotion_SetSpeedParameters_5(args...) Func_0200782a(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define ACTOR 8
#define FieldScene_RunScene371_02001b5c Func_02001b5c
#define FieldScene_RunScene371_02001c08 Func_02001c08
#define FieldScene_RunScene371_02002274 Func_02002274
#define FieldScene_RunScene371_0200281c Func_0200281c
#define FieldScene_RunScene371_02002858 Func_02002858
#define FieldScene_RunActorPresentationSequence Func_02002cb4
#define FieldScene_RunActorEightApproach Func_020032f0
#define FieldScene_RunScene371_0200357c Func_0200357c
#define FieldScene_RunOpeningAuxiliarySequence Func_020036dc
#define FieldScene_RunLateSequence Func_0200395c
#define RunEventScript01 Func_02000a8c
#define FieldScene_RunActorTransferSequence Func_02000c28
#define StoryReward_LookupBySelection Func_02003448
#define StoryScene_UpdateSelectedActorProgress Func_02003678
#define StoryScene_SetReferenceActor Func_020037bc
#define StoryScene_ActivateSharedState Func_020037c8
#define StoryScene_CompleteActor98 Func_020037d8
#define SceneEffect_RestoreBlendRegisters Func_020038fc
#define SceneActor_UpdateObjectByCounterBits Func_02003f10
#define OverlayObject_UpdateOnFrameParity Func_02003f4c
#define StoryEffect_AnimateVerticalPositive Func_02003fb4
#define StoryEffect_AnimateVerticalNegative Func_02004004

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} StorySpawnRecord;

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
};

struct StoryDialogueWork {
    u8 reserved000[370];
    u16 story_result;
};

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct StorySelection {
    u8 reserved000[500];
    s32 actor_id;
};

struct StorySelectionActor {
    u8 reserved00[6];
    u16 presentation;
};

struct StoryProgressWork {
    u8 reserved000[386];
    u16 state_one_marker;
};

/* Complete prologue-less shared-state activation leaf through its pool. */
struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

struct StoryCompletionWork {
    u8 reserved000[386];
    s16 scene_value;
};

/*
 * Per-frame step of a thirty-two frame rise -- resource_371.  The halfword
 * counter at +0x64 is incremented in place and compared as a signed sixteen
 * bit value; past 31 the effect is handed off and nothing else happens.  The
 * anchor pointer at +0x68 is loaded before the frame test and must stay
 * there.  The angle is the frame shifted left by ten, and the callee that
 * turns it into a 16.16 amplitude is identified only by that use.
 */
struct StoryVerticalEffectActor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor *anchor;  /* 0x68 */
};

/*
 * The mirror of the rise: the same counter at +0x64 and anchor at +0x68,
 * with the second amplitude store negated, the depth offset subtracted
 * rather than added, and a fixed depth bias of 0x100000.  Its magnitude
 * callee, like the one above, is identified only by use -- one angle in, one
 * 16.16 magnitude out.
 */
struct StoryVerticalEffectActor_02004004 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct StoryVerticalEffectActor_02004004 *anchor;  /* 0x68 */
};

extern u8 Data_0200811d[];
extern u8 Data_020081c5[];
extern u8 Value_00000847;
extern u8 Value_000002f1;
extern u8 Data_0200d27c[];
extern u8 Data_0200da2c[];
extern u8 Data_0200e3f4[];
extern u8 Data_0200db84[];
extern u8 Data_0200e04c[];
extern u8 Data_0200e154[];
extern u8 Data_0200e1e4[];
extern u8 Data_0200e25c[];
extern u8 Data_0200e28c[];
extern u8 Data_0200e2ec[];
extern u8 Data_0200e394[];
extern u8 Data_0200e3c4[];
extern u8 Value_0000085a;
extern u8 Data_0200e79c[];
extern u8 Data_00000000[];
extern u8 Data_00000800[];
extern u8 Data_00000c00[];
extern u8 Data_00000d00[];
extern u8 Data_00000e00[];
extern u8 Data_00000f00[];
extern u8 Data_00001000[];
extern u8 Data_02000434[];
extern s32 Data_02000240_t[][1];
extern u8 Data_02002090[];
extern u8 Data_04000208[];
extern u8 Data_0200cd98[];
extern u8 Data_0200cac4[];
extern u8 Data_0200ce78[];
extern u8 Data_0200caf4[];
extern s32 Data_0200e718[];
extern struct StorySharedState *Data_03001f30;

s32 Func_020041c2(s32, s32);
s32 Func_020041f4(void);
void Func_02004484(s32, s32);
s32 Func_0200421c();
void Func_020044d2();
s32 Func_02004264();
void Func_0200451a();
void Func_02004348();
u8 *Func_02004378();
void Func_02004618();
void Func_020043dc();
void Func_0200439c();
s32 Func_020044e4(s32);
s32 Func_020044f0(s32);
void Func_02000576(void);
s32 Func_0200453c(s32);
void Func_020044b8(u8 *, s32);
void Func_02004560(u8 *, s32);
void Func_02004580(u8 *, s32);
s32 Func_020045c0(s32);
void Func_020045d2(s32);
s32 Func_020046a8();
s32 Func_020046b2();
s32 Func_020046c0();
void Func_020046ee();
struct Object *Func_020047a6(u8 *);
struct Object *Func_020047b6(s32);
void Func_02004982(s32);
struct Object *Func_020047f6(u8 *);
struct Object *Func_02004806(s32);
void Func_020049d2(s32);
void Func_02006b88(void);
void Func_02006bbe(void);
void Func_02006d0c(void);
void Func_02006d3a(void);
void Func_02006b1c(s32 dialogue, s32 mode);
s32 Func_02006b4c(s32 flag);
void Func_02007410(void);
void Func_020075de(s32);
void Func_0200756e(s32, s32);
void Func_0200756e_a(s32, s32);
void Func_02007584(s32);
void Func_020072da(s32);
void Func_020075a4(void);
void Func_020075b8(void);
void Func_02007508(s32, s32);
u8 *Func_02007476(s32);
void Func_020073c4(u8 *, s32);
void Func_0200730a(s32);
void Func_0200757e(s32, s32);
void Func_02007328(void *, s32);
void Func_020075d0(s32, s32);
void Func_020075d0_a(s32, s32);
void Func_020075da(s32, s32);
void Func_020075f0(s32);
void Func_02007496(s32);
void Func_020075ee(s32, s32);
void Func_02007604(s32);
void Func_0200735a(s32);
void Func_020075e8(s32);
void Func_02007478(s32);
void Func_020074cc(void);
s32 Func_0200686a(s32 slot);
s32 Func_02006872(s32 slot);
s32 Func_0200687a(s32 slot);
s32 Func_02006882(s32 slot);
void Func_020009ea(s32, s32, s32);
void Func_02000a4a(s32, s32, s32);
void Func_02000a5a(s32, s32, s32);
void Func_02000a6a(s32, s32, s32);
s32 Func_02004946(s32);
void Func_02004ad0(s32);
void Func_02004b70(s32);
void Func_02004ade(s32);
s32 Func_0200811c(s32, s32);
void Func_02008204(s32, s32);
void Func_02000a7a(s32, s32, s32);
void Func_020049bc(void);
void Func_02004af2(s32);
void Func_020056f6();
void Func_0200570a();
void Func_0200571e();
void Func_02005732();
void Func_02005746();
void Func_0200575c();
void Func_02005772();
void Func_02005786();
void Func_0200579a();
void Func_020057ae();
void Func_020057c2();
void Func_020057d8();
void Func_02005960();
void Func_02005974();
void Func_02005988();
void Func_0200599c();
void Func_020059b0();
void Func_020059c6();
void Func_020059dc();
void Func_020059f0();
void Func_02005a04();
void Func_02005a18();
void Func_02005a2c();
void Func_02005a42();
void Func_0200581a();
void Func_0200582e();
void Func_02005842();
void Func_02005856();
void Func_0200586a();
void Func_02005880();
void Func_02005896();
void Func_020058aa();
void Func_020058be();
void Func_020058d2();
void Func_020058e6();
void Func_020058fc();
void Func_02005a84();
void Func_02005a98();
void Func_02005aac();
void Func_02005ac0();
void Func_02005ad4();
void Func_02005aea();
void Func_02005b00();
void Func_02005b14();
void Func_02005b28();
void Func_02005b3c();
void Func_02005b50();
void Func_02005b66();
void Func_020059e0();
s32 Func_02005a5e();
void Func_02005a80();
s32 Func_02005aa6();
s32 Func_02005aae();
void Func_02005c80();
void Func_020059a4();
void Func_02005ae4();
void Func_02005af4();
s32 Func_02005afe();
void Func_02005b4e();
void Func_02005b54();
void Func_02005b86();
void Func_02005b96();
void Func_02005c0e();
void Func_02005c1e();
void Func_02005c72();
void Func_02005c7a();
void Func_02005c9a();
void Func_02005ca6();
void Func_02005a30();
void Func_02005a54();
void Func_02005a7e();
void Func_02005b70();
s32 Func_02005b8a();
void Func_02005ba4();
void Func_02005bf2();
void Func_02005bfe();
void Func_02005c08();
void Func_02005c12();
void Func_02005c1e_a();
void Func_02005c9a_a();
void Func_02005cbc();
void Func_02005d18();
void Func_02005d22();
void Func_02005d4a();
void Func_02005d56();
void Func_02005ae0();
void Func_02005b04();
void Func_02005b2e();
void Func_02005c20();
s32 Func_02005c3a();
void Func_02005c54();
void Func_02005ca2();
void Func_02005cae();
void Func_02005cb8();
void Func_02005cc2();
void Func_02005cce();
void Func_02005d4a_a();
void Func_02005d6c();
void Func_02005dc8();
void Func_02005dd2();
void Func_02005dfa();
void Func_02005e06();
void Func_02005b90();
void Func_02005bb4();
void Func_02005bde();
void Func_02005cd0();
s32 Func_02005cea();
void Func_02005d04();
void Func_02005d52();
void Func_02005d5e();
void Func_02005d68();
void Func_02005d72();
void Func_02005d7e();
void Func_02005dfa_a();
void Func_02005e1c();
void Func_02005e78();
void Func_02005e82();
void Func_02005eaa();
void Func_02005eb6();
s32 Func_02004f1c();
void Func_02005c40();
void Func_02005c64();
void Func_02005ca0();
void Func_02005d80();
s32 Func_02005d9a();
void Func_02005dc6();
void Func_02005e02();
void Func_02005e20();
void Func_02005e20_a();
void Func_02005e22();
void Func_02005e2a();
void Func_02005e2e();
void Func_02005eaa_a();
void Func_02005ecc();
void Func_02005f28();
void Func_02005f44();
void Func_02005f6c();
void Func_02005f78();
void Func_02005d04_a();
void Func_02005d26();
void Func_02005e44();
s32 Func_02005e5e();
void Func_02005e64();
void Func_02005e72();
void Func_02005ec4();
void Func_02005ec4_a();
void Func_02005ee6();
void Func_02005ef0();
void Func_02005f06();
void Func_02005f6e();
void Func_02005f8e();
void Func_02005fe8();
void Func_02005fea();
void Func_0200600a();
void Func_02006016();
void Func_02005ecc_a();
void Func_02005eea();
void Func_02005ef0_a();
void Func_02005f0e();
void Func_02005f10();
void Func_02005f12();
void Func_02005f5a();
void Func_02005faa();
void Func_02005fd0();
void Func_02005fd6();
void Func_02005fe8_a();
void Func_0200600c();
void Func_02006024();
void Func_02006070();
void Func_0200607a();
void Func_020060a2();
void Func_020060a2_a();
void Func_020060ae();
void Func_020060e4();
void Func_020060f4();
void Func_0200611c();
void Func_0200641c();
void Func_0200642c();
void Func_02006458();
void Func_020064d2();
void Func_0200655c();
s32 Func_02006576();
void Func_02006598();
void Func_020065f0();
void Func_020065fe();
void Func_02006612();
void Func_0200662a();
void Func_0200662a_a();
void Func_0200664a();
void Func_02006650();
void Func_02006654();
void Func_0200665e();
void Func_02006672();
void Func_02006686();
void Func_02006696();
void Func_020066a4();
void Func_020066b4();
void Func_020066c6();
void Func_020066cc();
void Func_020066d2();
void Func_020066dc();
void Func_020066ea();
void Func_020066ec();
void Func_020066f2();
void Func_020066f6();
void Func_020066fc();
void Func_02006700();
void Func_0200670a();
void Func_02006712();
void Func_02006716();
void Func_0200671c();
void Func_02006722();
void Func_0200673e();
void Func_0200674e();
void Func_02006762();
void Func_02006764();
void Func_02006778();
void Func_0200677e();
void Func_02006798();
void Func_020067dc();
void Func_02006800();
void Func_02006824();
void Func_0200683a();
void Func_02006868();
void Func_02006874();
void Func_0200689c();
void Func_02006afc();
void Func_02006b2e();
void Func_02006bb6();
void Func_02006bd4();
s32 Func_02006bee();
void Func_02006c0a();
void Func_02006b38();
void Func_02006b74();
void Func_02006b8e();
void Func_02006bd0();
void Func_02006c0a_a();
s32 Func_02006c24();
void Func_02006cbc();
void Func_02006cd8();
void Func_02006e8e();
void Func_02006f44();
void Func_02006fa8();
void Func_0200701e();
void Func_0200702c();
void Func_0200704a();
void Func_02007056();
void Func_0200705e();
void Func_02007060();
void Func_020070d6();
void Func_020070dc();
void Func_020070de();
void Func_020070e2();
void Func_020070ec();
void Func_020070f4();
void Func_02007102();
void Func_02007124();
void Func_02007134();
void Func_0200713c();
void Func_02007150();
void Func_02007150_a();
void Func_0200715a();
void Func_0200715e();
void Func_02007164();
void Func_02007164_a();
void Func_0200716c();
void Func_02007172();
void Func_02007176();
void Func_02007178();
void Func_02007192();
void Func_02007194();
void Func_02007198();
void Func_0200719e();
void Func_020071a0();
void Func_020071a0_a();
void Func_020071a0_b();
void Func_020071a2();
void Func_020071b2();
void Func_020071b4();
void Func_020071b8();
void Func_020071c2();
void Func_020071c4();
void Func_020071c6();
void Func_020071ce();
void Func_020071d4();
void Func_020071da();
void Func_020071de();
void Func_020071e2();
void Func_020071f0();
void Func_020071f4();
void Func_020071fe();
void Func_020071fe_a();
void Func_02007202();
void Func_02007212();
void Func_02007212_a();
void Func_02007218();
void Func_0200721e();
void Func_02007220();
void Func_02007228();
void Func_02007238();
void Func_02007238_a();
void Func_02007248();
void Func_0200724a();
void Func_0200724c();
void Func_02007252();
void Func_0200725c();
s32 Func_02007260();
void Func_02007264();
void Func_02007268();
void Func_0200726e();
void Func_0200726e_a();
void Func_02007278();
void Func_0200729e();
void Func_020072a8();
void Func_020072b4();
void Func_020072bc();
void Func_020072c0();
void Func_020072cc();
void Func_02007302();
void Func_0200730a_a();
void Func_02007314();
void Func_0200731e();
void Func_02007334();
void Func_02007344();
void Func_0200734e();
void Func_02007360();
void Func_02007366();
void Func_02007370();
void Func_02007388();
void Func_02007390();
void Func_020073b8();
void Func_020073ba();
void Func_020073c6();
void Func_020073cc();
void Func_020073dc();
void Func_020073e2();
void Func_020073fa();
void Func_020073fc();
void Func_0200740e();
void Func_02007422();
void Func_0200742c();
void Func_02007484();
void Func_0200749a();
void Func_020074a6();
void Func_020074bc();
void Func_0200751e();
void Func_02007490();
void Func_020074b6();
void Func_020074d6();
void Func_020075d0_b();
s32 Func_02007622();
void Func_02007678();
void Func_0200767a();
void Func_0200767c();
void Func_0200769c();
void Func_020076a0();
void Func_020076aa();
void Func_020076ae();
void Func_020076b2();
void Func_020076ba();
void Func_020076c8();
void Func_020076d0();
void Func_020076e6();
void Func_020076fa();
void Func_020076fc();
void Func_02007730();
void Func_02007734();
void Func_020077a0();
void Func_020077c6();
void Func_020077f0();
void Func_0200781e();
void Func_0200782a();
void Func_0200772e();
s32 Func_02007778();
void Func_02007802();
void Func_0200780e();
void Func_0200785e();
void Func_0200786a();
s32 Func_0200787e();
s32 Func_020078b4();
s32 Func_020078c0();
void Func_020078e0();
void Func_020078ea();
s32 Func_020078f6();
void Func_020078fc();
void Func_02007946();
void Func_02007988();
void Func_02007a22();
void Func_02007a26();
void Func_02007a4c();
void Func_02007a50();
void Func_02007a7e();
void Func_02007a8a();
void Func_02007aac();
s32 Func_020078d6();
void Func_02007986();
s32 Func_0200798c();
void Func_020079e6();
s32 Func_020079f2();
void Func_02007a16();
s32 Func_02007a1c();
void Func_02007a38();
void Func_02007a5e();
void Func_02007a74();
void Func_02007a7e_a();
void Func_02007a8c();
void Func_02007abc();
void Func_02007aee();
void Func_02007be2();
s32 Func_02007af8();
s32 Func_02007b20();
void Func_02007b9e();
s32 Func_02007bd6();
s32 Func_02007c00();
void Func_02007c0c();
void Func_02007c26();
s32 Func_02007c66();
void Func_02007c70();
void Func_02007cbc();
void Func_02007d7a();
void Func_02007dc4();
s32 Func_02007dd4();
void Func_02007dda();
void Func_02007e52();
void Func_02007e9a();
void Func_02007ee2();
void Func_02007f20();
void Func_02007f2c();
void Func_02007f40();
void Func_02007f5e();
void Func_02007f9c();
void Func_02007f9e();
void Func_02007fe2();
void Func_02008014();
void Func_02008026();
void Func_02008046();
void Func_02008078();
void Func_020080b0();
void Func_020080c0();
void Func_020080e4();
void Func_020080e8();
void Func_02008120();
void Func_0200818c();
void Func_0200818e();
void Func_02008198();
void Func_020081a4();
void Func_02008318();
void Func_0200833a();
void Func_02008344();
void Func_02004d36();
void Func_02004d6c();
void Func_02004d98();
void Func_02004db4();
void Func_02004dd2();
void Func_02004de8();
void Func_02004dfa();
void Func_02004e04();
void Func_02004e0a();
void Func_02004e12();
void Func_02004e26();
void Func_02004e2c();
s32 Func_02004e30();
s32 Func_02004e30_a();
s32 Func_02004e66();
s32 Func_02004e66_a();
void Func_02004e6a();
void Func_02004e72();
void Func_02004e72_a();
void Func_02004e7c();
void Func_02004e92();
s32 Func_02004e9c();
s32 Func_02004e9c_a();
void Func_02004ea0();
void Func_02004ea8();
void Func_02004eb0();
void Func_02004eb2();
void Func_02004ebc();
void Func_02004ec8();
void Func_02004ed0();
void Func_02004ed6();
void Func_02004ede();
void Func_02004ee2();
void Func_02004ee4();
void Func_02004ee8();
void Func_02004ef8();
void Func_02004ef8_a();
void Func_02004efe();
void Func_02004f0c();
void Func_02004f1e();
void Func_02004f2c();
void Func_02004f3a();
void Func_02004f3c();
void Func_02004f4a();
void Func_02004f4c();
void Func_02004f5a();
void Func_0200501c();
void Func_0200504e();
void Func_02002586();
void Func_0200284e();
void Func_02002852();
void Func_02004dc8();
void Func_02004dec();
void Func_02004e2a();
void Func_02004efe_a();
void Func_02004f02();
void Func_02004f10();
u8 *Func_02004f2a();
void Func_02004f7e();
void Func_02004f88();
u8 *Func_02004fb0();
void Func_02004fca();
void Func_02004fcc();
void Func_02004fd0();
void Func_02004fd8();
void Func_02004fda();
void Func_02004fe4();
void Func_02004fe6();
void Func_02004fee();
void Func_02004ffc();
void Func_02005004();
void Func_0200500a();
void Func_02005012();
void Func_02005020();
u8 *Func_02005022();
void Func_0200503e();
void Func_02005042();
void Func_02005056();
void Func_02005058();
void Func_02005084();
void Func_02005084_a();
void Func_0200508a();
void Func_02005090();
void Func_02005098();
void Func_020050ac();
void Func_020050be();
void Func_020050ca();
void Func_020050cc();
void Func_020050d4();
void Func_020050da();
void Func_020050f8();
void Func_02005106();
void Func_02005106_a();
void Func_02005110();
void Func_02005114();
void Func_0200511c();
void Func_02005128();
void Func_02005136();
void Func_02005140();
void Func_02005144();
void Func_0200514a();
void Func_0200514a_a();
void Func_0200515e();
void Func_0200515e_a();
void Func_0200516a();
void Func_02005176();
void Func_02005182();
void Func_0200518c();
void Func_0200518e();
void Func_0200518e_a();
void Func_0200519a();
void Func_0200519c();
void Func_0200519c_a();
void Func_020051a2();
void Func_020051aa();
void Func_020051ac();
void Func_020051b2();
void Func_020051ba();
void Func_020051c4();
void Func_020051d6();
void Func_020051dc();
void Func_020051e0();
void Func_020051e2();
void Func_020051ec();
void Func_020051f8();
void Func_0200520a();
u8 *Func_02005210();
void Func_02005230();
void Func_02005244();
void Func_02005246();
void Func_0200525a();
void Func_0200525c();
void Func_02005266();
void Func_02005266_a();
void Func_0200526c();
void Func_02005274();
void Func_02005280();
void Func_02005294();
void Func_0200529a();
void Func_020052a2();
void Func_020052a6();
void Func_020052ae();
void Func_020052b0();
void Func_020052ba();
void Func_020052d0();
void Func_020052d0_a();
void Func_020052e6();
void Func_02005302();
void Func_02005304();
void Func_02005304_a();
void Func_02005318();
void Func_0200531c();
void Func_02005324();
void Func_02005336();
void Func_02005340();
void Func_02005346();
void Func_0200534c();
void Func_02005356();
void Func_02005358();
void Func_02005366();
void Func_02005366_a();
void Func_0200536e();
void Func_0200537c();
void Func_02005380();
void Func_02005382();
void Func_02005396();
void Func_02005398();
void Func_020053a4();
void Func_020053a6();
void Func_020053b0();
void Func_020053b0_a();
void Func_020053b2();
void Func_020053c4();
void Func_020053d2();
void Func_020053d6();
void Func_020053e2();
void Func_020053fc();
void Func_02005410();
void Func_02005412();
void Func_02005418();
void Func_0200541c();
void Func_02005422();
void Func_02005426();
void Func_02005436();
u8 *Func_02005464();
void Func_02005466();
void Func_02005478();
void Func_02005478_a();
u8 *Func_02005488();
u8 *Func_02005494();
void Func_02005496();
void Func_020054a2();
u8 *Func_020054a6();
void Func_020054ac();
u8 *Func_020054b2();
void Func_020054be();
void Func_020054d4();
void Func_020054d6();
void Func_020054ea();
void Func_020054ee();
void Func_020054fa();
void Func_02005588();
void Func_02005592();
void Func_02005596();
void Func_020055a4();
void Func_020055b6();
void Func_020055c0();
void Func_020055d0();
void Func_020055e2();
void Func_020055e8();
void Func_020055ec();
void Func_020055f4();
void Func_020055fe();
void Func_02005606();
void Func_02005612();
void Func_02005614();
void Func_02005620();
void Func_02005622();
void Func_02005624();
void Func_02005626();
void Func_02005630();
void Func_02005636();
void Func_0200563a();
void Func_0200563c();
void Func_02005646();
void Func_02005646_a();
void Func_0200564c();
void Func_0200564e();
void Func_02005650();
void Func_02005652();
void Func_02005652_a();
void Func_02005658();
void Func_0200565c();
void Func_0200565c_a();
void Func_0200565e();
void Func_02005662();
void Func_0200566c();
void Func_0200566e();
void Func_02005676();
void Func_0200567a();
void Func_02005680();
void Func_02005698();
void Func_0200569c();
void Func_020056aa();
void Func_020056b0();
void Func_020056c2();
void Func_020056ce();
void Func_020056d0();
void Func_020056d6();
void Func_020056e4();
void Func_0200570a_a();
void Func_02005736();
void Func_02005742();
void Func_0200576a();
void Func_0200576e();
u8 *Func_02005778();
void Func_02005782();
void Func_0200578e();
void Func_02005796();
void Func_020057a6();
void Func_020057ae_a();
void Func_020057b2();
void Func_020057ba();
void Func_020057be();
void Func_020057c4();
void Func_020057cc();
void Func_020057cc_a();
void Func_020057ce();
void Func_020057d4();
void Func_020057d8_a();
void Func_020057da();
void Func_020057e8();
void Func_020057fa();
void Func_02005810();
void Func_02005810_a();
void Func_02005818();
void Func_0200581c();
void Func_02005830();
void Func_02005836();
void Func_0200583c();
void Func_02005892();
void Func_020058aa_a();
void Func_020058c4();
void Func_020058ec();
void Func_02005914();
void Func_02005920();
s32 Func_02007708(s32);
struct StorySelectionActor *Func_02007982(s32 actor);
s32 Func_0200793e(s32 state);
s32 Func_02007944(s32 flag);
void Func_02007974(s32 state, s32 value);
void Func_02006e9e(s32 reference_actor);
void *Func_02007b28(s32 actor);
void Func_02007c22(s32 actor, s32 mode);
void Func_02007c34(s32 actor, s32 mode);
void Func_02007a90(s32 flag);
void Func_02007a92(s32 flag);
void Func_02007ab2(s32 flag, s32 value);
void Func_02007af0(u32 address, u32 value);
void Func_02007b0a(u32 address, u32 value);
void Func_02007b34(u32 address, u32 value);
void Func_0200818a(u8 *, s32);
void Func_02007f98(u8 *);
s32 Func_020080e0(s32, s32);
void Func_020081c8(u8 *, s32);
void Func_02007fd4(u8 *);
void Func_020081e4();
s32 Func_0200817c();
void Func_02008234();
s32 Func_020081cc();

/*
 * Story-progress triggers -- resource_371.  Each fires one event once the
 * progress word has reached nine tenths of the level word; each seventy-two
 * byte owner includes its three pool words.
 */

/* Actor callbacks for the story transfer scene. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address; the declarations stay old-style because the interfaces
 * are unknown.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2);
}

static __inline__ void Call4_020017fc(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

static __inline__ void Call4_02001888(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

static __inline__ void Call4_02001938(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

static __inline__ void Call4_020019e8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    return f();
}

static __inline__ void Call1_02001a98(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0);
}

static __inline__ void Call4_02001a98(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The actor index every call in this scene step operates on. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001b5c(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0);
}

static __inline__ void Call4_02001b5c(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    return f(a0, a1);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2_020032f0(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    f(a0, a1);
}

static __inline__ u8 *Pointer1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4_02000c28(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/*
 * The progress word is Data_02000240[284] read as a whole word and the level
 * word is the workspace at +428; both offsets are built in one register, so
 * the order in which the locals are declared is what reproduces the
 * reference.  Each Func_ name labels the call word of one call site rather
 * than a runtime address, so the same service appears under several names,
 * and the declarations stay old-style because the interfaces are unknown and
 * argument counts differ between call sites.
 */
void StoryProgress_TriggerEvent0808(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *workspace = Data_03001ebc;
    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_020041c2(*level * 9, 10)) {
        if ((u32)Func_020041f4() < 0x8000) {
            Func_02004484(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_0200421c(*level * 9, 10)) {
        Func_020044d2(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{
    extern s16 Data_02000240[];
    extern u8 *Data_03001ebc;

    s16 *state_table = Data_02000240;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = Data_03001ebc;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_02004264(*level * 9, 10)) {
        Func_0200451a(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryActor_AdvanceTimer(u8 *actor)
{
    extern u32 Data_03001e40;

    u16 *timer = (u16 *)(actor + 0x64);

    /*
     * Arm order decides the branch sense: the fall-through is the increment
     * and the taken branch is the call.  Swapping the arms inverts the test.
     */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Func_02004348(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{
    extern u32 Data_03001e40;

    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((Data_03001e40 & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((Data_03001e40 & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Func_02004378(0x11d, x, y, z);
    }
    Func_02004618(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Func_020043dc(spawned_actor, 0);
        Func_0200439c(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)Data_0200811d;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    extern u32 Data_03001e40;

    u8 *actor_flags;
    s32 fixed_scale;

    if (Func_020044e4(0x30) != 0) {
        return 0;
    }
    if (Func_020044f0(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)Data_020081c5;
    actor_flags = actor + 0x55;
    *actor_flags = 0;
    actor_flags += 0xF;
    *(u16 *)actor_flags = 0;
    actor_flags += 2;
    *(u16 *)actor_flags = 0;
    fixed_scale = 0x8000;
    *(s32 *)(actor + 0x18) = fixed_scale;
    *(s32 *)(actor + 0x1C) = fixed_scale;
    return 0;
}

u8 *SceneData_GetTableD27C(void)
{
    return Data_0200d27c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableDA2C(void)
{
    return Data_0200da2c;
}

s32 StoryActor_ApplyFlaggedMode(u8 *actor)
{
    extern u32 Data_03001e40;

    Func_02000576();
    if (Func_0200453c((s32)&Value_00000847) != 0) {
        Func_020044b8(actor, 2);
    }
    return 1;
}

s32 StoryActor_ResetPosition(u8 *actor)
{
    extern u32 Data_03001e40;

    s32 zero;
    Func_02004560(actor, 0);
    Func_02004580(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (Func_020045c0(0x8A0) != 0) {
        Func_020045d2((s32)&Value_000002f1);
        *(s32 *)(actor + 8) = zero;
        *(s32 *)(actor + 12) = zero;
    }
    return 0;
}

/*
 * The popped register is r1, so r0 survives the return and is the result.
 * The owner includes its alignment halfword and its one pool word, the
 * address of Data_03001e40 -- a live status word, not overlay image data.
 * The exclusive or against the bit just tested clears bit 0 only.  value is
 * a byte, so value >> 8 is zero and both branches do the same clear and
 * store; that shape is deliberate and decides the register allocation.
 */
s32 StoryActor_ClearActiveFlag(u8 *actor)
{
    extern u32 Data_03001e40;

    u8 *active_flags = actor + 0x54;
    u8 value = *active_flags;
    u32 active_bit = 1 + (value >> 8);

    if ((active_bit & value) != 0 && (Data_03001e40 & active_bit) != 0) {
        u8 cleared = 1;

        if (active_flags) {
            cleared ^= value;
            *active_flags = (u8)cleared;
        } else {
            cleared ^= value;
            *active_flags = (u8)cleared;
        }
    }
    return 1;
}

/* Each Func_ name identifies one call site's loader-relocated call word rather
 * than a runtime address, so several names here reach the same import. The
 * declarations are old-style because the interfaces are not established. */

/*
 * Selects the placement and spawn table for the current scene. The selector is
 * the signed halfword Data_02000240[225]; only 49 through 80 are covered and
 * everything else takes the default arm, which calls Func_020046ee before
 * returning. Cases 49 and 64 are conditional and fall through to the default
 * when their test fails. The case arms are in the order the reference uses,
 * not ascending, and that order is what reproduces it.
 */
u8 *StoryScene_SelectPlacementTable(void)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    s16 *scene_table = Data_02000240;
    s32 scene_id = scene_table[225];

    switch (scene_id) {
    case 49:
        if (Func_020046a8(0x94f) == 0 && Func_020046b2(0x941) != 0) {
            return Data_0200e154;
        }
        break;
    case 64:
        if (Func_020046c0(0x85a) == 0) {
            return Data_0200e04c;
        }
        break;
    case 65:
    case 70:
        return Data_0200e1e4;
    case 71:
        return Data_0200e28c;
    case 72:
        return Data_0200e394;
    case 73:
        return Data_0200e3c4;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return Data_0200e25c;
    case 80:
        return Data_0200e2ec;
    default:
        break;
    }

    Func_020046ee(0x235);
    return Data_0200db84;
}

/* Publishes one of two branch values at +0x170 of the scene state, chosen by
 * comparing the other actor's x against the subject's. */
void StoryScene_SetBranchValueFromX(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Func_020047a6(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Func_020047b6(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Func_02004982(0x7B);
}

/* The same branch value, chosen on z instead of x. */
void StoryScene_SetBranchValueFromZ(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Func_020047f6(actor_object - 0x64);
    scene_table = Data_02000240;
    other_actor = Func_02004806(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)0x03001ebc;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Func_020049d2(0x7B);
}

void SceneState_SetValues130_6_47(void)
{
    extern volatile s32 Data_03001e40;

    Func_020009ea(0x82, 6, 0x2F);
}

void SceneState_ApplyValues150And46And11(void)
{
    extern volatile s32 Data_03001e40;

    Func_02000a4a(0x96, 0x2E, 0x0B);
}

void SceneState_ApplyValues116And56And21(void)
{
    extern volatile s32 Data_03001e40;

    Func_02000a5a(0x74, 0x38, 0x15);
}

void SceneState_ApplyValues151And25And54(void)
{
    extern volatile s32 Data_03001e40;

    Func_02000a6a(0x97, 0x19, 0x36);
}

void FieldScene_RunStep7D3B1E(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    Func_02000a7a(0x7D, 0x3B, 0x1E);
}

u8 *SceneData_GetTableE3F4(void)
{
    return Data_0200e3f4;
}

void SceneState_ApplyFlag85aBranch(void)
{
    extern volatile s32 Data_03001e40;

    if (Func_02004946((s32)&Value_0000085a) == 0) {
        Func_02004ad0(101);
    } else {
        Func_02004b70(123);
        Func_02004ade(3);
    }
}

void FieldScene_RunStep74(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    Func_020049bc();
    Func_02004af2(74);
}

void RunEventScript01(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02004d6c();
    Func_02004ed0();
    Func_02004ee4();
    Func_02004ef8();
    Call1(Func_02004d36, 0x94f);
    Call3(Func_02004e0a, 11, 0x16e00000, 0x49c0000);
    Func_02004e04(11, 24, 8);
    Func_02004e12(11);
    Func_02004d98(60);
    Call3(Func_02004e2c, 12, 0x16e00000, 0x49c0000);
    Func_02004e26(12, 12, 24);
    Func_02004db4(30);
    Call3(Func_02004eb0, 11, 0x5000, 0);
    Call3(Func_02004ebc, 12, 0xd000, 0);
    Func_02004dd2(60);
    Func_02004e6a(11, 3);
    Func_02004e72(12, 3);
    Func_02004de8(120);
    Call3(Func_02004e7c, 8, 0x16f80000, 0x4b80000);
    Func_02004dfa(60);
    Func_02004e92(12, 2);
    record = Value1(Func_02004e30, 8);
    if (record != 0) {
        Func_02004e72_a(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004ea0(12);
    Func_02004eb2(12, 0, 0);
    ((void (*)())Func_02004e30_a)(60);
    Func_02004ec8(11, 2);
    record = Value1(Func_02004e66, 8);
    if (record != 0) {
        Func_02004ea8(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004ed6(11);
    Func_02004ee8(11, 0, 0);
    ((void (*)())Func_02004e66_a)(60);
    Func_02004efe(0, 2);
    record = Value1(Func_02004e9c, 8);
    if (record != 0) {
        Func_02004ede(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004f0c(0);
    Func_02004f1e(0, 0, 0);
    ((void (*)())Func_02004e9c_a)(60);
    Call3(Func_02004ee2, 8, 0x8000, 0x4000);
    Func_02004f2c(8, 56, 8);
    Func_02004f3a(8);
    Func_02004f3c(8, 40, 40);
    Func_02004f4a(8);
    Func_02004f4c(8, 8, 88);
    Func_02004f5a(8);
    Func_0200504e();
    Func_0200501c(108);
    Func_02004ef8_a();
}

void FieldScene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Pointer1(Func_02004f2a, 15);
    Func_02004f10();
    Call2(Func_020050ca, 0x14000, 1);
    Func_02004dc8(4);
    Func_02005084();
    Func_02005098();
    Func_020050ac();
    Call4_02000c28(Func_02005056, -1, -1, -1, 0);
    Func_02004dec(1);
    Call3(Func_02004f7e, 0, 0x19999, 0xcccc);
    Call3(Func_02004f88, 1, 0x19999, 0xcccc);
    Call3(Func_02004fcc, 0, 0x16fc, 0x628);
    Call3(Func_02005058, 0, 0x6000, 0);
    Call3(Func_02004ffc, 8, 0x16d80000, 0x6280000);
    Func_02004e2a(1);
    Func_02005042(8, 15);
    record = Func_02004fb0(8);
    Func_02004efe_a(record, 0);
    Call3(Func_02004fd0, 10, 0x19999, 0x6666);
    Call3(Func_02004fda, 11, 0x19999, 0x6666);
    Call3(Func_02004fe4, 12, 0x19999, 0x6666);
    Call3(Func_02004fee, 13, 0x19999, 0x6666);
    Func_0200518c(141);
    Call2(Func_02005004, 10, 0x200c9e4);
    Func_02004fca(20);
    Call2(Func_02005012, 11, 0x200cb50);
    Func_02004fd8(20);
    Call2(Func_02005020, 12, 0x200cc74);
    Func_02004fe6(20);
    Call2(Func_0200503e, 13, (s32)Data_0200cd98);
    Call1(Func_020051c4, 0x121);
    record = Pointer1(Func_02005022, 0);
    if (record != 0) {
        Func_02005090(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02005084_a, 1, 0x1704, 0x640);
    Call3(Func_02005110, 0, 0x2000, 0);
    Call3(Func_0200511c, 1, 0xa000, 20);
    Call3(Func_02005136, 0, 0x101, 0);
    Call3(Func_02005140, 1, 0x101, 60);
    Call3(Func_020050d4, 8, 0x16d80000, 0x6380000);
    Func_02004f02(1);
    Call1(Func_02005128, 0x1215);
    Func_0200514a(8, 0, 10);
    Call3(Func_0200515e, 0, 0x6000, 0);
    Call3(Func_0200516a, 1, 0x6000, 40);
    Call3(Func_02005176, 0, 0x2000, 0);
    Call3(Func_02005182, 1, 0xa000, 60);
    Call3(Func_0200518e, 0, 0x6000, 0);
    Call3(Func_0200519a, 1, 0x6000, 10);
    Func_02005280(141);
    Call2(Func_020050f8, 10, 0x200ca78);
    Func_020050be(20);
    Call2(Func_02005106, 11, 0x200cbe4);
    Func_020050cc(20);
    Call2(Func_02005114, 12, 0x200cd08);
    Func_020050da(10);
    Call3(Func_020051d6, 0, 0x4000, 0);
    Call3(Func_020051e2, 1, 0x4000, 10);
    Call2(Func_0200514a_a, 13, 0x200ce2c);
    Call1(Func_020052d0, 0x121);
    Func_02005106_a(20);
    Func_0200529a();
    Func_020051a2(10, 1);
    Func_020051aa(11, 1);
    Func_020051b2(12, 1);
    Func_020051ba(13, 1);
    Call4_02000c28(Func_0200525a, 0x16080000, -1, 0x6f80000, 1);
    Func_02005266();
    Func_02005144(20);
    Func_020052d0_a();
    Call3(Func_020051dc, 9, 0x16080000, 0x6d80000);
    Func_0200500a(1);
    Call3(Func_0200519c, 9, 0x13333, 0x9999);
    Call3(Func_020051e0, 9, 0x1608, 0x6c8);
    Call3(Func_020051ec, 9, 0x15f8, 0x6c8);
    Call3(Func_020051f8, 9, 0x15f8, 0x6f8);
    Func_0200518e_a(20);
    Func_02005246(9, 2);
    Func_0200519c_a(20);
    Call2(Func_020052ae, 9, 0x102);
    Func_020051ac(60);
    Func_020052a6(9, 0, 20);
    Func_02005266_a(9, 3);
    Func_020052b0(9, 0, 20);
    Call3(Func_0200525c, 8, 0x16180000, 0x6f80000);
    Func_0200508a(1);
    Func_020052a2(8, 0);
    record = Func_02005210(8);
    Func_0200515e_a(record, 1);
    Call3(Func_02005230, 8, 0xcccc, 0x6666);
    Call3(Func_02005274, 8, 0x1608, 0x6f8);
    Func_0200520a(20);
    Func_020052ba(8, 2);
    Call3(Func_02005304, 0x2008, 0, 10);
    Call3(Func_02005318, 8, 0x3000, 60);
    Call3(Func_02005324, 8, 0x8000, 10);
    Call2(Func_02005346, 8, 0x102);
    Func_02005244(60);
    Call3(Func_02005340, 9, 0x3000, 0);
    Call3(Func_0200534c, 8, 0x3000, 40);
    Call2(Func_0200536e, 8, 0x102);
    Func_0200526c(60);
    Func_0200531c(8, 2);
    Call3(Func_02005366, 0x2008, 0, 40);
    Func_02005336(9, 1);
    Func_02005380(9, 0, 10);
    Func_02005382(9, 0, 10);
    Call3(Func_020053a4, 8, 0x105, 60);
    Call3(Func_02005396, 0x2008, 0, 10);
    Func_02005366_a(8, 1);
    Func_02005356(8, 3);
    Call3(Func_020053b0, 0x2008, 0, 10);
    Call3(Func_020053d2, 9, 0x101, 60);
    Func_020053c4(9, 0, 20);
    Func_0200537c(8, 3);
    Func_02005478();
    Func_020054be(107);
    Call2(Func_02005412, 0x40000, 0x40000);
    Func_02002586();
    Call1(Func_020054d4, 0x121);
    Call3(Func_02005410, 8, 0x100, 0);
    Call3(Func_0200541c, 9, 0x100, 0);
    Call3(Func_02005418, 8, 0x8000, 0);
    Func_02005422(9, 0, 40);
    Call3(Func_02005496, 8, 0xb000, 0);
    Call3(Func_020054a2, 9, 0xb000, 0);
    Call2(Func_020054d6, 0x10000, 0x2000);
    Call4_02000c28(Func_020054ee, 0x15e80000, -1, 0x6c80000, 1);
    Func_020054fa();
    Call3(Func_02005466, 14, 0x15a80000, 0x6a80000);
    Func_02005294(1);
    Call3(Func_02005426, 14, 0x4ccc, 0x2666);
    Call2(Func_02005436, 14, 0x200cebc);
    Func_020053fc(160);
    *(s32 *)(actor + 72) = 0x1999;
    *(s32 *)(actor + 68) = 0x1999;
    *(s32 *)(actor + 24) = 0x18000;
    *(s32 *)(actor + 28) = 0x18000;
    {
        s32 shown = 0;

        *(u16 *)(actor + 100) = shown;
    }
    *(s32 *)(actor + 12) = 0x400000;
    {
        u8 *target = *(u8 **)(actor + 80);
        s32 shown = 0xf000;

        *(u16 *)(target + 30) = shown;
    }
    Func_02005398(actor, 0);
    Func_02005358(actor, 2);
    Func_020052e6(1);
    record = Func_02005464(15);
    Func_020053b2(record, 0);
    Call2(Func_02005304_a, 0x20097a5, 0xc80);
    do {
        Func_02005302(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Func_02005488(15);
    Func_020053d6(record, 0);
    record = Func_02005494(14);
    Func_020053e2(record, 0);
    Func_02005478_a(10);
    scale = 192;
    record = Func_020054a6(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Pointer1(Func_020054b2, 8);
    *(s32 *)(record + 40) = (scale << 11);
    Func_02005662(145);
    Call2(Func_020055b6, 0x40000, 0x40000);
    Func_0200284e();
    Func_02002852();
    Func_020054ac(60);
    Call2(Func_020055d0, 0x20000, 0x4000);
    Call4_02000c28(Func_020055e8, 0x16080000, -1, 0x6f80000, 1);
    Func_020055f4();
    Func_02005658();
    Call2(Func_020055e2, 9, 0x102);
    Call2(Func_020055ec, 8, 0x102);
    Func_020054ea(60);
    Call1(Func_020053b0_a, 0x20097a5);
    Func_020053a6(1);
    Func_02005588(14, 0, 0);
    Func_02005592(15, 0, 0);
    Call3(Func_02005606, 8, 0x8000, 10);
    Func_020055c0(8, 4, 40);
    Call3(Func_02005612, 0x2008, 0, 10);
    Func_02005624(9, 0, 10);
    Func_02005626(9, 0, 20);
    Call3(Func_0200563a, 8, 0xc000, 40);
    Call3(Func_0200563c, 0x2008, 0, 20);
    Func_020055fe(9, 4, 20);
    Func_02005650(9, 0, 10);
    Func_02005620(8, 1);
    Call3(Func_0200566c, 8, 0x8000, 10);
    Call3(Func_0200566e, 0x2008, 0, 10);
    Call2(Func_02005698, 9, 0x102);
    Func_02005596(80);
    Func_02005636(8, 3);
    Func_020055a4(20);
    Func_0200565c(9, 1);
    Func_0200564c(9, 3);
    Call3(Func_02005630, 8, 0x1618, 0x6f8);
    Func_02005652(8, 0, 0);
    Call3(Func_02005646, 9, 0x15f8, 0x6c8);
    Call3(Func_02005652_a, 9, 0x1608, 0x6c8);
    Call3(Func_0200565e, 9, 0x1608, 0x6d8);
    Func_02005680(9, 0, 0);
    Func_020057ce(141);
    Call2(Func_02005646_a, 10, (s32)Data_0200cac4);
    Call2(Func_0200564e, 11, 0x200cc30);
    Func_02005614(40);
    Call2(Func_0200565c_a, 12, 0x200cd54);
    Func_02005622(40);
    Call2(Func_0200567a, 13, (s32)Data_0200ce78);
    Func_020057be();
    Call3(Func_020056c2, 0, 0x170c0000, 0x6280000);
    Call3(Func_020056ce, 1, 0x17140000, 0x6400000);
    Call2(Func_0200576a, 0x40000, 0x8000);
    Call4_02000c28(Func_02005782, 0x16d80000, -1, 0x6480000, 1);
    Func_0200578e();
    action = (s32)Data_0200caf4;
    Func_020056b0(10, action);
    Func_02005676(20);
    Call2(Func_02005796, 0x6666, 0xccc);
    Call4_02000c28(Func_020057ae_a, 0x16d80000, -1, 0x6080000, 1);
    Func_020056d6(11, action);
    Func_0200569c(20);
    Func_020056e4(12, action);
    Func_020056aa(20);
    Call3(Func_020057a6, 0, 0x8000, 0);
    Call3(Func_020057b2, 1, 0x8000, 0);
    Func_0200570a_a(13, action);
    Func_020056d0(40);
    Call3(Func_020057cc, 0, 0xc000, 0);
    Call3(Func_020057d8_a, 1, 0xc000, 0);
    Func_02005736(13);
    Call1(Func_020058c4, 0x121);
    Call2(Func_02005818, 0x40000, 0x8000);
    Call4_02000c28(Func_02005830, 0x16f80000, -1, 0x6480000, 1);
    Func_0200583c();
    Call3(Func_02005810, 0, 0x2000, 0);
    Call3(Func_0200581c, 1, 0xa000, 80);
    Func_020057c4(0, 3);
    Func_020057d4(1, 3);
    Func_02005742(20);
    Func_020057da(1, 2);
    record = Pointer1(Func_02005778, 0);
    if (record != 0) {
        Func_020057ba(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020057e8(1);
    Func_020057fa(1, 0, 0);
    Call2(Func_02005892, 0xcccc, 0x1999);
    Call4_02000c28(Func_020058aa_a, 0x16d80000, -1, 0x6480000, 1);
    Call3(Func_020057cc_a, 0, 0xcccc, 0x6666);
    Call3(Func_02005810_a, 0, 0x16d8, 0x628);
    Func_02005914();
    Func_02005920();
    Call1(Func_0200576e, 0x85a);
    Func_020058ec(3);
    Func_02005836();
}

void FieldScene_RunScene371_0200155c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 record;

    Call4(Func_02005960, 0x160c0000, -1, 0x6f80000, 1);
    Func_020056f6(4);
    Call4(Func_02005974, 0x16040000, -1, 0x6fc0000, 1);
    Func_0200570a(4);
    Call4(Func_02005988, 0x160c0000, -1, 0x6f40000, 1);
    Func_0200571e(4);
    Call4(Func_0200599c, 0x160c0000, -1, 0x6fc0000, 1);
    Func_02005732(4);
    Call4(Func_020059b0, 0x16040000, -1, 0x6f40000, 1);
    Func_02005746(4);
    Call4(Func_020059c6, 0x16080000, -1, 0x6f80000, 1);
    Func_0200575c(4);
    Call4(Func_020059dc, 0x160a0000, -1, 0x6f80000, 1);
    Func_02005772(4);
    Call4(Func_020059f0, 0x16060000, -1, 0x6fa0000, 1);
    Func_02005786(4);
    Call4(Func_02005a04, 0x160a0000, -1, 0x6f60000, 1);
    Func_0200579a(4);
    Call4(Func_02005a18, 0x160a0000, -1, 0x6fa0000, 1);
    Func_020057ae(4);
    Call4(Func_02005a2c, 0x16060000, -1, 0x6f60000, 1);
    Func_020057c2(4);
    Call4(Func_02005a42, 0x16080000, -1, 0x6f80000, 1);
    Func_020057d8(4);
}

void FieldScene_RunScene371_02001680(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 record;

    Call4(Func_02005a84, 0x15ec0000, -1, 0x6c80000, 1);
    Func_0200581a(4);
    Call4(Func_02005a98, 0x15e40000, -1, 0x6cc0000, 1);
    Func_0200582e(4);
    Call4(Func_02005aac, 0x15ec0000, -1, 0x6c40000, 1);
    Func_02005842(4);
    Call4(Func_02005ac0, 0x15ec0000, -1, 0x6cc0000, 1);
    Func_02005856(4);
    Call4(Func_02005ad4, 0x15e40000, -1, 0x6c40000, 1);
    Func_0200586a(4);
    Call4(Func_02005aea, 0x15e80000, -1, 0x6c80000, 1);
    Func_02005880(4);
    Call4(Func_02005b00, 0x15ea0000, -1, 0x6c80000, 1);
    Func_02005896(4);
    Call4(Func_02005b14, 0x15e60000, -1, 0x6ca0000, 1);
    Func_020058aa(4);
    Call4(Func_02005b28, 0x15ea0000, -1, 0x6c60000, 1);
    Func_020058be(4);
    Call4(Func_02005b3c, 0x15ea0000, -1, 0x6ca0000, 1);
    Func_020058d2(4);
    Call4(Func_02005b50, 0x15e60000, -1, 0x6c60000, 1);
    Func_020058e6(4);
    Call4(Func_02005b66, 0x15e80000, -1, 0x6c80000, 1);
    Func_020058fc(4);
}

void FieldScene_RunScene371_020017a4(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005aa6, 15);
    record = Value1(Func_02005aae, 14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (Value1(Func_02005a5e, 0x200) == 0) {
            Func_02005c80(145);
            Func_020059e0(rec7, 3);
            Call1(Func_02005a80, 0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}

void FieldScene_RunScene371_020017fc(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005afe, 8);
    Func_02005ae4();
    Call4_020017fc(Func_02005c0e, -1, -1, -1, 0);
    Func_020059a4(1);
    Func_02005b86(0, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Func_02005c1e(8, 1);
    Func_02005c7a();
    Call3(Func_02005b54, 8, 0x6666, 0x3333);
    Call3(Func_02005b96, 8, 0x14a8, 0x918);
    Func_02005c9a();
    Func_02005ca6();
    Call1(Func_02005af4, 0x927);
    Func_02005c72(102);
    Func_02005b4e();
}

void FieldScene_RunScene371_02001888(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005b8a, 8);
    Func_02005b70();
    Call4_02001888(Func_02005c9a_a, -1, -1, -1, 0);
    Func_02005a30(1);
    Func_02005c12(0, 0, 0);
    Call3(Func_02005c1e_a, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005a54(1);
    Func_02005cbc(8, 1);
    Func_02005d18();
    Call3(Func_02005bf2, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005c08(8, 0x200d158);
    do {
        Func_02005a7e(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005d4a();
    Func_02005d56();
    Call1(Func_02005ba4, 0x927);
    Func_02005d22(103);
    Func_02005bfe();
}

void FieldScene_RunScene371_02001938(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005c3a, 8);
    Func_02005c20();
    Call4_02001938(Func_02005d4a_a, -1, -1, -1, 0);
    Func_02005ae0(1);
    Func_02005cc2(0, 0, 0);
    Call3(Func_02005cce, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005b04(1);
    Func_02005d6c(8, 1);
    Func_02005dc8();
    Call3(Func_02005ca2, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005cb8(8, 0x200d158);
    do {
        Func_02005b2e(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005dfa();
    Func_02005e06();
    Call1(Func_02005c54, 0x927);
    Func_02005dd2(104);
    Func_02005cae();
}

void FieldScene_RunScene371_020019e8(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005cea, 8);
    Func_02005cd0();
    Call4_020019e8(Func_02005dfa_a, -1, -1, -1, 0);
    Func_02005b90(1);
    Func_02005d72(0, 0, 0);
    Call3(Func_02005d7e, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005bb4(1);
    Func_02005e1c(8, 1);
    Func_02005e78();
    Call3(Func_02005d52, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005d68(8, 0x200d158);
    do {
        Func_02005bde(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005eaa();
    Func_02005eb6();
    Call1(Func_02005d04, 0x927);
    Func_02005e82(105);
    Func_02005d5e();
}

void FieldScene_RunScene371_02001a98(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005d9a, 8);
    Func_02005d80();
    Call4_02001a98(Func_02005eaa_a, -1, -1, -1, 0);
    Func_02005c40(1);
    Func_02005e22(0, 0, 0);
    Call3(Func_02005e2e, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005c64(1);
    Func_02005ecc(8, 1);
    Func_02005f28();
    Call3(Func_02005e02, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (Value0(Func_02004f1c) == 11) {
        Func_02005e20(8, 0x200d1b8);
    } else {
        Func_02005e2a(8, 0x200d158);
    }
    do {
        Func_02005ca0(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005f6c();
    Func_02005f78();
    Call1_02001a98(Func_02005dc6, 0x927);
    Func_02005f44(106);
    Func_02005e20_a();
}

void FieldScene_RunScene371_02001b5c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005e5e, 8);
    Func_02005e44();
    Call4_02001b5c(Func_02005f6e, -1, -1, -1, 0);
    Func_02005d04_a(1);
    Func_02005ee6(0, 0, 0);
    Call3(Func_02005ef0, 8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Func_02005d26(1);
    Func_02005f8e(8, 1);
    Func_02005fea();
    Call3(Func_02005ec4, 8, 0x6666, 0x3333);
    Call3(Func_02005f06, 8, 0x13c8, 0x918);
    Func_0200600a();
    Func_02006016();
    Call1_02001b5c(Func_02005e64, 0x93e);
    Call1_02001b5c(Func_02005e72, 0x927);
    Func_02005fe8(107);
    Func_02005ec4_a();
}

void FieldScene_RunScene371_02001c08(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 record;

    Func_02006070();
    Call2(Func_020060a2, 0x10000, 6);
    Func_020060ae();
    Func_0200607a();
    Func_02005faa(8, 2);
    Call1_02001b5c(Func_02005fd0, 0xc66);
    Func_02005fe8_a(8, 0);
    Func_02005f0e(30);
    Func_020060e4(111);
    Func_02005ecc_a(0, 2);
    Call1_02001b5c(Func_02005eea, 0x16f);
    Call1_02001b5c(Func_02005ef0_a, 0x171);
    Func_020060f4();
    Func_02005fd6(8, 4, 30);
    Call1_02001b5c(Func_0200600c, 0xc67);
    Func_02006024(8, 0);
    Call1_02001b5c(Func_02005f12, 0x16f);
    Call1_02001b5c(Func_02005f10, 0x171);
    Func_0200611c();
    Func_02005f5a(30);
    Func_020060a2_a(12, 6);
}

void FieldScene_RunScene371_02002274(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02006576, 10);
    Func_0200655c();
    Call4_02001b5c(Func_02006686, -1, -1, -1, 0);
    Func_0200641c(1);
    Func_020065fe(0, 0, 0);
    Func_0200642c(1);
    *(volatile s32 *)(rec7 + 24) = 0x18000;
    *(volatile s32 *)(rec7 + 28) = 0x18000;
    {
        s32 shown = 0x4000;

        *(volatile u16 *)(rec7 + 6) = shown;
    }
    Func_020066f6();
    Func_0200670a();
    Func_02006598(20);
    Call3(Func_0200662a, 10, 0x15680000, 0x8380000);
    Func_02006458(1);
    Func_0200677e(141);
    Call3(Func_020065f0, 10, 0x19999, 0x6666);
    Func_02006650(10, 2);
    Call3(Func_0200662a_a, 10, 0x156d, 0x858);
    Call2(Func_020066ea, 0x6666, 0xccc);
    Call4_02001b5c(Func_02006700, 0x15b80000, -1, 0x8580000, 1);
    Call3(Func_0200664a, 10, 0x159e, 0x858);
    Call3(Func_02006654, 10, 0x15a8, 0x86e);
    Call3(Func_0200665e, 10, 0x15e8, 0x878);
    Func_02006696(10, 1);
    Call1_02001b5c(Func_020067dc, 0x121);
    Func_02006612(20);
    Call3(Func_020066a4, 0, 0x15d80000, 0x8780000);
    Func_020064d2(1);
    Func_020066cc(0, 6, 0);
    Call3(Func_02006672, 0, 0x20000, 0x10000);
    Call3(Func_020066b4, 0, 0x15c8, 0x878);
    Func_0200673e(0, 0, 40);
    Func_02006824(141);
    Func_020066ec(10, 2);
    Call3(Func_020066c6, 10, 0x15f8, 0x878);
    Call3(Func_02006762, 0, 0xe000, 0);
    Call3(Func_020066dc, 10, 0x15f8, 0x838);
    Call3(Func_02006778, 0, 0xc000, 0);
    Call3(Func_020066f2, 10, 0x15bd, 0x838);
    Call3(Func_020066fc, 10, 0x15b8, 0x853);
    Call3(Func_02006798, 0, 0xa000, 0);
    Call3(Func_02006712, 10, 0x1572, 0x858);
    Call3(Func_0200671c, 10, 0x1568, 0x838);
    Func_0200674e(10, 0, 0);
    Call1_02001b5c(Func_0200689c, 0x121);
    Func_020066d2(40);
    Call4_02001b5c(Func_02006800, 0x15d80000, -1, 0x8580000, 1);
    Call3(Func_02006722, 0, 0xcccc, 0x6666);
    Call3(Func_02006764, 0, 0x15d8, 0x858);
    Func_02006868();
    Func_02006874();
    Func_0200683a(20);
    Func_02006716();
}

void FieldScene_RunScene371_0200281c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 record;

    Func_02006afc();
    Func_02006bb6(55, 0, 0);
    Call1_02001b5c(Func_02006bd4, 0x2642);
    Value2(Func_02006bee, *(volatile s32 *)Data_0200e79c, 0);
    Call3(Func_02006c0a, 55, 0x3000, 0);
    Func_02006b2e();
}

void FieldScene_RunScene371_02002858(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 record;

    Func_02006b38();
    Func_02006cbc();
    Call1_02001b5c(Func_02006c0a_a, 0x2643);
    Value2(Func_02006c24, *(volatile s32 *)Data_0200e79c, 0);
    Func_02006cd8();
    Call3(Func_02006b8e, 0, 0x10000, 0x8000);
    Call3(Func_02006bd0, 0, 0x1778, 0xd48);
    Func_02006b74();
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    Func_02006b88();
    Func_02006d0c();
    Func_02006b1c(0x264c, 1);
    if (Func_02006b4c(0x234) != 0) {
        Data_03001ebc->story_result = 1;
    }
    Func_02006d3a();
    Func_02006bbe();
}

void FieldScene_RunActorPresentationSequence(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 record;
    s32 base2_2000240;
    s32 base3_2000240;

    base2_2000240 = (s32)Data_02000240;
    base3_2000240 = (s32)Data_02000240;
    Func_02006f44(*(u8 *)((base2_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
    Func_02006fa8();
    Call2(Func_02007164, 0x10000, 0x12c);
    Call4_02001b5c(Func_020070de, -1, -1, -1, 0);
    Func_02007056(5, 19);
    Func_0200705e(8, 5);
    Func_02007060(0, 0, 0);
    Func_02006e8e(1);
    Call2(Func_020071a0, 0x18000, 16);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02007150, 0x10003, 1);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_02007178();
    Func_020071d4();
    Func_020071a0_a();
    Func_0200701e(40);
    Func_020070d6(5, 1);
    Func_0200702c(20);
    Call1_02001b5c(Func_02007102, 0x2913);
    Func_02007124(5, 0, 20);
    Func_020070f4(8, 2);
    Func_0200704a(20);
    Func_0200713c(8, 0, 20);
    Call3(Func_0200715e, 5, 0x107, 20);
    Func_02007150_a(5, 0, 20);
    Call3(Func_02007172, 8, 0x105, 80);
    Func_02007164_a(8, 0, 10);
    Func_02007134(5, 2);
    Func_02007176(5, 0, 20);
    Call3(Func_02007198, 8, 0x105, 100);
    Call3(Func_020071a2, 5, 0x105, 40);
    Func_02007194(5, 0, 20);
    Func_0200719e(8, 0, 10);
    Call3(Func_020071c2, 5, 0x102, 20);
    Func_020071b4(5, 0, 10);
    Call2(Func_020071de, 8, 0x102);
    Func_020070dc(80);
    Func_020071ce(8, 0, 20);
    Call3(Func_020071f0, 5, 0x105, 80);
    Func_020071e2(8, 0, 120);
    Func_020071b2(5, 1);
    Func_020071f4(5, 0, 40);
    Func_020071fe(8, 0, 20);
    Call3(Func_02007220, 5, 0x105, 40);
    Func_02007212(5, 0, 120);
    Call3(Func_0200716c, 9, 0x6666, 0x3333);
    Call3(Func_020071c6, 9, 0x1ddc0000, 0xd840000);
    Call3(Func_020071b8, 9, 0x1d94, 0xd8c);
    Call3(Func_020071c4, 9, 0x1d88, 0xda0);
    Func_0200715a(20);
    Call3(Func_0200724c, 0x6009, 0, 20);
    Call3(Func_0200726e, 8, 0x101, 0);
    Call3(Func_02007278, 5, 0x101, 60);
    Func_02007218(9, 3);
    Call2(Func_02007268, 0x6009, 0);
    Func_020070ec();
    Func_02007192(20);
    Call2(Func_020071da, 9, 0x200cf7c);
    Func_020071a0_b(80);
    Func_02007238(8, 1);
    Func_02007252(8, 4, 40);
    Func_0200724a(5, 1);
    Func_02007264(5, 4, 60);
    Call3(Func_020072c0, 8, 0x3000, 0);
    Call3(Func_020072cc, 5, 0xb000, 40);
    Call3(Func_0200721e, 8, 0x9999, 0x4ccc);
    Call3(Func_02007228, 5, 0x9999, 0x4ccc);
    Call2(Func_02007238_a, 8, 0x200d01c);
    Func_020071fe_a(20);
    Call2(Func_0200731e, 0xb333, 0x1666);
    Call4_02001b5c(Func_02007334, 0x1e380000, -1, 0xdc80000, 1);
    Func_0200725c(5, 0x200d0a8);
    do {
        Func_020072b4(10, 6);
        Func_020072bc(6, 8);
        Func_020070e2(1);
    } while (*(s16 *)(Func_02007260(5) + 100) == 0);
    Func_02007248(20);
    Call3(Func_02007344, 9, 0x8000, 20);
    Call2(Func_02007366, 8, 0x102);
    Call2(Func_02007370, 5, 0x102);
    Func_0200726e_a(40);
    Func_020073fa();
    Call11(Func_02007390, 5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Func_0200729e(20);
    Func_02007202();
    Call2(Func_020073c6, 0x10000, 0x2000);
    Call4_02001b5c(Func_020073dc, 0x1e580000, -1, 0xdc80000, 1);
    Call3(Func_020073b8, 9, 0x3000, 0);
    Call3(Func_0200730a_a, 8, 0x19999, 0xcccc);
    Call3(Func_02007314, 5, 0x19999, 0xcccc);
    Call3(Func_0200734e, 8, 0x1e7c, 0xdb8);
    Call3(Func_02007360, 5, 0x1e6c, 0xdd8);
    Func_02007388(8, 1);
    Func_02007484();
    Func_02007302(80);
    Func_020073ba(8, 1);
    Func_020073fc(8, 0, 20);
    Func_020073cc(5, 2);
    Call3(Func_0200740e, 0x1005, 0, 40);
    Call3(Func_02007422, 8, 0x8000, 20);
    Func_020073e2(8, 2);
    Func_0200742c(8, 0, 60);
    Func_020072a8();
    Func_0200751e(17);
    Func_020074a6(0, 0);
    Func_020074bc(120);
    Func_02007212_a(120);
    Func_0200749a((s32)Data_00000000, 10);
}

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    Func_02007410();
    Func_020075de(141);
    Func_0200756e(0, 0);
    Func_0200756e_a(0, 0);
    Func_02007584(1);
    Func_020072da(2);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 1;
    Func_020075a4();
    Func_020075b8();
    Func_02007508(0, 15);
    Func_020073c4(Func_02007476(0), 0);
    Func_0200730a(1);
    Func_0200757e(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)0x0200b1fd;
        Func_02007328(transition_callback, transition_delay);
    }
    Func_020075d0(0, 0);
    Func_020075d0_a(0x10004, 1);
    Func_020075da(0x10000, 2);
    Func_020075f0(40);
    Func_02007496(240);
    Func_020075ee(0, 0);
    Func_02007604(80);
    Func_0200735a(90);
    Func_020075e8(109);
    Func_02007478(282);
    Func_020074cc();
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void FieldScene_RunActorEightApproach(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 actor;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    Func_02007490(1);
    Object_SetModeById_1(ACTOR, 2);
    ObjectMotion_SetHorizontalPositionWithTerrain_1(ACTOR, 0x13080000, 0x3280000);
    actor = Scene_GetRecord_1(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Func_020074b6(1);
    RatioHistory_RecordAndScheduleCallback_1(0x13333, 1);
    ObjectMotion_SetHorizontalPositionWithTerrain_2(0, 0, 0);
    ObjectMotion_SetPositionAndReset_1(ACTOR, 1);
    Func_020074d6(1);
    SCENE_PHASE = 0x100;
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_1(ACTOR, 0x6666, 0x3333);
    ObjectMotion_SetPositionAndCommit_1(ACTOR, 0x12d8, 0x2c8);
    ObjectMotion_SetPositionAndCommit_2(ACTOR, 0x12a8, 0x268);
    ObjectMotion_SetSpeedParameters_2(ACTOR, 0x4ccc, 0x2666);
    ObjectMotion_SetPositionAndCommit_3(ACTOR, 0x12a8, 0x1d8);
    ObjectMotion_SetSpeedParameters_3(ACTOR, 0x3333, 0x1999);
    ObjectMotion_SetPositionAndCommit_4(ACTOR, 0x1298, 0x1c8);
    ObjectMotion_SetSpeedParameters_4(ACTOR, 0x1999, 0xccc);
    ObjectMotion_SetPositionAndCommit_5(ACTOR, 0x1298, 0x1b8);
    Object_SetModeById_2(ACTOR, 1);
    BattleRuntime_WaitIfModeZero_2(40);
    Func_0200781e();
    ObjectMotion_SetSpeedParameters_5();
    Func_020077f0(110);
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    extern s16 Data_02000240[];
    extern struct StoryDialogueWork *Data_03001ebc;

    s32 positive_total = Func_0200686a(0);
    s32 negative_total;

    positive_total += Func_02006872(2);
    negative_total = Func_0200687a(1);
    negative_total += Func_02006882(3);
    return positive_total - negative_total;
}

/* Complete reference-actor-54 selected-actor setup wrapper. */
s32 StoryReward_LookupBySelection(u32 selection)
{
    extern volatile s32 Data_03001e40;

    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (Func_02007708(flag_base + offset) != 0) return Data_0200e718[offset];
    }
    return 0;
}

void FieldScene_RunScene371_0200357c(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_0200787e, 8);
    Func_0200785e(60);
    Func_0200786a();
    Call2(Func_02007a22, 0x9999, 1);
    *(s32 *)(rec7 + 24) = 0x13333;
    *(s32 *)(rec7 + 28) = 0x13333;
    Func_02007988(8, 1);
    Func_0200772e(1);
    Func_02007946(0, 15);
    record = Func_020078b4(0);
    Func_02007802(record, 0);
    record = Func_020078c0(8);
    Func_0200780e(record, 0);
    Call3(Func_020078e0, 8, 0x6666, 0x3333);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Value2(Func_020078f6, 8, 0x200d218);
    Value2(Func_02007778, 0x200b4a1, 0xc80);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02007a26, 0x10003, 1);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 32;
    Func_02007a4c();
    Func_020078ea(120);
    Call2(Func_02007aac, 0x16666, 0x12c);
    Call1_02001b5c(Func_020078fc, 0x10e);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Func_02007a7e();
    Func_02007a8a();
    Func_02007a50(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{
    extern volatile s32 Data_03001e40;

    extern struct StorySelection Data_02000240;
    extern struct StoryProgressWork *Data_03001ebc;

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Func_02007982(Data_02000240.actor_id);
    scene = Data_03001ebc;
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_0200793e(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Func_02007944(0x106) == 0) {
            progress -= 1;
        }
    }
    Func_02007974(0x2f8, progress);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = Data_02000240;
    p6 = *(volatile s32 *)(base + 500);
    rec7 = Value1(Func_020079f2, (s32)p6);
    rec2 = Value1(Func_0200798c, 0x2f0);
    if (rec2 == 0) {
        Func_020079e6();
        Call2(Func_02007aee, (s32)p6, 0x101);
        Func_02007a7e_a((s32)p6, 9);
        record = Value1(Func_02007a1c, a0);
        if (record != 0) {
            Func_02007a5e((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007a8c((s32)p6);
        Func_02007be2(244);
        Value2(Func_020078d6, 0x200b679, 0xc80);
        rec7[85] = rec2;
        {
            s32 shifted = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;

            Func_02007986((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), shifted, *(volatile s32 *)((s32)rec7 + 16));
        }
        Func_02007abc((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Call1_02001b5c(Func_02007a16, 0x2f0);
        Call2(Func_02007a38, 0x2f8, 180);
        Func_02007a74();
        *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17c)) = rec2;
    }
}

void StoryScene_SetReferenceActor(void)
{
    extern volatile s32 Data_03001e40;

    Func_02006e9e(54);
}

void StoryScene_ActivateSharedState(void)
{
    extern volatile s32 Data_03001e40;

    Data_03001f30->active = 1;
}

/* Publish the actor-98 scene state and restore its selected actor. */
void StoryScene_CompleteActor98(void)
{
    extern volatile s32 Data_03001e40;

    extern u8 Data_02000240[];
    extern struct StoryCompletionWork *Data_03001ebc;

    u8 *selected_actor;

    if (Data_03001ebc->scene_value == 99) {
        Data_03001ebc->scene_value = 0;
    }
    Func_02007a92(0x2f0);
    Func_02007a90(0x2f1);
    Func_02007ab2(0x2f8, 0);
    Func_02007c22(98, 5);
    Data_02000240[0x22b] = 3;
    Func_02007c34(98, 7);
    selected_actor = Func_02007b28(*(s32 *)(Data_02000240 + 500));
    selected_actor[85] = 2;
}

/* Restore the blend registers using the active display bank's mask. */
void SceneEffect_RestoreBlendRegisters(void)
{
    extern u16 Data_0200e7a0;
    extern u32 Data_03001e40;

    Func_02007af0(0x04000050, 0x3f41);
    if ((Data_03001e40 & 2) != 0) {
        Func_02007b0a(0x04000052, Data_0200e7a0 | 0x0c);
    } else {
        Func_02007b34(0x04000052, Data_0200e7a0 | 0x10);
    }
}

void FieldScene_RunLateSequence(void)
{
    extern u8 Data_02000240[];
    extern u8 Data_0200e7a0[];
    extern u8 Data_03001ebc[];
    void Func_02008180();

    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Func_02007c66(Data_02000240_t[125][0]);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if (Value2(Func_02007af8, *(volatile s32 *)0x03001e40, 3) == 0) {
        mode = (u32)(Func_02007b20() << 2) >> 16;
        switch (mode) {
        case 0:
            Call4_02001b5c(Func_02007dc4, (sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Func_02007dc4((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Call4_02001b5c(Func_02007dc4, (sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Call4_02001b5c(Func_02007dda, (sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
    extern s32 Data_03001e40;
    void Func_02008180(u8 *, s32);

    if ((Data_03001e40 & 2) != 0) {
        Func_02008180(obj, 7);
    } else {
        Func_0200818a(obj, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02007f98(obj);
    }
}

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        Func_020081c8(obj, Func_020080e0((s32)(Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02007fd4(obj);
    }
}

void SceneState_RunSlotStepOnOddFrames(s32 arg0)
{
    extern volatile s32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_0200811c((u32)Data_03001e40 >> 1, 6);

        Func_02008204(arg0, slot);
    }
}

void StoryEffect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect)
{
    struct StoryVerticalEffectActor *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Func_020081e4(effect);
        return;
    }

    vertical_amplitude = Func_0200817c(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z + (0x10000 - vertical_amplitude) * 5 + 0x80000;
}

void StoryEffect_AnimateVerticalNegative(struct StoryVerticalEffectActor_02004004 *effect)
{
    struct StoryVerticalEffectActor_02004004 *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Func_02008234(effect);
        return;
    }

    vertical_amplitude = Func_020081cc(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = -vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}
