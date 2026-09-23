#include "MAKYURI_HEYA.H"

void Func_020067e8_wrapper();

/* Complete one-call wrapper through interworking return and alignment. */
void Func_0200646c_wrapper();

/* Complete one-call wrapper through interworking return and alignment. */
void Func_020064c0_wrapper();

/* Complete one-call wrapper through interworking return and alignment. */
void Func_02006364_wrapper();

/* Complete one-call wrapper through interworking return and alignment. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02002122_event();
void Func_020022e0_event();
void Func_020023fc_event();
void Func_02002424_event();
void Func_0200248c_event();
void Func_0200326a_event();
void Func_02003316_event();
void Func_02003324_event();
void Func_02005264_event();
void Func_02007500_a_event();
s32 Func_020075c2_event();
void Func_020075f4_event();
void Func_0200760e_event();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 t0;
    s32 t1;
} SceneEvent;

extern u8 SceneWorkCell[];

/*
 * resource_39c owner at 0x020051b0, 64 bytes.
 * Advances a frame counter and, while it is still within 31, reseeds the two
 * accumulators from the counter and re-anchors the position on the src
 * record. Returns whether the step ran.
 */
struct EffectAnchor {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

struct AnchoredEffect {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[4];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[68];
    s16 f100;                   /* +100 */
    u8 pad66[2];
    struct EffectAnchor *f104;       /* +104 */
};

extern u32 Data_03001e40;
u32 Func_02007a76_b(void);
u32 Func_02007a3e(void);
u32 Func_02007aac(void);
void Func_02002214(s32, s32, s32, s32, s32, s32, s32, s32 *);
void Func_0200224e(s32, s32, s32, s32, s32, s32, s32, s32 *);
void Func_02002288(s32, s32, s32, s32, s32, s32, s32, s32 *);
u32 Func_0200923a(void);
u32 Func_0200924e(void);
void Func_02003a32(s32, s32, s32, s32, s32, s32, s32, s32 *);
s32 *Func_02009548(s32);
u32 Func_02009426(void);
u32 Func_02009440(void);
u32 Func_02009452(void);
u32 Func_02009466(void);
void Func_02003c40(s32, s32, s32, s32, s32, s32, s32, s32 *);
u32 Func_020094e8(void);
u32 Func_02009502(void);
u32 Func_02009516(void);
void Func_02003cee(s32, s32, s32, s32, s32, s32, s32, s32 *);
u32 Func_0200958a(void);
u32 Func_0200959a(void);
void Func_02003d68(s32, s32, s32, s32, s32, s32, s32, s32 *);

/*
 * resource_39c palette-rotation owner at 0x02002648, complete 60-byte span
 * through its four-word pool. Once every eight frames, rotate the eight
 * BGR555 entries at palette indices 40-47 one place toward the front.
 */


/* Rotate seven adjacent palette entries once every eight frames. */

void Func_0200965a(s32, s32, s32 *);
u32 Func_02009646(void);
u32 Func_0200965c(void);
void Func_02003e2e(s32, s32, s32, s32, s32, s32, s32, s32 *);


extern u8 Value_02008cd1;

u8 *Func_02006a60(s32);
s32 Func_02006a9c(s32);
void Func_020069fa(s32, s32);


extern u8 Data_0200e010[];
extern u8 Value_00000874;

u8 *Func_02006efc(s32);
u8 *Func_02006f0e(s32);
u8 *Func_02006f2c(s32);
u8 *Func_02006f3a_a(s32);
u8 *Func_02006f3a_b(s32);
u8 *Func_02006f42(s32);
void Func_02006e90(s32, s32, s32);
s32 *Func_02007d9e(s32);
u32 Func_02007c64(void);
void Func_02005e06(s32, s32, s32);


extern s32 **Data_03001edc;
void Func_0200af6e_a(s32 *object, s32 arg1);
void Func_0200af6e_b(s32 *object, s32 arg1);

/*
 * Scene state interaction for resource_39c.  Data_03001edc is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Func_0200af6e_a and
 * Func_0200af6e_b are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSpawnedObject      Func_02000048
#define AcquireOverlayObject      Func_02005a2e
#define RunOverlayObjectCommand0  Func_02005aac
#define RunOverlayObjectCommand1  Func_02005ac4
#define RunOverlayObjectCommand14 Func_02005bfc
#define CreateOverlayObject Func_02005a86
#define SetOverlayObjectMode Func_02005b08
#define SetOverlayObjectSlot Func_02005c58

void *CreateOverlayObject(s32, s32, s32, s32);
void SetOverlayObjectMode(void *, s32);
void SetOverlayObjectSlot(void *, s32);

void *Func_02005a2e(s32, s32, s32, s32);
void Func_02005aac(void *, s32);
void Func_02005ac4(void *, s32);
void Func_02005bfc(void *, s32);
u8 *Func_0200ab38(s32 kind, s32 arg1, s32 arg2, s32 arg3);
void Func_0200ab3e(u8 *src, void *table);
void Func_0200ab50(u8 *rec, s32 arg1);
u8 **Func_0200b304(s32 group, s32 slot);
void Func_0200b33c(u8 *obj);

/*
 * Spawns a kind-24 object from three fields of the caller's object, sets
 * three of its byte fields, then ORs two low bits into its owner record's
 * flag byte -- a sibling routine in this overlay clears those bits instead,
 * and the two are deliberately not unified. The 88-byte owner includes its
 * one pool word, 0x0200de08, which is never dereferenced here and so stays
 * a raw literal. The field offsets are named by position only.
 */

/* Release the scene object's optional attachment and clear its slot. */

extern u8 *Data_03001ebc;
extern u8 Data_0200e064[];

void Func_0200689c(s32, s32);
void Func_02001ff8(s32);
void Func_02002018(s32);
void Func_02002038(s32);
s32 *Func_020076cc(s32);
s32 *Func_020076de(s32);
s32 *Func_020076fc(s32);
s32 *Func_0200770a(s32);
s32 *Func_02007712(s32);
void Func_02007660(s32, s32, s32);
void Func_02007708(s32);
s32 *Func_020080e0(s32);
void Func_020080b0(s32);

/*
 * resource_39c owner at 0x02000cc0, 16 bytes: invoke one fixed two-value
 * setup call with 12 and 21.
 *
 * The executable owner runs from `push {lr}` at 0x02000cc0 through the
 * void interworking return `pop {r0} / bx r0` at 0x02000cca-0x02000ccc.
 * m2c_guard independently measures those 14 code bytes and finds no
 * seeding hazard.  The zero halfword at 0x02000cce is alignment before
 * the next owner's prologue at 0x02000cd0, making the whole span 16 bytes.
 * Its sole call resolves through the overlay veneer to Func_0808a258.
 * The constants' game-level meanings are not established by this owner.
 */

void Func_02008354(s32, s32);

/* Complete scene-entry wrapper through return, alignment and its sole pool. */

extern u8 Data_02b20000[];
void Func_020017b8(void);
void Func_02002838(void);
void Func_020052c0(s32, s32, s32);
void Func_020028b4(void);
void Func_02004a2c(void);


extern s16 Data_02000240[];
extern u8 Value_00000039;
extern u8 Value_00000038;
extern u8 Value_00000037;
extern u8 Data_0200e50c[];
extern u8 Data_0200e35c[];
extern u8 Data_0200e23c[];
extern u8 Data_0200e0ec[];

extern s16 Data_02000240[];
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Value_00000039;
extern u8 Data_0200e700[];
extern u8 Data_0200e7a8[];
extern u8 Data_0200e838[];
extern u8 Data_0200e988[];
extern u8 Data_0200e6e8[];

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */


/* Complete eight-byte literal-address getter, including its sole pool word. */


extern s16 Data_02000240[];
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Data_0200ead8[];
extern u8 Data_0200ec10[];
extern u8 Data_0200ed60[];
extern u8 Data_0200eec8[];

extern u8 Data_0200a2c5[];

void Func_02007c7a(s32, s32);
void Func_02007c96(s32);


/*
 * resource_39c owner at 0x02004f90, 60 bytes.
 * Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names.
 */
struct SceneLookupRecord {
    u8 pad00[216];
    u16 tbl[1];                 /* +216 */
};
void Func_02006fc4();
s32 Func_02006ff4();
struct FieldActor *Func_02007058();
struct FieldActor *Func_02007068();
struct FieldActor *Func_02007070();
s32 Func_0200aa18();
s32 Func_0200aa20();
struct SceneLookupRecord *Func_0200aa1c();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200702a_rows();
void Func_02007040_rows();
void Func_0200705a_rows();
void Func_0200706e_rows();
void Func_020070ba_rows();
void Func_020070c8_rows();
void Func_0200713e_rows();
void Func_020071be_rows();
void Func_02007262_rows();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02006b4c_progress();
s32 Func_02006b56_progress();
s32 Func_02006b60_progress();
void Func_02006b72_progress();
void Func_02006b7a_progress();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02006f48_actor_gate();
s32 Func_02006f58_actor_gate();
void Func_02006f90_actor_gate();
struct FieldActor *Func_02006fb2_a_actor_gate();
struct FieldActor *Func_02006fbc_actor_gate();
struct FieldActor *Func_02006fe4_actor_gate();
struct FieldActor *Func_02006ff2_actor_gate();
struct FieldActor *Func_02006ffa_actor_gate();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02007100_opening();
void Func_02007114_opening();
void Func_0200712a_opening();
void Func_02007176_opening();
void Func_02007184_opening();
void Func_02007192_opening();
void Func_020071be_opening();
void Func_020071fa_opening();
void Func_0200733a_opening();
void Func_020073c6_opening();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

#define BattleRuntime_Reset_1(args...) Engine_EventBegin(args)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_2(a0, a1, a2) Call3(Engine_ActorSetSpeed, a0, a1, a2)
#define SceneWork_SetStepValue_1(a0) Call1(Engine_EventSetMessage, a0)
#define BattleRuntime_RunThenWaitIfModeZero_1(args...) Engine_EventShowMessageAndWait(args)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020081c6_tail, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_1(a0, a1, a2) Call3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ArmCallback_1(a0, a1, a2) Call3(Func_02008266_tail, a0, a1, a2)
#define Object_SetModeById_1(args...) Engine_ActorSetAnimation(args)
#define Scene_GetRecord_1(args...) Func_020081b4_tail(args)
#define BattleRuntime_WaitIfModeZero_1(args...) Engine_EventWait(args)
#define BattleRuntime_RunThenWaitIfModeZero_2(args...) Engine_EventShowMessageAndWait(args)
#define Object_SetModeById_2(args...) Engine_ActorSetAnimation(args)
#define Scene_GetRecord_2(args...) Func_020081d6_tail(args)
#define BattleRuntime_WaitIfModeZero_2(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_2(a0, a1, a2) Call3(Func_020082ae_tail, a0, a1, a2)
#define ObjectMotion_SetSpeedParameters_3(a0, a1) Value2(Func_020082a6_tail, a0, a1)
#define UiWork_WaitThenFinalizeCapacity_1(a0, a1) Value2(Engine_EventChooseYesNo, a0, a1)
#define BattleRuntime_WaitIfModeZero_3(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_1(args...) Func_02008270_a_tail(args)
#define BattleRuntime_RunThenWaitIfModeZero_3(args...) Engine_EventShowMessageAndWait(args)
#define BattleRuntime_WaitIfModeZero_4(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_2(args...) Func_020082aa_tail(args)
#define BattleRuntime_RunThenWaitIfModeZero_4(args...) Engine_EventShowMessageAndWait(args)
#define BattleRuntime_WaitIfModeZero_5(args...) Engine_EventWait(args)
#define ObjectMotion_ArmCallback_3(a0, a1, a2) Call3(Func_02008326_tail, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Call2(Engine_CameraSetSpeed, a0, a1)
#define ObjectMotion_PlaceWithinCameraBounds_1(a0, a1, a2, a3) Call4(Engine_CameraMoveTo, a0, a1, a2, a3)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_020082c6_tail, a0, a1, a2)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Engine_CameraWaitForMove(args)
#define BattleRuntime_WaitIfModeZero_6(args...) Engine_EventWait(args)
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1(args...) Func_02008318_tail(args)
#define BattleRuntime_WaitIfModeZero_7(args...) Engine_EventWait(args)
#define ObjectMotion_CallThenWaitForAnimationChange_3(args...) Func_0200830e_tail(args)
#define BattleRuntime_WaitIfModeZero_8(args...) Engine_EventWait(args)
#define BattleRuntime_RunThenWaitIfModeZero_5(args...) Engine_EventShowMessageAndWait(args)
#define GameFlag_Set_1(a0) Call1(Func_0200827e_b_tail, a0)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1(args...) Engine_EventEnd(args)
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000

extern u8 Data_00000036[];
extern s16 Data_02000240_t[][1];
void Func_02001f88_tail(void);
void Func_020037d8_tail(void);
void Func_02007798_tail();
s32 Func_020077c8_tail();
struct FieldActor *Func_0200782c_tail();
struct FieldActor *Func_0200783c_tail();
struct FieldActor *Func_02007844_tail();
s32 Func_020081b4_tail();
void Func_020081c6_tail();
s32 Func_020081d6_tail();
void Func_02008266_tail();
void Func_02008270_a_tail();
void Func_0200827e_b_tail();
s32 Func_020082a6_tail();
void Func_020082aa_tail();
void Func_020082ae_tail();
void Func_020082c6_tail();
void Func_0200830e_tail();
void Func_02008318_tail();
void Func_02008326_tail();
s32 Func_02009096_tail(s32);
s32 Func_020090aa_tail(s32);
void Func_0200860a_tail(s32, s32);
s32 Func_020090fc_tail(s32);
void Func_020090da_tail(s32, s32);
void Func_020090e2_a_tail(s32, s32);
void Func_0200912a_tail(s32);
s32 Func_02003756_tail();
void Func_02009128_tail();
struct FieldActor *Func_020091ca_tail();
struct FieldActor *Func_020091d6_tail();
void Func_020091de_tail();
struct FieldActor *Func_020091fc_tail();
void Func_02009272_tail();
void Func_02009296_tail();
void Func_020092be_tail();
s32 Func_0200921a_tail();
void Func_0200920e_tail();
struct FieldActor *Func_02009262_tail();
void Func_02009248_tail();
struct FieldActor *Func_020092b0_tail();
s32 Func_020092f2_tail();
struct FieldActor *Func_020092ea_tail();
void Func_02009454_b_tail();
void Func_02009482_tail();
void Func_0200949c_tail();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Pointer to the shared scene work record. */

/* Values below that look like 16.16 fixed-point fractions, named by that
 * fraction's decimal value. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020036ac(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02003788(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02004f44(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Sets up actor 3 and actor 20, branches on the result of a query call
 * against actor 0, then runs a shared closing sequence over both actors. */

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


/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void Func_02001998_run_scene_supplemental_sequence_one();
void Func_02002e8e_run_scene_supplemental_sequence_one();
void Func_02002ea4_run_scene_supplemental_sequence_one();
void Func_020053a2_run_scene_supplemental_sequence_one();
s32 Func_02007258_run_scene_supplemental_sequence_one();
void Func_0200731c_run_scene_supplemental_sequence_one();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void Func_02006a5e_run_scene_39c_02000ffc();
s32 Func_02006b00_run_scene_39c_02000ffc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */


/* The scene step counter at 0x1d8 of the shared scene work record. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

s32 Func_02006cd6_run_scene_39c_02001340();
void Func_02006d7e_run_scene_39c_02001340();
struct FieldActor *Func_02006e42_run_scene_39c_02001340();
struct FieldActor *Func_02006eb6_run_scene_39c_02001340();
void Func_02006ebe_run_scene_39c_02001340();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */


void Func_02000e7a_run_scene_sequence_c();

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void Func_02001aa6_run_scene_sequence_f();
void Func_02007326_run_scene_sequence_f();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth)
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

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        SetOverlayObjectMode(result, 0);
        SetOverlayObjectSlot(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}


