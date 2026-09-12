#include "types.h"

#define SetEffectRecordMode Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define AcquireOverlayObject      Func_020010da
#define RunOverlayObjectCommand0  Func_02001120
#define RunOverlayObjectCommand1  Func_02001138
#define RunOverlayObjectCommand14 Func_020011e8
#define CreateOverlayObject Func_02001132
#define SetOverlayObjectMode Func_0200117c
#define SetOverlayObjectSlot Func_02001244
#define OverlayObject_CreateConfigured Func_02000048
#define OverlayObject_CreateConfiguredObjectB Func_020000a0
#define SceneEffect_UpdateMotion Func_02000104
#define EffectDescriptorTable Data_0200929c
#define GetPartyEffect Func_0200125e
#define SpawnEffect Func_02001204
#define SetEffectVariant Func_0200120e
#define SetEffectDescriptor Func_02001228
#define SetEffectMode Func_0200137a
#define ScaleEffectDeltaFromAccumulated Func_020012d4
#define ScaleEffectDeltaFromOrigin Func_020012ec
#define ScaleEffectVerticalDelta Func_020012fa
#define SetEffectCallbackMode Func_02001328
#define SetEffectCallbackArgument Func_02001338
#define SceneDialogue_RunActorFifteenDialogue Func_02000864
#define SceneDialogue_RunActorFourteenFlagDialogue Func_0200058c
#define CalculateFixedPointPositionDistance Func_02000314
#define SceneActor_UpdateProximity Func_02000350
#define Scene_GetPartnerActor Func_02001532
#define Actor_CheckProximity Func_0200078e
#define Scene_GetPlayerActor Func_02001548
#define Actor_UpdateProximity Func_020007ce
#define UpdateActorProximity Func_02000400
#define PlaceActorAndSetSceneDelay Func_02000a80
#define SceneData_GetTable9450 Func_02000498
#define SceneData_ReturnZero Func_020004a0
#define SceneData_GetTable95A0 Func_020004a4
#define SceneData_GetTable95D8 Func_020004ac
#define SceneData_GetTable97dc Func_02000f28
#define ActorPresentation_SetupActorSceneCells Func_02000c1c
#define ActorPresentation_RunActorModeOneThenZero Func_020004b4
#define ActorPresentation_RunActor13AcceptanceDialogue Func_020007d4
#define ActorPresentation_SetupActorEighteenAt312_304 Func_02000af0
#define ActorPresentation_SetupActorZeroForSceneEight Func_02000b40
#define ActorPresentation_SetupActorZeroForSceneNine Func_02000b9c
#define ActorPresentation_SetupActorZeroForSceneTwelve Func_02000cc8
#define ActorPresentation_SetFlag200AndSceneCell23 Func_02000d4c
#define ActorPresentation_SetPairedSceneCells Func_02000e30
#define ActorPresentation_SetAlternatePairedSceneCells Func_02000e60
#define FieldScene_RunActorEighteenDialogue Func_02000e90
#define SceneDialogue_RunActor8FlagScene Func_020004d8
#define SceneDialogue_RunActor11Line Func_0200050c
#define SceneDialogue_RunActor12TwoFlagScene Func_0200052c
#define SceneDialogue_ShowLine1CB0ForActor13 Func_0200056c
#define SceneDialogue_RunActor16CountedDialogue Func_020006b4
#define SceneDialogue_RunActorEightTimedDialogue Func_02000710
#define SceneDialogue_RunActor11AcceptanceDialogue Func_02000754
#define SceneDialogue_RunActor12TimedTwoFlagScene Func_02000784
#define SceneDialogue_RunActorFifteenFacingPreservedDialogue Func_02000640
#define FieldScene_RunSupplementalSequenceOne Func_02000804
#define FieldScene_RunScene385_020009f8 Func_020009f8
#define FieldScene_SetActor21Values0And4 Func_02000d74
#define FieldScene_RunScene385_02000d84 Func_02000d84
#define FieldScene_SetupDescriptor9740 Func_02000ac8
#define FieldScene_SetupWithDescriptor976C Func_02000b18
#define FieldScene_SetupWithDescriptor97AE Func_02000bf8
#define SceneState_SetFlag200AndConfigureRegion55_26 Func_02000d24
#define SceneState_SetFlag947AndValue29dc Func_02000f00
#define SpawnConfiguredEffect Func_0200013c
#define FieldScene_RunScene385SequenceA Func_020008a8
#define SceneSetup_InitializeActorsAndFlags Func_02000f30

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

