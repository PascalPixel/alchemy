#include "TYPES.H"
#include "FIELD_EVENT.H"

#include "COLOSSO_LOG_ROLLING_STAGE.H"

extern u8 Data_0200ce3c[];
extern u8 *Data_03001ebc;

typedef struct ActorSpriteRef {
    u8 filler00[0x28];
    s16 *entry;
} ActorSpriteRef;

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x3c];
    ActorSpriteRef *sprite;
    u8 state;
} StageActor;

typedef struct ScaledStageObject {
    u8 pad00[12];
    s32 y;                      /* +12 */
    u8 pad10[32];
    s32 scale_x;                /* +48 */
    s32 scale_z;                /* +52 */
    u8 pad38[35];
    u8 state;                   /* +91 */
} ScaledStageObject;

typedef struct StageEffect {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[20];
    s32 vertical_motion;
    u8 reserved_2c[41];
    u8 state;
} StageEffect;

typedef struct StageEffect_02003d88 {
    s32 reserved_00;
    s32 reserved_04;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[65];
    u8 state;
    u8 reserved_56[14];
    s16 linked_effect_slot;
} StageEffect_02003d88;

typedef struct SceneControl {
    s16 enabled;
    s16 active;
    s16 scene_variant;
    s16 phase;
    s16 timer;
} SceneControl;

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern void Func_02004d72(void);       /* site 0x20024e4 -> Func_0200288c veneer */
extern s32 Func_02004f88(s32 actor, s32 slot); /* site 0x20024f2 -> Func_02002a94 veneer */
extern void Func_02005f92(s32 x, s32 z, s32 mode); /* site 0x2002538 -> Func_02003a58 veneer */
extern void Func_0200600e(s32 x, s32 z, s32 mode); /* site 0x2002550 -> Func_02003abc veneer #1 */
extern void Func_02006026(s32 x, s32 z, s32 mode); /* site 0x2002568 -> Func_02003abc veneer #2 */
extern void Func_02006094(void);         /* site 0x200257a -> Func_02003b18 veneer */
extern void Func_020050e2(s32 actor, s32 slot); /* site 0x2002590 -> Func_02002b50 veneer */
extern s32 Func_0200515c(s32 state, s32 actor, s32 slot); /* site 0x20025ae -> Func_02002bac veneer */
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern u8 Value_0000207c;
extern u16 Data_0200dbd0;
extern u16 Data_0200dba4;
extern u16 Data_0200dbdc;
extern s32 Data_0200dbe0;
extern u16 Data_0200dc38;
extern u16 Data_0200dbac;
extern s32 Data_0200dbb0;
extern s32 Data_0200d9a6;   /* default handler (mode not 2/3/4) */
extern s32 Data_0200ccba;   /* handler for mode == 2 */
extern s32 Data_0200d9d2;   /* handler for mode == 4 */
extern struct ModeRecord Data_0200cce2;
extern struct ModeRecord Data_0200da50;
extern void Func_020062e0(s32 mode);          /* Func_02002e54 veneer #1 */
extern void Func_0200686c(s32 style, s32 variant); /* Func_020033d8 veneer #1 */
extern void Func_02006324(s32 mode);          /* Func_02002e54 veneer #2 */
extern void Func_020068b0(s32 style, s32 variant); /* Func_020033d8 veneer #2 */
extern s32 Func_02008092(void);               /* Func_080f9048 veneer (loop check) */
extern void Func_0200634e(s32 mode);          /* Func_02002e54 veneer #3 */
extern void Func_020068da(s32 style, s32 variant); /* Func_020033d8 veneer #3 */
extern void Func_020068ee(s32 style, s32 variant); /* Func_020033d8 veneer #4 */
extern void Func_0200637c(s32 mode);          /* Func_02002e54 veneer #4 */
extern void Func_02006908(s32 style, s32 variant); /* Func_020033d8 veneer #5 */
extern void Func_02006396(s32 mode);          /* Func_02002e54 veneer #5 */
extern void Func_02006922(s32 style, s32 variant); /* Func_020033d8 veneer #6 */
extern void Func_020080cc(void);              /* Func_0808a4f0 veneer */
extern s16 Data_0200dace;
extern u16 Data_0200dc34;
extern u16 Data_0200dbc0;
extern u16 Data_0200db98;
extern u16 Data_0200dbb4;
extern u16 Data_0200dbcc;
extern u16 Data_0200dba0;
extern u16 Data_0200dc40;
extern u16 Data_0200dbe4;
extern u16 Data_0200dbfc;
extern u16 Data_0200db90;
extern u8 Data_0200ce50[];
extern u8 Data_0200ce74[];
extern u8 Data_0200cd80[];
extern u8 *Data_03001f3c;
extern u8 Data_0200bef1[];
extern u32 Data_0200cfc0[];

StageActor *Func_02007198(s32);
StageActor *Func_020071a6(s32);
void Func_02007210_b();
void Func_02007200(s32, s32);
void Func_02004e9a(s32);
void Func_0200742a(s32, s32);
void Func_02007434(s32, s32);
void Func_0200743e(s32, s32);
void Func_02007448(s32, s32);
void Func_02007452(s32, s32);
void Func_0200745c(s32, s32);
void Func_0200760c(void);
void Func_0200741c(s32, s32);
s32 Func_0200747e(s32);
void Func_020074d2(s32, s32);
void Func_02007c38(s32 taskAddress, s32 frameBudget);
void Func_020082d4(s32 taskAddress, s32 frameBudget);
void Func_02008330(s32 taskAddress, s32 frameBudget);
void Func_0200b91c(void);
void Func_0200836e(void (*callback)(void));
void Func_020083c8(s32 slot);
ScaledStageObject *Func_020086b0();
void Func_02008464();
void Func_02008488();
u8 *Func_020086f0(s32 object_id);
void Func_020084a4(void);
void Func_020084c8(u8 *object, s32 x, s32 y, s32 z);
void Func_020084d6(u8 *object);
u32 Func_02008568(void);
void Func_020085a6(s32, s32, s32 *);
StageEffect *Func_0200863a(s32, s32, s32, s32);
StageEffect *Func_020087b4(s16);
void Func_020086be(StageEffect *, s32, s32, s32);
u8 *Func_02008818_b();
u8 *Func_02008d2e();
s32 Func_02008d48();
u8 *Func_02008f08();
u8 *Func_02008f10();
void Func_02008dbe();
s32 Func_02008de2();
void Func_02008d8e();
void Func_02008ddc();
void Func_0200c0d0(void);
s32 Func_02008e56(void);
void Func_02008e26(s32, s32);
void Func_02008dfa(s32, s32);
SceneRecord *Func_0200905c();
SceneRecord *Func_02008c56(Position3 *, SceneRecord *);
SceneRecord *Func_02008c80(Position3 *, SceneRecord *);
SceneRecord *Func_02008cac(Position3 *, SceneRecord *);
s32 Func_0200905e(SceneRecord *, Position3 *);
void Func_02009042(SceneRecord *, s32, s32, s32);
void Func_02009052(SceneRecord *, s32, s32, s32);
void Func_02009066(SceneRecord *);
u8 *Func_020091bc();
void Func_02009046();
s32 *Func_02008dc6();
void Func_02009078();
s32 *Func_02008df8();

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence) -- declared/named as the literal per-site
 * targets, not the shared ultimate-destination symbol. */

/*
 * The two rate words and state byte remain byte-offset accesses because this
 * owner needs the temporary pointer walk to preserve register allocation.
 * Its six imported calls are per-site veneers whose semantics are unproven.
 */

/* Per-site veneers (raw sub_ symbols from the overlay .s). */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

                                /* allocate/fetch a record by (id, size) */

                                /* reserve a graphics handle */

                                /* release a graphics handle */

                                /* upload image data to a handle */

                                /* upload a palette ramp */

                                /* next palette slot index */

                                /* install a per-frame task (callback, rate) */

                                /* test a story flag */

                                /* scene record for an actor selector */

/* In-image data at file offset 0x3f14 (0x0200cd80 - 0x8000). */

/* The per-frame task this owner installs; in-image code at file offset
 * 0x33a0 (0x0200c0d0 - 0x8000). */

/* Store the caller's halfword in the scene workspace field at offset 220. */

                                /* scene record for a subject handle */

                                /* select presentation mode (record, mode) */

                                /* place the record at (x, y, z) */

                                /* re-attach the camera to a record */

                                /* terrain probe; positive result blocks */

                                /* wait n frames */

                                /* play a cue */

/* This overlay's own byte-exact occupancy lookup. */

/* In-image direction table at file offset 0x4154 (0x0200cfc0 - 0x8000):
 * sixteen packed steps, high half x, low half z. */

s32 *Func_02006e64();          /* entity by selector, established */

void Func_02006c06();          /* unestablished */

void Func_02006c2a();          /* established (record, x, y, z) */

void Func_02006c38();          /* unestablished, single argument */

s32 Func_02006f98();           /* Func_080770e0 veneer #1 */

s32 Func_02006fa2();           /* Func_080770e0 veneer #2 */

s32 Func_02006fc0();           /* Func_080770e0 veneer #3 */

s32 Func_02006fca();           /* Func_080770e0 veneer #4 */

s32 Func_02006fe4();           /* Func_080770e0 veneer #5 */

s32 Func_02006fee();           /* Func_080770e0 veneer #6 */

u8 *Func_020077bc();           /* veneer, item/party record by id, established */

s32 Func_020077ce();           /* veneer, established (handle, item) */

void Func_020077ea();          /* veneer, established (handle, slot) */

u8 *Func_020082f8();           /* scene-record accessor, established (veneer to Scene_GetRecord) */

void Func_020081e6();          /* unestablished */

s32 Func_0200829e(void);       /* established (veneer to Func_080153b8) */

void Func_02007362();          /* sibling item-28 owner, via per-site veneer */

u8 *Func_02008818_a();         /* veneer to Scene_GetRecord */

void Func_0200876a();          /* veneer to Object_SetPosition */

s32 Func_02006ca2();           /* local thunk to Func_020020e8, site A */

s32 Func_02006cb4();           /* local thunk to Func_020020e8, site B */

void Func_0200880a();          /* veneer to UiText_DrawQuantity, site A */

void Func_0200881c();          /* veneer to UiText_DrawQuantity, site B */

void Func_0200882c_a();          /* shared veneer, selector refresh + 0x96a */

void Func_020087ca();          /* veneer to Func_08009148 */

s32 *Func_02006e34_position();          /* entity by selector, established */
void Func_02006bd6_position();          /* unestablished */
void Func_02006bfa_position();          /* established (record, x, y, z) */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* AUDITED GENERATED CALL SCRIPT for FieldScene_RunSecondArrivalSequence:
 * state-routed scene setup and all 40 calls with their scene arguments. */