void SceneState_ApplyPair12And21(void)
{
    Func_0200689c(12, 21);
}

s32 FieldScene_RunScene39b_02000cc0(struct FieldActor *actor)
{
    struct {
        s32 priority;
        s32 palette;
        s32 start_scale_x;
        s32 start_scale_y;
        u8 unknown_10[24];
    } options;
    s32 velocity_y;

    options.palette = 7;
    if ((Data_03001e40 & 1) == 0) {
        options.palette = 5;
    }
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    options.priority = 0;
    velocity_y = -((((u32)Engine_RandomNext() * 8) >> 16) * 0x3333);
    Func_02000e7a_run_scene_sequence_c(actor->x.fixed + ((8 - (Data_03001e40 & 15)) << 16),
                                       actor->y.fixed + 0x1a0000, actor->z.fixed, 0, velocity_y, 0,
                                       EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                                       &options);
    return 0;
}

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
    s16 selector = Data_02000240[224];

    if (selector == (s32)&Value_00000039) {
        return (s32)Data_0200e50c;
    }
    if (selector == (s32)&Value_00000038) {
        return (s32)Data_0200e35c;
    }
    if (selector == (s32)&Value_00000037) {
        return (s32)Data_0200e23c;
    }
    return (s32)Data_0200e0ec;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableE614(void)
{
    return (u8 *)0x0200e614;
}

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000036) {
        return (s32)Data_0200e700;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)Data_0200e7a8;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)Data_0200e838;
    }
    if (v == (s32)&Value_00000039) {
        return (s32)Data_0200e988;
    }
    return (s32)Data_0200e6e8;
}

