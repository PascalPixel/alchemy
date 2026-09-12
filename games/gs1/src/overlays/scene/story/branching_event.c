#include "types.h"

#define CalculateFixedPointPositionDistance Func_02000030
#define StagedActorStepTable Data_0200afd4
#define GetStagedActor Func_02002f24
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002f88
#define SetStagedActorMode Func_02002f50
#define SelectStagedActorSlot Func_02002f40
#define StartStagedActorEffect Func_02003156
#define StartNextStagedActorMove Func_02002f8e
#define StartLeadStagedActorMove Func_02002f9e
#define FinishStagedActorMove Func_02002fac
#define FinishStagedActorEffect Func_02003178
#define SetStagedActorTransition Func_02002fbc
#define SceneActor_ResetMotionWhenAheadBlocked Func_020002a8
#define SceneActor_PushObjectAheadIfLevel Func_020026fc
#define TestActorPosition Func_02003344
#define ActorProbeOffsets Data_0200b02c
#define ActorSearchStep Data_0200afd4
#define StagedActor_FindClearPosition Func_02000474
#define Data_0200e1e8 Data_0200b02c
#define Data_0200e190 Data_0200afd4
#define Func_02006610 Func_0200347c
#define Func_0200661c Func_02003488
#define Func_020066b4 Func_02003520
#define Func_02006714 Func_02003560
#define Func_0200668a Func_02003506
#define Func_0200672c Func_02003578
#define Func_020066ea Func_02003556
#define Func_020066c4 Func_02003540
#define RefreshStagedActor Func_02003560_a
#define Func_02006614 Func_020034b8
#define Func_0200661e Func_020034c2
#define Func_0200687c Func_020036c0
#define Func_02006658 Func_020034f4
#define Func_02006776 Func_020035c2
#define Func_0200678e Func_020035da
#define Func_02006740 Func_020035ac
#define Func_020067a4 Func_020035f0
#define Func_020067bc Func_02003608
#define Func_020067d4 Func_02003620
#define Func_0200677a Func_020035e6
#define Func_020068f4 Func_02003738
#define Func_020068fa Func_0200373e
#define Func_020066b8 Func_0200355c
#define Func_02006752 Func_020035ee
#define Func_020067ae Func_0200364a
#define Func_020069d0 Func_02003814
#define OverlayObject_SpawnConfiguredObject Func_020009f4
#define OverlayObject_PrepareObjectWithCommand15 Func_02000a4c
#define OverlayObject_IntegrateVelocities Func_02000ab0
#define OverlayObject_WaitUntilIdle Func_02000cd0
#define ConfigurableEffectDescriptors Data_0200b134
#define GetPrimaryEffectRecord Func_0200395a
#define SpawnConfiguredEffectObject Func_020038d8
#define SetConfiguredEffectVariant Func_020038e2
#define ApplyConfiguredEffectDescriptor Func_020038fc
#define SetConfiguredEffectLink Func_02003a96
#define ComputeConfiguredEffectRateFromDelta Func_020039a0
#define ComputeConfiguredEffectRateFromOffset Func_020039b8
#define ComputeConfiguredEffectSecondRate Func_020039c6
#define SetConfiguredEffectCallbackMode Func_020039fc
#define ApplyConfiguredEffectCallbackArgument Func_02003a0c
#define SceneEffect_SpawnConfiguredEffect Func_02000ae8
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_SetValue8Mode66 Func_02000cc0
#define SceneState_ApplyRectAt32x78 Func_020024b0
#define SceneState_SetRuntimeByte34 Func_020026a8
#define SceneState_ApplyRectsAtActors8And9 Func_02002820
#define FieldScene_RunSupplementalSequenceOne Func_02000d4c
#define FieldScene_RunScene3c5SequenceA Func_02000eac
#define FieldScene_RunStep11 Func_02001184
#define FieldScene_RunStep12WithPosition Func_0200119c
#define FieldScene_RunStep13WithTwoPositions Func_020011c4
#define FieldScene_RunStep15 Func_020011fc
#define FieldScene_RunStepWithValue2693 Func_02001214
#define FieldScene_RunScene3c5_020024d0 Func_020024d0
#define FieldScene_RunScene3c5_02002548 Func_02002548
#define FieldScene_RunFlag985DialogueBranch Func_020025c8
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae
#define PrimaryOverlayData Data_0200b270
#define SecondaryOverlayData Data_0200b330
#define TertiaryOverlayData Data_0200b4f8
#define DefaultOverlayData Data_0200b558
#define SceneData_SelectByRuntimeSelector Func_02000f58
#define SceneData_SelectTableB5b8ByState Func_02000fac
#define SceneData_GetTableb5f8 Func_02000fd4
#define PrimaryOverlayData_02000fdc Data_0200b694
#define SecondaryOverlayData_02000fdc Data_0200b754
#define TertiaryOverlayData_02000fdc Data_0200b784
#define DefaultOverlayData_02000fdc Data_0200b88c
#define FieldScene_RunFourCallSequence Func_020010f4
#define ResetSceneParametersAndFinishSetup Func_02001158
#define SceneState_ConfigureRegion82_7AndApply768 Func_020027f4
#define SceneState_BranchOnActorEightOrNineTile Func_0200110c
#define FieldScene_RunBranchingActorSequence Func_02001238
#define FieldScene_RunActorEventSequence Func_02001b10
#define ActorPresentation_SetSceneCellByFlag985 Func_0200246c
#define ActorPresentation_PlaceActorTwelveAtTile20And12 Func_020026b8
#define SceneActor_RunSlotZeroFacingCheck Func_02002738
#define PrimaryOverlayData_020027a0 Data_0200b91c
#define SecondaryOverlayData_020027a0 Data_0200b9e8
#define TertiaryOverlayData_020027a0 Data_0200bac0
#define DefaultOverlayData_020027a0 Data_0200bc28
#define StagedActor_PushActorAhead Func_020000c4
#define SceneData_SelectDataByRuntimeSelector Func_02000fdc
#define SceneData_SelectTableB91cByRuntimeSelector Func_020027a0

#include "staged_actor.h"
#include "find_clear_actor_position.h"
#include "staged_actor_movement.h"
#include "spawn_configured_effect.h"
#include "select_overlay_data_by_runtime_selector.h"

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

/*
 * Per-frame integrator for an effect record -- resource_3c5. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct MotionEffect {
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
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Obj {
    u8 pad00[6];
    u16 f06;
};

extern u8 Data_02010000[];
extern s32 Data_0200b014[];
extern struct LevelCheckRecord *Func_02005552();   /* record by slot index */
extern struct LevelCheckRecord *Func_02004d68();   /* occupant one square ahead */
extern void Func_020027f2();                       /* push the object ahead */
extern u8 Value_000000b0;
extern u8 Data_0200b5b8[];