#define FieldScene_RunSecondArrivalSequence Func_02001df8

void Func_0200469a_arrival();
s32 Func_020048b0_arrival();
void Func_02004aaa_arrival();
void Func_02004b24_arrival();
s32 Func_020054a8_arrival();
void Func_02005766_arrival();
void Func_02005960_arrival();
void Func_020059ce_arrival();
void Func_020059de_arrival();
void Func_020059ee_arrival();
s32 Func_02005a00_arrival();
void Func_02005a0c_arrival();
void Func_02005a5c_arrival();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

#define Audio_PlayCue_1(args...) Func_02006034_a_head(args)
#define Object_SetModeById_1(args...) Func_02005f22_a_head(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Scene_GetRecord_1(args...) Func_02005ef0_head(args)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_02005f22_b_head, a0, a1, a2)
#define Scene_GetRecord_2(a0) Value1(Func_02005f5e_head, a0)
#define Scene_GetRecord_3(a0) Value1(Func_02005f66_head, a0)
#define Scene_GetRecord_4(a0) Value1(Func_02005f6e_head, a0)
#define ObjectMotion_SetPositionAndReset_1(args...) Func_02006056_a_head(args)
#define Scene_GetRecord_5(args...) Func_02005fba_head(args)
#define Scene_GetRecord_6(args...) Func_02005fc8_head(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02006034_b_head(args)
#define Scene_GetRecord_7(args...) Func_02005fda_head(args)
#define Scene_GetRecord_8(args...) Func_02005ff0_head(args)
#define ObjectMotion_EnableActionAndSetCallback_4(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02006056_b_head, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Value3(Func_020060fc_a_head, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_1(args...) Func_020061b4_head(args)
#define Object_LinkObjectAndSetCallback_2(args...) Func_02006214_head(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_02006112_a_head(args)
#define Object_LinkObjectAndSetCallback_3(args...) Func_02006238_head(args)
#define Object_LinkObjectAndSetCallback_4(args...) Func_02006240_head(args)
#define Object_LinkObjectAndSetCallback_5(args...) Func_02006254_head(args)
#define Object_LinkObjectAndSetCallback_6(args...) Func_0200625c_head(args)
#define Object_LinkObjectAndSetCallback_7(args...) Func_02006270_head(args)
#define Object_LinkObjectAndSetCallback_8(args...) Func_02006278_head(args)
#define BattleRuntime_WaitIfModeZero_12(args...) ((void (*)())Func_020060fc_b_head)(args)
#define Object_LinkObjectAndSetCallback_9(args...) Func_02006294_head(args)
#define Object_LinkObjectAndSetCallback_10(args...) Func_0200629c_head(args)
#define BattleRuntime_WaitIfModeZero_13(args...) Func_02006112_b_head(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Object_LinkObjectAndSetCallback_11(args...) Func_020062c2_head(args)
#define Object_LinkObjectAndSetCallback_12(args...) Func_020062e0_head(args)
#define Object_LinkObjectAndSetCallback_13(args...) Func_020062fe_head(args)
#define BattleEffect_SpawnLinkedResourceObject_6(a0, a1, a2) Call3(Func_02006320_a_head, a0, a1, a2)
#define Object_LinkObjectAndSetCallback_14(args...) Func_020063d0_a_head(args)
#define Object_LinkObjectAndSetCallback_15(args...) Func_020063d8_head(args)
#define Object_LinkObjectAndSetCallback_16(args...) Func_020063e6_head(args)
#define Object_LinkObjectAndSetCallback_17(args...) Func_020063ee_head(args)
#define ObjectMotion_SetVariantCallback_2(args...) Func_02006322_a_head(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_6(args...) Func_02006332_a_head(args)
#define Object_LinkObjectAndSetCallback_18(args...) Func_0200643a_head(args)
#define Object_LinkObjectAndSetCallback_19(args...) Func_02006472_head(args)
#define BattleRuntime_WaitIfModeZero_30(args...) Func_02006320_b_head(args)
#define BattleRuntime_WaitIfModeZero_31(args...) Func_02006332_b_head(args)
#define Object_SetModeById_8(args...) Func_020063d0_b_head(args)
#define Object_LinkObjectAndSetCallback_20(args...) Func_020064f0_head(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define PENDING_CALLBACK_FLAG (*(s32 *)0x0200db80)

extern u8 StageSceneWork[];
extern u8 Data_00002073[];
extern u8 Data_0200d950[];

void Func_02004c6a_head();
void Func_02004c8a_head();
void Func_02004cae_head();
s32 Func_02004d36_head();
s32 Func_02004d54_head();
s32 Func_02004d78_head();
void Func_02004d96_a_head();
void Func_02004d96_b_head();
s32 Func_02004d98_head();
void Func_02004e0e_head();
void Func_02004e2e_head();
void Func_02004e46_head();
s32 Func_02004ecc_head();
u8 *Func_02004ee4_head();
s32 Func_02004f1c_head();
u8 *Func_02004f3c_head();
s32 Func_02000f3a_head();
s32 Func_02000f68_head();
void Func_02000fd0_head();
void Func_02005014_head();
void Func_02005036_head();
s32 Func_02005154_head();
s32 Func_020051a0_head();
s32 Func_020051a8_head();
void Func_020051d6_head();
void Func_020051e2_head();
void Func_0200522a_head();
void Func_02005238_head();
void Func_02005246_head();
void Func_0200527a_head();
void Func_02003262_head();
void Func_02005e7e_head();
s32 Func_02005ef0_head();
void Func_02005f22_a_head();
void Func_02005f22_b_head();
s32 Func_02005f5e_head();
s32 Func_02005f66_head();
s32 Func_02005f6e_head();
s32 Func_02005fba_head();
s32 Func_02005fc8_head();
s32 Func_02005fda_head();
void Func_02005fee_head();
s32 Func_02005ff0_head();
void Func_02006002_head();
void Func_02006014_head();
void Func_02006028_head();
void Func_02006034_a_head();
void Func_02006034_b_head();
void Func_0200603e_head();
void Func_02006052_head();
void Func_02006056_a_head();
void Func_02006056_b_head();
void Func_0200606a_head();
void Func_0200607e_head();
void Func_020060bc_head();
void Func_020060d0_head();
s32 Func_020060fc_a_head();
s32 Func_020060fc_b_head();
void Func_02006112_a_head();
void Func_02006112_b_head();
void Func_02006140_head();
void Func_020061b4_head();
void Func_02006214_head();
void Func_02006238_head();
void Func_02006240_head();
void Func_02006254_head();
void Func_0200625c_head();
void Func_02006270_head();
void Func_02006278_head();
void Func_02006294_head();
void Func_0200629c_head();
void Func_020062c2_head();
void Func_020062e0_head();
void Func_020062fe_head();
void Func_02006320_a_head();
void Func_02006320_b_head();
void Func_02006322_a_head();
void Func_02006322_b_head();
void Func_02006332_a_head();
void Func_02006332_b_head();
void Func_020063d0_a_head();
void Func_020063d0_b_head();
void Func_020063d8_head();
void Func_020063e6_head();
void Func_020063ee_head();
void Func_0200643a_head();
void Func_02006472_head();
void Func_020064f0_head();
void Func_020093c1();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Slot at 0x0200db80 set just before installing one of the callbacks below. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

typedef struct StageObstacleActor {
    u8 unk_00[6];
    u16 attributes;
    s32 x;
    s32 y;
    s32 z;
    u8 unk_14[15];
    u8 direction_and_kind;
    u8 unk_24[12];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 unk_38[29];
    u8 state;
} StageObstacleActor;

typedef struct SceneParticle {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler20[0x1c];
    s32 anchor_y;
    u8 filler40[0x15];
    u8 state;
    u8 filler56[0xe];
    s16 velocity_x;
    s16 velocity_y;
} SceneParticle;

u8 *ColossoLogRollingStage_GetScriptData(void)
{
    return (u8 *)0x0200d000;
}

s32 ColossoLogRollingStage_GetMessageData(void)
{
    return 0;
}

u8 *ColossoLogRollingStage_GetActorData(void)
{
    return (u8 *)0x0200d090;
}

u8 *ColossoLogRollingStage_GetEffectData(void)
{
    return (u8 *)0x0200d0a8;
}

typedef void (*SceneTaskEntry)(void);

extern s32 Data_0200d480;
extern s32 Data_0200d484;

extern void Func_020049ea_reset_and_run_scene_task(SceneTaskEntry);

void ColossoLogRollingStage_ResetAndRunSceneTask(void)
{
    SceneTaskEntry entry;

    Data_0200d484 = 0;
    entry = (SceneTaskEntry) 0x0200804D;
    Data_0200d480 = 0;
    Func_020049ea_reset_and_run_scene_task(entry);
    entry();
}

extern s32 Func_02004a06_start_scene_task(s32, s32);

void ColossoLogRollingStage_StartSceneTask(void)
{
    Func_02004a06_start_scene_task(0x0200804D, 0xC80);
}

extern s32 Data_0200d480;
extern s32 Data_0200d484;

void Func_02004a0e_wait_for_scene_task();
void Func_02004a1e_wait_for_scene_task();

void ColossoLogRollingStage_WaitForSceneTask(void)
{
    s32 polls;

    /* 素直な while ループ。goto 版では初回の読みがテストへ沈む。
     * A plain while loop. The goto-scaffolded version let gcc sink the first
     * read of Data_0200d480 into the test block, where the reference loads it
     * before the loop. And the frame count is a literal ten: the reference
     * emits `movs r0, #10`, which a Value_ symbol cannot produce. */
    Func_02004a0e_wait_for_scene_task(10);

    polls = 0;
    while (Data_0200d480 != 3 || Data_0200d484 != 1) {
        Func_02004a1e_wait_for_scene_task(1);
        polls++;
        if (polls > 119) {
            return;
        }
    }
}

extern u8 *Data_03001ebc;

StageObstacleActor *Func_02004c36_nudge_stage_actors_left();

void ColossoLogRollingStage_NudgeActorsLeft(void)
{
    extern s16 Data_02000240[];
    u8 *workspace = Data_03001ebc;
    s16 *table = Data_02000240;
    s32 id = *(s32 *)&table[250];
    StageObstacleActor *subject = *(StageObstacleActor **)(workspace + 480);
    StageObstacleActor *actor = Func_02004c36_nudge_stage_actors_left(id);
    s32 z = *(s16 *)((u8 *)actor + 0x12);

    /* Nudge both records left while the actor occupies rows 183 through 186. */
    if ((u32)(z - 183) <= 3) {
        subject->x += -0xcccc;
        actor->x += -0xcccc;
    }
}

