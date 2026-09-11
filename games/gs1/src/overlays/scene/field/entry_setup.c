#include "types.h"

#define NULL ((void *)0)
#define RunOverlayObjectCommand0 Func_02004e8c
#define RunOverlayObjectCommand1 Func_02004f8c
#define Effect_SpawnConfigured Func_02000118
#define Actor_CalculateFixedPointDistance Func_020002f0
#define SceneData_FindSlotAtPosition Func_0200032c
#define Actor_PickHighestSlotAtSameTileAndRelease Func_020023d4
#define StagedActorDirectionSteps Data_0200d0e8
#define GetStagedActor Func_02005230
#define FindActorAtPosition Func_020006e6
#define FindActorAtForwardPosition Func_02000712
#define FindActorAbovePosition Func_0200073e
#define CheckStagedActorMove Func_0200528c
#define SetStagedActorMode Func_0200523c
#define WaitSceneFrames Func_02005214
#define PlaySoundCue Func_02005472
#define SetStagedActorMoveTarget Func_0200528a
#define SetLeadActorMoveTarget Func_0200529a
#define UpdateStagedActorState Func_020052a8
#define UpdateLeadActorState Func_020052a8_a
#define FinalizeStagedActorUpdate Func_02005494
#define RunStagedActorTransition Func_02000374
#define State_FillGridCellByte2 Func_020004f4
#define Actor_ApplySlotsMatchingKind212 Func_02000558
#define State_StoreLookupZeroToWord24 Func_020008a0
#define State_ClearWorkspaceWord24 Func_020008b8
#define State_SetFlag953 Func_02000f1c
#define State_ApplySixRectsAfter161 Func_02001048
#define State_MarkActorAndApplyRectAtTile Func_02001a14
#define State_RunConditionalStep Func_0200226c
#define State_ApplyStepToSlots15To18 Func_02003050
#define State_PassZeroAndMinusOneRecord Func_02003fe4
#define State_PassRange0To1 Func_02003ffc
#define State_PassRangeNeg1To0 Func_02004014
#define State_CallHandlerWithFlagPair Func_02004030
#define State_Call4520With432And32 Func_020045f0
#define OvObj_UpdateEveryFourFrames Func_020005ac
#define OvObj_ApplyZero Func_02000690
#define OvObj_WaitUntilIdle Func_0200094c
#define OvObj_ResetObjectWhenFlags12Set Func_02001a50
#define Effect_SpawnRandomEveryEightFramesB Func_020005e4
#define Effect_SpawnTwoRandomizedParticles Func_020006a0
#define Effect_RunObjectZeroColorSequence Func_02000754
#define Effect_SpawnNineRadialEffects Func_02000b08
#define Effect_SpawnRandomizedParticleEveryFourFrames Func_02000b98
#define Effect_SpawnEffect284AtCell Func_02000c5c
#define Scene_GetRecord_1(args...) Func_0200568a(args)
#define GameFlag_IsSet_1(a0) Value1(Func_0200565a, a0)
#define Battle_Reset_1(args...) Func_02005684(args)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_0200578a, a0, a1, a2, a3)
#define Motion_SetHPosTerrain_1(args...) Func_02005712(args)
#define ObjectGroup_ConfigureChildValue_1(args...) Func_0200575a(args)
#define Scene_GetRecord_2(args...) Func_020056d8(args)
#define Battle_WaitMode0_1(args...) Func_020057fa(args)
#define Motion_SetSpeed_1(args...) Func_0200580e(args)
#define Audio_PlayCue_1(args...) Func_0200583c(args)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_02005702, a0, a1, a2)
#define Motion_CommitPositionAndActivate_1(args...) Func_0200583c_a(args)
#define ObjectGroup_ConfigureChildValue_2(args...) Func_02005794(args)
#define Scene_GetRecord_3(args...) Func_02005712_a(args)
#define Motion_CommitPositionAndActivate_2(args...) Func_0200585a(args)
#define BattleFx_PlayQueuedSound_1(args...) Func_02005874(args)
#define Battle_SchedShoulder_1(args...) Func_02005720(args)
#define BattleRuntime_Reset_1_02002f30(args...) Func_02007dc8(args)
#define ObjectMotion_SetSpeedParameters_1_02002f30(a0, a1, a2) Call3(Func_02007df6, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_02007e2c, a0, a1, a2)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02007eb8, a0, a1, a2)
#define BattleFx_RunRisingObjectSequence_1(args...) Func_02007e92(args)
#define BattleRuntime_WaitIfModeZero_1_02002f30(args...) Func_02007e08(args)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002f30(args...) Func_02007e22(args)
#define Scene_RunOpeningAuxiliarySequence Func_020007d8
#define Scene_RunSupplementalSequenceOne Func_020009c8
#define Scene_RunActorEightTenStepLoop Func_02000f38
#define Scene_RunScene3c8SequenceC Func_02001150
#define Scene_RunScene3c8SequenceD Func_02001218
#define Scene_RunFlag986ActorOneScene Func_020014f4
#define Scene_RunScene3c8SequenceB Func_02002f30
#define Scene_RunStep230With44 Func_02004600
#define FieldScene_RunScene3c8_02004a2c Func_02004a2c
#define Actor_SetFlagBitByRelativeDepth Func_020008c8
#define Actor_SwapPositionsByDepth Func_02000910
#define Scene_SetFlag987AtActorTwelveTile Func_02000ff0
#define Actor_ApplyKind45AtActorsElevenAndTwelve Func_0200199c
#define Actor_ApplyPositionsOfActors11And12 Func_020019c0
#define Actor_UpdateSlots11And12ByTile Func_02001a9c
#define Actor_SetHeightAboveLinkedRecord Func_020022a4
#define Actor_TryMoveActorZeroTwoTilesAhead Func_02000cc8
#define SceneData_SelectTableBySceneB5ToBa Func_02000e04
#define SceneData_ReturnZero Func_02000e7c
#define SceneData_GetTablee740 Func_02000e80
#define SceneData_SelectTableBySceneAndApply Func_02000e88
#define SceneData_SelectTableBySceneId Func_02002f8c
#define Dialogue_RunActorElevenDialogue Func_02000fbc
#define Dialogue_RunLine2682 Func_02001024
#define State_ApplySixRectsAfterFlag161 Func_020010cc
#define State_RunActor13AtColumn42Setup Func_02001644
#define State_ApplyRectAt19_44AndRunThree Func_0200196c
#define Scene_RunFiveCallSequence Func_02001628
#define Scene_PlaceAndPinSlots8To10 Func_020018f8
#define Scene_RunGuardedThreeStepSetup Func_020019f0
#define Scene_RunThreeCallSequence Func_020026e4
#define Scene_DrawTilesWhenCheckClear Func_02002ee4
#define Scene_RunGuardedRectStep Func_02001f28
#define Actor_ClearActorModeAndSetState5 Func_0200300c
#define ConfigureOverlayObject Func_02000030
#define OvObj_SpawnWithMode14 Func_02000058
#define OvObj_PrepareObjectWithCommand15 Func_02000098
#define OvObj_IntegrateVelocities Func_020000e0
#define OvObj_ApplyLowNibbleOfField100 Func_02000594

#include "overlay_object.h"
#include "staged_actor.h"

/*
 * Effect spawning for overlay resource_39f.  Every cutscene beat in the
 * overlay creates its effects through this routine.
 */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 dur;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[15];
    u8 unk23;
    u8 filler24[49];
    u8 unk55;
} Struct_1a14;

struct Args_02003fe4 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02003ffc {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004014 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004030 {
    s32 first;
    s32 unused;
    s32 last;
};

struct BusyObject {
    u8 filler00[0x28];
    s32 busy;
};

typedef struct {
    u8 filler0[9];
    u8 unk9;
} Struct_Sub;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[48];
    s32 unk44;
    u8 filler48[8];
    Struct_Sub *unk50;
} Struct_1a50;

struct Object_020005e4 { u8 unknown_00[8]; s32 x; s32 y; s32 z; };

struct EffectParams_020005e4 { s32 unk00; s32 mode; s32 color1; s32 color2; u8 unknown_10[0x18]; };

struct Object_020006a0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams_020006a0 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

struct EffectObject {
    u8 filler00[0x30];
    s32 color;
    u8 filler34[0x38];
    s32 callback;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct SceneObject_02000b98 {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x24];
    s32 near;
    u8 filler3c[4];
    s32 far;
};

struct EffectParams_02000b98 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    s32 unk10;
    s32 unk14;
    u16 angle;
    u8 filler1a[0xa];
    s32 callback;
};