typedef s32(*IwramIntegerSquareRoot)(s32);
struct StagedActor *Func_02002f24(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02002f88(struct StagedActor *arg0, s32 *arg1);
void Func_02002f50(struct StagedActor *arg0, s32 arg1);
void Func_02002f40(s32 arg0);
void Func_02003156(s32 arg0);
void Func_02002f8e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002f9e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02002fac(struct StagedActor *arg0);
void Func_02002fbc(struct StagedActor *arg0, s32 arg1);
Ent *Func_02000342(Desc *, Ent *);
s32 Func_0200310a(Ent *, Desc *);
u8 *Func_020037ae(s32 kind, s32 x, s32 y, s32 z);
void Func_0200381c(u8 *object, s32 mode);
void Func_02003904(u8 *object, s32 mode);
void Func_02003834(u8 *object, s32 mode);
u8 *Func_02003806(s32 kind, s32 x, s32 y, s32 z);
void Func_02003878(u8 *object, s32 mode);
void Func_02003960(u8 *object, s32 mode);
void Func_02003a5e();
void Func_02003c00();
void Func_020052a6();
s32 *Func_02005678();
void Func_0200572a();
void Func_02005732();
void Func_02005626();
void Func_02005638();
void Func_02005650();
s32 *Func_020056ce();
void Func_0200566e();
s32 Func_02003a8a();
void Func_0200190c();
void Func_02003af4();
void Func_02003b1e();
s32 Func_02003b7c();
s32 Func_02003b80();
void Func_02003b90();
s32 Func_02003bca();
s32 Func_02003bd6();
void Func_02003bde();
s32 Func_02003be2();
s32 Func_02003bec();
void Func_02003bf6();
s32 Func_02003c0a();
s32 Func_02003c1c();
void Func_02003c4e();
void Func_02003c5c();
void Func_02003c68();
void Func_02003c8a();
s32 Func_02003cc0();
void Func_02003cc6();
s32 Func_02003ccc();
void Func_02003cce();
void Func_02003cf4();
void Func_02003d08();
void Func_02003d3e();
void Func_02003d44();
void Func_02003d72();
void Func_02003dbe();
void Func_02001c0a();
void Func_02003c4e_a();
void Func_02003c78();
void Func_02003cea();
void Func_02003cf6();
s32 Func_02003d24();
s32 Func_02003d30();
s32 Func_02003d3c();
void Func_02003d44_a();
s32 Func_02003d46();
s32 Func_02003d50();
s32 Func_02003d70();
s32 Func_02003d7c();
void Func_02003d86();
void Func_02003d8c();
void Func_02003de4();
void Func_02003e0a();
void Func_02003e5a();
void Func_02003e6e();
void Func_02003e78();
void Func_02003eaa();
void Func_02003fc0();
void Func_020022e4();
void Func_020040c2();
void Func_02003fd6();
void Func_02003fd8();
void Func_02001ba4();
void Func_0200230c();
void Func_020040ea();
void Func_02003ffe();
void Func_02004000();
s32 Func_02001bcc();
void Func_02001bdc();
void Func_02002344();
void Func_02004122();
void Func_02004036();
void Func_02004038();
void Func_0200235c();
void Func_0200413a();
void Func_0200404e();
void Func_02004050();
void Func_020040b0();
void Func_02004038_a();
void Func_0200406c();
void Func_02004046();
s32 Func_020052f0();
void Func_020052fc();
void Func_02005302();
void Func_02005324();
s32 Func_02005346();
void Func_02005358();
void Func_02005364();
void Func_02005368();
void Func_0200538a();
void Func_02005390();
void Func_02005478();
s32 Func_02005368_a();
void Func_02005374();
void Func_02005382();
void Func_0200539c();
s32 Func_020053be();
void Func_020053d0();
void Func_020053d8();
void Func_020053d8_a();
void Func_020053dc();
void Func_020053e0();
void Func_02005402();
void Func_02005408();
void Func_020054f0();
void Func_0200540a();
s32 Func_020053fc();
void Func_02005400();
void Func_02005400_a();
void Func_02005586();
void Func_020053ea();
void Func_02005440();
void Func_02005456();
void Func_02004afa();
void Func_0200550a();
void Func_02005524();
void Func_02005480();
void Func_02003f30();
void Func_020011c0();
void Func_02002130();
void Func_02003f44();
void Func_0200408a();
void Func_0200402c();
void Func_020040d0();
void Func_020040dc();
void Func_020055e2(s32, s32, s32, s32, s32, s32);
void Func_02005590(s32);
void Func_02005638_a(s32);
s32 *Func_02003f62();
s32 *Func_02003f6a();
s32 *Func_02003f78();
void Func_0200223c();
void Func_0200408e();
void Func_0200405e();
void Func_02004064();
void Func_02004080();
void Func_020040b0_a();
void Func_020040b8();
void Func_020040de();
void Func_020040f6();
void Func_02004104();
void Func_0200412a();
void Func_0200412c();
void Func_0200413a_a();
void Func_02004140();
void Func_02004148();
void Func_02004166();
void Func_0200416a();
void Func_0200416c();
void Func_0200417a();
void Func_02004188();
void Func_0200419c();
void Func_0200419e();
void Func_020041b4();
void Func_020041b4_a();
void Func_020041b6();
void Func_020041ca();
void Func_020041e4();
void Func_020041f4();
void Func_020041f4_a();
void Func_020041f8();
void Func_020041fa();
void Func_020041fe();
void Func_02004202();
void Func_02004208();
void Func_0200420e();
void Func_02004210();
void Func_02004212();
void Func_02004214();
void Func_02004218();
void Func_0200421c();
void Func_0200421e();
void Func_0200421e_a();
void Func_02004220();
void Func_02004222();
void Func_0200422a();
void Func_0200422e();
void Func_02004230();
void Func_02004234();
void Func_0200423a();
void Func_0200423e();
void Func_02004244();
void Func_0200424c();
void Func_0200425a();
void Func_0200426a();
void Func_0200427c();
void Func_0200427c_a();
void Func_0200428a();
void Func_02004292();
void Func_02004296();
void Func_020042a6();
void Func_020042ae();
void Func_020042ae_a();
void Func_020042b0();
void Func_020042c2();
void Func_020042c2_a();
void Func_020042d4();
void Func_020042dc();
void Func_020042f0();
void Func_020042f8();
void Func_02004306();
void Func_0200430c();
void Func_02004314();
void Func_02004322();
void Func_02004330();
void Func_02004334();
void Func_02004336();
void Func_0200433c();
void Func_02004348();
void Func_02004356();
void Func_02004364();
void Func_02004364_a();
void Func_02004368();
void Func_0200436a();
void Func_0200436e();
void Func_02004372();
void Func_0200437c();
void Func_02004380();
void Func_02004384();
void Func_0200438e();
void Func_0200438e_a();
void Func_02004394();
void Func_020043a8();
s32 Func_020043b6();
void Func_020043b8();
void Func_020043ba();
void Func_020043c6();
void Func_020043c8();
void Func_020043de();
void Func_020043e2();
void Func_020043ea();
void Func_020043ec();
void Func_020043fa();
void Func_02004404();
void Func_02004408();
void Func_0200440a();
void Func_02004416();
void Func_02004416_a();
void Func_02004424();
void Func_0200442a();
void Func_02004432();
void Func_02004442();
s32 Func_02004446();
void Func_0200446a();
void Func_02004474();
void Func_02004478();
void Func_02004482();
void Func_02004486();
void Func_02004488();
void Func_0200448a();
void Func_02004498();
void Func_020044a6();
void Func_020044b4();
void Func_020044ce();
void Func_020044d0();
void Func_020044dc();
void Func_020044ea();
void Func_020044f4();
void Func_020044f8();
void Func_02004506();
void Func_0200451c();
void Func_02004520();
void Func_0200452a();
void Func_0200452c();
s32 Func_0200453a();
s32 Func_0200453a_a();
void Func_02004544();
void Func_0200454a();
void Func_02004556();
void Func_0200455a();
void Func_0200455a_a();
void Func_02004566();
void Func_02004568();
void Func_02004580();
void Func_02004586();
void Func_02004590();
void Func_0200459c();
void Func_0200459c_a();
void Func_020045a0();
void Func_020045ae();
void Func_020045b4();
void Func_020045b8();
void Func_020045c8();
s32 Func_020045ca();
s32 Func_020045ca_a();
void Func_020045d6();
void Func_020045e0();
void Func_020045e2();
void Func_020045f0();
void Func_020045fa();
void Func_02004608();
void Func_02004608_a();
void Func_02004616();
void Func_0200461c();
void Func_02004628();
void Func_02004638();
void Func_0200463a();
void Func_02004648();
void Func_02004652();
void Func_02004656();
void Func_02004662();
void Func_02004670();
void Func_02004678();
s32 Func_0200467e();
s32 Func_0200467e_a();
void Func_02004688();
void Func_020046b4();
void Func_020046ba();
void Func_020046c2();
void Func_020046ca();
void Func_020046d0();
void Func_020046e0();
void Func_020046e0_a();
void Func_020046f2();
void Func_020046f4();
void Func_020046f8();
void Func_020046fa();
void Func_02004700();
s32 Func_0200470e();
void Func_02004712();
void Func_02004712_a();
void Func_02004720();
void Func_02004724();
void Func_0200473a();
void Func_0200473e();
void Func_02004740();
void Func_0200475e();
void Func_02004762();
void Func_02004774();
void Func_02004792();
void Func_02004792_a();
void Func_020047a2();
void Func_020047aa();
void Func_020047ac();
void Func_020047b8();
void Func_020047ba();
void Func_020047c6();
void Func_020047cc();
void Func_020047d2();
void Func_020047d6();
void Func_020047fa();
void Func_020047fe();
void Func_0200480c();
void Func_0200480c_a();
void Func_02004814();
void Func_02004818();
void Func_0200481a();
void Func_0200483c();
void Func_02004848();
void Func_02004850();
void Func_02004876();
void Func_02004876_a();
void Func_0200487e();
void Func_020048a4();
void Func_020048a8();
void Func_020048a8_a();
void Func_020048b2();
void Func_020048b4();
void Func_020048bc();
s32 Func_020048c2();
void Func_020048c6();
void Func_020048cc();
void Func_020048e2();
void Func_020048e4();
s32 Func_020048f2();
void Func_020048f4();
void Func_020048f6();
void Func_020048fc();
void Func_02004902();
void Func_02004908();
void Func_0200490a();
void Func_02004914();
void Func_0200491c();
s32 Func_02004922();
void Func_0200492a();
void Func_0200492c();
void Func_02004930();
void Func_0200493a();
void Func_0200493e();
void Func_02004944();
void Func_0200494c();
void Func_0200495c();
void Func_0200496a();
void Func_0200497c();
void Func_02004394_a();
void Func_02004936();
void Func_02004952();
void Func_02004980();
void Func_0200498a();
void Func_020049b0();
void Func_020049b6();
void Func_020049de();
void Func_020049e4();
void Func_020049f6();
void Func_020049fc();
void Func_02004a00();
void Func_02004a24();
void Func_02004a3a();
void Func_02004a5a();
void Func_02004a84();
void Func_02004a86();
void Func_02004a8e();
void Func_02004a90();
void Func_02004a9e();
void Func_02004aaa();
void Func_02004aac();
void Func_02004ab0();
void Func_02004ac2();
void Func_02004ac4();
void Func_02004ac6();
void Func_02004acc();
void Func_02004ad0();
void Func_02004ad2();
void Func_02004adc();
void Func_02004ae2();
void Func_02004aea();
void Func_02004af0();
void Func_02004b00();
void Func_02004b0c();
void Func_02004b18();
void Func_02004b18_a();
void Func_02004b1a();
void Func_02004b20();
void Func_02004b24();
void Func_02004b26();
void Func_02004b2e();
void Func_02004b32();
void Func_02004b3c();
void Func_02004b3e();
void Func_02004b4a();
void Func_02004b4a_a();
void Func_02004b58();
void Func_02004b5e();
void Func_02004b72();
void Func_02004b78();
void Func_02004b80();
void Func_02004b88();
void Func_02004b8e();
void Func_02004b9c();
void Func_02004b9c_a();
void Func_02004ba0();
void Func_02004ba2();
void Func_02004ba8();
void Func_02004baa();
void Func_02004bb8();
void Func_02004bbc();
void Func_02004bc4();
void Func_02004bc8();
void Func_02004bd4();
void Func_02004be2();
void Func_02004be2_a();
void Func_02004bee();
void Func_02004bee_a();
void Func_02004bf0();
void Func_02004bfe();
void Func_02004c04();
void Func_02004c0a();
void Func_02004c16();
void Func_02004c22();
void Func_02004c24();
void Func_02004c32();
void Func_02004c34();
void Func_02004c38();
void Func_02004c40();
void Func_02004c56();
void Func_02004c5c();
void Func_02004c60();
void Func_02004c64();
void Func_02004c7a();
void Func_02004c8a();
void Func_02004c9a();
void Func_02004c9c();
void Func_02004ca0();
void Func_02004ca2();
void Func_02004cb2();
void Func_02004cc6();
void Func_02004cd0();
void Func_02004cdc();
void Func_02004d04();
void Func_02004d10();
void Func_02004d18();
void Func_02004d22();
void Func_02004d4c();
void Func_02004d64();
void Func_02004d74();
void Func_02004d76();
void Func_02004d7e();
void Func_02004d84();
void Func_02004d8a();
void Func_02004d90();
void Func_02004d96();
void Func_02004d9e();
void Func_02004da2();
void Func_02004dac();
void Func_02004dae();
void Func_02004dba();
void Func_02004dba_a();
void Func_02004dc6();
void Func_02004dc8();
void Func_02004dcc();
void Func_02004dd2();
void Func_02004dda();
void Func_02004dec();
void Func_02004df4();
void Func_02004dfe();
void Func_02004e00();
void Func_02004e00_a();
void Func_02004e1c();
void Func_02004e1e();
void Func_02004e2a();
void Func_02004e2a_a();
void Func_02004e34();
void Func_02004e38();
void Func_02004e42();
void Func_02004e46();
void Func_02004e52();
void Func_02004e5c();
void Func_02004e5e();
void Func_02004e62();
void Func_02004e68();
void Func_02004e86();
void Func_02004e8c();
void Func_02004e9a();
void Func_02004ea6();
void Func_02004eb8();
void Func_02004ec2();
s32 Func_02004ed0();
void Func_02004ed0_a();
void Func_02004ed6();
void Func_02004eda();
void Func_02004edc();
void Func_02004ef0();
void Func_02004ef8();
void Func_02004efe();
s32 Func_02004f1a();
void Func_02004f1c();
void Func_02004f1c_a();
void Func_02004f20();
void Func_02004f2a();
void Func_02004f34();
void Func_02004f40();
void Func_02004f46();
void Func_02004f4a();
void Func_02004f4c();
void Func_02004f54();
void Func_02004f58();
void Func_02004f64();
void Func_02004f66();
void Func_02004f70();
void Func_02004f74();
void Func_02004f78();
void Func_02004f82();
void Func_02004f90();
void Func_02004f90_a();
void Func_02004f9e();
void Func_02004fa4();
void Func_02004fac();
void Func_02004fba();
void Func_02004fcc();
void Func_02004fce();
void Func_02004fd6();
void Func_02004fe8();
void Func_02004ff2();
s32 Func_02004ff6();
void Func_02004ffa();
void Func_02005000();
void Func_02005006();
void Func_02005008();
void Func_02005020();
void Func_02005026();
void Func_02005034();
void Func_02005034_a();
void Func_02005046();
void Func_02005050();
void Func_02005054();
s32 Func_02005064();
void Func_0200506a();
void Func_02005072();
void Func_02005084();
void Func_02005086();
void Func_0200508c();
void Func_0200509a();
void Func_020050a0();
void Func_020050a6();
void Func_020050ae();
void Func_020050b4();
void Func_020050b6();
void Func_020050c0();
void Func_020050c8();
void Func_020050ce();
void Func_020050dc();
void Func_020050ea();
void Func_020050fc();
void Func_020050fc_a();
void Func_02005100();
void Func_0200510a();
void Func_02005124();
void Func_02005128();
void Func_02005134();
void Func_0200513e();
void Func_0200513e_a();
void Func_02005142();
void Func_0200514e();
void Func_02005158();
void Func_0200515c();
void Func_0200517a();
void Func_02005180();
void Func_02005182();
void Func_02005184();
void Func_0200519c();
void Func_020051b4();
void Func_020051b6();
void Func_020051be();
void Func_020051c4();
void Func_020051c8();
void Func_020051cc();
s32 Func_020051ce();
void Func_020051d4();
void Func_020051dc();
void Func_020051e2();
void Func_020051e4();
void Func_020051e6();
void Func_020051ee();
void Func_020051f0();
void Func_020051f4();
s32 Func_020051fe();
void Func_02005208();
void Func_02005216();
void Func_02005220();
void Func_02005228();
s32 Func_0200522e();
void Func_02005236();
void Func_02005238();
void Func_02005246();
void Func_02005250();
void Func_02005258();
void Func_02005268();
void Func_02005276();
void Func_02005284();
void Func_02005288();
void Func_02005298();
void Func_02005302_a();
void Func_0200530e();
void Func_0200531a();
void Func_02005326();
void Func_02005332();
void Func_0200533e();
s32 Func_0200528c();
void Func_0200526c();
void Func_02005282();
s32 *Func_02005510();
void Func_020054cc();
struct Obj *Func_0200558e();
s32 Func_02004da4();
s32 Func_020056ba();
s32 Func_020056be();
void Func_02004e90();

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Copy the low two mode bits into the object's owner record. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call4_02000eac(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_020024d0(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1_02002548(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */
static __inline__ void ResetSceneParameters(s32 a, s32 b, s32 c, s32 mode)
{
    Func_0200408a(a, b, c, mode);
}

/* Scene calls are bound to loader-runtime addresses by the translation unit. */

/* The translation unit binds scene calls at the loader-runtime base. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001238(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_02001238(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2_02001238(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call3_02001238(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4_02001238(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001238(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 Func_02000030(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 *Func_0200006c(s32 *arg0)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void StagedActor_PushActorAhead(void)
{

    s32 dest[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *block;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dest[0] = lead->x.value + (step & 0xffff0000);
    dest[1] = lead->y;
    step <<= 16;
    dest[2] = lead->z.value + step;
    next = FindNextStagedActor(dest, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dest[0] = next->x.value + (step & 0xffff0000);
    dest[1] = next->y;
    step <<= 16;
    dest[2] = next->z.value + step;
    block = FindBlockingStagedActor(dest, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    dest[0] = next->x.value;
    dest[1] = next->y + 0x100000;
    dest[2] = next->z.value;
    block = FindElevatedBlockingStagedActor(dest, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dest[0] = next->x.value + (step & 0xffff0000);
    dest[1] = next->y;
    step <<= 16;
    dest[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dest) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dest[0], dest[1], dest[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dest[0], dest[1], dest[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dest[0];
    next->z.value = dest[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    SetStagedActorTransition(lead, 1);
}

s32 Func_02000244(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = (u8 *)Data_03001e70;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = Data_02010000;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}

s32 SceneActor_ResetMotionWhenAheadBlocked(Ent *a)
{
    extern s32 Data_0200afd4[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200afd4[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_02000342(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200b014;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200afd4[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_0200310a(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 StagedActor_FindClearPosition(s32 *a)
{
#include "find_clear_actor_position_body.inc"
}

void Func_02000608(
    StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}

void Func_020009dc(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_3c5.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and issue command
 * 15.  The three callees are declared at their in-overlay entry points, not at
 * veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */
u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_020037ae(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Func_0200381c(object, 0);
        Func_02003904(object, 14);
        Func_02003834(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

u8 *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_02003806(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 mask = ~12;
        u32 low;
        mask = mask & owner[9];
        owner[9] = (u8)(mask | 4);
        object[85] = 0;
        object[89] = 8;
        Func_02003878(object, 0);
        Func_02003960(object, 15);
        low = 0xfe;
        low = low & object[35];
        object[35] = (u8)(low | 2);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

void OverlayObject_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

void SceneEffect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
#include "spawn_configured_effect_body.inc"
}

void SceneState_SetValue8Mode66(void)
{
    Func_02003c00(8, 66);
}

void OverlayObject_WaitUntilIdle(s32 *obj)
{
    s32 i = 60;

    while (i != 0) {
        Func_02003a5e(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_02000cf0(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Func_02003a8a(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec4;
    s32 zero;
    s32 t;
    s32 nv;
    u8 *p8;
    u8 *p6;
    u8 slot28[40];
    u8 slot16[12];

    Func_02003b90(a0);
    Call4(Func_02003c8a, -1, -1, -1, 0);
    Func_02003b1e();
    Func_02003af4(1);
    *(volatile s32 *)(Func_02003bca(0) + 12) = 0x820000;
    *(volatile s32 *)(Func_02003bd6(0) + 72) = 0x8000;
    zero = 0;
    *(volatile s32 *)(Func_02003be2(0) + 68) = zero;
    *(u8 *)(Func_02003bec(0) + 85) = zero;
    Func_02003cf4();
    Func_02003d08();
    Func_02003bde(30);
    Func_02003d44(204);
    *(u8 *)(Func_02003c0a(0) + 85) = 3;
    Func_02003bf6(24);
    rec4 = Value1(Func_02003c1c, 0);
    p8 = slot28;
    *(s32 *)(p8 + 4) = 7;
    *(s32 *)(p8 + 36) = 0x2008cf1;
    *(s32 *)(p8 + 8) = 0xcccc;
    *(s32 *)(p8 + 12) = 0xcccc;
    i = 0;
    p6 = slot16;
    for (; i < 17; i++) {
        *(s32 *)(p6) = Func_02003b7c(i << 12);
        *(s32 *)(p6 + 4) = 0;
        t = Func_02003b80(i << 12);
        nv = *(s32 *)(p6);
        nv = nv + nv / 2;
        *(s32 *)(p6 + 8) = t;
        *(s32 *)(p6) = nv;
        Func_0200190c(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
              *(s32 *)(rec4 + 16), nv,
              *(s32 *)(p6 + 4), t, 0x1090001, (s32)p8);
    }
    Func_02003dbe(188);
    Call2(Func_02003d3e, 0, 0x101);
    Func_02003cce(0, 22);
    Call3(Func_02003c4e, 0x50000, 0x50000, 0x10000);
    Call3(Func_02003c5c, -1, -1, 0xe666);
    Func_02003c68();
    Call2(Func_02003d72, 0, 0x100);
    *(volatile s32 *)(Func_02003cc0(0) + 72) = 0x10000;
    *(volatile s32 *)(Func_02003ccc(0) + 68) = 0x4000;
    Func_02003cc6();
}

void FieldScene_RunScene3c5SequenceA(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;

    Func_02003cea();
    Call4_02000eac(Func_02003de4, -1, -1, -1, 0);
    Func_02003c78();
    Func_02003c4e_a(1);
    record = Func_02003d24(0);
    *(volatile s32 *)(record + 12) = 0x820000;
    record = Func_02003d30(0);
    *(volatile s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Func_02003d3c(0);
    *(volatile s32 *)(record + 68) = v5;
    *(u8 *)(Func_02003d46(0) + 85) = v5;
    record = Func_02003d50(0);
    Func_02003cf6(record, 0);
    Func_02003e5a();
    Func_02003e6e();
    Func_02003d44_a(10);
    Func_02003eaa(204);
    *(u8 *)(Func_02003d70(0) + 85) = 3;
    record = Func_02003d7c(0);
    *(volatile s32 *)(record + 40) = -0x50000;
    Func_02003d86(0);
    Func_02001c0a();
    Func_02003e0a(0, 15);
    Func_02003e78(a0);
    Func_02003d8c();
}

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Return this overlay's state block. */
s32 SceneData_SelectByRuntimeSelector(void)
{
    extern s16 Data_02000240[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

s32 SceneData_SelectTableB5b8ByState(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_000000b0) {
        return (s32)Data_0200b5b8;
    }
    return 0;
}

u8 *SceneData_GetTableb5f8(void) { return (u8 *)0x0200b5f8; }

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000fdc[];
    extern u8 SecondaryOverlayData_02000fdc[];
    extern u8 TertiaryOverlayData_02000fdc[];
    extern u8 DefaultOverlayData_02000fdc[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000fdc;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000fdc;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000fdc;
    }
    return (s32)DefaultOverlayData_02000fdc;
}

void FieldScene_RunFourCallSequence(void)
{
    Func_02003f30();
    Func_020011c0();
    Func_02002130();
    Func_02003f44();
}

void SceneState_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Func_02003f62(9);

    if ((Func_02003f6a(0)[2] >> 20) <= 12) {
        p = Func_02003f78(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                Func_0200223c();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                Func_0200223c();
                return;
            }
        }
    }
    Func_0200408e();
}

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    Func_0200402c(0, 6, 0);
    Func_020040d0();
    Func_020040dc();
}

void FieldScene_RunStep11(void)
{
    Func_02003fc0();
    Func_020022e4();
    Func_020040c2(11);
    Func_02003fd6();
}

void FieldScene_RunStep12WithPosition(void)
{
    Func_02003fd8();
    Func_02001ba4(0x1d00000, 0, 0x1220000, 223);
    Func_0200230c();
    Func_020040ea(12);
    Func_02003ffe();
}

void FieldScene_RunStep13WithTwoPositions(void)
{
    Func_02004000();
    Func_02001bcc(0x8f0000, 0, 0x1220000, 223);
    Func_02001bdc(0x790000, 0, 0x11e0000, 253);
    Func_02002344();
    Func_02004122(13);
    Func_02004036();
}

void FieldScene_RunStep15(void)
{
    Func_02004038();
    Func_0200235c();
    Func_0200413a(15);
    Func_0200404e();
}

void FieldScene_RunStepWithValue2693(void)
{
    Func_02004050();
    Func_020040b0(0, 1);
    Func_02004038_a(0x2693, 1);
    Func_0200406c();
}

void FieldScene_RunBranchingActorSequence(void)
{
    extern u8 Data_03001ebc[];

    s32 record;

    Call1_02001238(Func_0200405e, 0x988);
    Call1_02001238(Func_02004064, 0x98a);
    Func_02004080();
    Func_020041b4();
    Call1_02001238(Func_0200412a, 0x2702);
    Call3_02001238(Func_020040b8, 0, 0x10000, 0x8000);
    Call3_02001238(Func_020040de, 0, 0x128, 0x160);
    Call3_02001238(Func_0200416a, 0, 0xc000, 0);
    Func_020040b0_a(10);
    Call4_02001238(Func_020041fe, 10, 16, 0, 0xc000);
    Call4_02001238(Func_0200420e, 1, -8, 16, 0xc000);
    Call4_02001238(Func_0200421c, 2, 8, 16, 0xc000);
    Call4_02001238(Func_0200422a, 3, 24, 16, 0xc000);
    Func_02004140(3);
    Func_020040f6(20);
    Func_020041b6(14, 0);
    Func_02004104(10);
    Call2_02001238(Func_020041f8, 0x30000, 0x6000);
    Call4_02001238(Func_02004212, 0x1180000, -1, 0x1200000, 1);
    Func_0200421e();
    Func_0200412c(20);
    Func_0200419c(11, 3);
    Func_0200413a_a(30);
    Func_020041fa(11, 0);
    Func_02004148(10);
    Call3_02001238(Func_02004214, 14, 0xc000, 0);
    Call3_02001238(Func_02004220, 11, 0xc000, 0);
    Func_02004166(30);
    Func_0200416c(10);
    Func_020041f4(14, 2);
    Func_0200417a(20);
    Func_0200423a(14, 0);
    Func_02004188(10);
    Func_02004208(13, 2);
    Func_02004218(12, 2);
    Func_0200419e(40);
    Func_0200421e_a(13, 2);
    Func_0200422e(12, 2);
    Func_020041b4_a(40);
    Func_02004234(13, 2);
    Func_02004244(12, 2);
    Func_020041ca(40);
    Call3_02001238(Func_020042a6, 12, 0x102, 50);
    Func_02004296(12, 0);
    Func_020041e4(10);
    Func_020042ae(13, 0, 0);
    Func_020041f4_a(25);
    Func_0200427c(13, 2);
    Func_02004202(20);
    Func_020042c2(13, 0);
    Func_02004210(10);
    Call3_02001238(Func_020042dc, 12, 0x8000, 0);
    Func_02004222(20);
    Func_02004292(13, 4);
    Func_02004230(20);
    Func_020042f0(13, 0);
    Func_0200423e(10);
    Func_020042ae_a(11, 4);
    Func_0200424c(20);
    Func_0200430c(11, 0);
    Func_0200425a(20);
    Call3_02001238(Func_02004334, 13, 0x107, 40);
    Func_0200426a(10);
    Call3_02001238(Func_02004336, 13, 0x4000, 0);
    Func_0200427c_a(20);
    Func_0200433c(13, 0);
    Func_0200428a(20);
    Call3_02001238(Func_02004364, 13, 0x101, 75);
    Call3_02001238(Func_0200436e, 14, 0x101, 60);
    Call3_02001238(Func_0200436a, 12, 0x4000, 0);
    Func_020042b0(20);
    Call3_02001238(Func_0200437c, 11, 0x4000, 0);
    Func_020042c2_a(20);
    Call3_02001238(Func_0200438e, 14, 0x4000, 0);
    Func_020042d4(30);
    Func_02004394(14, 0);
    Call4_02001238(Func_020043de, 0x1180000, -1, 0x1400000, 1);
    Func_020043ea();
    Func_020042f8(20);
    Func_02004368(10, 3);
    Func_02004306(30);
    Func_020043c6(10, 0);
    Func_02004314(10);
    Func_02004384(10, 4);
    Func_02004322(20);
    Func_020043e2(10, 0);
    Func_02004330(20);
    Call3_02001238(Func_0200440a, 14, 0x105, 60);
    Func_020043fa(14, 0);
    Func_02004348(10);
    Func_020043b8(3, 4);
    Func_02004356(20);
    Func_02004416(3, 0);
    Func_02004364_a(10);
    Func_020043ec(2, 2);
    Func_02004372(20);
    Func_02004432(2, 0);
    Func_02004380(10);
    Func_02004408(1, 2);
    Func_0200438e_a(20);
    Value2(Func_02004446, 1, 0);
    if (Value2(Func_020043b6, 0, 0) == 0) {
        Func_020043a8(30);
        Call3_02001238(Func_02004474, 10, 0x8000, 0);
        Func_020043ba(30);
        Func_0200442a(10, 3);
        Func_020043c8(30);
        Func_02004488(10, 0);
        bump_step_02001238(1);
    } else {
        Func_02004404(30);
        Call3_02001238(Func_020044d0, 10, 0x8000, 0);
        Func_02004416_a(30);
        Func_02004486(10, 4);
        Func_02004424(20);
        bump_step_02001238(1);
        Func_020044f4(10, 0);
    }
    Func_02004442(10);
    Call3_02001238(Func_0200451c, 14, 0x101, 60);
    Call3_02001238(Func_02004482, 14, 0x10000, 0x8000);
    Func_020045b4(14, 0, 16);
    Func_0200446a(20);
    Func_0200452a(14, 0);
    Func_02004478(10);
    Call3_02001238(Func_02004544, 10, 0xc000, 0);
    Func_0200448a(35);
    Func_0200454a(10, 0);
    Func_02004498(10);
    Func_02004520(14, 2);
    Func_020044a6(20);
    Func_02004566(14, 0);
    Func_020044b4(20);
    Call3_02001238(Func_02004590, 3, 0x102, 40);
    Func_02004580(3, 0);
    Func_020044ce(10);
    Func_02004556(2, 2);
    Func_020044dc(20);
    Func_0200459c(2, 0);
    Func_020044ea(10);
    Func_0200455a(1, 3);
    Func_020044f8(30);
    Func_020045b8(1, 0);
    Func_02004506(20);
    Call3_02001238(Func_020045e2, 14, 0x100, 40);
    Value2(Func_020045ca, 14, 0);
    if (Value2(Func_0200453a, 0, 0) == 0) {
        Func_0200452c(30);
        Func_0200459c_a(14, 4);
        ((void (*)())Func_0200453a_a)(20);
        Func_020045fa(14, 0);
        bump_step_02001238(1);
    } else {
        Func_0200455a_a(30);
        ((void (*)())Func_020045ca_a)(14, 4);
        Func_02004568(20);
        bump_step_02001238(1);
        Func_02004638(14, 0);
    }
    Func_02004586(10);
    Call3_02001238(Func_02004662, 10, 0x102, 50);
    Func_02004652(10, 0);
    Func_020045a0(10);
    Func_02004628(13, 2);
    Func_020045ae(20);
    Call3_02001238(Func_020045e0, 13, 0x14ccc, 0xa666);
    Func_02004712(13, 0, 16);
    Func_020045c8(20);
    Func_02004688(13, 0);
    Func_020045d6(10);
    Call3_02001238(Func_02004608, 12, 0x14ccc, 0xa666);
    Func_0200473a(12, 0, 16);
    Func_020045f0(20);
    Call3_02001238(Func_020046ca, 12, 0x107, 50);
    Func_020046ba(12, 0);
    Func_02004608_a(10);
    Func_02004678(14, 3);
    Func_02004616(30);
    Func_0200461c(10);
    Call3_02001238(Func_020046f8, 10, 0x102, 60);
    Call3_02001238(Func_020046f4, 10, 0x8000, 0);
    Func_0200463a(25);
    Func_020046fa(10, 0);
    Func_02004648(10);
    Func_020046d0(10, 2);
    Func_02004656(20);
    Value2(Func_0200470e, 10, 0);
    if (Value2(Func_0200467e, 0, 0) == 0) {
        Func_02004670(30);
        Func_020046e0(10, 3);
        ((void (*)())Func_0200467e_a)(30);
        Func_0200473e(10, 0);
        bump_step_02001238(1);
    } else {
        Func_020046b4(30);
        Func_02004724(10, 4);
        Func_020046c2(20);
        bump_step_02001238(1);
        Func_02004792(10, 0);
    }
    Func_020046e0_a(10);
    Call3_02001238(Func_020047ac, 10, 0xc000, 0);
    Func_020046f2(35);
    Func_02004762(14, 3);
    Func_02004700(30);
    Call3_02001238(Func_020047cc, 14, 0xb000, 0);
    Func_02004712_a(40);
    Func_020047d2(14, 0);
    Func_02004720(10);
    Func_020047ba(12, 13, 50);
    Func_02004792_a(12, 3);
    Func_020047a2(13, 3);
    Func_02004740(30);
    Call3_02001238(Func_0200480c, 12, 0x4000, 0);
    Call3_02001238(Func_02004818, 13, 0x4000, 0);
    Func_0200475e(20);
    Func_020047c6(12, 3);
    Func_020047d6(13, 3);
    Func_02004774(30);
    Call3_02001238(Func_020047aa, 12, 0x10000, 0x8000);
    Call3_02001238(Func_020047b8, 13, 0x10000, 0x8000);
    Func_020048e2(12, 32, 0);
    Func_020048f4(13, 32, 0);
    Func_020048f6(12, 0, 16);
    Func_02004908(13, 16, 0);
    Call3_02001238(Func_020047fe, 13, 0x158, 0x138);
    Call3_02001238(Func_02004814, 12, 0x158, 0x150);
    Func_0200483c(13, 1);
    Call3_02001238(Func_020048a8, 12, 0x8000, 0);
    Call3_02001238(Func_020048b4, 13, 0x8000, 0);
    Func_020047fa(10);
    Call3_02001238(Func_020048c6, 14, 0x4000, 0);
    Func_0200480c_a(20);
    Func_020048cc(14, 0);
    Func_0200481a(10);
    Call3_02001238(Func_02004850, 14, 0x10000, 0x8000);
    Call3_02001238(Func_02004876, 14, 0x148, 0x138);
    Call3_02001238(Func_02004902, 14, 0x8000, 0);
    Func_02004848(30);
    Call3_02001238(Func_0200487e, 11, 0x10000, 0x8000);
    Call3_02001238(Func_020048a4, 11, 0x148, 0x148);
    Call3_02001238(Func_02004930, 11, 0x8000, 0);
    Func_02004876_a(20);
    Call3_02001238(Func_020048a8_a, 1, 0x13333, 0x9999);
    Call3_02001238(Func_020048b2, 2, 0x13333, 0x9999);
    Call3_02001238(Func_020048bc, 3, 0x13333, 0x9999);
    Func_020048fc(1, 2);
    record = Value1_02001238(Func_020048c2, 0);
    if (record != 0) {
        Func_020048e4(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200490a(1);
    Func_0200491c(1, 0, 0);
    Func_0200492c(2, 2);
    record = Value1_02001238(Func_020048f2, 0);
    if (record != 0) {
        Func_02004914(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200493a(2);
    Func_0200494c(2, 0, 0);
    Func_0200495c(3, 2);
    record = Value1_02001238(Func_02004922, 0);
    if (record != 0) {
        Func_02004944(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200496a(3);
    Func_0200497c(3, 0, 0);
    Func_0200492a(10);
    Func_0200493e();
}

void FieldScene_RunActorEventSequence(void)
{
    extern u8 Data_03001ebc[];

    s32 record;

    Call1_02001238(Func_02004936, 0x989);
    Func_02004952();
    Func_02004a86();
    Call1_02001238(Func_020049fc, 0x272f);
    Call3_02001238(Func_0200498a, 0, 0x10000, 0x8000);
    Call3_02001238(Func_020049b0, 0, 0x128, 0x138);
    Func_02004a3a(0, 0, 0);
    Func_02004980(10);
    Func_02004acc(1, 0, 16, 0);
    Call4_02001238(Func_02004adc, 2, -16, -8, 0);
    Call4_02001238(Func_02004aea, 3, -16, 24, 0);
    Func_02004a00(3);
    Func_020049b6(20);
    Call2_02001238(Func_02004aaa, 0x30000, 0x6000);
    Call4_02001238(Func_02004ac4, 0x1180000, -1, 0x1480000, 1);
    Func_02004ad0();
    Func_020049de(10);
    Func_020049e4(10);
    Call3_02001238(Func_02004ab0, 10, 0xb000, 0);
    Func_020049f6(10);
    Call3_02001238(Func_02004ad2, 10, 0x100, 40);
    Func_02004ac2(10, 0);
    Func_02004a84(10, 4, 13);
    Func_02004a8e(10, 4, 30);
    Func_02004a24(10);
    Call3_02001238(Func_02004b00, 11, 0x100, 0);
    Call3_02001238(Func_02004b0c, 12, 0x100, 0);
    Call3_02001238(Func_02004b18, 13, 0x100, 0);
    Call3_02001238(Func_02004b24, 14, 0x100, 40);
    Func_02004a5a(10);
    Call3_02001238(Func_02004b26, 14, 0xb000, 0);
    Call3_02001238(Func_02004b32, 11, 0xb000, 0);
    Call3_02001238(Func_02004b3e, 12, 0xb000, 0);
    Call3_02001238(Func_02004b4a, 13, 0xb000, 0);
    Func_02004a90(30);
    Func_02004b18_a(14, 2);
    Func_02004a9e(20);
    Func_02004b5e(14, 0);
    Func_02004aac(20);
    Call3_02001238(Func_02004b88, 13, 0x102, 40);
    Func_02004b78(13, 0);
    Func_02004ac6(20);
    Call3_02001238(Func_02004ba0, 12, 0x101, 50);
    Call3_02001238(Func_02004b9c, 12, 0x8000, 0);
    Func_02004ae2(25);
    Func_02004ba2(12, 0);
    Func_02004af0(10);
    Call3_02001238(Func_02004bbc, 14, 0x8000, 0);
    Call3_02001238(Func_02004bc8, 11, 0x8000, 0);
    Call3_02001238(Func_02004bd4, 13, 0x8000, 0);
    Func_02004b1a(30);
    Func_02004b20(10);
    Func_02004ba8(3, 2);
    Func_02004b2e(20);
    Func_02004bee(3, 0);
    Func_02004b3c(10);
    Func_02004bc4(2, 2);
    Func_02004b4a_a(20);
    Func_02004c0a(2, 0);
    Func_02004b58(10);
    Call3_02001238(Func_02004c34, 1, 0x102, 40);
    Func_02004c24(1, 0);
    Func_02004b72(20);
    Func_02004be2(10, 4);
    Func_02004b80(20);
    Func_02004c40(10, 0);
    Func_02004b8e(10);
    Func_02004c16(14, 2);
    Func_02004b9c_a(25);
    Func_02004c5c(14, 0);
    Func_02004baa(10);
    Func_02004c32(10, 2);
    Func_02004bb8(20);
    Call3_02001238(Func_02004bee_a, 10, 0x10000, 0x8000);
    Call3_02001238(Func_02004d22, 10, 0, -40);
    Func_02004c9c(10, 0, 0);
    Func_02004be2_a(20);
    Func_02004ca2(10, 0);
    Func_02004bf0(10);
    Func_02004c60(14, 3);
    Func_02004bfe(30);
    Func_02004c04(10);
    Call3_02001238(Func_02004cd0, 13, 0x4000, 0);
    Call3_02001238(Func_02004cdc, 12, 0xc000, 0);
    Func_02004c22(30);
    Func_02004c8a(12, 3);
    Func_02004c9a(13, 3);
    Func_02004c38(30);
    Call3_02001238(Func_02004d04, 13, 0x8000, 0);
    Call3_02001238(Func_02004d10, 12, 0x8000, 0);
    Func_02004c56(20);
    Func_02004cc6(10, 3);
    Func_02004c64(30);
    Func_02004d4c(10, 1);
    Call3_02001238(Func_02004dc8, 10, 0, -32);
    Func_02004394_a();
    Call1_02001238(Func_02004c7a, 0x301);
    Call4_02001238(Func_02004d84, -1, -1, -1, 0);
    Func_02004ca0();
    Call3_02001238(Func_02004d74, 10, 0x102, 40);
    Func_02004d64(10, 0);
    Func_02004cb2(10);
    Call3_02001238(Func_02004d7e, 14, 0xb000, 0);
    Call3_02001238(Func_02004d8a, 11, 0xb000, 0);
    Call3_02001238(Func_02004d96, 12, 0xb000, 0);
    Call3_02001238(Func_02004da2, 13, 0xb000, 0);
    Call3_02001238(Func_02004dae, 0, 0xc000, 0);
    Call3_02001238(Func_02004dba, 1, 0xc000, 0);
    Call3_02001238(Func_02004dc6, 2, 0xc000, 0);
    Call3_02001238(Func_02004dd2, 3, 0xc000, 0);
    Func_02004d18(30);
    Call3_02001238(Func_02004df4, 11, 0x100, 0);
    Call3_02001238(Func_02004e00, 12, 0x100, 0);
    Call3_02001238(Func_02004e1e, 13, 0x100, 0);
    Call3_02001238(Func_02004e2a, 14, 0x100, 70);
    Call4_02001238(Func_02004e5c, 0x1180000, -1, 0x1380000, 1);
    Func_02004e68();
    Func_02004d76(10);
    Call3_02001238(Func_02004e52, 12, 0x102, 40);
    Func_02004e42(12, 0);
    Func_02004d90(10);
    Func_02004e00_a(13, 4);
    Func_02004d9e(20);
    Func_02004e5e(13, 0);
    Func_02004dac(10);
    Func_02004e34(2, 2);
    Func_02004dba_a(30);
    Call3_02001238(Func_02004e86, 2, 0x2000, 0);
    Func_02004dcc(20);
    Func_02004e8c(2, 0);
    Func_02004dda(10);
    Call3_02001238(Func_02004ea6, 10, 0x4000, 0);
    Func_02004dec(30);
    Call3_02001238(Func_02004eb8, 3, 0xe000, 0);
    Func_02004dfe(30);
    Call3_02001238(Func_02004eda, 3, 0x102, 40);
    Call3_02001238(Func_02004ed6, 0, 0x3000, 0);
    Func_02004e1c(20);
    Func_02004edc(3, 0);
    Func_02004e2a_a(10);
    Func_02004e9a(1, 4);
    Func_02004e38(20);
    Func_02004ef8(1, 0);
    Func_02004e46(20);
    Call3_02001238(Func_02004f20, 14, 0x103, 50);
    Call3_02001238(Func_02004f1c, 14, 0x8000, 0);
    Func_02004e62(20);
    Value2(Func_02004f1a, 14, 0);
    Func_02004f34(0, 0, 0);
    Call3_02001238(Func_02004f40, 11, 0x8000, 0);
    Call3_02001238(Func_02004f4c, 12, 0x8000, 0);
    Call3_02001238(Func_02004f58, 13, 0x8000, 0);
    Call3_02001238(Func_02004f64, 2, 0x1000, 0);
    Call3_02001238(Func_02004f70, 3, 0xe000, 0);
    if (Value2(Func_02004ed0, 0, 0) == 0) {
        Func_02004ec2(30);
        Func_02004f4a(14, 2);
        Func_02004ed0_a(20);
        Func_02004f90(14, 0);
        bump_step_02001238(1);
    } else {
        Func_02004ef0(30);
        Func_02004f78(14, 2);
        Func_02004efe(20);
        bump_step_02001238(1);
        Func_02004fce(14, 0);
    }
    Func_02004f1c_a(10);
    Func_02004fa4(10, 2);
    Func_02004f2a(20);
    Func_02005084(10, 0, 16);
    Call3_02001238(Func_02005000, 10, 0x2000, 0);
    Func_02004f46(20);
    Func_02005006(10, 0);
    Func_02004f54(10);
    Call3_02001238(Func_02005020, 14, 0xa000, 0);
    Func_02004f66(20);
    Func_02004fd6(14, 4);
    Func_02004f74(20);
    Func_02005034(14, 0);
    Func_02004f82(10);
    Func_02004ff2(10, 3);
    Func_02004f90_a(30);
    Func_02005050(10, 0);
    Func_02004f9e(10);
    Func_02005026(14, 2);
    Func_02004fac(20);
    Value2(Func_02005064, 14, 0);
    Func_02004fba(40);
    Call3_02001238(Func_02005086, 10, 0x5000, 0);
    Func_02004fcc(20);
    Call3_02001238(Func_020050a6, 10, 0x101, 60);
    if (Value2(Func_02004ff6, 0, 0) == 0) {
        Func_02004fe8(30);
        Call3_02001238(Func_020050b4, 10, 0x2000, 0);
        Func_02004ffa(20);
        Func_0200506a(10, 3);
        Func_02005008(30);
        Func_020050c8(10, 0);
        bump_step_02001238(1);
    } else {
        Func_02005034_a(30);
        Call3_02001238(Func_02005100, 10, 0x2000, 0);
        Func_02005046(20);
        Func_020050b6(10, 4);
        Func_02005054(20);
        bump_step_02001238(1);
        Func_02005124(10, 0);
    }
    Func_02005072(10);
    Call3_02001238(Func_0200514e, 14, 0x102, 50);
    Func_0200513e(14, 0);
    Func_0200508c(15);
    Func_020050fc(10, 3);
    Func_0200509a(30);
    Func_020050a0(20);
    Func_02005128(14, 2);
    Func_020050ae(40);
    Call3_02001238(Func_0200517a, 14, 0x8000, 0);
    Func_020050c0(20);
    Func_02005180(14, 0);
    Func_020050ce(10);
    Func_0200513e_a(14, 3);
    Func_020050dc(30);
    Func_0200519c(14, 0);
    Func_020050ea(10);
    Call3_02001238(Func_020051b6, 10, 0x5000, 0);
    Func_020050fc_a(25);
    Func_02005184(10, 2);
    Func_0200510a(20);
    Call3_02001238(Func_02005158, 10, 0x138, 0x138);
    Func_020051e2(0, 0, 0);
    Call3_02001238(Func_020051ee, 10, 0x8000, 0);
    Func_02005134(25);
    Func_020051f4(10, 0);
    Func_02005142(10);
    Func_020051dc(0, 1, 0);
    Func_020051e6(3, 2, 0);
    Func_0200515c(30);
    Func_020051c4(0, 3);
    Func_020051cc(1, 3);
    Func_020051d4(3, 3);
    Func_020051e4(2, 3);
    Func_02005182(30);
    Call3_02001238(Func_020051b4, 1, 0x13333, 0x9999);
    Call3_02001238(Func_020051be, 2, 0x13333, 0x9999);
    Call3_02001238(Func_020051c8, 3, 0x13333, 0x9999);
    Func_02005208(1, 2);
    record = Value1_02001238(Func_020051ce, 0);
    if (record != 0) {
        Func_020051f0(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02005216(1);
    Func_02005228(1, 0, 0);
    Func_02005238(2, 2);
    record = Value1_02001238(Func_020051fe, 0);
    if (record != 0) {
        Func_02005220(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02005246(2);
    Func_02005258(2, 0, 0);
    Func_02005268(3, 2);
    record = Value1_02001238(Func_0200522e, 0);
    if (record != 0) {
        Func_02005250(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02005276(3);
    Func_02005288(3, 0, 0);
    Func_02005236(10);
    Call3_02001238(Func_02005302_a, 0, 0xc000, 0);
    Call3_02001238(Func_0200530e, 10, 0xb000, 0);
    Call3_02001238(Func_0200531a, 14, 0xb000, 0);
    Call3_02001238(Func_02005326, 11, 0xb000, 0);
    Call3_02001238(Func_02005332, 12, 0xb000, 0);
    Call3_02001238(Func_0200533e, 13, 0xb000, 0);
    Func_02005284(30);
    Func_02005298();
}

void ActorPresentation_SetSceneCellByFlag985(void)
{
    if (Func_0200528c(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Func_0200526c(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Func_02005282(34, 78, 1, 2, k5, k6);
    }
}

void SceneState_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Func_020052a6(32, 78, 1, 2, k5, k6);
    }
}

void FieldScene_RunScene3c5_020024d0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1_020024d0(Func_020052f0, 0x985) == 0) {
        Call1(Func_02005302, 0x985);
        Func_02005478(157);
        Func_02005324();
        Call3(Func_02005358, 8, 0x118, 240);
        Call3(Func_02005364, 9, 0x148, 240);
        Func_0200538a(8);
        Func_02005390(9);
        Call6(Func_020052fc, 81, 14, 4, 1, 17, 14);
        Func_02005368();
        if (Value1_020024d0(Func_02005346, 0x989) == 0) {
            Func_02004046();
        }
    }
}

void FieldScene_RunScene3c5_02002548(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1_02002548(Func_02005368_a, 0x985) != 0) {
        Call1(Func_02005382, 0x985);
        Func_020054f0(157);
        Func_0200539c();
        Call3(Func_020053d0, 8, 0x128, 240);
        Call3(Func_020053dc, 9, 0x138, 240);
        Func_02005402(8);
        Func_02005408(9);
        Call6(Func_02005374, 0, 14, 4, 1, 17, 14);
        Func_020053e0();
        if (Value1_02002548(Func_020053be, 0x301) != 0) {
            Call1(Func_020053d8, 0x301);
        } else {
            Call1(Func_020053d8_a, 0x301);
        }
    }
}

void FieldScene_RunFlag985DialogueBranch(void)
{
    extern u8 *Data_03001ebc;

    u8 *base = Data_03001ebc;
    s16 *h;

    Func_0200540a();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (Func_020053fc(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Func_02005400(0x1528, 1);
            Func_02005586(155);
            Func_020053ea(35, 78, 1, 2, k5, k6);
            Func_02005440(10);
            Func_02005400_a(34, 78, 1, 2, k5, k6);
            Func_02005456(10);
            Func_02004afa();
        }
    } else {
        Func_0200550a(0x2756);
        Func_02005524(-1, 0);
    }
    Func_02005480();
}

void SceneState_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x34) = 1;
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Func_02005510(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Func_020054cc(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

void SceneActor_PushObjectAheadIfLevel(void)
{
    extern s32 Data_0200afd4[];

    struct LevelCheckRecord *p = Func_02005552(0);
    struct LevelCheckRecord *q = Func_02004d68(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    Func_020027f2();
}

void SceneActor_RunSlotZeroFacingCheck(void)
{
    extern u8 SceneStateBytes[];

    struct Obj *p = Func_0200558e(0);
    s32 x = Func_02004da4();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (SceneStateBytes[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = Func_020056ba();
        }
        if (m == 0x4000) {
            r = Func_020056be();
        }
    }
    if (r != 0) {
        if (SceneStateBytes[498] != 1) {
            Func_02004e90();
        }
    }
}

s32 SceneData_SelectTableB91cByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_020027a0[];
    extern u8 SecondaryOverlayData_020027a0[];
    extern u8 TertiaryOverlayData_020027a0[];
    extern u8 DefaultOverlayData_020027a0[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_020027a0;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_020027a0;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_020027a0;
    }
    return (s32)DefaultOverlayData_020027a0;
}

void SceneState_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Func_020055e2(82, 7, 1, 2, a, b);
    Func_02005590(1);
    Func_02005638_a(768);
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Func_02005678(8);

    Func_0200572a(8, 1);
    Func_02005732(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Func_02005626(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Func_02005638(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Func_02005650(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Func_020056ce(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Func_0200566e(3, 3, 1, 1, k5, k6);
    }
}