extern void Func_02004c0e_configure_grid_region(s32);
extern void Func_02004baa_configure_grid_region(s32, s32, s32, s32, s32, s32);

void ColossoLogRollingStage_ConfigureGridRegion(void)
{
    Func_02004c0e_configure_grid_region(0x360);
    {
        s32 width = 49;
        s32 height = 61;

        Func_02004baa_configure_grid_region(47, 61, 1, 4, width, height);
    }
}

typedef struct PrimaryStageObject {
    u8 pad00[8];
    s32 x;
    u8 pad0C[4];
    s32 y;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_z;
    u8 pad20[0x10];
    s32 move_rate_x;
    s32 move_rate_z;
} PrimaryStageObject;

void Func_02004bae_configure_primary_object_set(PrimaryStageObject *, s32, s32, s32);
void Func_02004bc4_configure_primary_object_set(PrimaryStageObject *, s32, s32, s32);
void Func_02004c0e_configure_primary_object_set(s32, s32, s32, s32, s32, s32);
void Func_02004c20_configure_primary_object_set(s32, s32, s32, s32, s32, s32);
void Func_02004c72_configure_primary_object_set(s32);
PrimaryStageObject *Func_02004c94_configure_primary_object_set(s32);
PrimaryStageObject *Func_02004ca2_configure_primary_object_set(s32);
PrimaryStageObject *Func_02004cbc_configure_primary_object_set(s32);

void ColossoLogRollingStage_ConfigurePrimaryObjects(void)
{
    PrimaryStageObject *object;

    object = Func_02004c94_configure_primary_object_set(9);
    object->scale_x = 0x10000;
    object->scale_z = 0x10000;

    object = Func_02004ca2_configure_primary_object_set(11);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Func_02004bae_configure_primary_object_set(object, object->x, 0x200000, object->y);

    object = Func_02004cbc_configure_primary_object_set(10);
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xCCCC;
    Func_02004bc4_configure_primary_object_set(object, object->x, 0x40000, object->y);

    Func_02004c72_configure_primary_object_set(0x362);
    Func_02004c0e_configure_primary_object_set(15, 12, 1, 1, 13, 12);
    Func_02004c20_configure_primary_object_set(14, 12, 1, 1, 9, 12);
}

void FieldScene_RunClosingAuxiliarySequence(void)
{
    extern u8 Data_02000240[];
    u32 i;
    u8 *p9;
    s32 rec;
    s32 rec7;
    u8 *record;
    u8 *p6;

    u8 *base = Data_02000240;

    p6 = *(u8 **)(base + 500);
    rec = GameFlag_IsSet(0x362);
    if (rec == 0) {
        record = Value1(Func_02004d36_head, 10);
        if ((s32)record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02004d96_a_head((s32)p6);
        record = Value1(Func_02004d54_head, 11);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Func_02004c6a_head, (s32)record, *(s32 *)((s32)record + 8), 0x200000, *(s32 *)((s32)record + 16));
        record = Value1(Func_02004d78_head, 10);
        record[85] = rec;
        *(s32 *)((s32)record + 52) = 0x6666;
        *(s32 *)((s32)record + 48) = 0xcccc;
        Call4(Func_02004c8a_head, (s32)record, *(s32 *)((s32)record + 8), 0x40000, *(s32 *)((s32)record + 16));
        rec7 = Func_02004d98_head((s32)p6);
        p9 = rec7 + 85;
        *p9 = rec;
        *(s32 *)(rec7 + 52) = 0x6666;
        *(s32 *)(rec7 + 48) = 0xcccc;
        Call4(Func_02004cae_head, rec7, *(s32 *)(rec7 + 8), 0x40000, *(s32 *)(rec7 + 16));
        Actor_SetSpriteFlags(rec7, 1);
        Actor_WaitForMove((s32)p6);
        Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
        Task_Wait(2);
        Actor_SetSpriteFlags(rec7, 1);
        *p9 = 3;
        *(s32 *)(rec7 + 20) = *(s32 *)(rec7 + 12);
        Call1(Func_02004d96_b_head, 0x367);
    }
}

typedef struct SecondaryStageObject {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x1c];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} SecondaryStageObject;

void Func_02004e8a_configure_secondary_object_set();
SecondaryStageObject *Func_02004e38_configure_secondary_object_set();
void Func_02004d50_configure_secondary_object_set();
SecondaryStageObject *Func_02004e5e_configure_secondary_object_set();
void Func_02004d6e_configure_secondary_object_set();
void Func_02004ec4_configure_secondary_object_set();
void Func_02004db8_configure_secondary_object_set();
void Func_02004cbe_configure_secondary_object_set();
void Func_02004e44_configure_secondary_object_set();

void ColossoLogRollingStage_ConfigureSecondaryObjects(void)
{
    extern s16 Data_02000240[];
    s16 *table;
    SecondaryStageObject *object;

    table = Data_02000240;

    Func_02004e8a_configure_secondary_object_set(*(s32 *)&table[250], 1);

    object = Func_02004e38_configure_secondary_object_set(11);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Func_02004d50_configure_secondary_object_set(object, object->x, 0x40000, object->z);

    object = Func_02004e5e_configure_secondary_object_set(10);
    object->state = 0;
    object->move_rate_z = 0x6666;
    object->move_rate_x = 0xcccc;
    Func_02004d6e_configure_secondary_object_set(object, object->x, 0x200000, object->z);

    Func_02004ec4_configure_secondary_object_set(10);
    {
        s32 stack_first = 9;
        s32 stack_second = 12;
        Func_02004db8_configure_secondary_object_set(0, 25, 1, 1, stack_first, stack_second);
    }
    Func_02004cbe_configure_secondary_object_set(2);
    Func_02004e44_configure_secondary_object_set(0x367);
}

void FieldScene_RunFinalAuxiliarySequence(void)
{
    u8 *rec;
    u8 *b1;
    u8 *t;
    u8 *b2;
    u8 *b3;
    s32 two;
    s32 zero;
    s32 a;
    s32 b;

    rec = (u8 *)Value1(Func_02004ecc_head, 12);
    a = (*(s32 *)((s32)rec + 8) >> 20);
    if (a == 9) {
        b = (*(s32 *)((s32)rec + 16) >> 20);
        if (b == 12) {
            b1 = Func_02004ee4_head(12);
            Actor_SetSpriteFlags((s32)b1, 0);
            t = b1 + 35;
            zero = 0;
            two = 2;
            *t = two;
            t += 50;
            *t = zero;
            *(s32 *)((s32)b1 + 52) = 0x6666;
            *(s32 *)((s32)b1 + 48) = 0xcccc;
            Call4(Func_02004e0e_head, (s32)b1, *(s32 *)((s32)b1 + 8), 0x40000, *(s32 *)((s32)b1 + 16));
            b2 = (u8 *)Value1(Func_02004f1c_head, 11);
            b2[35] = two;
            *(s32 *)((s32)b2 + 52) = 0x6666;
            *(s32 *)((s32)b2 + 48) = 0xcccc;
            Call4(Func_02004e2e_head, (s32)b2, *(s32 *)((s32)b2 + 8), 0x200000, *(s32 *)((s32)b2 + 16));
            b3 = Func_02004f3c_head(10);
            *(s32 *)((s32)b3 + 52) = 0x6666;
            *(s32 *)((s32)b3 + 48) = 0xcccc;
            Call4(Func_02004e46_head, (s32)b3, *(s32 *)((s32)b3 + 8), 0x40000, *(s32 *)((s32)b3 + 16));
            GameFlag_Set(0x368);
            Map_CopyCellAttributes(15, 12, 1, 1, 13, b);
            Map_CopyCellAttributes(1, 25, 1, 1, a, b);
        }
    }
}

void Func_02000a28_run_setup_completion_hooks(void);
void Func_02004ba8_run_setup_completion_hooks(void);

void ColossoLogRollingStage_RunSetupCompletionHooks(void)
{
    Func_02004ba8_run_setup_completion_hooks();
    Func_02000a28_run_setup_completion_hooks();
}

extern StageObstacleActor *Func_02004fac_configure_actor_thirteen(s32);
extern void Func_02004f72_configure_actor_thirteen(s32, s32);
extern void Func_02004ef6_configure_actor_thirteen(s32, s32, s32, s32, s32, s32);
extern void Func_02004f06_configure_actor_thirteen(s32, s32, s32, s32, s32, s32);

void ColossoLogRollingStage_ConfigureActorThirteen(void)
{
    StageObstacleActor *actor;
    s32 x;

    actor = Func_02004fac_configure_actor_thirteen(13);
    x = actor->x >> 20;
    Func_02004f72_configure_actor_thirteen(880, x);
    Func_02004ef6_configure_actor_thirteen(18, 10, 3, 1, 18, 11);
    Func_02004f06_configure_actor_thirteen(17, 11, 1, 1, x, 11);
}

void ColossoLogRollingStage_NoopSetupHook(void)
{
}

extern void Func_02004c00_run_setup_hook(void);

void ColossoLogRollingStage_RunSetupHook(void)
{
    Func_02004c00_run_setup_hook();
}

extern StageObstacleActor *Func_02005002_activate_clear_obstacle_actors(s32);
extern s32 Func_02004f26_activate_clear_obstacle_actors(s32, s32, s32);
extern void Func_02004f60_activate_clear_obstacle_actors(s32, s32, s32, s32, s32, s32);
extern void Func_02004f7a_activate_clear_obstacle_actors(s32, s32, s32, s32, s32, s32);
extern void Func_02004ffa_activate_clear_obstacle_actors(s32);

void ColossoLogRollingStage_ActivateClearObstacleActors(void)
{
    StageObstacleActor *actor;
    s32 slot;
    s32 x;
    s32 z;
    s32 x2;
    s32 z2;

    for (slot = 15; slot <= 17; slot++) {
        actor = Func_02005002_activate_clear_obstacle_actors(slot);
        if (Func_02004f26_activate_clear_obstacle_actors(0, actor->x, actor->z) == 0) {
            actor->direction_and_kind = 2;
            actor->state = 0;
            x = actor->x >> 20;
            z = actor->z >> 20;
            Func_02004f60_activate_clear_obstacle_actors(83, 13, 1, 1, x, z);
            x2 = actor->x >> 20;
            z2 = actor->z >> 20;
            Func_02004f7a_activate_clear_obstacle_actors(83, 13, 1, 1, x2, z2 + 52);
            Func_02004ffa_activate_clear_obstacle_actors(slot + 517);
        }
    }
}