void FieldScene_CallHelper67e8(void)
{
    Func_020067e8_wrapper();
}

void FieldScene_Forward646c(void)
{
    Func_0200646c_wrapper();
}

void FieldScene_RunSingleStep(void)
{
    Func_020064c0_wrapper();
}

void FieldScene_CallHelper6364(void)
{
    Func_02006364_wrapper();
}

void SceneDialogue_RunLine1637(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered(0x1637, 1);
    Engine_EventEnd();
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 selector = Data_02000240[224];

    if (selector == (s32)&Value_00000036) {
        return (s32)Data_0200ead8;
    }
    if (selector == (s32)&Value_00000037) {
        return (s32)Data_0200ec10;
    }
    if (selector == (s32)&Value_00000038) {
        return (s32)Data_0200ed60;
    }
    return (s32)Data_0200eec8;
}

void SceneActor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Func_02006a60(0);
    Engine_EventBegin();
    Engine_AudioPlayCue(0xe4);
    FIELD_AT_OFFSET(v, s32, 0x6c) = (s32)&Value_02008cd1;
    FIELD_AT_OFFSET(v, s32, 0x30) = 0x3333;
    Engine_ActorSetAnimation(0, 2);
    Engine_ActorSetDestinationOffset(0, 0, -6);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetChildValue(0, 15);
    Func_020069fa(Func_02006a9c(0), 0);
    FIELD_AT_OFFSET(v, s32, 0x6c) = 0;
    Engine_EventWait(30);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(a);
    Engine_EventEnd();
}