union MotionWork {
  struct {
    u32 unk_00[2];
    s32 x, y, z;
    u32 unk_14;
    s32 accum_x, accum_y;
    u32 unk_20[4];
    s32 rate_x, rate_y;
    u32 unk_38[3];
    s32 velocity_x, velocity_y, velocity_z;
    u16 *record;
    u8 unk_54[16];
    u16 angle_step;
  } fields;
  u8 bytes[102];
};

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

struct SceneActor_02000350 {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_020004b4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct Presentation_02000c1c {
    u8 unk_00[9];
    u8 flags;
};

struct SceneActor_02000c1c {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation_02000c1c *presentation;
};

/*
 * Complete actor-18 dialogue/restoration scene.  If cue 231 remains available
 * and its movement scene has not set flag 0x858, the shared scene marker at
 * +370 is enabled before the dialogue scene closes.
 */
struct SceneWork_02000e90 {
    u8 reserved000[370];
    u16 actor18_marker;
};

/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern u8 SceneMessage_ActorFourteenBase;
extern const unsigned char Data_02009450[];
extern const unsigned char Data_020095a0[];
extern const unsigned char Data_020095d8[];
extern const u8 Data_020097dc[];
extern void Func_02001942(s32 dialogue);                 /* Func_0808a170 */
extern struct Actor_020007d4 *Func_020018e0(s32 actor);  /* Scene_GetRecord */
extern void Func_02000ca0(s32 actor);                    /* Func_020004b4 */
extern struct Actor_020007d4 *Func_020018f2(s32 actor);  /* Scene_GetRecord */
extern const u8 Data_02009756[];
extern void Func_020018c2_a(s32 dialogue);                 /* Func_0808a170 */
extern struct Actor_02000754 *Func_02001860(s32 actor);  /* Scene_GetRecord */
extern void Func_02000c20(s32 actor);                    /* Func_020004b4 */
extern struct Actor_02000754 *Func_02001872(s32 actor);  /* Scene_GetRecord */
extern u8 Data_00000002[];
extern u8 Data_00001cc0[];
extern const u8 Data_02009740[];
extern const u8 Data_0200976c[];
extern const u8 Data_020097ae[];
extern s16 SceneState_Table[];

void *Func_020010da(s32, s32, s32, s32);
void Func_02001120(void *, s32);
void Func_02001138(void *, s32);
void Func_020011e8(void *, s32);
struct SceneActor *Func_02001696(s32);
void Func_02001696_a(void);
void Func_0200171e(s32);
void Func_020016f6(s32, s32);
void Func_02001720(s32, s32, s32);
s32 Func_02001690(s32);
void Func_0200177a(s32, s32, s32);
void Func_02001774(s32, s32, s32);
void Func_0200177e(s32, s32, s32);
void Func_020016ce(s32);
void Func_02001774_a(s32);
void Func_02001796(s32, s32, s32);
void Func_02001680(s32);
void Func_02001714(void);
void Func_020016f6_a(s32);
struct SceneActor *Func_0200196a(s32);
void Func_02001960(void);
void Func_020019e6(s32);
void Func_02000d38(s32);
void Func_02001978(void);
struct SceneActor *Func_0200198e(s32);
void Func_02000e04();
s32 Func_02001970();
void Func_020019a2();
void Func_020019bc();
s32 Func_020019c0();
s32 Func_020019f4();
void Func_020019fe();
void Func_02001a06();
s32 Func_02001a1e();
s32 Func_02001a1e_a();
void Func_02001a1e_b();
void Func_02001a3a();
s32 Func_02001a44();
s32 Func_02001a44_a();
void Func_02001a4a();
s32 Func_02001a56();
s32 Func_02001a64();
void Func_02001a90();
void Func_02001aa6();
typedef s32(*IwramIntegerSquareRoot)(s32);
s32 Func_0200068c(s32 *, s32 *);
u32 Func_020013fc(s32, s32);
void Func_02001442(struct SceneActor *, s32);
void Func_02001458(struct SceneActor *, s32);
u8 *Func_02001532();
s32 Func_0200078e();
u8 *Func_02001548();
void Func_020007ce();
u8 *Func_02001a82(int);
void Func_02001a9a(int, int, int);
void Func_02001a48(u8 *, int);
void Func_02001aca(int, int, int);
void Func_02001af4(int, int, int);
void Func_02001a34(int);
void Func_02001a84(u8 *, int);
void Func_02001ba0(s32, s32, s32);
void Func_02001bc2(s32, s32, s32);
void Func_02001c56(s32);
void Func_020015a2(void);
void Func_020015fa(s32 actor, s32 mode);
void Func_02001642(s32 actor, s32 mode);
void Func_020015be(void);
void Func_02001ca6(s32 value);
void Func_02001b88(const void *tbl, s32 value, s32 actor);
void Func_0200158e(s32 x, s32 y, s32 value);
void Func_0200160e(s32, s32, s32);
void Func_02001be4(const void *, s32, s32);
void Func_02001c0a(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001c48(s32);
void Func_02001d02(s32);
void Func_0200166a(s32, s32, s32);
void Func_02001c40(const void *, s32, s32);
void Func_02001c66(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001ca4(s32);
void Func_02001d5e(s32);
void Func_02001794(s32, s32, s32);
void Func_02001d6c(const void *, s32, s32);
void Func_02001d92(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001dd0(s32);
void Func_02001e8a(s32);
struct SceneActor *Func_02001d28(s32);
void Func_02001de4(s32);
void Func_02001cd6(s32, s32, s32, s32, s32, s32);
void Func_02001ce6(s32, s32, s32, s32, s32, s32);
void Func_02001d3c(s32);
void Func_02001cfc(s32, s32, s32, s32, s32, s32);
void Func_02001d0c(s32, s32, s32, s32, s32, s32);
void Func_02001d62(s32);
void Func_02001d30(s32, s32, s32, s32, s32, s32);
void Func_02001738(s32, s32, s32);
void Func_02001e26(s32 flag);
void Func_02001ede(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 v1, s32 v2);
void Func_02001f0e(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 v1, s32 v2);
void Func_02001f7c(void);
void Func_02002002(s32 dialogue);
void Func_02001fda(s32 actor, s32 mode);
void Func_02002004(s32 actor, s32 mode, s32 value);
void Func_02001f92(s32 frames);
void Func_02002032(s32 actor, s32 mode);
void Func_02001ffa(s32 actor, s32 mode);
s32 Func_02001f80(s32 cue);
s32 Func_02001f96(s32 flag);
void Func_02001fdc(void);
void Func_020015c4(void);
void Func_02001636(s32 actor, s32 mode, s32 value);
void Func_020015b4(s32 flag);
void Func_0200165a(s32 dialogue);
void Func_02001672(s32 actor, s32 mode);
void Func_020015ee(void);
void Func_02001674(s32 actor, s32 mode, s32 value);
void Func_020009d6(s32 actor);
void Func_0200168a(s32 actor, s32 mode, s32 value);
void Func_02001608(s32 flag);
void Func_0200160e_a(s32 flag);
void Func_020016b4(s32 dialogue);
void Func_020016cc(s32 actor, s32 mode);
void Func_02001648(void);
void Func_020016d4(s32 actor, s32 mode, s32 value);
void Func_02000a36(s32 actor);
void Func_020017a0(void);
void Func_02001826(s32 dialogue);
void Func_02001820(s32 actor, s32 mode, s32 value);
s32 Func_02001840(s32 actor, s32 mode);
s32 Func_020017d0(s32 actor, s32 mode);
void Func_0200186c(s32 actor, s32 mode);
void Func_020017c4(s32 flag);
void Func_020017f0(void);
void Func_020017fc(void);
void Func_02001864(s32 actor, s32 mode);
void Func_02001802(s32 frames);
void Func_0200187c(s32 actor, s32 mode, s32 frames);
void Func_020017fa(s32 flag);
void Func_020018a0(s32 dialogue);
void Func_020018c2(s32 actor, s32 mode, s32 frames);
void Func_02001836(void);
void Func_02001870(void);
void Func_020018d8(s32 actor, s32 mode);
void Func_02001876(s32 frames);
void Func_020018f0(s32 actor, s32 mode, s32 frames);
void Func_0200186e(s32 flag);
void Func_02001874(s32 flag);
void Func_0200191a(s32 dialogue);
void Func_0200193c(s32 actor, s32 mode, s32 frames);
void Func_020018b0(void);
struct Actor_02000640 *Func_0200174c(s32 actor);
void Func_02001750(void);
void Func_020017d6(s32 dialogue);
void Func_020017ae(s32 actor, s32 mode);
void Func_020017d8(s32 actor, s32 mode, s32 value);
void Func_0200180a(s32 actor, s32 mode, s32 frames);
void Func_020016fe(s32 frames);
void Func_02001792(void);
void Func_02000ce2();
void Func_02000dce();
s32 Func_020018de();
void Func_02001900();
s32 Func_0200190a();
void Func_02001910();
void Func_0200193c_a();
u8 *Func_02001952();
void Func_02001990();
void Func_020013bc();
s32 Func_02001ace();
void Func_02001ae4();
void Func_02001ae8();
void Func_02001b0c();
void Func_02001b1c();
void Func_02001b32();
void Func_02001b38();
void Func_02001b64();
void Func_02001b66();
void Func_02001b76();
void Func_02001b86();
void Func_02001b92();
void Func_02001bb2();
void Func_02001bb4();
void Func_02001f26(s32 actor, s32 mode, s32 value);
void Func_02001e62();
void Func_02001e74();
void Func_02001e76();
void Func_02001e98();
void Func_02001eae();
void Func_02001eb4();
void Func_02001ec4();
void Func_02001ed4();
void Func_02001eda();
void Func_02001ee0();
void Func_02001ee4();
void Func_02001ee6();
void Func_02001f0c();
void Func_02001f0e_a();
void Func_02001f1e();
void Func_02001f28();
void Func_02001f2e();
void Func_02001f4c();
void Func_02001f9a();
void Func_02001c7e(s32 value);
void Func_02001b60(const void *tbl, s32 value, s32 actor);
void Func_02001566(s32 x, s32 y, s32 value);
void Func_02001cce(s32 value);
void Func_02001bb0(const void *descriptor, s32 value, s32 actor);
void Func_020015b4_a(s32 x, s32 y, s32 value);
void Func_02001dae(s32 value);
void Func_02001c90(const void *descriptor, s32 value, s32 actor);
void Func_02001692(s32 x, s32 y, s32 value);
void Func_02001df6(s32 flag);
void Func_02001fec(void);
void Func_02001fbc(s32 resource, s32 enabled);
void Func_02001fc4(s32 resource, s32 enabled);
void Func_02002008(void);
s32 SceneFlag_Check(s32 flag);
void SceneFlag_Set(s32 flag);
void SceneMap_UpdateRect(s32, s32, s32, s32, s32, s32);
void SceneMap_CopyRect(s32, s32, s32, s32, s32, s32);
void SceneState_SetMode(s32 mode);
u8 *SceneActor_Find(s32 actor);
void SceneActor_Place(s32 actor, s32 x, s32 z);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

/* The four declarations name this overlay's import veneers. */

/* Complete table-address getter through its sole return and literal pool. */

/* Complete overlay-local table-address getter through its literal pool. */

/* Complete in-image scene-descriptor getter through its literal pool. */

/*
 * Every call is named at its decoded bl site: the two Scene_GetRecord reads go
 * through two DIFFERENT per-site import veneers, and the dialogue and
 * Func_020004b4 calls also resolve to their own sites, so the semantic
 * single-name imports cannot reproduce the reference displacements.
 */

/* Complete scene-variant six actor-zero setup wrapper through its pool. */

/* Complete actor-mode scene wrapper through its return and alignment. */

/* Complete paired scene-cell presentation update. */

/* Complete alternate paired scene-cell presentation update. */

/*
 * Both calls are named at their decoded bl sites, 0x02001f0e and 0x02001f18:
 * the semantic targets are the main-image pair Func_080091c0/Func_08009180,
 * but a direct bl cannot encode that displacement, and only the RAM-resident
 * sites reproduce the reference bytes.  The reference also materializes both
 * stacked constants before either store, which named locals preserve.
 */

/* Complete actor-8 flag-setting dialogue scene through its two-word pool. */

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

/* Complete actor-8 timed flag/dialogue scene through its two-word pool. */

/* Complete actor-12 timed two-flag dialogue scene through its pool. */

/* Complete scene-variant five actor-zero setup wrapper through its pool. */

/* Complete scene-variant seven actor-zero setup wrapper through its pool. */

/* Complete scene-variant ten actor-zero setup wrapper through its pool. */

/* Complete paired scene-resource enable wrapper through its two-word pool. */

static __inline__ s32 Scene_QueryFlag(s32 (*func)(s32), s32 flag)
{
    extern u8 Data_03001ebc[];

    return func(flag);
}

static __inline__ void Scene_SetFlag(void (*func)(s32), s32 flag)
{
    extern u8 Data_03001ebc[];

    func(flag);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    extern u8 Data_03001ebc[];

    func(a, b, c);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call0(void (*f)())
{
    extern u8 Data_03001ebc[];

    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    extern u8 *Data_03001ebc;

    Func_02001ba0(actor, horizontal, vertical);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * Complete actor-21 mode-four reset wrapper.
 *
 * Call symbol resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(`push {lr} / movs r0,#21 / movs r1,#0 /
 * movs r2,#4 / bl sub_02001f26 / pop {r0} / bx r0`): the per-site overlay
 * veneer is sub_02001f26, not a main-image address guessed from elsewhere.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000640(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
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
static __inline__ void Call1_02000d74(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void PlaceActor(void (*place)(s32, s32, s32),
                                 s32 actor, s32 x, s32 z)
{
    void *Func_020000a0(s32, s32, s32, s32);

    place(actor, x, z);
}

static __inline__ void UpdateRect(void (*update)(s32, s32, s32, s32, s32, s32),
                                 s32 x, s32 z, s32 width, s32 height,
                                 s32 sourceX, s32 sourceZ)
{
    void *Func_020000a0(s32, s32, s32, s32);

    update(x, z, width, height, sourceX, sourceZ);
}

void Func_02001e02(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);

void Func_02001ee8(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);

void Func_02001f18(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);

void Func_02001dda(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 a, s32 b);

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObjectB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void SceneEffect_UpdateMotion(union MotionWork *work)
{
    extern u8 Data_03001ebc[];

    u16 *record;
    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
    extern u8 Data_03001ebc[];

#include "configured_effect_spawn_body.inc"
}

s32 Func_02000314(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_UpdateProximity(struct SceneActor_02000350 *actor, struct SceneActor_02000350 *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (Func_0200068c(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)Func_020013fc(target->z - actor->z,
                                      *targetPos - *actorPos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Func_02001442(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Func_02001458(actor, 2);
    }
    return result;
}

s32 UpdateActorProximity(u8 *actor)
{
    extern u8 *Data_03001ebc;

    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];
    u16 *flags = (u16 *)(actor + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    if ((*flags & 1) != 0) {
        partner = Scene_GetPartnerActor(15);
    } else {
        partner = Scene_GetPartnerActor(14);
    }
    if (Actor_CheckProximity(actor, partner, 32, 0) != 0) {
        return 0;
    }

    player = Scene_GetPlayerActor(0);

    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Actor_UpdateProximity(actor, player, range, force);
    return 0;
}

const void *SceneData_GetTable9450(void)
{
    return Data_02009450;
}

/* Complete zero-return leaf; no calls and no argument read. */
int SceneData_ReturnZero(void)
{
    return 0;
}

const void *SceneData_GetTable95A0(void)
{
    return Data_020095a0;
}

const void *SceneData_GetTable95D8(void)
{
    return Data_020095d8;
}

void ActorPresentation_RunActorModeOneThenZero(s32 actor)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    Func_020015a2();
    Func_020015fa(actor, 1);
    Func_02001642(actor, 0);
    Func_020015be();
}

void SceneDialogue_RunActor8FlagScene(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    Func_020015c4();
    Func_02001636(8, 0, 2);
    Func_020015b4(0x305);
    Func_0200165a(0x1cab);
    Func_02001672(8, 0);
    Func_020015ee();
}

void SceneDialogue_RunActor11Line(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    /*
 * Complete actor-11 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 753-765, still unadopted at the
 * time of writing): `bl sub_0200167a`, `bl sub_02001674`, then
 * `bl .L_020009d6` -- the third call targets a local branch label at
 * 0x020009d6, not the guessed 0x020004b4.
 */
void Func_0200167a(s32 dialogue);

    Func_0200167a(0x1cae);
    Func_02001674(11, 0, 2);
    Func_020009d6(11);
}

void SceneDialogue_RunActor12TwoFlagScene(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    /*
 * Complete actor-12 two-flag dialogue scene through its three-word pool.
 *
 * Call symbols are per-site, not per-import: the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 766-783, still unadopted) spells
 * every call as `bl sub_02000xxx`, and the two calls that both ultimately
 * reach GameFlag_Set go through TWO DIFFERENT overlay-local veneers,
 * sub_02001608 and sub_0200160e -- not the same symbol twice.
 */
void Func_02001618(void);

    Func_02001618();
    Func_0200168a(12, 0, 2);
    Func_02001608(0x306);
    Func_0200160e_a(0x868);
    Func_020016b4(0x1caf);
    Func_020016cc(12, 0);
    Func_02001648();
}

void SceneDialogue_ShowLine1CB0ForActor13(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    /*
 * Complete actor-13 dialogue wrapper through its one-word pool.
 *
 * Call symbols resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(lines 757-769, still unadopted at the
 * time of writing): `bl sub_020016da`, `bl sub_020016d4`, `bl sub_02000a36`.
 */
void Func_020016da(s32 dialogue);

    Func_020016da(0x1cb0);
    Func_020016d4(13, 0, 2);
    Func_02000a36(13);
}

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
    extern u8 Data_03001ebc[];

    struct SceneActor *actor = Func_02001696(14);
    s16 facing = (s16)actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Func_02001696_a();
    text = (s32)&SceneMessage_ActorFourteenBase;
    Func_0200171e(text);
    Func_020016f6(14, 0);
    Func_02001720(14, 0, 2);
    if (Scene_QueryFlag(Func_02001690, 0x300) == 0) {
        Scene_Call3(Func_0200177a, 14, 256, 60);
        Func_02001774(14, 0, 10);
        Func_0200177e(14, 0, 10);
        Scene_SetFlag(Func_020016ce, 0x300);
    }
    Func_02001774_a(text + 2);
    Func_02001796(14, 0, 10);
    actor->facing = facing;
    Func_02001680(1);
    Func_02001714();
    {
        s32 shown = 1;
        actor->state_flags = shown;
    }
    Func_020016f6_a(0x307);
}

void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void)
{
    extern u8 Data_03001ebc[];

    struct Actor_02000640 *actor;
    s16 facing0;

    actor = Func_0200174c(15);
    facing0 = (s16)actor->facing;
    actor->state_flags |= 2;
    Func_02001750();
    Func_020017d6(0x1cb4);
    Func_020017ae(15, 0);
    Func_020017d8(15, 0, 2);
    Func_0200180a(15, 0, 10);
    actor->facing = (u16)facing0;
    Func_020016fe(1);
    Func_02001792();
    actor->state_flags = 0;
}

void SceneDialogue_RunActor16CountedDialogue(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    Func_020017a0();
    Func_02001826(0x1cb5);
    Func_02001820(16, 0, 2);
    Func_02001840(16, 0);
    if (Func_020017d0(0, 0) != 0) {
        Data_03001ebc->branch_counter += 1;
    }
    Func_0200186c(16, 0);
    Func_020017c4(0x308);
    Func_020017f0();
}

void SceneDialogue_RunActorEightTimedDialogue(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    Func_020017fc();
    Func_02001864(8, 1);
    Func_02001802(20);
    Func_0200187c(8, 0, 20);
    Func_020017fa(0x305);
    Func_020018a0(0x1cab);
    Func_020018c2(8, 0, 20);
    Func_02001836();
}

void SceneDialogue_RunActor11AcceptanceDialogue(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    Func_020018c2_a(0x1cbd);
    Func_02001860(11)->accepted = 1;
    Func_02000c20(11);
    Func_02001872(11)->accepted = 0;
}

void SceneDialogue_RunActor12TimedTwoFlagScene(void)
{
    extern struct SceneWork_020006b4 *Data_03001ebc;

    Func_02001870();
    Func_020018d8(12, 1);
    Func_02001876(20);
    Func_020018f0(12, 0, 20);
    Func_0200186e(0x306);
    Func_02001874(0x868);
    Func_0200191a(0x1caf);
    Func_0200193c(12, 0, 20);
    Func_020018b0();
}

void ActorPresentation_RunActor13AcceptanceDialogue(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    Func_02001942(0x1cbf);
    Func_020018e0(13)->accepted = 1;
    Func_02000ca0(13);
    Func_020018f2(13)->accepted = 0;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;

    {
        u16 *flags = (u16 *)(Func_0200190a(14) + 100);
        u16 bits = (u16)(s32)Data_00000002;

        *flags |= bits;
    }
    Func_02001900();
    if (Value1(Func_020018de, 0x307) != 0) {
        Func_02001990((s32)Data_00001cc0);
        Func_02000ce2(14);
    } else {
        Func_02000dce();
        Call1_02000640(Func_02001910, 0x307);
    }
    Func_0200193c_a();
    {
        u8 *record = Func_02001952(14);
        s32 shown = 1;

        *(volatile u16 *)((s32)record + 100) = shown;
    }
}

void SceneDialogue_RunActorFifteenDialogue(void)
{
    extern u8 Data_03001ebc[];

    {
        struct SceneActor *actor = Func_0200196a(15);
        actor->state_flags |= 2;
    }
    Func_02001960();
    Func_020019e6(0x1cc1);
    Func_02000d38(15);
    Func_02001978();
    {
        s32 clear = 0;
        struct SceneActor *actor = Func_0200198e(15);
        actor->state_flags = clear;
    }
}

void FieldScene_RunScene385SequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    rec7 = Value1(Func_02001970, 0x308);
    if (rec7 == 0) {
        Func_020019a2();
        *(u8 *)(Func_020019c0(16) + 91) = 1;
        Func_02001a06(16, 1);
        Call2((void (*)())Func_02001a1e, 16, 1);
        Func_020019bc(20);
        Call1(Func_02001a4a, 0x1cb5);
        Call3((void (*)())Func_02001a44, 16, 0, 2);
        Value2(Func_02001a64, 16, 0);
        if (Value2(Func_020019f4, 0, 0) != 0) {
            bump_step(1);
        }
        Func_02001a90(16, 0);
        *(u8 *)(Func_02001a1e_a(16) + 91) = rec7;
        Func_02001a3a(16, 2);
        Call0(Func_02001a1e_b);
        Call1(Func_020019fe, 0x308);
    } else {
        Call1(Func_02001aa6, 0x1cc2);
        *(u8 *)(Func_02001a44_a(16) + 91) = 1;
        Func_02000e04(16);
        v5 = 0;
        *(u8 *)(Func_02001a56(16) + 91) = v5;
    }
}

void Func_02000970(int actor, int x, int z, int field40)
{
    extern u8 *Data_03001ebc;

    u8 *record = Func_02001a82(actor); int frames;
    Func_02001a9a(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Func_02001a48(record, 0);
    Func_02001aca(actor, x, z); Func_02001af4(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Func_02001a34(1); if (*(s16 *)(record + 42) == 0) break; }
    Func_02001a84(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunScene385_020009f8(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001ae4();
    Func_02001bb2(100);
    Func_02001ae8(40);
    if (Value1(Func_02001ace, 0x867) == 0) {
        Call2(Func_02001bb4, 21, 0x102);
        Func_02001b66(21, 4, 0);
        Func_02001b0c(12);
        Func_02001b76(21, 4, 0);
        Func_02001b1c(20);
        Call4(Func_020013bc, 21, 0x188, 104, 0x70000);
        Func_02001b32(20);
        Call3(Func_02001b86, 21, 0x198, 104);
        Call3(Func_02001b92, 21, 0x198, 120);
        Call1_02000640(Func_02001b38, 0x867);
    }
    Func_02001b64();
}

void PlaceActorAndSetSceneDelay(s32 x, s32 y, s32 delay)
{
    extern u8 *Data_03001ebc;

    s32 zero = 0;

    SetScale(zero, 0x8000, 0x4000);
    Func_02001bc2(zero, x, y);
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_02001c56(delay);
}

void FieldScene_SetupDescriptor9740(void)
{
    Func_02001c7e(158);
    Func_02001b60(Data_02009740, 56, 19);
    Func_02001566(408, 320, 5);
}

void ActorPresentation_SetupActorEighteenAt312_304(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    Func_02001ca6(158);
    Func_02001b88(Data_02009756, 50, 18);
    Func_0200158e(312, 304, 6);
}

void FieldScene_SetupWithDescriptor976C(void)
{
    Func_02001cce(158);
    Func_02001bb0(Data_0200976c, 44, 17);
    Func_020015b4_a(216, 288, 7);
}

void ActorPresentation_SetupActorZeroForSceneEight(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    struct SceneActor_020004b4 *actor = Func_02001c48(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001d02(158);
    Func_02001be4((const void *)0x02009782, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Func_02001c0a(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200160e(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNine(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    struct SceneActor_020004b4 *actor = Func_02001ca4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001d5e(158);
    Func_02001c40((const void *)0x02009798, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Func_02001c66(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200166a(296, 176, 9);
}

void FieldScene_SetupWithDescriptor97AE(void)
{
    Func_02001dae(158);
    Func_02001c90(Data_020097ae, 38, 6);
    Func_02001692(120, 144, 10);
}

void ActorPresentation_SetupActorSceneCells(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    struct SceneActor_02000c1c *actor = Func_02001d28(0);
    struct Presentation_02000c1c *record = actor->presentation;
    u8 flags;

    Func_02001de4(188);
    Func_02001cd6(42, 33, 34, 16, 2, 2);
    Func_02001ce6(42, 35, 36, 16, 2, 2);
    Func_02001d3c(4);
    Func_02001cfc(40, 33, 34, 16, 2, 2);
    Func_02001d0c(40, 35, 36, 16, 2, 2);
    Func_02001d62(4);
    {
        s32 mode = 3;
        s32 value = 16;
        Func_02001d30(33, 21, 2, 2, mode, value);
    }
    actor->state_23 &= ~1;
    flags = record->flags;
    flags |= 12;
    record->flags = flags;
    Func_02001738(64, 272, 11);
}

void ActorPresentation_SetupActorZeroForSceneTwelve(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    struct SceneActor_02000c1c *actor = Func_02001dd0(0);
    struct Presentation_02000c1c *presentation = actor->presentation;
    u8 flags;

    Func_02001e8a(158);
    Func_02001d6c((const void *)0x020097c4, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Func_02001d92(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001794(72, 160, 12);
}

void SceneState_SetFlag200AndConfigureRegion55_26(void)
{
    Func_02001df6(0x200);
    {
        s32 a = 23;
        s32 b = 26;
        Func_02001dda(55, 26, 4, 2, a, b);
    }
}

void ActorPresentation_SetFlag200AndSceneCell23(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    Func_02001e26(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Func_02001e02(23, 23, 4, 2, first_value, second_value);
    }
}

void FieldScene_SetActor21Values0And4(void)
{
    extern u8 Data_03001ebc[];

    Func_02001f26(21, 0, 4);
}

void FieldScene_RunScene385_02000d84(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02001e62(231);
    Func_02001e76();
    Func_02001e74(10);
    Func_02001ee4(18, 2);
    Call3(Func_02001eae, 18, 0xcccc, 0x6666);
    Call3(Func_02001eda, 18, 216, 0x198);
    Func_02001e98(10);
    Call3(Func_02001f4c, 18, 0x4000, 20);
    Func_02001f0e_a(18, 6, 0);
    Func_02001eb4(30);
    Func_02001f1e(18, 6, 0);
    Func_02001ec4(30);
    Func_02001f2e(18, 6, 0);
    Func_02001ed4(30);
    Call3(Func_02001f28, 18, 216, 0x188);
    Func_02001ee6(10);
    Call3(Func_02001f9a, 18, 0x4000, 20);
    Call1_02000d74(Func_02001ee0, 0x858);
    Func_02001f0c();
}

void ActorPresentation_SetPairedSceneCells(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    s32 v1 = 13;
    s32 v2 = 25;

    Func_02001ede(41, 43, 1, 1, v1, v2);
    Func_02001ee8(40, 42, 12, 22, 3, 3);
}

void ActorPresentation_SetAlternatePairedSceneCells(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    s32 v1 = 13;
    s32 v2 = 25;

    Func_02001f0e(37, 43, 1, 1, v1, v2);
    Func_02001f18(36, 42, 12, 22, 3, 3);
}

void FieldScene_RunActorEighteenDialogue(void)
{
    extern struct SceneWork_02000e90 *Data_03001ebc;

    Func_02001f7c();
    Func_02002002(0x1342);
    Func_02001fda(18, 0);
    Func_02002004(18, 0, 0);
    Func_02001f92(2);
    Func_02002032(18, 0);
    Func_02001ffa(18, 1);

    if (Func_02001f80(231) != -1 && Func_02001f96(0x858) == 0) {
        Data_03001ebc->actor18_marker = 1;
    }

    Func_02001fdc();
}

void SceneState_SetFlag947AndValue29dc(void)
{
    Func_02001fec();
    Func_02001fbc(0x947, 1);
    Func_02001fc4(0x29dc, 1);
    Func_02002008();
}

const u8 *SceneData_GetTable97dc(void)
{
    return Data_020097dc;
}

s32 SceneSetup_InitializeActorsAndFlags(void)
{
    void *Func_020000a0(s32, s32, s32, s32);

    u8 *actor;
    s16 *scene;
    s32 mode;

    if (SceneFlag_Check(0x200))
        UpdateRect(SceneMap_UpdateRect, 55, 26, 4, 2, 23, 26);
    OverlayObject_CreateConfiguredObjectB(0x800000, 0, 0x1a40000, 223);
    SceneMap_CopyRect(45, 41, 8, 45, 3, 3);
    SceneState_SetMode(1);
    actor = SceneActor_Find(14);
    *(u32 *)(actor + 108) = 0x02008401;
    {
        u8 *actor = SceneActor_Find(14);
        s32 mode = 1;
        *(u16 *)(actor + 100) = mode;
    }
    mode = 0;
    actor = SceneActor_Find(15);
    *(u32 *)(actor + 108) = 0x02008401;
    *(u16 *)(SceneActor_Find(15) + 100) = mode;
    if (SceneFlag_Check(0x858))
        PlaceActor(SceneActor_Place, 18, 0xd80000, 0x1880000);
    if (SceneState_Table[225] <= 2 && !SceneFlag_Check(52) && !SceneFlag_Check(0x109))
        SceneFlag_Set(0x867);
    if (SceneFlag_Check(0x867) && !SceneFlag_Check(52))
        PlaceActor(SceneActor_Place, 21, 0x1980000, 0x780000);
    scene = SceneState_Table;
    if (scene[225] == 11)
        SceneFlag_Set(0x12f);
    if (scene[225] == 13)
        SceneFlag_Set(0x120);
    return 0;
}