extern StageObstacleActor *Func_0200507e_show_actor_position_message(s32);
extern void Func_02004ffe_show_actor_position_message(s32, s32, s32, s32);

void ColossoLogRollingStage_ShowActorPositionMessage(void)
{
    extern s16 Data_02000240[];
    StageObstacleActor *actor;
    s16 *table;
    s32 x;
    s32 z;
    s32 message_id;

    table = Data_02000240;
    actor = Func_0200507e_show_actor_position_message(*(s32 *)&table[250]);
    x = actor->x >> 20;
    message_id = 23;
    z = actor->z >> 20;
    if (x == 81 && z == 12) {
        if ((actor->attributes & 0xE000) == 0x4000) {
            message_id = 253;
        }
        Func_02004ffe_show_actor_position_message(0, x << 20, z << 20, message_id);
    }
}

extern s32 Func_02004fea_check_obstacle_destination(s32, s32, s32);
extern StageObstacleActor *Func_020050da_check_obstacle_destination(s32);
extern StageObstacleActor *Func_020050f4_check_obstacle_destination(s32);
extern StageObstacleActor *Func_0200510a_check_obstacle_destination(s32);

s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z)
{
    StageObstacleActor *actor;

    if (Func_02004fea_check_obstacle_destination(0, x, z) == 255) {
        return -2;
    }
    actor = Func_020050da_check_obstacle_destination(15);
    x = x >> 20;
    z = z >> 20;
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Func_020050f4_check_obstacle_destination(16);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Func_0200510a_check_obstacle_destination(17);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    return 0;
}

extern s32 Func_02000dc4_check_path_clearance(s32, s32);
extern s32 Func_02000dd2_check_path_clearance(s32, s32);
extern s32 Func_02000de2_check_path_clearance(s32, s32);
extern s32 Func_02000df2_check_path_clearance(s32, s32);

s32 ColossoLogRollingStage_CheckPathClearance(s32 x, s32 y)
{
    if (Func_02000dc4_check_path_clearance(x, y - 0x180000) != 0
     || Func_02000dd2_check_path_clearance(x, y - 0x80000) != 0
     || Func_02000de2_check_path_clearance(x, y + 0x80000) != 0
     || Func_02000df2_check_path_clearance(x, y + 0x180000) != 0) {
        return -1;
    }
    return 0;
}

void FieldScene_RunEarlySequence(void)
{
    extern u8 Data_02000240[];
    extern u16 Data_0200cc38[];
    s32 leader;
    s32 log_actor;
    u8 *state;
    s32 tile;
    s32 x;
    s32 steps;
    s32 next_x;
    s32 column;
    s32 acceleration;
    s32 direction;
    s32 cell_step;
    s32 record;
    s32 selected_actor;
    s32 z;
    volatile s32 *keys;
    s32 cell_center[3];

    state = Data_02000240;
    leader = Value1(Func_020051a0_head, *(s32 *)(state + 500));
    log_actor = Value1(Func_020051a8_head, 31);
    steps = 0;
    tile = Data_0200cc38[*(u16 *)(leader + 6) >> 13];
    selected_actor = *(s32 *)(state + 500);
    cell_center[0] = (*(s32 *)(leader + 8) & -0x100000) + 0x80000;
    cell_center[1] = *(s32 *)(leader + 12);
    cell_center[2] = (*(s32 *)(leader + 16) & -0x100000) + 0x80000;
    Call3(Func_02005036_head, 0x100000, tile, (s32)cell_center);
    x = *(s32 *)(log_actor + 8);
    z = *(s32 *)(log_actor + 16);
    if ((cell_center[0] - x >= 0 ? cell_center[0] - x : x - cell_center[0]) > 0x80000
        || (cell_center[2] - z >= 0 ? cell_center[2] - z : z - cell_center[2]) > 0x200000) {
        goto far;
    }
    keys = (volatile s32 *)0x03001ae8;
    if ((*keys & 32) != 0) {
        direction = 2;
        cell_step = -8;
        for (;;) {
            next_x = x - 0x100000;
            if (Value2(Func_02000f3a_head, next_x, z) != 0) {
                goto moved;
            }
            steps++;
            x = next_x;
        }
    }
    if ((*keys & 16) == 0) {
        return;
    }
    direction = 3;
    cell_step = 8;
    for (;;) {
        next_x = x + 0x100000;
        if (Value2(Func_02000f68_head, next_x, z) != 0) {
            goto moved;
        }
        steps++;
        x = next_x;
    }
moved:
    if (steps == 0) {
        return;
    }
    Map_CopyCellAttributes(74, 8, 1, 4, *(s32 *)(log_actor + 8) >> 20, 9);
    Map_CopyCellAttributes(120, 60, 8, 5, 74, 60);
    Event_Begin();
    Actor_SetAnimation(selected_actor, 8);
    Event_Wait(6);
    *(s32 *)(log_actor + 48) = 0x8000;
    acceleration = 0x3333;
    *(s32 *)(log_actor + 52) = acceleration;
    Value2(Engine_ObjectSetAnimation, log_actor, direction);
    Func_020051d6_head(log_actor, x, 0, z);
    Event_Wait(6);
    Actor_SetAnimation(selected_actor, 2);
    record = Value2(Func_02005154_head, 27, 0xccc);
    Func_020051e2_head(*(s32 *)(record + 0x1e0), log_actor);
    Actor_SetSpeed(selected_actor, 0x8000, acceleration);
    Audio_PlayCue(239);
    Object_SetAnimation(leader, 2);
    Func_0200522a_head(leader, ((steps * cell_step) << 16) + *(s32 *)(leader + 8), 0,
                       *(s32 *)(leader + 16));
    Func_02005238_head(leader);
    Value2(Engine_ObjectSetAnimation, leader, 1);
    Func_02005246_head(log_actor);
    if (x >= 0x5300000) {
        GameFlag_Set(0x369);
        Actor_SetAnimation(31, 3);
        Actor_SetDestinationOffset(31, 18, 6);
        Event_Wait(30);
        Object_SetAnimation(log_actor, 8);
        Func_0200527a_head(log_actor);
        *(u8 *)(log_actor + 35) = 2;
        column = 84;
        Map_CopyCellAttributes(86, 10, 1, 2, column, 10);
        Map_CopyCellAttributes(86, 9, 1, 1, column, 12);
        Audio_PlayCue(0x120);
        Audio_PlayCue(240);
    } else {
        Object_SetAnimation(log_actor, 1);
        Audio_PlayCue(0x120);
        Audio_PlayCue(213);
        column = x >> 20;
        Map_CopyCellAttributes(85, 9, 1, 4, column, 9);
        Map_CopyCellAttributes(85, 9, 1, 4, column, 61);
    }
    Event_Wait(15);
    Event_End();
    return;
far:
    Func_02005014_head();
    Func_02000fd0_head();
}

extern void Func_0200534e_set_scene_event_values(s32);
extern void Func_0200534c_set_scene_event_values(s32);
extern void Func_020055d4_set_scene_event_values(s32);
extern void Func_020055da_set_scene_event_values(s32);

s32 ColossoLogRollingStage_SetSceneEventValues(void)
{
    Func_0200534e_set_scene_event_values(1);
    Func_0200534c_set_scene_event_values(2);
    Func_020055d4_set_scene_event_values(288);
    Func_020055da_set_scene_event_values(217);
    return 0;
}

typedef struct StageMotionEffect {
    u8 filler00[0x30];
    s32 move_rate_x;
    s32 move_rate_z;
    u8 filler38[0x1d];
    u8 state;
} StageMotionEffect;

extern StageMotionEffect *Func_02005462_configure_scene_event_effect(s32);
extern void Func_02005340_configure_scene_event_effect(StageMotionEffect *, s32);
extern void Func_02005350_configure_scene_event_effect(StageMotionEffect *, s32);
extern void Func_02005426_configure_scene_event_effect(s32);

void ColossoLogRollingStage_ConfigureSceneEventEffect(void)
{
    StageMotionEffect *effect;
    s32 move_rate;

    effect = Func_02005462_configure_scene_event_effect(30);
    effect->state = 0;
    move_rate = 0x19999;
    effect->move_rate_z = move_rate;
    effect->move_rate_x = move_rate;
    Func_02005340_configure_scene_event_effect(effect, 2);
    Func_02005350_configure_scene_event_effect(effect, 0x0200CC48);
    Func_02005426_configure_scene_event_effect(0x363);
}

extern void Func_0200561a_wait_for_scene_event_task(s32);
extern void Func_02005448_wait_for_scene_event_task(s32);
extern void Func_020052d6_wait_for_scene_event_task(s32);
extern void Func_020052ea_wait_for_scene_event_task(s32);
extern void Func_020052fa_wait_for_scene_event_task(s32);
extern void Func_02005310_wait_for_scene_event_task(s32);

void ColossoLogRollingStage_WaitForSceneEventTask(void)
{
    s32 *status;
    s32 value;

    Func_0200561a_wait_for_scene_event_task(28);
    Func_02005448_wait_for_scene_event_task(0x361);
    Func_020052d6_wait_for_scene_event_task(10);
    value = *(s32 *)0x0200D480;
    if (value != 1 && value != 3) {
        status = (s32 *)0x0200D480;
        do {
            Func_020052ea_wait_for_scene_event_task(1);
            value = *status;
        } while (value != 1 && value != 3);
    }
    Func_020052fa_wait_for_scene_event_task(1);
    Func_02005310_wait_for_scene_event_task(0x0200804D);
}

extern StageObstacleActor *Func_020054fa_offset_active_actor(s32);
extern void Func_020055e2_offset_active_actor(s32, s32);
extern void Func_020053e2_offset_active_actor(StageObstacleActor *, s32);
extern void Func_02005428_offset_active_actor(StageObstacleActor *, s32, s32, s32);
extern void Func_02005436_offset_active_actor(StageObstacleActor *);

void ColossoLogRollingStage_OffsetActiveActor(void)
{
    extern s16 Data_02000240[];
    StageObstacleActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = Data_02000240;
    slot = (s32 *)&table[250];
    actor = Func_020054fa_offset_active_actor(*slot);
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Func_020055e2_offset_active_actor(*slot, 258);
    Func_020053e2_offset_active_actor(actor, 5);
    z = actor->z & 0xFFF00000;
    Func_02005428_offset_active_actor(actor, actor->x, actor->y, z + 0x180000);
    Func_02005436_offset_active_actor(actor);
}