struct EffectObject_02000c5c {
    u8 filler00[0xc];
    s32 timer;
    u8 filler10[0x13];
    u8 mode;
    u8 filler24[0xc];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
    u8 filler56[3];
    u8 phase;
    u8 filler5a[9];
    u8 flag;
    u8 filler64[8];
    s32 callback;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_A;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Struct_B;

struct Actor_020008c8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flags23; };

struct Position {
    u32 filler00[2];
    s32 x;
    s32 y;
    s32 z;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_0ff0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_199c;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct_19c0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
} Struct_1a9c;

typedef struct {
    s16 filler0[50];
    s32 unk64;
} Struct_22a4;

typedef struct {
    s32 filler0[3];
    s32 unkC;
} Struct_22a4b;

/*
 * Per-frame motion for an effect record in resource_3c8.
 */
struct Sprite_0200096c {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect_0200096c {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite_0200096c *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct SceneObject_02000cc8 {
    u8 filler00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x14];
    s32 accel;
    u8 filler2c[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
};

union VecView {
    struct Vec v;
    s16 h[6];
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[61];
    u8 unk55;
} Struct_1644;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_18f8;

extern struct Descriptor *Data_0200d1d4[];
extern u32 Data_0200d0e8[];
extern u8 *Data_03001e70;
extern u8 Data_02010000[];
extern u8 Value_00000212;
extern u32 Data_03001e40;
extern u8 Value_00000986;
extern u8 Value_000000b5;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Data_0200dd68[];
extern u8 Data_0200e020[];
extern u8 Data_0200e230[];
extern u8 Data_0200e350[];
extern u8 Data_0200e548[];
extern u8 Data_0200ddc8[];
extern u8 Data_0200e740[];
extern u8 Value_000000b6;
extern u8 Data_0200e8ec[];
extern u8 Data_0200e904[];
extern u8 Data_0200e9c4[];
extern u8 Data_0200eb74[];
extern u8 Data_0200ec04[];
extern u8 Data_0200ec64[];
extern u8 Data_0200ecf4[];
extern u8 Data_0200ee44[];
extern u8 Data_0200f120[];
extern u8 Data_0200f300[];
extern u8 Data_0200f3b4[];
extern u8 Data_0200f4f8[];
extern u8 Data_0200ef1c[];

void Func_02004e8c(void *, s32);
void Func_02004f8c(void *, s32);
void Func_020000b2(void *, s32);
void *Func_02004e4e(s32, s32, s32, s32);
void Func_02004ed6(void *, s32);
void Func_020000f6(void *, s32);
void *Func_02004e8e(s32, s32, s32, s32);
struct Effect *Func_02004fe6();
struct Effect *Func_02004f44();
void Func_02004f4e();
void Func_02004f68();
void Func_02005132();
s32 Func_02004ff4();
s32 Func_0200500c();
s32 Func_0200501a();
void Func_02005068();
void Func_02005078();
typedef s32(*IwramSqrt020002f0)(s32);
u8 *Func_020072a2();
u8 *Func_020072aa();
void Func_020072f6();
void Func_02007252();
void Func_02007340();
void Func_02007456();
void Func_020072f2();
struct StagedActor *Func_02005230(s32 arg0);
struct StagedActor *Func_020006e6(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000712(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
s32 Func_0200528c(struct StagedActor *arg0, s32 *arg1);
void Func_0200523c(struct StagedActor *arg0, s32 arg1);
void Func_02005214(s32 arg0);
void Func_02005472(s32 arg0);
void Func_0200528a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200529a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020052a8();
void Func_020052a8_a();
void Func_02005494(void);
void Func_020054b8(s32 *, s32);
s32 Func_02005752(s32);
void Func_02005db4(void);
void Func_02005d94(s32, s32);
void Func_02005dc8(void);
void Func_02005ed4(s32);
void Func_02005e98(s32, s32, s32, s32, s32, s32);
void Func_02005e94(s32, s32, s32, s32, s32, s32);
void Func_02005ea4(s32, s32, s32, s32, s32, s32);
void Func_02005ed0(s32, s32, s32, s32, s32, s32);
void Func_02005ec8(s32, s32, s32, s32, s32, s32);
void Func_02005ed8(s32, s32, s32, s32, s32, s32);
void Func_0200687a(s32, s32, s32, s32, s32, s32);
void Func_02007106(void);
s32 Func_02002f3e(void);
void Func_020070c2(s32, s32, s32, s32, s32, s32);
void Func_02002606(void);
void Func_02007132(void);
void Func_020041fa(void);
void Func_02007f06(s32);
void Func_0200803c();
void Func_02008054();
void Func_0200806e();
void Func_02008088();
void Func_02008b1a();
void Func_020054de(void *, s32);
void Func_020054fa(void *, s32);
void Func_02005502(void *, s32);
void Func_020055d4(s32);
void Func_020054da(void *, s32);
void Func_020056fe(s32);
s32 Func_02001ad4(s32, s32, s32, s32);
void Func_020023d0(Struct_1a50 *);
void Func_0200687a_a(s32);
void Func_02003454(void);
s32 Func_020053b2();
s32 Func_020053d0();
s32 Func_020053e4();
s32 Func_020053f8();
s32 Func_020053f6();
void Func_0200078e();
s32 Func_02005470();
s32 Func_020054cc();
s32 Func_0200081a();
s32 Func_02000852();
struct EffectObject *Func_0200560a(s32);
void Func_020055f8(void);
void Func_02005766(s32);
void Func_02005676(s32, s32);
void Func_0200566a(s32, s32, s32);
void Func_02005678(s32);
void Func_020056c8(s32, s32);
s32 Func_02005646(s32);
void Func_020055e4(s32, s32);
void Func_02005636(s32);
void Func_0200577a(void);
void Func_02005786(void);
void Func_02005764(s16);
void Func_02005660(void);
struct SceneObject *Func_020059c2(void);
s32 Func_020058fc(s32);
s32 Func_02005900(s32);
s32 Func_020058ee(s32, s32);
void Func_02000c8a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
s32 Func_02005966(void);
void Func_02005bd0(s32);
s32 Func_020059a6(void);
s32 Func_020059a0(s32, s32);
s32 Func_020059be(void);
s32 Func_020059b8(s32, s32);
void Func_02000d52(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);
struct EffectObject *Func_02005a56(s32, s32, s32, s32);
void Func_02005ace(struct EffectObject *, s32);
void Func_02005a66(struct EffectObject *, s32);
void Func_02005a96(struct EffectObject *, s32);
s32 Func_0200565a();
void Func_02005676_a();
void Func_02005684();
u8 *Func_0200568a();
void Func_020056b0();
u8 *Func_020056d8();
void Func_02005702();
u8 *Func_02005712();
void Func_02005720();
void Func_0200575a();
void Func_0200578a();
void Func_02005794();
void Func_020057fa();
void Func_0200580e();
void Func_0200583c();
void Func_0200583c_a();
void Func_0200585a();
void Func_02005874();
u8 *Func_02005712_a();
void Func_02000b9a();
void Func_0200579c();
void Func_020057e6();
s32 Func_02005812();
s32 Func_02005816();
void Func_02005870();
s32 Func_02005882();
void Func_020058a8();
void Func_020058b8();
void Func_02005906();
void Func_02005914();
void Func_02005920();
void Func_02005972();
void Func_0200597e();
void Func_0200599e();
void Func_020059d6();
void Func_020059ea();
void Func_02005a0e();
void Func_02005a1e();
void Func_02005a42();
void Func_02005a8e();
void Func_02005ac6();
void Func_02005dd2(void);
void Func_02005e5a(s32, s32);
void Func_02005e90(s32);
void Func_02005eae(s32, s32, s32);
void Func_02005e96(s32, s32);
void Func_02005d0c(s32);
void Func_02005ea4_a(s32, s32);
void Func_02005d1a(s32);
void Func_02005e04(s32);
void Func_02005e86(s32, s32, s32);
void Func_02005dda(s32, s32, s32, s32, s32, s32);
void Func_02005e46(void);
void Func_02005ff0();
s32 Func_02006002();
void Func_02006020();
void Func_0200602e();
void Func_02006036();
s32 Func_0200603c();
void Func_0200604e();
s32 Func_02006050();
void Func_02006064();
u8 *Func_02006074();
void Func_02006098();
void Func_020060a4();
void Func_020060a6();
void Func_020060fc();
void Func_0200610c();
void Func_0200612c();
void Func_02006154();
void Func_020060ae();
void Func_020060c6();
void Func_020060ec();
void Func_020060f2();
void Func_020060f8();
s32 Func_020060fa();
s32 Func_02006104();
void Func_02006106();
void Func_0200610c_a();
void Func_02006116();
void Func_02006120();
s32 Func_02006126();
s32 Func_02006126_a();
void Func_02006128();
void Func_02006136();
void Func_02006136_a();
void Func_02006138();
void Func_02006146();
void Func_02006146_a();
void Func_02006156();
void Func_02006166();
void Func_02006176();
void Func_02006186();
void Func_02006196();
void Func_020061a6();
void Func_020061c8();
void Func_020061dc();
void Func_020061ec();
void Func_020061fc();
void Func_02006200();
void Func_02006200_a();
void Func_0200620c();
void Func_0200620e();
void Func_02006216();
void Func_0200621c();
void Func_0200621c_a();
void Func_02006220();
void Func_0200622c();
void Func_02006236();
void Func_0200623c();
void Func_0200624c();
void Func_0200625c();
void Func_02006262();
void Func_0200626c();
void Func_0200627c();
void Func_02006302();
void Func_02006328();
void Func_02006350();
void Func_020063fe();
void Func_02006416();
void Func_02006416_a();
void Func_02006422();
void Func_0200642c();
void Func_02006430();
void Func_0200643c();
void Func_0200638c(void);
Struct_A *Func_020063aa(s32);
s32 Func_02006382();
void Func_02006394();
Struct_A *Func_020063ca(s32);
void Func_02006420(s32, s32, s32);
void Func_020063ea();
void Func_020063ea_a();
void Func_0200641e(s32, s32, s32);
void Func_0200642a(s32, s32, s32);
void Func_02006484(s32, s32, s32);
void Func_02006472(s32, s32);
void Func_020063f8(s32);
void Func_020064b6(s32);
void Func_020064d0(s32, s32, s32);
void Func_020064e2(s32, s32, s32);
void Func_020064fe(s32, s32, s32);
void Func_020064c0(s32, s32, s32);
void Func_0200642e(s32);
void Func_020064be(s32, s32);
void Func_020064be_a(s32, s32);
void Func_0200643c_a(s32);
void Func_0200650e(s32, s32, s32);
void Func_020064d6(s32, s32);
void Func_0200645c(s32);
void Func_020064b8(s32, s32, s32);
void Func_020064e0(s32, s32);
Struct_B *Func_02006496(s32);
void Func_020064c8(s32, s32, s32);
void Func_020064ee(s32);
void Func_02006500(s32, s32, s32);
void Func_020064ac(void);
s32 Func_02002fc4();
void Func_02007dc8();
void Func_02007df6();
void Func_02007e08();
void Func_02007e22();
void Func_02007e2c();
void Func_02007e92();
void Func_02007eb8();
void Func_02007f26();
void Func_02008b2a();
void Func_02009808();
void Func_02009852();
void Func_0200987a();
void Func_02009894();
void Func_020098b0();
void Func_020098c8();
void Func_020098dc();
s32 Func_020098e6();
s32 Func_020098ee();
void Func_02009920();
void Func_0200992c();
void Func_0200994e();
void Func_020099ac();
void Func_020099de();
void Func_020099ec();
void Func_020099f6();
void Func_02009a0a();
void Func_02009a1c();
void Func_02009a48();
void Func_02009a56();
void Func_02009a5c();
void Func_02009a8a();
void Func_02009ac6();
void Func_02009ad6();
void Func_02009ade();
void Func_02009b5c();
void Func_02009b7c();
void Func_02009b84();
struct Actor_020008c8 *Func_0200577c();
struct Position *Func_020057c2(s32);
struct Position *Func_020057ca(s32);
void Func_020056e8(s32);
Struct_0ff0 *Func_02005ea2(s32);
void Func_02005e90_a(void);
void Func_02005e8e(s32);
void Func_02005eb2(void);
Struct_199c *Func_02006852(s32);
void Func_02006818(s32, s32, s32, s32);
Struct_19c0 *Func_02006876(s32);
void Func_02006842(s32, s32, s32, s32);
void Func_02006934(void);
Struct_1a9c *Func_02006952(s32);
void Func_02002400(void);
void Func_020034ce(Struct_1a9c *);
void Func_02003512(Struct_1a9c *);
Struct_1a9c *Func_02006974(s32);
void Func_02002422(void);
void Func_020034f0(Struct_1a9c *);
void Func_02003534(Struct_1a9c *);
void Func_02006984(void);
Struct_22a4b *Func_0200715e(s32);
s32 Func_0200572a();
struct SceneObject *Func_02005b84(s32);
void Func_02005ae6(s32, s32, struct Vec *);
s32 Func_02005b5e(struct SceneObject *, struct Vec *);
s32 Func_0200105a(struct Vec *, struct SceneObject *);
void Func_02005b2c(s32, s32, struct Vec *);
s32 Func_02001094(struct Vec *, struct SceneObject *);
s32 Func_02005bb0(struct SceneObject *, struct Vec *);
void Func_02005c10(void);
void Func_02005b58(struct SceneObject *, s32);
void Func_02005b2e(s32);
void Func_02005d8c(s32);
void Func_02005b6c(struct SceneObject *, s32);
void Func_02005c02(struct SceneObject *, s32);
void Func_02005ca0(s32, s32, s32);
void Func_02005ba8(struct SceneObject *, s32);
void Func_02005c20(struct SceneObject *, s32);
void Func_02005c82(void);
void Func_02005d78(u8 *);
void Func_02005e54(void);
void Func_02005f0a(s32);
void Func_02005f24(s32, s32, s32);
void Func_02005eec(s32, s32);
void Func_02005f2c(s32, s32);
void Func_02005e80(void);
void Func_02005ebc(void);
void Func_02005f2c_a(s32, s32);
void Func_02005ea4_b(s32, s32);
void Func_02005ed8_a(void);
void Func_02005f50(s32);
void Func_02005f1c(s32, s32, s32, s32, s32, s32);
void Func_02005f18(s32, s32, s32, s32, s32, s32);
void Func_02005f28(s32, s32, s32, s32, s32, s32);
void Func_02005f54(s32, s32, s32, s32, s32, s32);
void Func_02005f4c(s32, s32, s32, s32, s32, s32);
void Func_02005f5c(s32, s32, s32, s32, s32, s32);
Struct_1644 *Func_020064f8();
Struct_1644 *Func_020064f8_a();
void Func_020064e6(void);
void Func_020064ec(s32);
void Func_02006662(s32);
void Func_020064ac_a(s32, s32, s32, s32, s32, s32);
void Func_02006530(void);
void Func_02006806(void);
void Func_020067ba(s32, s32, s32, s32, s32, s32);
void Func_02001cfe(void);
void Func_02003286(void);
void Func_0200682e(void);
void Func_020064c0_a(void);
void Func_020019a4(void);
void Func_020064c2(s32);
void Func_020064d6_a(void);
void Func_02002b32(void);
void Func_020067ac(s32);
void Func_02006798(void);
void Func_0200674c(s32, s32, s32, s32, s32, s32);
void Func_02006760(s32, s32, s32, s32, s32, s32);
Struct_18f8 *Func_020067e2(s32);
void Func_02006784(s32, s32, s32, s32, s32, s32);
void Func_020067fc(void);
void Func_02006888(void);
s32 Func_020026c0(void);
void Func_0200339c(void);
void Func_02001d78(void);
void Func_020033c8(void);
void Func_020068a8(void);
void Func_0200757c(void);
void Func_02002a60(void);
void Func_0200758c(void);
void Func_02007d7e(void);
s32 Func_02003bb6(void);
void Func_02007d3a(s32, s32, s32, s32, s32, s32);
void Func_02007d4e(s32, s32, s32, s32, s32, s32);
void Func_02003292(void);
void Func_02007dbe(void);
void Func_02005a3a(void);
void Func_02006dc2(void);
s32 Func_02002bfa(void);
void Func_02006d7e(s32, s32, s32, s32, s32, s32);
void Func_020022c2(void);
void Func_02006dee(void);
void Func_02003c9e(void);
u8 *Func_02007ebe(s32);
void Func_02007e72(u8 *, s32);
void Func_02007e0a(u8 *, s32);
void Func_02007fa2(s32, s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_02001218(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02002f30(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value1_02004a2c(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}




void ConfigureOverlayObject(struct OverlayObject *object, s32 parameter)
{
    object->unknown_55 = 0;
    object->unknown_59 = 8;
    RunOverlayObjectCommand0(object, 0);
    RunOverlayObjectCommand1(object, parameter);
}

void *OvObj_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *obj;
    u8 *p;
    s32 mask;
    u8 flag;

    obj = Func_02004e4e(arg3, arg0, arg1, arg2);
    if (obj != 0) {
        p = *(u8 **)((u8 *)obj + 0x50);
        mask = 13;
        flag = p[9];
        mask = -mask;
        mask &= flag;
        p[9] = mask;
        Func_020000b2(obj, 0xE);
        Func_02004ed6(obj, 1);
        return obj;
    }
    return 0;
}

void *OvObj_PrepareObjectWithCommand15(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *object;
    u8 *rec;
    s32 flags;
    s32 result;

    object = Func_02004e8e(arg3, arg0, arg1, arg2);
    if (object != 0) {
        rec = *(u8 **)((u8 *)object + 0x50);
        flags = rec[9];
        flags = (flags & -13) | 4;
        rec[9] = flags;
        Func_020000f6(object, 0xF);
        flags = *((u8 *)object + 0x23);
        result = 2;
        result |= flags;
        *((u8 *)object + 0x23) = result;
        return object;
    }
    return 0;
}

void OvObj_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

/* Returns the party record; only its presentation block at +80 is read. */

/*
 * Creates an effect at (x, y, z) with the constant velocity (vx, vy, vz) that
 * the per-frame integrator applies, then sets whichever optional fields
 * `flags` selects from `options`.  The 472-byte owner includes its three pool
 * words 0x0200b058, 0x020080e1 and 0xffff0000.
 */
void Effect_SpawnConfigured(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    u32 off;
    struct Effect *party;
    u32 bits;
    s32 mask;
    u32 tmp;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *blk;
    u32 opt;
    u16 *tag;
    s32 dur;
    s32 delta0;
    s32 acc;
    party = Func_02004fe6(0);

    /* 128 << 13.  With this bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02004f44(options->kind, x, y, z);
    } else {
        effect = Func_02004f44(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    blk = block;

    Func_02004f4e(effect, (flags + 1) & 15);
    off = (flags & 15) << 2;
    Func_02004f68(effect, Data_0200d1d4[off >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;

    /* 0x020080e1 is the per-frame integrator's loader-relocated call word,
     * not a runtime address. */
    effect->callback = 0x020080e1;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    /* Bits 2 and 3 of the effect's mode byte are copied from the party's. */
    bits = party->sprite->flags9 & 12;
    tmp = *(volatile u8 *)&block->flags9;
    mask = ~12;
    block->flags9 = (u8)((tmp & mask) | bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    /* The rest is optional detail, skipped unless a high flag bit is set and
     * an options record was supplied. */
    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {                   /* 128 << 9 */
        Func_02005132(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {                   /* 128 << 10 */
        effect->flags23 &= 0xfe;
        opt = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)blk + 9) & mask)
                             | (opt << 2));
    }

    if ((flags & 0x80000) != 0) {                   /* 128 << 12 */
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {                   /* 128 << 11 */
        const struct Descriptor *desc =
            Data_0200d1d4[off >> 2];
        s32 delta;

        /* This 0x80000 test reads the register the previous block left live
         * and must not be respelled as a fresh load of `flags`.  With a
         * destination supplied the step is measured from it, otherwise the
         * target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            delta0 = *(volatile const s32 *)&options->target30;
            acc = *(volatile const s32 *)&effect->accum18;
            delta0 -= acc;
            effect->rate30 = Func_02004ff4(delta0,
                                           desc->dur);
            delta = options->target34;
            dur = desc->dur;
            delta -= effect->accum1c;
        } else {
            delta0 = options->target30;
            delta0 += (s32)0xffff0000;
            effect->rate30 = Func_0200500c(delta0,
                                           desc->dur);
            delta = options->target34;
            dur = desc->dur;
            delta += (s32)0xffff0000;
        }

        /* Only the first call is per-arm.  Both arms join onto one second
         * call site, so each arm computes its own delta and the call is
         * spelled once here. */
        effect->rate34 = Func_0200501a(delta, dur);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Func_02005068(effect, 1);
        Func_02005078(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {                  /* 128 << 15 */
        block->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {                  /* 128 << 16 */
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {                 /* 128 << 17 */
        effect->callback = options->callback;
    }
}

s32 Actor_CalculateFixedPointDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt020002f0) 0x030001D8)(dxsq + dysq + dzsq);
}

/*
 * CALL SYMBOLS ARE PER-SITE: the raw assembly spells each of these eight
 * calls as a direct `bl sub_020072xx` to an address inside this overlay's
 * own 0x0200xxxx range (verified via `arm-none-eabi-objdump -dr -M
 * force-thumb` on the assembled .o, which resolves the *ABS* targets before
 * linking) -- lifted verbatim, not the veneer-math final target names this
 * file used before. The one true indirect call (selector's local-effect
 * dispatch) is routed automatically through this overlay's own
 * `_call_via_rN` bank; Func_02002f68 is correct for it, unchanged.
 */
s32 *SceneData_FindSlotAtPosition(s32 *pos)
{
    extern u8 *Data_03001ebc;
    void Func_02000b08();

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((pos[0] >> 20) == (p[2] >> 20)
            && (pos[1] >> 20) == (p[3] >> 20)
            && (pos[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void RunStagedActorTransition(void)
{
    s32 target_position[3];
    struct StagedActor *leader;
    struct StagedActor *actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 packed_step;
    s32 move_rate;
    s32 transition_busy;

    leader = GetStagedActor(0);
    direction_index = leader->direction_and_kind >> 12;
    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = leader->x.value + (packed_step & 0xffff0000);
    target_position[1] = leader->y;
    packed_step <<= 16;
    target_position[2] = leader->z.value + packed_step;
    actor = FindActorAtPosition(target_position, leader);
    if (actor == 0) return;

    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    blocking_actor = FindActorAtForwardPosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    target_position[0] = actor->x.value;
    target_position[1] = actor->y + 0x100000;
    target_position[2] = actor->z.value;
    blocking_actor = FindActorAbovePosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    actor->transition_mode = 2;
    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    if (CheckStagedActorMove(actor, target_position) > 0) return;

    transition_busy = actor->transition_busy;
    if (transition_busy != 0) return;

    SetStagedActorMode(leader, 8);
    move_rate = 0x3333;
    WaitSceneFrames(15);
    PlaySoundCue(185);
    actor->move_rate_x = move_rate;
    actor->move_rate_z = move_rate;
    SetStagedActorMoveTarget(actor, target_position[0], target_position[1], target_position[2]);
    leader->move_rate_x = move_rate;
    leader->move_rate_z = move_rate;
    SetLeadActorMoveTarget(leader, target_position[0], target_position[1], target_position[2]);
    UpdateStagedActorState(actor);
    FinalizeStagedActorUpdate();
    actor->x.value = target_position[0];
    actor->z.value = target_position[2];
    actor->unknown_24 = transition_busy;
    actor->unknown_2c = transition_busy;
    leader->unknown_38 = 0x80000000;
    leader->unknown_40 = 0x80000000;
    leader->unknown_24 = transition_busy;
    leader->unknown_2c = transition_busy;
    leader->x.value = leader->x.parts.cell << 16;
    leader->z.value = leader->z.parts.cell << 16;
    UpdateLeadActorState(leader, 1);
}

s32 State_FillGridCellByte2(u32 no, s32 x, s32 y, u32 w, u32 h, s32 val)
{
    u8 *g = Data_03001e70;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (no <= 2) {
            u32 off = no * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = Data_02010000;
        }
        base += (x + (y << 7)) * 4;
        for (i = 0; i < h; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < w; j++) {
                p[2] = (u8)val;
                p += 4;
            }
        }
    }
    return 0;
}

/* Named shorthand for one fixed argument pair, in overlay resource_3c8. */
void Actor_ApplySlotsMatchingKind212(void)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;
    s32 lim = (s32)&Value_00000212;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];
        u32 h = *(u16 *)((u8 *)p + 100);
        s32 t = h << 16;

        if ((t >> 20) == lim) {
            s32 m = 15;
            m &= h;
            Func_020054b8(p, m);
        }
    }
}

s32 OvObj_ApplyLowNibbleOfField100(void *obj)
{
    Func_020054de(obj, *(u16 *)((u8 *)obj + 100) & 15);
    return 0;
}

s32 OvObj_UpdateEveryFourFrames(void *obj)
{
    if ((*(u32 *)0x03001e40 & 3) == 0)
        Func_020054fa(obj, 7);
    else
        Func_02005502(obj, 0);

    if ((*(u32 *)0x03001e40 & 7) == 0)
        Func_020055d4(138);
    return 0;
}

s32 Effect_SpawnRandomEveryEightFramesB(struct Object_020005e4 *object)
{
    struct EffectParams_020005e4 params;
    s32 phase, x, y, speed;
    phase = *(u32 *)0x03001e40 & 7;
    if (phase != 0) goto done;
    params.unk00 = 3 - (s32)((u32)(Func_020053b2() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Func_020053d0() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Func_020053e4() * 32) >> 16)) << 16);
    speed = Func_020053f6(((s32)((u32)(Func_020053f8() * 5) >> 16) << 16) + 0x00050000, 10);
    Func_0200078e(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
    return 0;
}

s32 OvObj_ApplyZero(void *obj)
{
    Func_020054da(obj, 0);
    return 0;
}

s32 Effect_SpawnTwoRandomizedParticles(struct Object_020006a0 *obj)
{
    struct EffectParams_020006a0 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Func_02005470() * 8) >> 16) * 0x3333;
    phase = Data_03001e40 & 15;

    Func_0200081a(
        obj->x + ((8 - phase) << 16),
        obj->y + 0x001a0000,
        obj->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    phase = Data_03001e40 & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Func_02000852(
            obj->x
                + (((s32)((u32)(Func_020054cc() * 9) >> 16) - 4) << 16),
            obj->y,
            obj->z,
            0,
            0,
            0,
            0x000a0000,
            &params);
    }

    return 0;
}

void Effect_RunObjectZeroColorSequence(void)
{
    struct EffectObject *obj;
    u8 *state;

    state = *(u8 **)0x03001ebc;
    obj = Func_0200560a(0);
    Func_020055f8();
    Func_02005766(228);
    obj->callback = 0x020086a1;
    obj->color = 0x3333;
    Func_02005676(0, 2);
    Func_0200566a(0, 0, -6);
    Func_02005678(0);
    Func_020056c8(0, 15);
    Func_020055e4(Func_02005646(0), 0);
    obj->callback = 0;
    Func_02005636(30);
    Func_0200577a();
    Func_02005786();
    Func_02005764(*(s16 *)(state + 0x16c));
    Func_02005660();
}

/* Runs a guarded one-shot setup on an entry record: only fires the first
 * time (while a global 0x109 lookup is still unset), positions the entry
 * from its own stored coordinates, drives an effect/param sequence, then
 * writes a stage byte and an override field on the entry before returning. */
void Scene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *entry;
    u8 *guard;
    u8 *sub;

    entry = Scene_GetRecord_1(0);
    guard = GameFlag_IsSet_1(0x109);
    if (guard == 0) {
        Battle_Reset_1();
        Motion_CamBounds_1(-1, -1, -1, 0);
        /* Stage byte at +85 of the entry record. */
        entry[85] = guard;
        /* Position, from the entry's own s16 coordinates at +10/+18
         * (converted to 16.16 fixed point; the y term is offset by -16.0). */
        Motion_SetHPosTerrain_1(0, (*(s16 *)(entry + 10) << 16), ((*(s16 *)(entry + 18) << 16) + -0x100000));
        ObjectGroup_ConfigureChildValue_1(0, 15);
        sub = Scene_GetRecord_2(0);
        Func_02005676_a(sub, 0);
        Battle_WaitMode0_1();
        Motion_SetSpeed_1();
        Audio_PlayCue_1(228);
        /* Override field at +108 of the entry record; holds an EWRAM
         * address while the effect sequence below runs. */
        *(s32 *)(entry + 108) = 0x20086a1;
        Motion_SetSpeed_2(0, 0x6666, 0x3333);
        Motion_CommitPositionAndActivate_1(0, 0, 8);
        ObjectGroup_ConfigureChildValue_2(0, 0);
        sub = Scene_GetRecord_3(0);
        Func_020056b0(sub, 1);
        Motion_CommitPositionAndActivate_2(0, 0, 8);
        entry[85] = 3;
        /* Restore the +108 override field to the original (unset) value. */
        *(s32 *)(entry + 108) = guard;
        BattleFx_PlayQueuedSound_1();
        Battle_SchedShoulder_1();
    }
}

void State_StoreLookupZeroToWord24(void)
{
    extern u8 *Data_03001ebc;

    *(s32 *)(*(u8 **)0x03001ee0 + 24) = Func_02005752(0);
}

void State_ClearWorkspaceWord24(void)
{
    extern u8 *Data_03001ebc;

    *(s32 *)(*(u8 **)0x03001ee0 + 24) = 0;
}

s32 Actor_SetFlagBitByRelativeDepth(struct Actor_020008c8 *actor)
{
    struct Actor_020008c8 *ref;
    u8 *fp;
    u8 flags;
    ref = Func_0200577c(0);
    fp = &actor->flags23;
    flags = *fp | 2;
    *fp = flags;
    if (ref->z < actor->z) {
        s32 diff = actor->z - ref->z;
        s32 lim;
        diff += 0x00040000;
        lim = actor->y;
        lim += diff;
        if (ref->y <= lim) {
            flags &= 0xfd;
            *fp = flags;
        }
    }
    return 0;
}

void Actor_SwapPositionsByDepth(s32 group, s32 index)
{
    struct Position *first;
    struct Position *second;
    s32 value;

    first = Func_020057c2(group);
    second = Func_020057ca(index);
    if (first->z <= second->z) {
        value = first->x;
        first->x = second->x;
        second->x = value;

        value = first->y;
        first->y = second->y;
        second->y = value;

        value = first->z;
        first->z = second->z;
        second->z = value;
        Func_020056e8(1);
    }
}

void OvObj_WaitUntilIdle(struct BusyObject *obj)
{
    s32 cnt;
    s32 busy;

    cnt = 60;
    while (cnt != 0) {
        Func_020056fe(1);
        busy = obj->busy;
        cnt--;
        if (busy == 0) break;
    }
}

/*
 * Alias for a loader-relocated call word into IWRAM, not a runtime address.
 * It turns a value and a duration into a per-frame step.
 */

/*
 * Owner at 0x0200096c.  Add the velocity at +68/+72/+76 into the position at
 * +8/+12/+16, decay the X and Z velocities, accumulate the rates at +48/+52
 * into +24/+28, and advance the sprite angle by the record's step.  The Z
 * decay must stay written as a signed divide by 16 -- the negative bias and
 * arithmetic shift are what that division compiles to.
 */
void Func_0200096c(struct Effect_0200096c *effect)
{
    s32 velocity_z;
    struct Sprite_0200096c *sprite;
    s32 velocity_x;

    /* The block keeps the Z load after the Y store. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Func_0200572a(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_03001ebc[];

    s32 rec;
    s32 v8;
    s32 sh;
    s32 record;
    s32 half;
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w4;
    u8 *p10;
    u8 *p16;
    u8 slot28[40];
    u8 slot16[12];

    rec = Value1(Func_02005882, 0);
    Func_02005870();
    Call4(Func_02005972, -1, -1, -1, 0);
    Func_020057e6();
    Func_0200579c(1);
    *(volatile s32 *)(rec + 12) = 0x820000;
    *(volatile s32 *)(rec + 72) = 0x8000;
    *(volatile s32 *)(rec + 68) = 0;
    *(volatile u8 *)(rec + 85) = 0;
    Func_020059d6();
    Func_020059ea();
    Func_020058a8(30);
    Func_02005a1e(204);
    *(volatile u8 *)(rec + 85) = 3;
    Func_020058b8(24);
    p10 = slot28;
    *(s32 *)(p10 + 4) = 7;
    *(s32 *)(p10 + 36) = 0x200896d;
    *(s32 *)(p10 + 8) = 0xcccc;
    *(s32 *)(p10 + 12) = 0xcccc;
    v8 = 0;
    p16 = slot16;
    do {
        sh = v8 << 12;
        *(s32 *)(p16) = Func_02005812(sh);
        *(s32 *)(p16 + 4) = 0;
        record = Func_02005816(sh);
        *(s32 *)(p16 + 8) = record;
        half = *(s32 *)p16;
        *(s32 *)(p16) = half + half / 2;
        w0 = *(volatile s32 *)(rec + 8);
        w2 = *(volatile s32 *)(rec + 16);
        w1 = *(volatile s32 *)(rec + 12);
        w4 = *(volatile s32 *)(p16 + 4);
        Func_02000b9a(w0, w1, w2, (*(s32 *)p16), w4, record, 0x1090001, (s32)p10);
        v8 = (v8 + 1);
    } while ((u32)v8 <= 16);
    Func_02005a8e(188);
    Call2(Func_02005a0e, 0, 0x101);
    Func_0200599e(0, 22);
    Call3(Func_02005906, 0x50000, 0x50000, 0x10000);
    Call3(Func_02005914, -1, -1, 0xe666);
    Func_02005920();
    Call2(Func_02005a42, 0, 0x100);
    Func_02005ac6();
    *(volatile s32 *)(rec + 72) = 0x10000;
    *(volatile s32 *)(rec + 68) = 0x4000;
    Func_0200597e();
}

void Effect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020059c2();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x0200896d;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Func_020058fc(v);
        vec.y = 0;
        z = Func_02005900(v);
        x = vec.x;
        vec.z = z;
        x = x + Func_020058ee(x, 3);
        vec.x = x;
        Func_02000c8a(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

s32 Effect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject_02000b98 *obj)
{
    struct EffectParams_02000b98 params;
    s32 y;
    s32 a;
    s32 b;
    s32 rnd;

    if ((Data_03001e40 & 3) != 0) {
        return 0;
    }
    if ((u32)(Func_02005966() * 6) >> 16 == 0) {
        if (obj->near != 0x80000000 || obj->far != 0x80000000) {
            Func_02005bd0(246);
        }
    }
    y = 0;
    params.angle = 286;
    params.color1 = 0x10000;
    params.color2 = 0x10000;
    params.unk10 = -327;
    params.unk14 = -327;
    rnd = Func_020059a6();
    a = Func_020059a0(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    rnd = Func_020059be();
    b = Func_020059b8(((((u32)(rnd * 9)) >> 16) - 4) << 16, 10);
    Func_02000d52(obj->x, obj->y, obj->z - 0x10000, a, y, b, 0x001c0001, &params);
    return 0;
}

struct EffectObject_02000c5c *Effect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
    struct EffectObject_02000c5c *obj;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    obj = Func_02005a56(284, sx, 0, sz);
    if (obj == 0) {
        return 0;
    }
    obj->scale_x = 0x10000;
    obj->scale_y = 0x10000;
    Func_02005ace(obj, 0);
    Func_02005a66(obj, 7);
    obj->state = 0;
    obj->timer = 0;
    obj->phase = 0;
    obj->mode = 2;
    obj->callback = 0x02008b99;
    obj->flag = 0;
    Func_02005a96(obj, arg2);
    return obj;
}

s32 Actor_TryMoveActorZeroTwoTilesAhead(void)
{
    struct SceneObject_02000cc8 *obj;
    struct Vec vec;
    u8 *state;
    u8 old;
    s32 m;

    obj = Func_02005b84(0);
    state = &obj->state;
    old = *state;
    vec.x = (obj->x & 0xfff00000) + 0x80000;
    vec.y = obj->y;
    vec.z = (obj->z & 0xfff00000) + 0x80000;
    m = (obj->angle + 0x2000) & 0xc000;
    Func_02005ae6(0x100000, m, &vec);
    if (Func_02005b5e(obj, &vec) != 1 && Func_0200105a(&vec, obj) == 0) {
        vec.x = (obj->x & 0xfff00000) + 0x80000;
        vec.y = obj->y;
        vec.z = (obj->z & 0xfff00000) + 0x80000;
        Func_02005b2c(0x200000, (obj->angle + 0x2000) & 0xc000, &vec);
        if (Func_02001094(&vec, obj) == 0 && Func_02005bb0(obj, &vec) == 0) {
            Func_02005c10();
            Func_02005b58(obj, 6);
            Func_02005b2e(6);
            Func_02005d8c(152);
            Func_02005b6c(obj, 7);
            obj->scale_x = 0x30000;
            obj->scale_y = 0x20000;
            obj->accel = 0x40000;
            *state &= 0x7e;
            Func_02005c02(obj, 0);
            Func_02005ca0(0, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
            Func_02005ba8(obj, 6);
            Func_02005c20(obj, 1);
            *state = old;
            Func_02005c82();
            return 1;
        }
    }
    return 0;
}

/* Contiguous unnamed leaf-owner run for resource_3c8. */
s32 SceneData_SelectTableBySceneB5ToBa(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000b5) {
        return (s32)Data_0200dd68;
    }
    if (v == (s32)&Value_000000b7) {
        return (s32)Data_0200e020;
    }
    if (v == (s32)&Value_000000b8) {
        return (s32)Data_0200e230;
    }
    if (v == (s32)&Value_000000b9) {
        return (s32)Data_0200e350;
    }
    if (v == (s32)&Value_000000ba) {
        return (s32)Data_0200e548;
    }
    return (s32)Data_0200ddc8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablee740(void)
{
    return Data_0200e740;
}

u8 *SceneData_SelectTableBySceneAndApply(void)
{
    extern s16 Data_02000240[];

    u8 *ret;
    s16 *tbl;
    s16 v;

    tbl = Data_02000240;
    v = tbl[224];
    if (v == (s32)(u32)&Value_000000b5) {
        return Data_0200e904;
    }
    if (v == (s32)(u32)&Value_000000b6) {
        ret = Data_0200e9c4;
    } else if (v == (s32)(u32)&Value_000000b7) {
        ret = Data_0200eb74;
    } else if (v == (s32)(u32)&Value_000000b8) {
        ret = Data_0200ec04;
    } else if (v == (s32)(u32)&Value_000000b9) {
        ret = Data_0200ec64;
    } else if (v == (s32)(u32)&Value_000000ba) {
        ret = Data_0200ecf4;
    } else {
        goto no_match;
    }
    Func_02005d78(ret);
    return ret;

no_match:
    return Data_0200e8ec;
}

void State_SetFlag953(void)
{
    extern u8 *Data_03001ebc;

    Func_02005db4();
    Func_02005d94(0x953, 1);
    Func_02005dc8();
}

void Scene_RunActorEightTenStepLoop(void)
{
    extern u8 Data_03001ebc[];

    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Func_02005dd2();
    Func_02005e5a(8, 3);
    Func_02005e90(0x266d);
    n = 10;
    w = 8;
    Func_02005eae(8, 0, 20);
    do {
        Func_02005e96(8, 15);
        Func_02005d0c(2);
        Func_02005ea4_a(8, 0);
        Func_02005d1a(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    Func_02005e04(0x981);
    Func_02005e86(8, 0, 0);
    a = 7;
    b = 16;
    Func_02005dda(7, 17, 2, 1, a, b);
    Func_02005e46();
}

void Dialogue_RunActorElevenDialogue(void)
{
    Func_02005e54();
    Func_02005f0a(0x2670);
    Func_02005f24(11, 0, 20);
    Func_02005eec(11, 2);
    Func_02005f2c(11, 0);
    Func_02005e80();
}

void Scene_SetFlag987AtActorTwelveTile(void)
{
    Struct_0ff0 *s;

    s = Func_02005ea2(12);
    Func_02005e90_a();
    if (s->unk8 >> 20 == 54 || s->unk10 >> 20 == 6) {
        Func_02005e8e(0x987);
    }
    Func_02005eb2();
}

void Dialogue_RunLine2682(void)
{
    Func_02005ebc();
    Func_02005f2c_a(0, 1);
    Func_02005ea4_b(0x2682, 1);
    Func_02005ed8_a();
}

void State_ApplySixRectsAfter161(void)
{
    extern u8 *Data_03001ebc;

    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_02005ed4(0x161);
    x = 23;
    y = 8;
    Func_02005e98(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Func_02005e94(35, 8, 23, 8, b, a);
    Func_02005ea4(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Func_02005ed0(57, 55, 3, 3, x, y);
    Func_02005ec8(57, 55, 46, 55, a, a);
    Func_02005ed8(121, 55, 110, 55, a, a);
}

void State_ApplySixRectsAfterFlag161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_02005f50(0x161);
    x = 23;
    y = 8;
    Func_02005f1c(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Func_02005f18(36, 8, 23, 8, b, a);
    Func_02005f28(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Func_02005f54(53, 55, 3, 3, x, y);
    Func_02005f4c(53, 55, 46, 55, a, a);
    Func_02005f5c(117, 55, 110, 55, a, a);
}

void Scene_RunScene3c8SequenceC(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    u8 *record;
    s32 v2;

    rec7 = Value1(Func_02006002, 0);
    Func_02005ff0();
    Call2(Func_02006020, 0, 0x200d21c);
    Func_0200602e(0);
    Func_020060a6(0, 6);
    *(volatile s32 *)(rec7 + 40) = 0x40000;
    Call3(Func_02006036, 0, 0x40000, 0x20000);
    if ((*(volatile s32 *)(rec7 + 16) >> 20) <= 54) {
        *(u8 *)(Func_0200603c(0) + 90) &= 254;
        v2 = 210;
    } else {
        *(u8 *)(Func_02006050(0) + 90) &= 254;
        v2 = 238;
    }
    Func_02006098(0, *(s16 *)(rec7 + 10), (v2 << 2));
    Func_0200604e(1);
    {
        u8 *record = Func_02006074(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Func_02006064(20);
    *(volatile s32 *)(rec7 + 108) = 0x20085e5;
    Call3(Func_02006154, 0, 0x102, 60);
    Func_020060fc(0, 4);
    Func_0200612c(0, 0);
    Func_0200610c(0, 4);
    *(volatile s32 *)(rec7 + 108) = 0;
    Func_020060a4();
}

void Scene_RunScene3c8SequenceD(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)Data_03001ebc;
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        s32 shown = 1;

        *(volatile u16 *)(((s32)p7 + 0xcb6)) = shown;
    }
    Func_020060c6();
    Func_02006136(0, 1);
    Call2(Func_020060ae, 0x2688, 1);
    Call2(Func_02006200, 0x10000, 0);
    Call2(Func_02006200_a, 0x10005, 0);
    Func_02006216(120);
    Func_020060ec(100);
    Func_02006262(142);
    Func_020060f8(30);
    Call2(Func_02006220, 0x7fff, 0);
    Func_02006236(60);
    Func_0200610c_a(70);
    if (Value1_02001218(Func_020060fa, 0x982) == 0) {
        if (Value1_02001218(Func_02006104, 0x983) == 0) {
            if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
                Call1(Func_02006120, 0x982);
            } else {
                Call1(Func_02006128, 0x983);
            }
        }
    }
    if (Value1_02001218(Func_02006126, 0x982) == 0) {
        Call1(Func_02006138, 0x982);
        Call1(Func_02006146, 0x983);
        Call6(Func_020060f2, 103, 27, 89, 27, 7, 8);
        Call6(Func_02006106, 41, 90, 27, 92, 3, 2);
        Call6(Func_02006116, 41, 90, 29, 93, 3, 2);
        Call6(Func_02006126_a, 41, 90, 27, 94, 3, 2);
        Call6(Func_02006136_a, 41, 90, 27, 96, 3, 2);
        Call6(Func_02006146_a, 41, 90, 29, 97, 3, 2);
        Call6(Func_02006156, 41, 96, 25, 91, 3, 2);
        Call6(Func_02006166, 41, 92, 25, 93, 3, 2);
        Call6(Func_02006176, 41, 96, 25, 95, 3, 2);
        Call6(Func_02006186, 41, 96, 25, 97, 3, 2);
        Call6(Func_02006196, 41, 96, 27, 96, 3, 2);
        Call6(Func_020061a6, 41, 96, 29, 97, 3, 2);
    } else {
        Call1(Func_0200620e, 0x983);
        Call1(Func_0200621c, 0x982);
        Call6(Func_020061c8, 111, 27, 89, 27, 7, 8);
        Call6(Func_020061dc, 41, 90, 25, 91, 3, 2);
        Call6(Func_020061ec, 41, 90, 25, 93, 3, 2);
        Call6(Func_020061fc, 41, 90, 25, 95, 3, 2);
        Call6(Func_0200620c, 41, 90, 25, 97, 3, 2);
        Call6(Func_0200621c_a, 41, 90, 27, 96, 3, 2);
        Call6(Func_0200622c, 41, 90, 29, 97, 3, 2);
        Call6(Func_0200623c, 41, 94, 27, 92, 3, 2);
        Call6(Func_0200624c, 41, 96, 29, 93, 3, 2);
        Call6(Func_0200625c, 41, 94, 27, 94, 3, 2);
        Call6(Func_0200626c, 41, 96, 27, 96, 3, 2);
        Call6(Func_0200627c, 41, 96, 29, 97, 3, 2);
    }
    Call2(Func_02006416, 0x10000, 0);
    Func_0200642c(20);
    Func_02006302(40);
    Call2(Func_020063fe, 0x8000, 0x1000);
    Call4(Func_02006416_a, 0x1c80000, -1, 0x21e0000, 1);
    Func_02006422();
    Func_02006328(50);
    Call4(Func_02006430, 0x1c80000, -1, 0x1a70000, 1);
    Func_0200643c();
    Func_02006350();
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcb6);
        s32 shown = 0;

        *target = shown;
    }
}

void Scene_RunFlag986ActorOneScene(void)
{
    extern u8 Data_03001ebc[];

    Struct_A *o;
    Struct_B *u;
    s32 g;
    s32 m1;
    s32 m2;
    s32 h;
    s32 k;

    g = 0x986;
    m1 = 0xcccc;
    m2 = 0x6666;
    h = 0x100;
    k = 0x338;
    Func_0200638c();
    o = Func_020063aa(12);
    if (o->unk8 >> 20 == 53) {
        if (Func_02006382(g) == 0) {
            Func_02006394(g);
            o = Func_020063ca(0);
            if (o != 0) {
                Func_02006420(1, o->unk8, o->unk10);
            }
            Func_020063ea(1, m1, m2);
            Func_0200641e(1, k, 88);
            Func_0200642a(1, k, 104);
            Func_02006484(1, 0, 0);
            Func_020063ea_a(20);
            Func_02006472(1, 4);
            Func_020063f8(20);
            Func_020064b6(0x2691);
            Func_020064d0(1, 0, 20);
            Func_020064e2(1, 0, 10);
            Func_020064fe(1, h, 60);
            Func_020064c0(1, 0, 0);
            Func_0200642e(20);
            Func_020064be(1, 2);
            Func_0200643c_a(20);
            Func_0200650e(1, 0, 20);
            Func_020064be_a(0, 3);
            Func_020064d6(1, 3);
            Func_0200645c(30);
            Func_020064b8(1, k, 88);
            Func_020064e0(1, 2);
            u = Func_02006496(0);
            if (u != 0) {
                Func_020064c8(1, u->unkA, u->unk12);
            }
            Func_020064ee(1);
            Func_02006500(1, 0, 0);
            Func_020064ac();
        }
    }
}

void Scene_RunFiveCallSequence(void)
{
    void Func_0200226c_r(s32, s32);

    Func_020064c0_a();
    Func_020019a4();
    Func_020064c2(20);
    Func_020064d6_a();
    Func_02002b32();
}

void State_RunActor13AtColumn42Setup(void)
{
    Struct_1644 *obj;
    s32 val;
    s32 a;
    s32 b;

    obj = Func_020064f8(13);
    Func_020064e6();
    if (obj->unk8 >> 20 == 42) {
        Func_020064ec(30);
        Func_02006662(188);
        obj->unk55 = 0;
        val = 0xfffe0000;
        obj->unk14 = val;
        obj->unkC = val;
        Func_020064f8_a(0x200);
        a = 3;
        b = 5;
        Func_020064ac_a(44, 117, 41, 117, a, b);
    }
    Func_02006530();
}

void Scene_PlaceAndPinSlots8To10(void)
{
    void Func_0200226c_r(s32, s32);

    u32 i;
    Struct_18f8 *rec;
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_020067ac(8);
    Func_02006798();
    x = 12;
    y = 44;
    Func_0200674c(19, 44, 4, 1, x, y);
    x = 11;
    y = 51;
    Func_02006760(17, 51, 2, 2, x, y);
    i = 0;
    do {
        rec = Func_020067e2(i + 8);
        a = rec->unk8 >> 20;
        b = rec->unk10 >> 20;
        Func_02006784(12, 50, 1, 1, a, b);
        i++;
    } while (i <= 2);
    Func_0200226c_r(10, 9);
    Func_020067fc();
}

void State_ApplyRectAt19_44AndRunThree(void)
{
    s32 x;
    s32 y;

    Func_02006806();
    x = 12;
    y = 44;
    Func_020067ba(19, 44, 4, 1, x, y);
    Func_02001cfe();
    Func_02003286();
    Func_0200682e();
}

void Actor_ApplyKind45AtActorsElevenAndTwelve(void)
{
    u32 i;
    Struct_199c *p;

    i = 0;
    do {
        p = Func_02006852(i + 11);
        i++;
        Func_02006818(0, p->unk8, p->unk10, 45);
    } while (i <= 1);
}

void Actor_ApplyPositionsOfActors11And12(void)
{
    u32 i;
    Struct_19c0 *p;

    i = 0;
    do {
        p = Func_02006876(i + 11);
        if (p->unkC > -0x100000) {
            Func_02006842(0, p->unk8, p->unk10, 255);
        }
        i++;
    } while (i <= 1);
}

void Scene_RunGuardedThreeStepSetup(void)
{
    void Func_0200226c_r(s32, s32);

    Func_02006888();
    if (Func_020026c0() == 0) {
        Func_0200339c();
        Func_02001d78();
        Func_020033c8();
    }
    Func_020068a8();
}

void State_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    extern u8 *Data_03001ebc;

    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Func_0200687a(9, 24, 1, 1, x, z);
}

void OvObj_ResetObjectWhenFlags12Set(Struct_1a50 *o)
{
    Struct_Sub *q;
    s32 v;
    s32 z;
    s32 t;
    s32 m;

    q = o->unk50;
    v = q->unk9;
    if ((v & 12) == 12) {
        m = -13;
        m &= v;
        m |= 4;
        {
            u8 *pq = &q->unk9;
            *pq = m;
        }
        z = 0;
        o->unk44 = z;
        t = Func_02001ad4(o->unk8, 0, 0x2000000, 223);
        Func_020023d0(o);
        o->unk8 = z;
        o->unk10 = z;
        Func_0200687a_a(t);
    } else {
        Func_02003454();
    }
}

void Actor_UpdateSlots11And12ByTile(void)
{
    Struct_1a9c *o;

    Func_02006934();
    o = Func_02006952(11);
    if (o->unk8 >> 20 == 8) {
        Func_02002400();
        Func_020034ce(o);
    } else {
        Func_02003512(o);
    }
    o = Func_02006974(12);
    if (o->unk8 >> 20 == 7) {
        Func_02002422();
        Func_020034f0(o);
    } else {
        Func_02003534(o);
    }
    Func_02006984();
}

void Scene_RunGuardedRectStep(void)
{
    s32 x;
    s32 y;

    Func_02006dc2();
    if (Func_02002bfa() == 0) {
        x = 45;
        y = 43;
        Func_02006d7e(109, 43, 7, 5, x, y);
        Func_020022c2();
    }
    Func_02006dee();
    Func_02003c9e();
}

void State_RunConditionalStep(void)
{
    extern u8 *Data_03001ebc;

    Func_02007106();
    if (Func_02002f3e() == 0) {
        s32 k5 = 44, k6 = 39;
        Func_020070c2(108, 39, 13, 7, k5, k6);
        Func_02002606();
    }
    Func_02007132();
    Func_020041fa();
}

s32 Actor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Func_0200715e(((s16 *)obj)[50]);
    ((s32 *)obj)[3] = rec->unkC + 0x100000;
    return 0;
}

/*
 * resource_3c8 owner at 0x020023d4, 168 bytes: among scene slots 8-13,
 * locate candidates sharing the selected slot's x/z tile, retain the highest
 * candidate at least one 16.16 unit above its y value, store that candidate id
 * at selected+100, then move/release the selected slot and run its local effect.
 *
 * Complete owner: high-register prologue and four-byte frame at 0x020023d4
 * through the sole interworking return at 0x02002468-0x02002474, followed by
 * alignment and one referenced pool word through 0x0200247b.  Eight static
 * calls across seven targets match independently; the two scene-accessor call
 * sites sit inside the bounded six-iteration loop.
 */
void Actor_PickHighestSlotAtSameTileAndRelease(s32 selector)
{
    extern u8 *Data_03001ebc;
    void Func_02000b08();

    u8 *cand;
    s32 highest = (s32)0xffb00000;
    u8 *sel = 0;
    u32 i;

    for (i = 0; i <= 5; i++) {
        s32 no = i + 8;
        s32 cand_y;

        if (no == selector) {
            continue;
        }

        cand = Func_020072a2(no);
        sel = Func_020072aa(selector);

        if ((*(s32 *)(cand + 8) >> 20)
                != (*(s32 *)(sel + 8) >> 20)
            || (*(s32 *)(cand + 16) >> 20)
                != (*(s32 *)(sel + 16) >> 20)) {
            continue;
        }

        cand_y = *(s32 *)(cand + 12) + 0x100000;
        if (highest <= cand_y) {
            *(u16 *)(sel + 100) = (u16)no;
            highest = cand_y;
        }
    }

    Func_020072f6(selector, 0x40000, 0x20000);
    Func_02007252(sel,
                  *(s32 *)(sel + 8),
                  highest,
                  *(s32 *)(sel + 16));
    Func_02007340(selector);
    Func_02007456(188);
    Func_02002f68(selector);
    Func_020072f2(30);
}

void Scene_RunThreeCallSequence(void)
{
    void Func_0200226c_r(s32, s32);

    Func_0200757c();
    Func_02002a60();
    Func_0200758c();
}

void Scene_DrawTilesWhenCheckClear(void)
{
    void Func_0200226c_r(s32, s32);

    Func_02007d7e();
    if (Func_02003bb6() == 0) {
        { s32 k5 = 5, k6 = 48; Func_02007d3a(69, 48, 4, 2, k5, k6); }
        { s32 j5 = 9, j6 = 37; Func_02007d4e(73, 37, 9, 13, j5, j6); }
        Func_02003292();
    }
    Func_02007dbe();
    Func_02005a3a();
}

/* Runs a fixed sequence of setup calls with literal parameters; most share
 * a leading 0 argument. */
void Scene_RunScene3c8SequenceB(void)
{
    extern u8 Data_03001ebc[];

    BattleRuntime_Reset_1_02002f30();
    ObjectMotion_SetSpeedParameters_1_02002f30(0, 0x8000, 0x4000);
    Motion_SetPosReset_1(0, 0x208, 0x2c8);
    Motion_ArmCb_1(0, 0x4000, 10);
    (void)Func_02002fc4(0x2080000, 0, 0x3100000, 223);
    BattleFx_RunRisingObjectSequence_1(0, 6, 0);
    BattleRuntime_WaitIfModeZero_1_02002f30(60);
    Func_02007f26(20); /* main:0808a248 */
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1_02002f30();
}

u8 *SceneData_SelectTableBySceneId(void)
{
    extern u8 Data_02000240[];

    s32 off = 0x1c0;
    s32 v = *(s16 *)(Data_02000240 + off);

    if (v == (s32)&Value_000000b5) {
        return Data_0200ee44;
    }
    if (v == (s32)&Value_000000b6) {
        return Data_0200ef1c;
    }
    if (v == (s32)&Value_000000b7) {
        return Data_0200f120;
    }
    if (v == (s32)&Value_000000b8) {
        return Data_0200f300;
    }
    if (v == (s32)&Value_000000b9) {
        return Data_0200f3b4;
    }
    if (v == (s32)&Value_000000ba) {
        return Data_0200f4f8;
    }
    return Data_0200ef1c;
}

void Actor_ClearActorModeAndSetState5(s32 no)
{
    u8 *p;
    s32 mask;

    p = Func_02007ebe(no);
    p[0x55] = 0;
    mask = 252;
    mask &= p[0x59];
    p[0x59] = mask;
    Func_02007e72(p, 0);
    Func_02007e0a(p, 5);
    Func_02007fa2(no, 3);
    {
        s32 v = 2;
        v |= p[0x23];
        p[0x23] = v;
    }
}

void State_ApplyStepToSlots15To18(void)
{
    extern u8 *Data_03001ebc;

    u32 i;

    i = 15;
    do {
        Func_02007f06(i);
        i++;
    } while (i <= 18);
}

void State_PassZeroAndMinusOneRecord(void)
{
    extern u8 *Data_03001ebc;

    struct Args_02003fe4 args;

    args.first = 0;
    args.last = -1;
    Func_0200803c(&args);
}

void State_PassRange0To1(void)
{
    extern u8 *Data_03001ebc;

    struct Args_02003ffc args;

    args.first = 0;
    args.last = 1;
    Func_02008054(&args);
}

void State_PassRangeNeg1To0(void)
{
    extern u8 *Data_03001ebc;

    struct Args_02004014 args;

    args.first = -1;
    args.last = 0;
    Func_0200806e(&args);
}

void State_CallHandlerWithFlagPair(void)
{
    extern u8 *Data_03001ebc;

    struct Args_02004030 args;

    args.first = 1;
    args.last = 0;
    Func_02008088(&args);
}

/*
 * Func_02008b1a is the relocated call word for the in-overlay routine at
 * image offset 0x4520, not a runtime address.  The 16-byte owner loads no
 * literal, so it carries no pool word and no alignment halfword.  432 is
 * built from a shifted immediate and passed straight to the callee as a
 * value, not used as a displacement.
 */
void State_Call4520With432And32(void)
{
    extern u8 *Data_03001ebc;

    Func_02008b1a(432, 32);
}

void Scene_RunStep230With44(void)
{
    extern u8 Data_03001ebc[];

    Func_02008b2a(0x230, 44);
}

void FieldScene_RunScene3c8_02004a2c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Value1_02004a2c(Func_020098e6, 0);
    rec8 = Value1_02004a2c(Func_020098ee, 20);
    Func_020098dc();
    Call4(Func_020099de, -1, -1, -1, 0);
    Func_02009852();
    Func_02009808(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Func_02009a48();
    Func_02009a5c();
    Func_02009a8a(204);
    Func_02009920(30);
    *p8 = 3;
    Func_0200992c(24);
    Call2(Func_02009a1c, 0, 0x101);
    Func_020099ac(0, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Func_0200987a(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Func_02009894(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Func_020098b0(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Func_020098c8(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Call2(Func_02009ac6, 0, 0x100);
    Func_02009a56(0, 1);
    Func_020099ec(40);
    *(s32 *)(rec + 108) = 0x200c969;
    Func_020099f6(60);
    Func_02009ad6(0, 1);
    Func_02009ade(20, 1);
    Func_02009b7c(17);
    Call1(Func_02009b84, 0x134);
    Call1(Func_02009a0a, 0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Func_0200994e(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Func_02009b5c(21);
}