void FieldScene_RunScene39c_02000ffc(s32 a0)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_AudioPlayCue(228);
    Call3(Engine_ActorSetSpeed, 0, 0x6666, 0x3333);
    Engine_ActorSetSpritePriority(0, 2);
    Call3(Engine_ActorSetDestinationOffset, 0, 0, -8);
    record = Func_02006b00_run_scene_39c_02000ffc(0);
    Func_02006a5e_run_scene_39c_02000ffc(record, 0);
    Engine_EventWait(8);
    Engine_ActorSetPosition(0, ((a0 << 19) + 0x80000), 0);
    Engine_EventWait(30);
}

void SceneState_ApplyWork16cMinus50A(void)
{
    Func_02001ff8(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}

void SceneState_ApplyWork16cMinus50(void)
{
    Func_02002018(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}

void SceneState_ApplyWork16cMinus50B(void)
{
    Func_02002038(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}

void FieldScene_RunScene39c_020010c0(void)
{
    if (Value1(Func_02006b4c_progress, 0x310) != 0
        && Value1(Func_02006b56_progress, 0x311) != 0
        && Value1(Func_02006b60_progress, 0x312) != 0) {
        Call1(Func_02006b72_progress, 0x876);
        Engine_EventWait(30);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
        Engine_AudioPlayCue(141);
        Engine_EventWait(60);
        *(s32 *)(Data_03001ebc + 0x1c0) = 0x100;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Call1(Engine_AudioPlayCue, 0x121);
        Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        Func_02006b7a_progress();
        Engine_EventRequestExit(13);
    } else {
        Call1(Engine_GameFlagClear, 0x876);
    }
}

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void FieldScene_RunScene39c_02001340(void)
{
    u8 *work;

    work = Data_03001ebc;
    Engine_EventBegin();
    Value2(Func_02006cd6_run_scene_39c_02001340, 0x200ba65, 0xc80);
    Call3(Engine_ActorSetSpeed, 0, 0x28000, 0x14000);
    Engine_ActorSetAnimation(0, 1);
    Func_02006e42_run_scene_39c_02001340(0)->unknown_5a &= 254;
    Engine_AudioPlayCue(228);
    if (*(s16 *)(work + 0x16c) == 2) {
        Call3(Engine_ActorSetDestination, 0, 232, 616);
    } else if (*(s16 *)(work + 0x16c) == 3) {
        Call3(Engine_ActorSetDestination, 0, 360, 728);
    } else if (*(s16 *)(work + 0x16c) == 4) {
        Call3(Engine_ActorSetDestination, 0, 248, 792);
    } else {
        Call3(Func_02006ebe_run_scene_39c_02001340, 0, 696, 592);
        Call3(Engine_ActorSetDestination, 0, 696, 600);
        Call1(Engine_EventWait, 30);
    }
    Engine_ActorWaitForMove(0);
    SetFlagBits(&Func_02006eb6_run_scene_39c_02001340(0)->unknown_5a, 1);
    Call1(Func_02006d7e_run_scene_39c_02001340, 0x200ba65);
    Engine_EventEnd();
}

void FieldScene_RunFourCallSequence(void)
{
    Engine_EventBegin();
    Func_020017b8();
    Engine_EventEnd();
    Func_02002838();
}

void FieldScene_RunActorElevenAtTile5And13(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = FIELD_AT_OFFSET(Func_02006efc(11), s32, 8) / 0x100000;
    y = FIELD_AT_OFFSET(Func_02006f0e(11), s32, 16) / 0x100000;
    Engine_EventBegin();
    if (x == 5 && y == 13) {
        FIELD_AT_OFFSET(Func_02006f2c(11), s32, 12) += 0xfffe0000;
        p = Func_02006f3a_a(11);
        FIELD_AT_OFFSET(p, s32, 0x3c) = FIELD_AT_OFFSET(Func_02006f42(11), s32, 12);
        Engine_MapCopyCellsTo(5, 2, 5, 11, 1, 1);
        Engine_AudioPlayCue(0xd9);
        Func_02006e90((s32)Data_0200e010, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Engine_MapCopyCellAttributes(9, 5, 1, 1, s0, s1);
        }
        Func_02006f3a_b((s32)&Value_00000874);
    }
    Engine_EventEnd();
}

void FieldScene_RunScene39c_020014cc(void)
{
    s32 x;
    s32 z;
    struct FieldActor *actor;

    if (Value1(Func_02006f58_actor_gate, 0x256) == 0) {
        x = Func_02006fb2_a_actor_gate(0)->x.part.pixel;
        z = Func_02006fbc_actor_gate(0)->z.part.pixel;
        if ((u32)(x - 84) <= 7 && z > 211 && z <= 219) {
            Engine_EventBegin();
            Call1(Func_02006f90_actor_gate, 0x256);
            Engine_EventWait(5);
            Func_02006fe4_actor_gate(0)->y.fixed += -0x20000;
            actor = Func_02006ff2_actor_gate(0);
            FIELD_AT_OFFSET(actor, s32, 0x3c) = Func_02006ffa_actor_gate(0)->y.fixed;
            Call6(Engine_MapCopyCellsTo, 5, 2, 5, 11, 1, 1);
            Engine_AudioPlayCue(217);
            Call3(Func_02006f48_actor_gate, 0x200e010, 9, 7);
            Engine_EventEnd();
        }
    }
}

void Func_02001568(void)
{
    struct FieldActor *actor;

    if (Value1(Func_02006ff4, 0x256) != 0) {
        Engine_EventBegin();
        Call1(Engine_GameFlagClear, 0x256);
        Func_02007058(0)->y.fixed += 0x20000;
        actor = Func_02007068(0);
        FIELD_AT_OFFSET(actor, s32, 0x3c) = Func_02007070(0)->y.fixed;
        Engine_EventWait(5);
        Call6(Engine_MapCopyCellsTo, 7, 2, 5, 11, 1, 1);
        Engine_AudioPlayCue(217);
        Call3(Func_02006fc4, 0x200e03a, 9, 7);
        Engine_EventEnd();
    }
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 v;

    if (a0 != 0) {
        Engine_AudioPlayCue(219);
    }
    i = (u32)a1;
    if (i < (u32)a2) {
        do {
            Call6(Engine_MapCopyCellsTo, 45 - (s32)(i << 1), 32, 44 - (s32)(i << 1), 32, (s32)(i + 1), 6);
            Call6(Engine_MapCopyCellsTo, 45 - (s32)i, 51, 45 - (s32)i, 32, 1, 6);
            v = 109 - (s32)i;
            Call6(Engine_MapCopyCellsTo, v, 32, 108 - (s32)i, 32, 1, 4);
            Call6(Engine_MapCopyCellsTo, v, 51, v, 32, 1, 4);
            if (a0 != 0) {
                Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
                Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
                Engine_EventWait(a0);
            }
            i = i + 1;
        } while (i < (u32)a2);
    }
    Call6(Engine_MapCopyCellAttributes, 42, 52, 4, 5, 42, 33);
}

void Func_020016c4(s32 a0)
{
    u32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 six;
    s32 pos;
    s32 four;
    Engine_AudioPlayCue(219);
    six = 6;
    i = 0;
    x = 41;
    y = 40;
    do {
        Call6(Engine_MapCopyCellsTo, y, 32, x, 32, 3 - (s32)i, six);
        k = 2;
        Call6(Engine_MapCopyCellsTo, 39, 51, y, 32, 1, six);
        pos = (s32)i + 106;
        four = 4;
        Engine_MapCopyCellsTo(105, 51, pos, 32, k, four);
        if (a0 != 0) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_EventWait(a0);
        }
        i = i + 1;
        x = x + 2;
        y = y + 2;
    } while (i <= 2);
    Call1(Engine_AudioPlayCue, 0x120);
    Call6(Engine_MapCopyCellAttributes, 106, 33, 4, 5, 42, 33);
    Func_020071be_rows();
}

void FieldScene_ApplyTable2b20000(void)
{
    Func_020052c0((s32)Data_02b20000, 0, 0x2480000);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 *rec;
    s32 outer;
    s32 shift4;
    s32 v8;
    s32 inner;
    s32 neg;
    s32 nsh;
    s32 va0;
    s32 vb0;
    s32 va;
    s32 vb;
    s32 next;
    s32 slot20[10];

    Call6(Engine_MapCopyCellsTo, 78, 59, 110, 36, 1, 1);
    Call6(Engine_MapCopyCellsTo, 76, 59, 109, 36, 1, 1);
    rec = slot20;
    rec[1] = 7;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        shift4 = (outer << 4);
        inner = 0;
        nsh = -(outer << 20);
        v8 = (0x2d80000 + nsh);
        do {
            if ((inner & 1) != 0) {
                va0 = Engine_RandomNext();
                va = (((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                neg = -inner;
                vb0 = Engine_RandomNext();
                vb = (((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                Func_02001998_run_scene_supplemental_sequence_one(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
                Engine_EventWait(1);
            } else {
                neg = -inner;
            }
            Func_020053a2_run_scene_supplemental_sequence_one(((s32)((s32)(neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
            inner = (inner + 1);
            v8 = (v8 + -0x10000);
        } while ((u32)inner <= 7);
        Call6(Engine_MapCopyCellsTo, 76, 59, (108 - outer), 36, 2, 1);
        next = outer + 1;
        Func_02002e8e_run_scene_supplemental_sequence_one(a0, outer, next);
        outer = next;
    } while ((u32)next <= 1);
    Engine_EventWait(a0);
    Func_02002ea4_run_scene_supplemental_sequence_one(0, next, (next + 1));
    Engine_AudioPlayCue(211);
    Value2(Func_02007258_run_scene_supplemental_sequence_one, 0x2009791, 0xc80);
    Func_0200731c_run_scene_supplemental_sequence_one();
}

void Func_020018f8(void)
{
    s32 *rec;
    s32 outer;
    s32 inner;
    s32 base;
    s32 raw;
    s32 pos;
    s32 shown;
    s32 arr[10];

    Call6(Engine_MapCopyCellsTo, 78, 58, 110, 36, 1, 1);
    rec = arr;
    rec[1] = 5;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        base = -0x20000;
        inner = 1;
        do {
            if ((inner & 1) != 0) {
                raw = Value0(Engine_RandomNext);
                shown = ((0x248 - (s32)((u32)((raw << 2) + raw) >> 16)) & 0xffff) << 16;
                pos = (base - (outer << 19)) + 0x2d80000;
                Func_02001aa6_run_scene_sequence_f(pos, 0, shown, -0x4000, 0, 0, 0x90000, (s32)rec);
                Engine_EventWait(1);
            }
            inner = inner + 1;
            base = base + -0x20000;
        } while ((u32)inner <= 7);
        Call6(Engine_MapCopyCellsTo, 111, 35, (109 - outer), 36, 1, 1);
        outer = outer + 1;
    } while ((u32)outer <= 2);
    Call1(Func_02007326_run_scene_sequence_f, 0x2009791);
}

void Func_020019b8(void)
{
    SceneEvent ev;
    s32 kind;
    s32 shape;
    s32 three;

    Engine_EventBegin();
    if (Func_02002122_event(&ev) != 0) {
        kind = ev.f1;
        if (kind == 8) {
            shape = ev.f2;
            if ((shape >> 20) == 11) {
                Func_020022e0_event(ev);
                Engine_EventWait(30);
                Engine_AudioPlayCue(211);
                Func_02005264_event();
                three = 3;
                Engine_MapCopyCellsTo(76, 60, 74, 38, three, 1);
                Engine_MapCopyCellsTo(77, 60, 76, 38, 2, 1);
                Engine_MapCopyCellsTo(75, 58, 86, 41, 1, three);
                Engine_MapCopyCellsTo(75, 59, 86, 43, 1, 2);
                Engine_MapCopyCellsTo(76, 59, 80, 49, 2, 1);
                Engine_MapCopyCellsTo(77, 59, 82, 49, 2, 1);
                Func_02007500_a_event(0x302);
            } else {
                ev.t1 = 0x200b949;
                Engine_MapCopyCellsTo(75, 57, 86, 41, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 86, 42, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 86, 43, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 86, 44, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 80, 49, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 81, 49, 1, 1);
                Engine_MapCopyCellsTo(71, 59, 82, 49, 1, 1);
                Engine_MapCopyCellsTo(78, 58, 83, 49, 1, 1);
                Func_020023fc_event(ev);
                Engine_GameFlagClear(0x302);
            }
        } else if (kind == 10) {
            if ((ev.t0 >> 20) == 40) {
                Func_02002424_event(ev);
                if (Value1(Func_020075c2_event, 0x307) == 0) {
                    Engine_CameraSetSpeed(0x18000, 0x3000);
                    Engine_CameraMoveTo(0x2ca0000, -1, 0x2500000, 1);
                    Engine_CameraWaitForMove();
                    Call1(Func_020075f4_event, 0x307);
                    Func_02003316_event(5);
                    Engine_EventWait(50);
                } else {
                    Func_02003324_event(5);
                }
                Func_0200760e_event(0x306);
            } else if ((ev.t0 >> 20) == 42) {
                ev.t1 = 0x20098f9;
                Func_0200248c_event(ev);
                Func_0200326a_event(5);
                Engine_GameFlagClear(0x306);
            }
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunFourSteps(void)
{
    Engine_EventBegin();
    Func_02001f88_tail();
    Engine_EventEnd();
    Func_020037d8_tail();
}

void SceneState_RunWhenActor8AtTile10x23(void)
{
    s32 x = Func_020076cc(8)[2] / 0x100000;
    s32 y = Func_020076de(8)[4] / 0x100000;

    Engine_EventBegin();
    if (x == 10 && y == 23) {
        s32 *p;
        Func_020076fc(8)[3] += 0xfffe0000;
        p = Func_0200770a(8);
        p[15] = Func_02007712(8)[3];
        Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
        Engine_AudioPlayCue(0xd9);
        Func_02007660((s32)Data_0200e064, 10, 18);
        Engine_MapCopyCellAttributes(10, 16, 1, 1, x, 19);
        Func_02007708(0x878);
    }
    Engine_EventEnd();
}

void FieldScene_RunScene39cSequenceA(void)
{
    struct FieldActor *actor;

    if (Value1(Func_020077c8_tail, 0x256) != 0) {
        Engine_EventBegin();
        Call1(Engine_GameFlagClear, 0x256);
        Func_0200782c_tail(0)->y.fixed += 0x20000;
        actor = Func_0200783c_tail(0);
        FIELD_AT_OFFSET(actor, s32, 0x3c) = Func_02007844_tail(0)->y.fixed;
        Engine_EventWait(5);
        Call6(Engine_MapCopyCellsTo, 8, 29, 10, 23, 1, 1);
        Engine_AudioPlayCue(217);
        Call3(Func_02007798_tail, 0x200e08e, 10, 18);
        Engine_EventEnd();
    }
}

void SceneEffect_SpawnParticleRowsByMode(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Engine_MapCopyCellsTo(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Engine_MapCopyCellsTo(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Engine_MapCopyCellsTo(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Engine_AudioPlayCue(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Func_02002214((0x319 - ((Func_02007a3e() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Func_0200224e((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Func_02007a76_b)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Func_02002288(0x3380000 - (i << 17) - (j << 19), 0,
                                  (((Func_02007aac() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Engine_EventWait(1);
            }
        }
        if (mode == 0)
            Engine_MapCopyCellsTo(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Engine_MapCopyCellsTo(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Engine_MapCopyCellsTo(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}

void SceneActor_UseActorNinePositionWithYOffset(void)
{
    s32 *p = Func_02007d9e(9);
    u32 v = Func_02007c64();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    Func_02005e06(p[2], b, c);
}

s32 SceneData_LoadBlockA2c5(void)
{
    s32 n = 0xc80;

    Func_02007c7a((s32)Data_0200a2c5, n);
    return 0;
}

s32 SceneData_ApplyTableA2c5AndReturnZero(void)
{
    Func_02007c96((s32)Data_0200a2c5);
    return 0;
}

void FieldScene_RunFourStepSequence(void)
{
    Engine_EventBegin();
    Func_020028b4();
    Func_02004a2c();
    Engine_EventEnd();
}

void SceneState_ApplyRectWhenActor20AtColumn28(void)
{
    s32 col;

    Engine_EventBegin();
    col = Func_020080e0(20)[2] / 0x100000;
    if (col == 28) {
        Func_020080b0(840);
        {
            s32 a = 31;
            s32 b = 20;

            Engine_MapCopyCellAttributes(29, 20, 1, 1, a, b);
        }
    }
    Engine_EventEnd();
}

void SceneEffect_RotatePaletteEntries40To47(void)
{
    unsigned int index;
    u16 *dst;
    u16 *src;
    u32 front;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (u16 *)0x05000050;
    front = *dst;
    index = 0;
    *(u16 *)0x0500005e = front;

    src = (u16 *)0x05000052;
    while (index <= 6) {
        *dst++ = *src++;
        index++;
    }
}

void FieldScene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    BattleRuntime_Reset_1();
    ObjectMotion_SetSpeedParameters_1(3, FX16_0_8, FX16_0_4);
    ObjectMotion_SetSpeedParameters_2(0, FX16_0_8, FX16_0_4);
    SceneWork_SetStepValue_1(0x1577);
    BattleRuntime_RunThenWaitIfModeZero_1(3, 0, 20);
    ObjectMotion_SetPositionAndReset_1(3, 0x348, 0x288);
    BattleEffect_SpawnLinkedResourceObject_1(3, 0x100, 60);
    ObjectMotion_ArmCallback_1(3, FX16_0_5, 20);
    Object_SetModeById_1(3, 16);
    record = Scene_GetRecord_1(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    BattleRuntime_WaitIfModeZero_1(20);
    BattleRuntime_RunThenWaitIfModeZero_2(3, 0, 20);
    Object_SetModeById_2(3, 1);
    record = Scene_GetRecord_2(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    BattleRuntime_WaitIfModeZero_2(20);
    ObjectMotion_ArmCallback_2(3, FX16_0_25, 20);
    ObjectMotion_SetSpeedParameters_3(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        BattleRuntime_WaitIfModeZero_3(20);
        ObjectMotion_CallThenWaitForAnimationChange_1(3, 3);
        BattleRuntime_RunThenWaitIfModeZero_3(3, 0, 20);
        *(u16 *)((SCENE_WORK + 0x1d8)) += 1;
    } else {
        *(u16 *)((SCENE_WORK + 0x1d8)) += 1;
        BattleRuntime_WaitIfModeZero_4(20);
        ObjectMotion_CallThenWaitForAnimationChange_2(3, 4);
        BattleRuntime_RunThenWaitIfModeZero_4(3, 0, 20);
    }
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_ArmCallback_3(3, FX16_0_75, 20);
    ObjectMotion_SetSpeedLimitAndAcceleration_1(FX16_0_8, FX16_0_1);
    ObjectMotion_PlaceWithinCameraBounds_1(0x3480000, -1, 0x2780000, 1);
    ObjectMotion_SetPositionAndReset_2(3, 0x348, 0x278);
    Object_CommitPositionThenWaitIfModeZero_1();
    BattleRuntime_WaitIfModeZero_6(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(3, 2);
    BattleRuntime_WaitIfModeZero_7(10);
    ObjectMotion_CallThenWaitForAnimationChange_3(3, 4);
    BattleRuntime_WaitIfModeZero_8(20);
    BattleRuntime_RunThenWaitIfModeZero_5(3, 0, 20);
    GameFlag_Set_1(0x870);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void SceneDialogue_RunActor3TimedLine(void)
{
    Engine_EventBegin();
    Func_02008354(3, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage(0x157d);
    Engine_EventShowMessageAndWait(3, 0, 20);
    Engine_EventEnd();
}

void SceneEffect_RotatePaletteEntries97To103(void)
{
    unsigned int i;
    u16 *dst;
    u16 *src;
    u32 first;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (u16 *)0x050000c2;
    first = *dst;
    i = 0;
    *(u16 *)0x050000ce = first;

    src = (u16 *)0x050000c4;
    while (i <= 5) {
        *dst++ = *src++;
        i++;
    }
}

void FieldScene_RunScriptedSteps0And1576(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 1);
    Engine_MessageShowCentered(0x1576, 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedSteps0And953(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 1);
    Engine_MessageShowCentered(0x953, 1);
    Engine_EventEnd();
}

void FieldScene_RunFlag881Dialogue(void)
{

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 1);
    if (Func_02009096_tail(0x881) == 0)
        Engine_MessageShowCentered(0x1636, 1);
    else
        Engine_MessageShowCentered(0x1635, 1);
    if (Func_020090aa_tail(0xb9) != -1) {
        s16 *slot = (s16 *)Data_03001ebc + 185;
        s32 one = 1;

        *slot = one;
    }
    Engine_EventEnd();
}

void FieldScene_RunActor184Sequence(void)
{
    Engine_EventBegin();
    Engine_AudioPlayCue(0x53);
    Engine_ItemShowFound(0xb8, 3);
    Func_0200860a_tail(0xb9, 0xb8);
    Func_020090da_tail(Func_020090fc_tail(0xb8), 1);
    Func_020090e2_a_tail(0xb8, 2);
    Engine_MessageShowCentered(0x1638, 1);
    Func_0200912a_tail(512);
    Engine_EventEnd();
}

void FieldScene_RunScene39cSequenceB(void)
{
    Engine_EventBegin();
    if (Data_02000240_t[224][0] == (s32)Data_00000036) {
        Call3_020036ac(Func_020091de_tail, 0, 0x1d8, 0x258);
        Call3_020036ac(Func_02009272_tail, 0, 0x4000, 10);
        Call4(Engine_CameraMoveTo, 0x1d00000, -1, 0x2900000, 1);
        Func_02009128_tail(Func_020091ca_tail(0), 0);
        Func_02003756_tail(Func_020091d6_tail(0)->x.fixed, 0, 0x2be0000, 223);
        Call6(Engine_MapCopyCellsTo, 92, 46, 92, 40, 3, 2);
        FIELD_AT_OFFSET(Func_020091fc_tail(0), s32, 72) = 0x8000;
        Engine_ActorSetSpritePriority(0, 2);
        Call3_020036ac(Func_02009296_tail, 0, 6, -1);
        *(s32 *)(Data_03001ebc + 0x1c0) = 0x203;
        Engine_EventWait(60);
        Engine_EventRequestExit(8);
    } else {
        Call3_020036ac(Func_020092be_tail, 0, 6, -1);
    }
    Engine_EventEnd();
}

void FieldScene_RunScene39b_02001208(void)
{
    struct FieldActor *actor;
    s32 done;

    actor = Func_02009262_tail(0);
    done = Value1_02003788(Func_0200921a_tail, 0x109);
    if (done == 0) {
        Engine_EventBegin();
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        actor->motion_flags = 0;
        Value3(Func_020092f2_tail, 0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) - 0x100000);
        Engine_ActorSetChildValue(0, 15);
        Func_0200920e_tail(Func_020092b0_tail(0), 0);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_AudioPlayCue(228);
        actor->update = (void (*)(union FieldObject *))0x2008cd1;
        Call3(Engine_ActorSetSpeed, 0, 0x6666, 0x3333);
        Func_02009454_b_tail(0, 0, 8);
        Engine_ActorSetChildValue(0, 0);
        Func_02009248_tail(Func_020092ea_tail(0), 1);
        actor->sprite->priority = 1;
        Func_02009482_tail(0, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        Func_0200949c_tail();
        Engine_EventEnd();
    }
}

void SceneEffect_SpawnParticleRowsAndDrawTiles(void)
{
    s32 buf[10];
    u32 i, j;

    Engine_MapCopyCellsTo(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = ((Func_0200923a() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = ((Func_0200924e() << 3) >> 16) * 0x3333 + 0xffff3334;

                Func_02003a32(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Engine_EventWait(1);
            }
        }
        Engine_MapCopyCellsTo(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Engine_MapCopyCellsTo(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}

void SceneEffect_SpawnParticleEveryFourthFrame(void)
{
    s32 buf[10];
    s32 *p = Func_02009548(0);
    s32 m = Data_03001e40 & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Func_02009426() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + (((Func_02009440() << 2) >> 16) << 16);
            s32 a = ((Func_02009452() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Func_02009466() << 3) >> 16) * 0x3333 + 0xffff3334;

            Func_02003c40(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = Data_03001e40 & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Func_020094e8() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = ((Func_02009502() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Func_02009516() << 3) >> 16) * 0x3333 + 0xffff3334;

            Func_02003cee(x, y, z, a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z)
{
    s32 buf[10];

    buf[1] = 7;
    buf[0] = 1;
    buf[2] = 0xb333;
    buf[3] = 0xb333;
    {
        s32 a = x + (((Func_0200958a() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + (((Func_0200959a() << 3) >> 16) << 16) + 0xfffc0000;

        Func_02003d68(a, y, b, 0, 0, 0, 0xb0000, buf);
    }
}

void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w)
{
    s32 desc[10];
    s32 tmp[3];
    u32 i;

    Engine_AudioPlayCue(0xd8);
    i = 0;
    do {
        if ((i & 1) != 0) {
            desc[1] = 7;
            if ((i & 2) != 0)
                desc[1] = 5;
            desc[2] = 0x9999;
            desc[3] = 0x9999;
            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            Func_0200965a((6 - (i >> 1)) * 0x1999, w, tmp);
            {
                s32 a = x + ((6 - ((Func_02009646() * 6) >> 16)) << 16);
                s32 b = z + ((6 - ((Func_0200965c() * 6) >> 16)) << 16);

                Func_02003e2e(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
            }
        }
        Engine_TaskWait(2);
        i++;
    } while (i <= 11);
}

void FieldScene_RunScene39c_02004f44(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)0x03001f30;
    Call3_02004f44(Engine_ActorSetPosition, 11, 0x3480000, 0x2580000);
    Engine_PsynergyBegin(93, 1);
    Engine_PsynergySetTarget(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
}

void Func_02004f90(s32 key, s32 value)
{
    s32 slot = Func_0200aa18(key);

    if (slot != -1) {
        s32 index = Func_0200aa20(slot, key);

        if (index != -1) {
            Func_0200aa1c(slot)->tbl[index] = value;
        }
    }
}

void OverlayObject_SpawnKind24AtObject(u8 *src)
{
    u8 *obj;
    u8 *rec;

    obj = Func_0200ab38(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
    if (obj == 0) {
        return;
    }

    rec = *(u8 **)(obj + 0x50);
    Func_0200ab3e(obj, (void *)0x0200de08);

    obj[0x55] = 0;
    obj[0x22] = 1;
    obj[0x23] = 2;

    if (rec != 0) {
        Func_0200ab50(rec, 2);
        rec[0x26] = 0;
        rec[9] |= 0x0C;
    }
}

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work)
{
    struct EffectAnchor *src = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Engine_MathSin(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = src->f8;
    work->f12 += 0x10000;
    work->f16 = src->f16;
    return 1;
}

s32 SceneEffect_AdvanceScaleOverSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n * 3) << 10;
    *(s32 *)(o + 28) = (n * 3) << 10;
    return 1;
}

s32 OverlayObject_StepScaleUpSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n << 11) + 0x10000;
    *(s32 *)(o + 28) = (n << 11) + 0x10000;
    return 1;
}

void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *rec = *Data_03001edc;
    s32 *target;

    if (rec[0] == 0) {
        return;
    }

    rec[0] = 0;
    Engine_GameFlagClear(0x161);

    target = (s32 *)rec[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Func_0200af6e_a(target, (s32)0x0200de2c);
        Func_0200af6e_b(target, 7);
        rec[5] = 0;
    }
}

void OverlayObject_ReleasePublishedAttachmentB(void)
{
    u8 **slot = Func_0200b304(35, 4);
    u8 *state;
    u8 *obj;

    if (slot == 0)
        return;

    state = *slot;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;

    Func_0200b33c(obj);
    *(u8 **)(state + 20) = 0;
}