extern StageObstacleActor *Func_02005556_clamp_and_offset_active_actor(s32);
extern void Func_02005440_clamp_and_offset_active_actor(StageObstacleActor *, s32);
extern void Func_02005486_clamp_and_offset_active_actor(StageObstacleActor *, s32, s32, s32);
extern void Func_02005494_clamp_and_offset_active_actor(StageObstacleActor *);
extern void Func_0200566e_clamp_and_offset_active_actor(s32, s32);
extern void Func_02005620_clamp_and_offset_active_actor(s32, s32, s32);

void ColossoLogRollingStage_ClampAndOffsetActiveActor(void)
{
    extern s16 Data_02000240[];
    StageObstacleActor *actor;
    s16 *table;
    s32 *slot;
    s32 z;

    table = Data_02000240;
    slot = (s32 *)&table[250];
    actor = Func_02005556_clamp_and_offset_active_actor(*slot);
    if (actor->x > 0x2980000) {
        actor->x = 0x2980000;
    }
    actor->move_rate_z = 0x10000;
    actor->move_rate_x = 0x20000;
    Func_02005440_clamp_and_offset_active_actor(actor, 5);
    z = actor->z & 0xFFF00000;
    Func_02005486_clamp_and_offset_active_actor(actor, actor->x, actor->y, z + 0xC0000);
    Func_02005494_clamp_and_offset_active_actor(actor);
    Func_0200566e_clamp_and_offset_active_actor(*slot, 258);
    Func_02005620_clamp_and_offset_active_actor(*slot, 6, 0);
}

void ColossoLogRollingStage_NoopSceneEventHook(void)
{
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_02000240[];
    u8 *base;
    s32 p10;
    s32 p8;
    s32 base7_2073;
    s32 threea0;
    s32 mode;

    p10 = *(s32 *)StageSceneWork;
    base = Data_02000240;
    p8 = *(s32 *)(base + 500);
    mode = *(s16 *)(base + 450);
    if (mode == 2) {
        Event_Begin();
        base7_2073 = (s32)Data_00002073;
        threea0 = (a0 << 1) + a0;
        Event_SetMessage(threea0 + base7_2073);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(p8, 0) == 0) {
            s32 t1 = base7_2073 + 1;
            Event_SetMessage(threea0 + t1);
            Event_ShowMessage(a0, 0);
            *(s32 *)((0x1c0 + p10)) = 0x200;
            *(s32 *)((0x1c8 + p10)) = 15;
            Event_CloseScreen();
            Event_WaitForScreen();
            Func_02003262_head(a0);
            Event_OpenScreen();
            Event_WaitForScreen();
        } else {
            s32 t2 = base7_2073 + 2;
            Event_SetMessage(threea0 + t2);
            Event_ShowMessage(a0, 0);
        }
        Event_End();
    }
}

/* Sets up actors 0-3 and a batch of movement/scale/rotation animation
 * records, then plays them through a long fixed sequence of moves,
 * rotations, and callback-driven steps. */
extern s32 Func_02005504_run_scene_event_if_ready(void);
extern void Func_020058ac_run_scene_event_if_ready(void);

void ColossoLogRollingStage_RunSceneEventIfReady(void)
{
    if (Func_02005504_run_scene_event_if_ready() == 0) {
        Func_020058ac_run_scene_event_if_ready();
    }
}

extern s32 Func_02005518_finish_or_continue_scene_event(void);
extern void Func_020058c0_finish_or_continue_scene_event(void);
extern void Func_02001312_finish_or_continue_scene_event(void);

void ColossoLogRollingStage_FinishOrContinueSceneEvent(void)
{
    if (Func_02005518_finish_or_continue_scene_event() == 0) {
        Func_020058c0_finish_or_continue_scene_event();
    } else {
        Func_02001312_finish_or_continue_scene_event();
    }
}

s32 ColossoLogRollingStage_GetSceneEventState(void)
{
    return 0x0200D488;
}

s32 ColossoLogRollingStage_AdvanceParticleMotion(SceneParticle *particle)
{
    particle->x += particle->velocity_x << 8;
    particle->y += particle->velocity_y << 8;
    particle->scale_x += 0x666;
    particle->scale_y += 0x666;
    particle->velocity_x += 5;
    particle->velocity_y -= 1;
    return 0;
}

extern SceneParticle *Func_02005de2_spawn_periodic_scene_particle(s32);
extern s32 Func_02005c1a_spawn_periodic_scene_particle(s32, s32);
extern SceneParticle *Func_02005e1a_spawn_periodic_scene_particle(s32);
extern SceneParticle *Func_02005e26_spawn_periodic_scene_particle(s32);
extern void Func_02005e84_spawn_periodic_scene_particle(s32, s32, s32);
extern s32 Func_02005e3a_spawn_periodic_scene_particle(s32);
extern void Func_02005d80_spawn_periodic_scene_particle(s32, s32);
extern void Func_02005e8a_spawn_periodic_scene_particle(s32, s32);

void ColossoLogRollingStage_SpawnPeriodicParticle(void)
{
    SceneParticle *particle;
    SceneParticle *source;
    s32 x;
    s32 y;
    s32 kind;
    s32 count;

    particle = Func_02005de2_spawn_periodic_scene_particle(0);
    count = *(s32 *)0x0200DB80 + 1;
    kind = 41;
    x = particle->x;
    y = particle->y;
    *(s32 *)0x0200DB80 = count;
    switch (Func_02005c1a_spawn_periodic_scene_particle(count, 180)) {
    case 10:
        break;
    case 20:
        kind = 42;
        break;
    case 30:
        kind = 43;
        break;
    default:
        return;
    }
    particle = Func_02005e1a_spawn_periodic_scene_particle(kind);
    if (particle == 0) {
        return;
    }
    source = Func_02005e26_spawn_periodic_scene_particle(0);
    if (source != 0) {
        Func_02005e84_spawn_periodic_scene_particle(kind, source->x, source->z);
    }
    Func_02005d80_spawn_periodic_scene_particle(Func_02005e3a_spawn_periodic_scene_particle(kind), 0);
    particle->state = 0;
    particle->scale_x = 0x6666;
    particle->scale_y = 0x6666;
    {
        s32 t = 0x40000;
        particle->x = x + t;
        t += y;
        particle->y = t;
        particle->anchor_y = t;
    }
    particle->velocity_x = 25;
    particle->velocity_y = 128;
    Func_02005e8a_spawn_periodic_scene_particle(kind, 0x0200D96C);
}

void FieldScene_RunMultiPhaseActorSequence(s32 a0)
{
    s32 record;
    s32 data_table_addr;
    s32 callback_target;

    Actor_Destroy(39);
    Actor_Destroy(40);
    Func_02005e7e_head(1);
    Audio_PlayCue_1(17);
    Event_Begin();
    Actor_SetPosition(8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Object_SetModeById_1(8, 10);
    } else {
        Actor_SetAnimation(8, 8);
    }
    ObjectMotion_EnableActionAndSetCallback_1(8, 0x200d668);
    Actor_SetPosition(0, 0x5e00000, 0xc00000);
    record = Scene_GetRecord_1(0);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    ObjectMotion_EnableActionAndSetCallback_2(0, 0x200d738);
    Actor_SetAnimation(0, 35);
    ObjectMotion_SetSpeedParameters_1(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_SetPosition(1, 0x5b80000, 0xb80000);
    Actor_SetPosition(2, 0x5b80000, 0xc80000);
    Actor_SetPosition(3, 0x5a80000, 0xc00000);
    record = Scene_GetRecord_2(1);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_3(2);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_4(3);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    ObjectMotion_SetPositionAndReset_1(0, 0);
    SCENE_PHASE = 0x100;
    ColorBuffer_ApplyTarget(0x10001, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_SetMessage(0x20f1);
    Event_Wait(60);
    data_table_addr = (s32)Data_0200d950;
    Actor_EnableActionCallback(0, data_table_addr);
    record = Scene_GetRecord_5(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Scene_GetRecord_6(0);
    *(s32 *)(record + 28) = 0x10000;
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 36);
    record = Scene_GetRecord_7(0);
    *(s32 *)(record + 8) += 0x30000;
    Event_Wait(10);
    record = Scene_GetRecord_8(0);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(20);
    ObjectMotion_EnableActionAndSetCallback_4(0, 0x200d808);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    ObjectMotion_SetPositionAndReset_2(1, 0x5e0, 176);
    Actor_FaceDirection(1, 0x4000, 10);
    BattleEffect_SpawnLinkedResourceObject_1(1, 0x100, 20);
    Event_ShowMessage(2, 0);
    Object_LinkObjectAndSetCallback_1(1, 2);
    Event_Wait(30);
    Actor_WalkToAndWait(2, 0x5d0, 176);
    Actor_WalkTo(1, 0x5f0, 184);
    Actor_WalkToAndWait(2, 0x5e0, 176);
    Actor_SetAnimation(1, 1);
    Actor_FaceDirection(1, 0x6000, 0);
    Actor_FaceDirection(2, 0x4000, 10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(10);
    Object_LinkObjectAndSetCallback_2(2, 1);
    Event_Wait(30);
    ObjectMotion_CallThenWaitForAnimationChange_2(1, 4);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Object_LinkObjectAndSetCallback_3(1, 3);
    Object_LinkObjectAndSetCallback_4(2, 3);
    Actor_WalkToAndWait(3, 0x5d0, 184);
    Object_LinkObjectAndSetCallback_5(2, 0);
    Object_LinkObjectAndSetCallback_6(1, 0);
    Actor_ShowEmote(1, 0x102, 60);
    Object_LinkObjectAndSetCallback_7(2, 1);
    Object_LinkObjectAndSetCallback_8(1, 2);
    Event_Wait(40);
    Actor_RunRepeatedMotion(3, 2);
    BattleRuntime_WaitIfModeZero_12(10);
    Object_LinkObjectAndSetCallback_9(2, 3);
    Object_LinkObjectAndSetCallback_10(1, 3);
    BattleRuntime_WaitIfModeZero_13(20);
    Event_ShowMessage(3, 0);
    ObjectMotion_EnableActionAndSetCallback_5(0, 0x200d8ac);
    Actor_RunRepeatedMotion(1, 2);
    Object_LinkObjectAndSetCallback_11(1, 0);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Actor_RunRepeatedMotion(2, 2);
    Object_LinkObjectAndSetCallback_12(2, 0);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Actor_RunRepeatedMotion(3, 2);
    Object_LinkObjectAndSetCallback_13(3, 0);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Actor_EnableActionCallback(0, data_table_addr);
    Event_Wait(60);
    callback_target = (s32)Func_020093c1;
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_02005fee_head, callback_target, 0xc80);
    Event_Wait(5);
    Func_02006002_head(callback_target);
    Event_Wait(55);
    Actor_ShowEmote(1, 0x101, 60);
    Call2(Func_02006014_head, callback_target, 0xc80);
    Event_Wait(20);
    Func_02006028_head(callback_target);
    Event_Wait(40);
    Actor_ShowEmote(2, 0x101, 60);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_0200603e_head, callback_target, 0xc80);
    Event_Wait(35);
    Func_02006052_head(callback_target);
    Event_Wait(25);
    Actor_ShowEmote(3, 0x102, 60);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_0200606a_head, callback_target, 0xc80);
    Event_Wait(35);
    Func_0200607e_head(callback_target);
    Event_Wait(25);
    BattleEffect_SpawnLinkedResourceObject_6(2, 0x102, 60);
    Object_LinkObjectAndSetCallback_14(3, 2);
    Object_LinkObjectAndSetCallback_15(2, 3);
    Event_Wait(60);
    Object_LinkObjectAndSetCallback_16(3, 0);
    Object_LinkObjectAndSetCallback_17(2, 0);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_020060bc_head, callback_target, 0xc80);
    Event_Wait(35);
    Func_020060d0_head(callback_target);
    Event_Wait(25);
    Actor_ShowEmote(3, 0x108, 60);
    Actor_StartRepeatedMotion(1, 3);
    ObjectMotion_SetVariantCallback_2(2, 3);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(3, 3);
    Object_LinkObjectAndSetCallback_18(3, 2);
    Object_LinkObjectAndSetCallback_19(1, 2);
    PENDING_CALLBACK_FLAG = 9;
    Call2(Func_02006140_head, callback_target, 0xc80);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimation(3, 3);
    Event_Wait(60);
    Actor_WalkTo(3, 0x5b8, 200);
    BattleRuntime_WaitIfModeZero_30(5);
    Actor_WalkTo(2, 0x558, 184);
    BattleRuntime_WaitIfModeZero_31(3);
    Actor_WalkToAndWait(1, 0x5e8, 184);
    Actor_WalkTo(1, 0x558, 184);
    Actor_WaitForMove(3);
    Object_SetModeById_8(3, 1);
    Object_LinkObjectAndSetCallback_20(3, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(3, 0x598, 200);
    Actor_WalkTo(3, 0x558, 184);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_Wait(30);
    Actor_SetPosition(1, 0x5e80000, 0xb00000);
    Actor_SetPosition(2, 0x5b80000, 0xc00000);
    Actor_SetPosition(3, 0x6180000, 0xc80000);
    Func_02006322_b_head();
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(1);
    Event_SetMessage(0x214c);
    Event_ShowMessage(1, 0);
    Event_ShowMessage(2, 0);
    Event_ShowMessage(3, 0);
    Event_Wait(60);
    Event_End();
}

void Func_02001df8(s32 scene)
{
    extern s16 Data_02000240[];
    s32 state;

    if (Data_02000240[225] == 2) {
        Func_0200469a_arrival();
        return;
    }
    Event_Begin();
    state = Func_020048b0_arrival(scene, 2);
    if (state == 0) {
    Event_SetMessage(8375);
    Camera_SetSpeed(196608, 24576);
    Camera_MoveTo(24641536, -1, 9961472, 1);
    Camera_WaitForMove();
    Event_Wait(30);
    Event_ShowMessage(scene, 0);
    Func_020054a8_arrival(0, 280, 200);
    Actor_SetSpeed(0, 98304, 49152);
    Func_02005a00_arrival(0, 280, 152);
    Call3(Func_02005a0c_arrival, 0, 296, 152);
    Event_Wait(10);
    Leader_CheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Value3(Engine_ActorFaceDirection, 0, 49152, 15);
    Leader_CheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Value3(Engine_ActorFaceDirection, 0, 0, 15);
    Leader_CheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_FaceDirection(0, 16384, 15);
    Event_ShowMessage(scene, 0);
    Value3(Func_02005960_arrival, 96, 40, 0);
    Func_020059ce_arrival(128, 40, 10);
    Event_Wait(30);
    Func_020059de_arrival(160, 40, 10);
    Event_Wait(30);
    Func_020059ee_arrival(160, 72, 10);
    Event_Wait(30);
    Event_ShowMessage(scene, 0);
    Func_02005a5c_arrival();
    Func_02005766_arrival(0);
    Camera_FollowActor(0, 0);
    Func_02004aaa_arrival(scene, 2);
    } else if (state == 1) {
        Event_SetMessage(0x20b6);
        Event_ShowMessage(scene, 0);
    }
    Value3(Func_02004b24_arrival, state, scene, 2);
    Event_End();
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240_t[][1];
void Func_0200499e_motion();
s32 Func_02004bb4_motion();
void Func_02004dd0_motion();
s32 Func_02004e4c_motion();
s32 Func_020057b2_motion();
void Func_02005a8c_motion();
void Func_02006b02_motion();
void Func_02006b10_motion();
void Func_02006b24_motion();
void Func_02006b32_motion();
void Func_02006b44_motion();
void Func_02006b52_motion();
void Func_02006b64_motion();
void Func_02006b72_motion();
s32 Func_02006bda_motion();
void Func_02006cd4_motion();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

void FieldScene_RunFourStepActorMotion(s32 a0)
{
    s32 result;
    s32 actor;
    s32 x;
    s32 y;
    s32 raised_y;
    s32 shifted_x;

    if (Data_02000240_t[225][0] == 2) {
        Func_0200499e_motion();
    } else {
        Event_Begin();
        result = Value2(Func_02004bb4_motion, a0, 4);
        if (result == 0) {
            Event_SetMessage(0x20bf);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3580000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Event_Wait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Value3(Func_020057b2_motion, 0, 0x330, 200);
            Actor_SetSpeed(0, 0x10000, 0x8000);
            Actor_WalkToAndWait(0, 0x348, 200);
            Value3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            Func_02006cd4_motion();
            Camera_MoveTo(-1, -1, -1, 0);
            Actor_SetSpeed(0, 0x8000, 0x4000);
            actor = Value1(Func_02006bda_motion, 0);
            y = *(s32 *)(actor + 12);
            x = *(s32 *)(actor + 8);
            Actor_SetSpeed(0, 0x8000, 0x4000);
            Actor_SetAnimation(0, 10);
            raised_y = 0x60000 + y;
            Func_02006b02_motion(actor, x, raised_y, *(s32 *)(actor + 16));
            Func_02006b10_motion(actor);
            Actor_SetAnimation(0, 14);
            shifted_x = 0x400000 + x;
            Func_02006b24_motion(actor, shifted_x, raised_y, *(s32 *)(actor + 16));
            Func_02006b32_motion(actor);
            Actor_SetAnimation(0, 10);
            Func_02006b44_motion(actor, shifted_x, y + 0x360000, *(s32 *)(actor + 16));
            Func_02006b52_motion(actor);
            Actor_SetAnimation(0, 15);
            Func_02006b64_motion(actor, x + 0x300000, y + 0x360000, *(s32 *)(actor + 16));
            Func_02006b72_motion(actor);
            Actor_SetAnimation(0, 12);
            Event_ShowMessage(a0, 0);
            Func_02005a8c_motion(0);
            Camera_FollowActor(0, 0);
            Func_02004dd0_motion(a0, 4);
        } else if (result == 1) {
            Event_SetMessage(0x20be);
            Event_ShowMessage(a0, 0);
        }
        Value3(Func_02004e4c_motion, result, a0, 4);
        Event_End();
    }
}

void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e34_position(selector);
    if (record != 0) {
        Func_02006bd6_position();
        Object_SetAnimation(record, 5);
        Func_02006bfa_position(record, x << 16, record[3], z << 16);
    }
}

void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    s32 *record;

    record = Func_02006e64(selector);
    if (record != 0) {
        Func_02006c06();
        Object_SetAnimation(record, 5);
        Func_02006c2a(record, x << 16, record[3], z << 16);
        Func_02006c38(record);
        Object_SetAnimation(record, 1);
    }
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240_t[][1];
void Func_0200469e_opening();
void Func_020046b8_opening();
s32 Func_02004710_opening();
void Func_02004bd6_opening();
s32 Func_02004dec_opening();
void Func_02004fcc_opening();
s32 Func_02005048_opening();
s32 Func_020059ea_opening();
void Func_02005c88_opening();
void Func_02005cd0_opening();
void Func_02005d36_opening();
s32 Func_02006e2e_opening();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02004bd6_opening();
    } else {
        Event_Begin();
        rec2 = Value2(Func_02004dec_opening, a0, 5);
        if (rec2 != 0) {
        } else {
            Event_SetMessage(0x20c3);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Event_Wait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Value3(Func_020059ea_opening, 0, 0x3d8, 184);
            Actor_SetSpeed(0, 0x18000, 0xc000);
            Func_020046b8_opening(0, 0x3e0, 184);
            Actor_SetSpeed(0, 0x4ccc, 0x2666);
            Call3(Func_0200469e_opening, 0, 0x460, 184);
            Event_Wait(120);
            Actor_SetAttachedEffect(0, 0x101);
            Event_Wait(120);
            Func_02005cd0_opening(0);
            Actor_SetAnimation(0, 1);
            Actor_SetAttachedEffect(0, 0x100);
            Actor_ShowEmote(0, 0x105, 0);
            rec7 = Func_02006e2e_opening(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Task_Wait(1);
            }
            Actor_ShowEmote(0, 0x103, 60);
            Value3(Func_02004710_opening, 0, 0x460, 184);
            Event_ShowMessage(a0, 0);
            Func_02005d36_opening(0);
            {
                u8 *flag = (u8 *)Data_02000240_t;

                flag[498] = 1;
            }
            Func_02005c88_opening(0);
            Camera_FollowActor(0, 0);
            Func_02004fcc_opening(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Event_SetMessage(0x20c2);
            Event_ShowMessage(a0, 0);
        }
        L_02002494:;
        Value3(Func_02005048_opening, rec2, a0, 5);
        Event_End();
    }
}

void ColossoLogRollingStage_RunLogRollingInteraction(s32 actor)
{
    extern s16 Data_02000240[];

    s32 state;

    if (Data_02000240[225] == 2) {
        Func_02004d72();
        return;
    }

    Event_Begin();
    state = Func_02004f88(actor, 6);

    if (state == 0) {
        Event_SetMessage(0x20c7);
        Camera_SetSpeed(0x30000, 0x6000);
        Camera_MoveTo(0x5080000, -1, 0x980000, 1);
        Camera_WaitForMove();
        Event_Wait(30);
        Event_ShowMessage(actor, 0);
        Func_02005f92(0xb4, 0x58, 0);
        Event_Wait(60);
        Event_ShowMessage(actor, 0);
        Func_0200600e(0x20, 0x54, 10);
        Event_Wait(30);
        Event_ShowMessage(actor, 0);
        Func_02006026(0x60, 0x54, 30);
        Event_Wait(60);
        Event_ShowMessage(actor, 0);
        Func_02006094();
        Event_Wait(2);
        Camera_FollowActor(0, 0);
        Func_020050e2(actor, 6);
    } else if (state == 1) {
        Event_SetMessage(0x20c6);
        Event_ShowMessage(actor, 0);
    }

    Func_0200515c(state, actor, 6);
    Event_End();
}

void ColossoLogRollingStage_RestoreActorPositions(void)
{
    s32 center;

    {
        s32 x = Func_02006f98(896);
        s32 z = Func_02006fa2(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Actor_SetPosition(1, x, z);
    }
    {
        s32 x = Func_02006fc0(912);
        s32 z = Func_02006fca(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Actor_SetPosition(2, x, z);
    }
    {
        s32 x = Func_02006fe4(928);
        s32 z = Func_02006fee(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Actor_SetPosition(3, x, z);
    }
}

void ColossoLogRollingStage_MarkSceneProgress(void)
{
    extern s16 Data_02000240[];

    u8 *state;
    s16 *table;
    s32 slotValue;
    s32 value;
    u16 *field;

    state = *(u8 **)0x03001ebc;
    table = Data_02000240;
    slotValue = *(s32 *)&table[250];
    if (slotValue != 0) {
        if ((s16)*(u16 *)(state + 382) >> 10 == slotValue) {
            if (GameFlag_IsSet(0x141) != 0) {
                field = (u16 *)(state + 386);
                value = 99;
                *field = value;
            }
        }
    }
}

void ColossoLogRollingStage_SelectNearestObstacle(void)
{
    extern s16 Data_02000240[];

    u8 *state;
    s16 *table;
    StageActor *target;
    StageActor *actor;
    s32 *frame;
    s32 active_slot;
    s32 slot;
    s32 best;
    s32 best_slot;
    s32 dx;
    s32 adx;
    s32 dz;
    s32 base;
    s32 z;

    state = *(u8 **)0x03001ebc;
    best_slot = 8;
    best = 0x100000;
    table = Data_02000240;
    active_slot = *(s32 *)&table[250];
    target = Func_02007198(active_slot);
    Event_Begin();
    for (slot = 8; slot <= 66; slot++) {
        actor = Func_020071a6(slot);
        if (actor == 0) {
            continue;
        }
        if (actor->state != 1) {
            continue;
        }
        if (*actor->sprite->entry != 165) {
            continue;
        }
        dx = (target->x - actor->x) / 65536;
        dz = (target->z - actor->z) / 65536;
        if (dz > 0) {
            continue;
        }
        adx = dx;
        if (adx < 0) {
            adx = -adx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (adx + dz < best) {
            best_slot = slot;
            best = adx + dz;
        }
    }
    Event_SetMessage(0x2085);
    Event_ShowMessage(best_slot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Event_Wait(20);
    Event_CloseScreen();
    Event_WaitForScreen();
    base = active_slot * 16;
    Func_02007200(base + 880, target->x >> 20);
    z = target->z >> 20;
    Func_02007210_b(base + 888, z);
    active_slot = active_slot + 1;
    if (active_slot > 3) {
        Event_RequestExit(10);
        GameFlag_Set(282);
    } else {
        Func_02004e9a(active_slot);
        Event_OpenScreen();
        Event_WaitForScreen();
        *frame = 0;
    }
    Event_End();
}

#define GetPartyInteractionRecord Func_020073ca_party
#define GetPartyMemberCount Func_0200739a_party

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

PartyInteractionRecord *Func_020073ca_party(void);
s32 Func_0200739a_party(void);

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

void RunPartyCountInteractionCopyB(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    Event_Begin();

    if (GetPartyMemberCount() <= 1) {
        Event_SetMessage(0x20e5);
        if (Event_AskYesNo(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            Actor_WalkTo(actorId, x, y + 0x40);
            Event_Wait(15);
            Actor_WalkToAndWait(0, x, y);
            Actor_WalkToAndWait(0, x, y + 0x20);
            Event_CloseScreen();
            Event_WaitForScreen();
            Event_RequestExit(11);
        }
    } else {
        Event_SetMessage(0x20e8);
        Event_ShowMessage(actorId, 0);
    }

    Event_End();
}

void ColossoLogRollingStage_ClearSavedActorPositions(void)
{
    Func_0200742a(896, 0);
    Func_02007434(904, 0);
    Func_0200743e(912, 0);
    Func_02007448(920, 0);
    Func_02007452(928, 0);
    Func_0200745c(936, 0);
}

s32 ColossoLogRollingStage_RunStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 Data_02000240[];

    s32 stage_variant;
    s32 script_id;
    s32 result;

    Func_0200760c();
    Func_0200741c(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Event_SetMessage(script_id);
    Event_ShowMessage(actor_handle, 0);
    if (GameFlag_IsSet(interaction_base + 512) != 0) {
        return 2;
    }
    if (GameFlag_IsSet(interaction_base + 520) != 0) {
        result = Func_0200747e(0);
        if (result == 1) {
            return 2;
        }
        if (result == 2 || result == -1) {
            return 3;
        }
        return result;
    }
    GameFlag_Set(interaction_base + 520);
    Event_SetMessage((s32)&Value_0000207c);
    Event_OpenMessage(actor_handle, 0);
    return Event_ChooseYesNo(0, 0);
}

void ColossoLogRollingStage_InitializeStateInteraction(s32 actor_handle, s32 interaction_base)
{
    extern s16 Data_02000240[];

    s32 stage_variant;
    s32 script_id;

    Func_020074d2(interaction_base, 5);
    stage_variant = Data_02000240[224];
    if (stage_variant == (s32)&Value_0000008f) {
        script_id = (s32)&Value_00002076;
    } else if (stage_variant == (s32)&Value_00000090) {
        script_id = (s32)&Value_00002078;
    } else {
        script_id = (s32)&Value_0000207a;
    }
    Event_SetMessage(script_id + 1);
    Event_ShowMessage(actor_handle, 0);
}


/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_020075b4_middle();
s32 Func_020075de_middle();
void Func_02007640_middle();
s32 Func_0200764c_a_middle();
s32 Func_0200764c_b_middle();
void Func_02007654_middle();
void Func_0200767e_middle();
void Func_02007690_middle();
s32 Func_0200772a_middle();
void Func_0200778a_a_middle();
void Func_0200778a_b_middle();
void Func_020077a0_middle();
void Func_020077bc_middle();
s32 Func_020077e2_middle();
s32 Func_02007816_middle();
void Func_020078fc_middle();

void Func_020077ae_middle();
/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    extern u8 Data_02000240[];
    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1(Func_020075de_middle, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_020075b4_middle);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Event_SetMessage(0x2083);
            Func_0200778a_a_middle(owner, 0);
            return;
        }
        if (GameFlag_IsSet(base + 512) != 0) {
            Event_SetMessage(0x2084);
            Func_0200778a_a_middle(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Task_Wait(6);
        } else {
            Event_SetMessage(0x207d);
            Event_OpenMessage(owner, 0);
            state = Value2(Func_0200764c_a_middle, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_02007640_middle((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_02007654_middle((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_02007816_middle);
            for (i = 0; i < count; i++) {
                Func_0200767e_middle((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02007690_middle((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Event_SetMessage(0x207e);
    Func_0200778a_a_middle(owner, 0);
    return;
L_main:
    ((void (*)())Func_0200764c_b_middle)(obj, 1);
    Call1(Func_0200778a_b_middle, 0x207f);
    Event_ShowMessage(owner, 0);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetSpeed(obj, 0x10000, 0x8000);
    Actor_SetSpeed(owner, 0x10000, 0x8000);
    record = Value1(Func_0200772a_middle, 0);
    if (record != 0) {
        Actor_SetPosition(obj, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    hi = p11 + 16;
    Actor_WalkToAndWait(obj, p9, hi);
    lo = p9 + 16;
    Value3(Engine_ActorWalkToAndWait, 0, lo, hi);
    Actor_FaceEachOther(obj, 0, 30);
    Actor_SetAnimation(obj, 3);
    tail = hi - 32;
    Actor_SetAnimationAndWait(0, 3);
    Actor_WalkToAndWait(owner, p9, tail);
    Value3(Engine_ActorWalkTo, owner, lo, tail);
    Func_020078fc_middle(0, obj);
    Actor_WalkToAndWait(obj, p9, tail);
    Actor_SetAnimation(owner, 1);
    Actor_FaceDirection(owner, 0x8000, 0);
    Actor_WalkToAndWait(obj, p9, p11 - 48);
    Actor_WalkToAndWait(owner, p9, tail);
    Actor_WalkToAndWait(owner, p9, p11);
    Func_020077a0_middle(obj);
    GameFlag_Set(base + 512);
    rec = Value1(Func_020077e2_middle, obj);
    sx = *(s32 *)(rec + 8) >> 20;
    Func_020077ae_middle((obj << 4) + 880, sx);
    sy = *(s32 *)(rec + 16) >> 20;
    Func_020077bc_middle((obj << 4) + 888, sy);
}

void ColossoLogRollingStage_ApplyItemToMatchingSlots(s32 handle, s32 item)
{
    u8 *record;
    s32 slot;

    record = Func_020077bc(handle);
    Func_020077ce(handle, item);

    for (slot = 0; slot <= 14; slot++) {
        if (*(u16 *)(record + 216 + slot * 2) == item) {
            Func_020077ea(handle, slot);
        }
    }
}

void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter)
{
    s32 handler;

    Data_0200dbd0 = (u16)mode;
    Data_0200dba4 = (u16)(parameter << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200aee9;
        Func_02007c38(task, budget);
    }

    handler = (s32)&Data_0200d9a6;
    if (mode == 2) {
        handler = (s32)&Data_0200ccba;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200d9d2;
    }
    if (mode == 3) {
        if (parameter != 0) {
            handler = (s32)&Data_0200cce2;
        } else {
            handler = (s32)&Data_0200da50;
        }
    }

    Data_0200dbdc = 0;
    Data_0200dbe0 = handler;
    Data_0200dc38 = 0;
    Data_0200dbac = 0;
    Data_0200dbb0 = 0;
}

/*
 * resource_3bc scripted transition owner at 0x02003468, 268 bytes including
 * alignment and its three-word pool.  Mode zero is the short opening; every
 * other mode runs the complete multi-stage transition and publishes flag
 * 0x123 when it closes.
 *
 * Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including every repeated Func_0808a010,
 * Audio_PlayCue, Func_02002e54, Func_020033d8, Func_0808a018/360/370/020
 * call) -- declared/named as the literal per-site targets, not the shared
 * ultimate-destination symbol.
 */
void ColossoLogRollingStage_RunScriptedTransition(s32 mode)
{
    if (mode == 0) {
        Event_Begin();
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(30);
        Audio_PlayCue(0x59);
        Func_020062e0(0);
        Func_0200686c(1, 0);
        Event_Wait(120);
        Event_End();
        return;
    }

    Audio_PlayCue(0xf7);
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    {
        s16 *base = (s16 *)0x0200d9a6;
        *(s16 *)((u8 *)base + 30) = (s16)(mode * 60);
    }
    Event_Wait(30);
    Audio_PlayCue(mode + 0x5a);
    Func_02006324(mode);
    Func_020068b0(1, 0);
    Event_Wait(120);

    goto check_transition;
wait_transition:
    Task_Wait(1);
check_transition:
    if (Func_02008092() != 0)
        goto wait_transition;

    Audio_PlayCue(0x121);
    Func_0200634e(5);
    Func_020068da(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Func_020068ee(2, 1);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Func_0200637c(6);
    Func_02006908(2, 0);
    Audio_PlayCue(0xec);
    Event_Wait(60);
    Func_02006396(7);
    Func_02006922(4, 0);
    Audio_PlayCue(0xed);
    Func_020080cc();
    Event_End();
    GameFlag_Set(0x123);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};
extern struct ModeRecord Data_0200da50;
extern struct ModeRecord Data_0200cce2;
void Func_0200640a_sequence();
void Func_02006436_sequence();
s32 Func_02006996_sequence();
s32 Func_020069c2_sequence();
s32 Func_020081b0_sequence();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

void FieldScene_RunScene3bcSequenceA(s32 a0)
{
    s32 kind;

    Audio_PlayCue(247);
    Event_OpenScreen();
    Event_WaitForScreen();
    Data_0200da50.span = a0 * 60;
    Data_0200cce2.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Event_Wait(30);
        Audio_PlayCue(86);
        Func_0200640a_sequence(8);
        Value2(Func_02006996_sequence, 3, 1);
        Event_Wait(-a0 * 60 + 60);
        kind = 0;
    } else {
        Event_Wait(30);
        Audio_PlayCue(a0 + 90);
        Func_02006436_sequence(4);
        Value2(Func_020069c2_sequence, 3, 0);
        Event_Wait(a0 * 60 + 60);
        kind = 8;
    }
    Actor_ShowEmote(kind, 0x105, 0);
    while (Value0(Func_020081b0_sequence)!= 0) {
        Task_Wait(1);
    }
    Audio_PlayCue(19);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Event_CloseScreen();
    Event_WaitForScreen();
}

void ColossoLogRollingStage_ResetActorMotion(s32 selector)
{
    u8 *record;

    record = Func_020082f8(selector);
    Func_020081e6();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

void ColossoLogRollingStage_EnsurePaletteHandle(void)
{
    s16 *cursor = &Data_0200dace;

    if (*cursor == -1) {
        *cursor = Func_0200829e();
    }
}

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Func_02007362(first_value, second_value, mode);

    Data_0200dc34 = (u16)first_value;
    Data_0200dbc0 = (u16)second_value;
    Data_0200db98 = (u16)(mode & 3);
    Data_0200dbb4 = 0;
    Data_0200dbcc = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_020082d4(task, budget);
    }
}

void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    Data_0200dba0 = (u16)first_value;
    Data_0200dc40 = (u16)second_value;
    Data_0200dbe4 = Data_0200dc34;
    Data_0200dbfc = Data_0200dbc0;
    Data_0200dbcc = (u16)mode;
    Data_0200db90 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_02008330(task, budget);
    }
}

void ColossoLogRollingStage_StopPaletteTask(void)
{
    Func_0200836e(Func_0200b91c);
    Func_020083c8(Data_0200dace);
    Data_0200dace = -1;
}

void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z)
{
    ScaledStageObject *object = Func_020086b0(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        Func_02008464(object);
        Object_SetAnimation(object, 5);
        Func_02008488(object, x << 16, object->y, z << 16);
    }
}

void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    u8 *object = Func_020086f0(object_id);

    if (object == 0) {
        return;
    }

    {
        s32 move_rate = 0x14000;
        u8 *state_byte = object;
        u8 zero = 0;
        *(s32 *)(object + 0x30) = move_rate;
        *(s32 *)(object + 0x34) = move_rate >> 1;
        state_byte += 0x5b;
        *state_byte = zero;
    }

    Func_020084a4();
    Object_SetAnimation(object, 5);
    Func_020084c8(object, x << 16, *(s32 *)(object + 12), z << 16);
    Func_020084d6(object);
    Object_SetAnimation(object, 1);
}

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void WriteU32AsHex(u8 *hex_text, u32 value)
{
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = Data_0200ce3c[value & 15];
        value >>= 4;
        hex_text--;
    }
}

void ColossoLogRollingStage_NoopSceneHook(void)
{
}

void ColossoLogRollingStage_SetBalanceStateReady(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}

void ColossoLogRollingStage_WaitForBalanceState(void)
{
    extern s16 Data_02001000;

    s16 *status = &Data_02001000;

    while (*status != 9) {
        Task_Wait(1);
    }
}

void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source)
{
    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Func_02008568();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Random_Next();
        radius = Random_Next();
        Func_020085a6(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Func_0200863a(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Actor_SetSpriteFlags(effect, 0);
            Object_SetScript(effect, (s32)Data_0200ce50);
            Object_SetAnimation(effect, 1);
            Object_SetAnimation(effect, 0);
        }
    }
}

s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect_02003d88 *source)
{
    StageEffect_02003d88 *effect = Func_020087b4(source->linked_effect_slot);

    Func_020086be(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Object_SetScript(effect, (s32)Data_0200ce74);
    Audio_PlayCue(83);
    source->linked_effect_slot = 0;
    return 0;
}

s32 ColossoLogRollingStage_PositionActiveActor(s32 first_handle, s32 second_handle)
{
    extern u8 Data_02000240[];

    u8 *workspace = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *record;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = GameFlag_IsSet(0x211);

    shared = Data_02000240;
    record = Func_02008818_a(*(s32 *)(shared + 500));

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        x = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        x = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(workspace + 236) + 0x100000;
        cuep = (u16 *)(workspace + 228);
    } else {
        z = *(s32 *)(workspace + 236) - 0x100000;
        cuep = (u16 *)(workspace + 226);
    }

    waitp = (s16 *)(record + 100);
    *waitp = *cuep;
    *(s32 *)(record + 52) = 0x4000;
    *(s32 *)(record + 48) = 0x10000;

    Func_0200876a(record, x, 0, z);
    Func_02008818_b(0x211);
    Object_SetScript(record, (void *)0x0200db24);

    while (*waitp != 0) {
        Task_Wait(1);
    }

    if (flag == 0) {
        Func_02006ca2(0, first_handle);
        Func_0200880a(first_handle, 2);
    } else {
        Func_02006cb4(0, second_handle);
        Func_0200881c(second_handle, 2);
    }

    shared = Data_02000240;
    Func_0200882c_a(*(s32 *)(shared + 500), 1);
    Message_ShowCentered(0x96a, 3);
    Func_020087ca(record);

    return flag;
}

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_02008d2e(59, 0x7170);
    handle = Func_02008d48(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_02008f08(first_actor);
    second_record = Func_02008f10(second_actor);

    if (GameFlag_IsSet(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02008dbe(Data_0200cd80, handle);

    extent = Func_02008de2();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Vram_Load((s16)extent, 512, handle);

    Func_02008d8e((s32)Func_0200c0d0 + 1, 0xc76);

    Func_02008ddc(handle);
}

void ColossoLogRollingStage_InitializeSceneControl(void)
{
    extern SceneControl Data_02001000;

    u8 *scene_state = Data_03001f3c;
    SceneControl *control = &Data_02001000;

    Func_02008e26(Func_02008e56(), (s32)(scene_state + 240));
    if (GameFlag_IsSet(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(scene_state + 224);
        control->timer = 0;
        control->phase = 0;
    }
    {
        s32 event_id = 0xc85;

        Func_02008dfa((s32)Data_0200bef1, event_id);
    }
}

void ColossoLogRollingStage_SetSceneControlValue(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}

s32 *FindActorAtWholeTilePosition(s32 *position)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 actor_index;

    for (actor_index = 8; actor_index <= 65; actor_index++) {
        s32 *actor = slots[actor_index];

        if ((position[0] >> 20) == (actor[2] >> 20)
            && (position[1] >> 20) == (actor[3] >> 20)
            && (position[2] >> 20) == (actor[4] >> 20)) {
            return actor;
        }
    }
    return 0;
}

void ColossoLogRollingStage_PushStagedActor(void)
{
    extern s16 Data_02000240[];

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 direction;
    Position3 position;
    u32 data_index = 250;
    s32 zero;
    s32 subject_handle;

    subject_handle = *(s32 *)((u8 *)Data_02000240 + (data_index << 1));
    subject = Func_0200905c(subject_handle);

    direction = subject->facing >> 12;

    step = Data_0200cfc0[direction];
    position.x = subject->x + (s32)(step & 0xffff0000);
    position.y = subject->y;
    step <<= 16;
    position.z = subject->z + (s32)step;

    target = Func_02008c56(&position, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    blocker = Func_02008c80(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    position.x = target->x;
    position.y = target->y + 0x100000;      /* 128 << 13 */
    position.z = target->z;

    blocker = Func_02008cac(&position, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200cfc0[direction];
    position.x = target->x + (s32)(step & 0xffff0000);
    position.y = target->y;
    step <<= 16;
    position.z = target->z + (s32)step;

    if (Func_0200905e(target, &position) > 0) {
        return;
    }

    Object_SetAnimation(subject, 8);
    Task_Wait(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_02009042(target, position.x, position.y, position.z);

    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_02009052(subject, position.x, position.y, position.z);

    Audio_PlayCue(0xee);
    Func_02009066(target);
    Audio_PlayCue(0x120);                                /* 144 << 1 */

    target->x = position.x;
    target->z = position.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Object_SetAnimation(subject, 1);
}

/* This overlay's own byte-exact occupancy lookup. */
s32 *ColossoLogRollingStage_FindActorAhead(void)
{
    extern s16 Data_02000240[];

    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Func_020091bc(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_02009046(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_02008dc6(position, record);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_02009078(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_02008df8(position, record);
    }

    return occupant;
}
