#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ObjectMotion_SetAngleToward_1(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Engine_EventWait, a0)
#define SceneWork_SetStepValue_1(a0) Value1(Engine_EventSetMessage, a0)
#define SceneWork_SetStepValue_2(a0) Value1(Engine_EventSetMessage, a0)
#define Scene_GetRecord_1(a0) Value1(Func_0200264e, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define SceneWork_SetStepValue_3(a0) Value1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_2(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define ObjectMotion_EnableActionAndResetMotion_1(a0) Value1(Engine_ActorStop, a0)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_020026dc_b, a0, a1)
#define BattleRuntime_WaitIfModeZero_2(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_3(a0, a1) Value2(Engine_ActorSetAttachedEffect, a0, a1)
#define BattleRuntime_WaitIfModeZero_4(a0) Value1(Engine_EventWait, a0)
#define BattleEvent_RunActionAndWait_3(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define Scene_GetRecord_2(a0) Value1(Func_02002708, a0)
#define ACTOR_FLAGS_OFFSET 89
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003d68(args)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Value2(Engine_CameraSetSpeed, a0, a1)
#define Scene_GetRecord_1_020019e8(a0) Value1(Func_02003cac, a0)
#define Scene_GetRecord_2_020019e8(args...) Func_02003e8c(args)
#define Scene_GetRecord_3(args...) Func_02003eb0(args)
#define Scene_GetRecord_4(args...) Func_02004110(args)
#define Scene_GetRecord_5(args...) Func_02004130(args)
#define ACTOR_FLAGS_OFFSET_020019e8 90
#define ACTOR_DONE_OFFSET 100

struct Object {
    u8 filler00[24];
    s32 x;
    s32 z;
    u8 filler20[68];
    s16 counter;
    s16 mode;
};

/*
 * Prepare the scene service selected by index zero, set its halfword at +6,
 * and run the four follow-up services in the order present in the overlay.
 *
 * This owner starts at the saved-link prologue at 0x02001990 and returns at
 * 0x020019b8.  The next saved-link prologue is at 0x020019bc, so the two zero
 * bytes between them are alignment and are deliberately outside this source.
 */
struct SceneService_02001990 {
    u16 unknown00[3];
    u16 value06;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

/* Layout of the record returned for an actor: only the word at offset
 * 0x64 (100) is touched here. */
struct SceneRecord {
    u8 pad[100];
    u16 field_0x64;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[80];
    u16 unk64;
    s16 unk66;
} T_0200154c;

/*
 * resource_399 owner at 0x02001704, 72 bytes.
 *
 * Resets one actor: clears the state byte at +85 and the halfword at +100,
 * drops bit 0 of the flag byte at +35, sets the two-bit mode field of the
 * linked record to 1, and re-centres the two 20.12 offsets on half a unit.
 *
 * The mode write is a bitfield assignment, not mask arithmetic. Spelling it
 * `(f9 & ~12) | 4` lets the compiler narrow the mask to a byte and emit
 * `movs r3,#243`; the reference builds -13 as `movs r3,#13 / negs r3,r3`,
 * which is what the bitfield path produces.
 */
struct Rec_399 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Work_399 {
    u8 pad00[24];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[3];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_399 *f80;        /* +80 */
    u8 pad54[1];
    u8 f85;                     /* +85 */
    u8 pad56[14];
    u16 f100;                   /* +100 */
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} T_020018c4;

typedef struct {
    u8 filler0[23];
    u8 unk17;
} State;

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02001fa4 {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern s16 Data_0200a3f0[];
extern u8 Value_00000033;
extern u8 Data_0200a8a0[];
extern u8 Data_0200a798[];
extern u8 Data_0200a9e0[];
extern u8 Data_0200aa58[];
extern u8 Data_0200aad0[];
extern u8 Data_0200adb8[];
extern u8 Data_0200ac80[];
extern u8 Value_0200b1d6;
extern u8 Data_03001ebc[];
extern u8 Data_0200a74c[];
extern u8 Data_0200a760[];
extern s32 Data_0200b1f0[];
extern s32 Data_020096ad[];
extern s16 Data_0200a430[];
extern u32 Data_03001ae8;
s32 Func_02002210(s32, s32);
s32 Func_0200221e(s32, s32);
s32 Func_02002282(s32, s32);
struct SceneService_02001990 *Func_02003be2(s32 index);
void Func_02004138(T *, s32);
void Func_020023c6();
void Func_020026dc_b();
void *Func_0200264e();
void *Func_02002708();
s32 Func_02002c8a();
s32 Func_02002d0a_a();
s32 Func_02003906();
s32 Func_0200393c();
s32 Func_02003968();
s32 Func_02003976();
s32 Func_020039a2();
s32 Func_020039ae();
s32 Func_020039da();
s32 Func_020039e6();
s32 Func_02003a12();
s32 Func_02003a48();
s32 Func_02003cac();
s32 Func_02003d68();
s32 Func_02003eb0();
s32 Func_02004110();
s32 Func_02004130();
void Data_0200a218();
void Data_0200a370();
void Data_0200a378();
s32 Func_02003270();
s32 Func_02003284();
void Func_0200336a();
void Func_02003374();
void Func_0200337e();
s32 Func_02003482();
s32 Func_020034d6();
void Func_02003510();
s32 Func_02003632();
s32 Func_020036a8();
s32 Func_020036b8();
s32 Func_020036f0();
void Func_020036fc();
s32 Func_02003696();
s32 Func_02003700();
void Func_0200376e();
void Func_02003728_a(s32, s32, s32 *);
void Func_02003ab2(void);
T *Func_02003b12(s32);
void Func_02003b42(s32);
void Func_02003b62(s32);
void Func_02009f24();
struct Subject_02001fa4 *Func_02004358();
s32 Func_0200420c();
void Func_020041ba();
s32 Func_02004226_a();
s32 Func_02004230();
void Func_0200423e();
void Func_02004226_b();
void Func_0200425c();
s32 Func_0200428e();
void Func_02004298();
void Func_020042a6();
void Func_02004278();
s32 Func_020042e4();
void Func_020042da();
void Func_020042e8();

/* The three returned addresses are even, so they name in-image script blocks
 * rather than Thumb entry points. */

/* Engine_GameFlagIsSet and Engine_GameFlagIsSet are the same flag query reached from two
 * mutually exclusive arms; each names its own loader-relocated call word. */

/* Reviewed owner resource_399:0x02000f90, 1468 bytes.
 *
 * A long primary-script scene body for overlay resource_399. It arranges
 * actor records 0, 1, 2, 3, 8, 9 and 10 through position, animation, camera
 * and timing calls, waits twice on a per-record halfword at offset 100,
 * plays three sound cues, toggles bit 0 of a record flag byte at offset 90
 * for actors 0, 1 and 2, and stamps the shared scene work record at
 * 0x03001ebc (phase word 0x1c0, related word 0x1c8) at the start and again
 * at the end before setting one game flag.
 *
 * Loader-relocated overlay calls: every Func_02xxxxxx symbol below names the
 * pre-relocation call word the stored image holds at its site, not a function
 * body in this address range. The loader rewrites those BL pairs to the
 * overlay's own veneer island (0x0200a1xx-0x0200a3xx at runtime), which in
 * turn reaches the main image. Because the stored word is site-relative, one
 * runtime veneer is named by several such symbols and one symbol can name two
 * different veneers.
 *
 * NOTE FOR THE INTEGRATOR: eleven of the reference's site names are
 * genuinely ambiguous over this owner - ten cover two veneers each and
 * Func_0200365a covers three - so at 23 sites this draft spells the call
 * with another site's unambiguous name for the same veneer. That keeps the
 * emitted target correct and lets the owner score without a translation-unit
 * binding. On adoption, prefer this overlay's established convention
 * (Func_<site>_a / Func_<site>_b plus explicit absolute_symbols) as in
 * run_scene_399_three_actor_choreography.c. Three veneers have no
 * unambiguous site name at all (runtime 0x0200a218, 0x0200a370, 0x0200a378)
 * and are spelled here as Data_<runtime address> so they resolve by address.
 *
 * The complete substitution list, as reference site -> spelling used here:
 *   Func_02003252  0x02000fa0 -> Engine_ActorSetPosition   0x02001000 -> Engine_ActorSetSpeed
 *   Func_02003472  0x02001140 -> Engine_ActorShowEmote   0x02001198 -> Engine_ActorRunRepeatedMotion
 *   Func_02003490  0x0200115e -> Engine_ActorShowEmote   0x0200131e -> Engine_TaskWait
 *   Func_020035e2  0x020012d8 -> Engine_EventShowMessage   0x02001350 -> Engine_ActorWalkToAndWait
 *   Func_02003616  0x020013ac -> Engine_ActorStop   0x020013cc -> Func_02003270
 *   Func_0200365a  0x020012e8 -> Data_0200a370   0x020012f0 -> Engine_ColorBufferApplyTarget
 *                  0x02001338 -> Engine_ActorFaceDirection
 *   Func_02003670  0x020012f6 -> Data_0200a378   0x02001366 -> Engine_EventShowMessage
 *   Func_02003692  0x02001318 -> Data_0200a378   0x02001440 -> Engine_ActorSetSpeed
 *   Func_0200370e  0x02001494 -> Engine_ActorSetDestination   0x020014c4 -> Func_02003270
 *   Func_0200372c  0x0200140a -> Engine_ActorFaceDirection   0x02001502 -> Engine_EventWait
 *   Func_02003738  0x02001416 -> Engine_ActorFaceDirection   0x0200151e -> Data_0200a218
 *
 * Veneer roles. Every row below is the role name the overlay's exact-C
 * sibling resource_399:0x020019e8 already gives the same main-image entry
 * through an unambiguous site symbol, matched by main address:
 *   0x0200a218  GameFlag_Set                              (0x080770c8)
 *   0x0200a228  BattleRuntime_WaitIfModeZero              (0x0808a010)
 *   0x0200a230  BattleRuntime_Reset                       (0x0808a018)
 *   0x0200a238  BattleRuntime_ScheduleShoulderButtonModeUpdate
 *   0x0200a248  Scene_GetRecord                           (0x0808a080)
 *   0x0200a250  ObjectMotion_SetSpeedParameters           (0x0808a090)
 *   0x0200a288  ObjectMotion_ResetAndSetPositionInMode2   (0x0808a0c8)
 *   0x0200a290  ObjectMotion_SetPositionAndReset          (0x0808a0d0)
 *   0x0200a2b0  ObjectMotion_SetHorizontalPositionWithTerrain
 *   0x0200a2b8  Object_SetModeById                        (0x0808a100)
 *   0x0200a2c0  ObjectMotion_CallThenWaitForAnimationChange
 *   0x0200a2d8  ObjectMotion_SetVariantCallbackAndInvokeObject
 *   0x0200a300  SceneWork_SetStepValue                    (0x0808a170)
 *   0x0200a310  BattleRuntime_RunThenWaitIfModeZero       (0x0808a188)
 *   0x0200a320  ObjectMotion_ArmCallback                  (0x0808a1b8)
 *   0x0200a330  BattleEffect_SpawnLinkedResourceObject    (0x0808a1e8)
 *   0x0200a338  BattleRuntime_WaitIfModeZero              (0x0808a1f0)
 *   0x0200a340  ObjectMotion_SetSpeedLimitAndAcceleration (0x0808a208)
 *   0x0200a348  ObjectMotion_PlaceWithinCameraBounds      (0x0808a210)
 *   0x0200a380  BattleRuntime_WaitIfModeZero              (0x0808a360)
 * One further role is evidenced outside that sibling: 0x0200a3e8 reaches
 * main 0x080f9010, which battle/event_runtime/dispatch_queued_events.c calls
 * with a pending sound id, so its three uses here are sound cues.
 * The remaining veneers (0x0200a170, 0x0200a1e0, 0x0200a258, 0x0200a260,
 * 0x0200a268, 0x0200a270, 0x0200a278, 0x0200a2f8, 0x0200a308, 0x0200a350,
 * 0x0200a368, 0x0200a370, 0x0200a378) have no repository name yet and keep
 * their raw spellings.
 *
 * SceneState_StoreTable96adToWork is the one intra-overlay direct call; it reaches
 * SceneState_StoreTable96adToWork at 0x020016c8.
 *
 * STATUS: drafted, not adopted. 1468 candidate bytes against 1468 reference
 * bytes, 36 differing halfwords, 4 wrong instructions, topology equal,
 * residual class allocation-uncovered. All 142 reference call sites, both
 * `record != 0` guards, both wait loops, every literal constant and every
 * store are represented; the extent is exact and no code was dropped.
 *
 * Residual, now one coupled defect. The literal table address held in r5
 * across each of the three table groups (0x0200a74c, 0x0200a5ec, 0x0200a760)
 * is scheduled one to two calls earlier here than in the reference, which
 * issues each `ldr r5, [pc, ...]` at the group's first use. Because the third
 * hoisted load makes r5 live immediately after the last `|= 1` flag update,
 * that update also computes into r3 here where the reference clobbers the
 * now-dead constant register r5. Fixing the schedule would fix both; nothing
 * structural is missing.
 *
 * Measured source hypotheses, so the next reader does not repeat them:
 *  - Binding the wait flag's address before materialising the zero
 *    (`done = (u16 *)(... + 100); val = 0; *done = val;`) keeps the zero's
 *    live range off the call and restores the reference's scratch register.
 *    This removed the two `strh` mismatches: 40 -> 36 differing halfwords,
 *    12 -> 4 wrong instructions.
 *  - Storing the zero directly (`*(u16 *)(... + 100) = 0;`) makes GCC 2.96
 *    materialise the HImode constant from the literal pool, adding two pool
 *    words: 1476 bytes, 416 differing halfwords. The `val` temporary is
 *    load-bearing, not decoration.
 *  - Block-scoping the three table locals: neutral.
 *  - Splitting the shared table local into three: neutral.
 *  - Inlining the table constants at their call sites: regressed to 255.
 *  - Spelling the first table call directly rather than through Call3:
 *    regressed to 637; the Call/Value wrappers are load-bearing.
 * The triage router reports "do not probe register roles by respelling
 * source", so the remaining schedule residual is reported, not chased. */

/* Veneers with no unambiguous reference site name over this owner. */

/* Byte offset of the flag byte on an actor record; bit 0 is cleared for
 * actors 0, 1 and 2 and set again after they are repositioned. */

/* Halfword at offset 100 of an actor record: the scene waits until it
 * becomes non-zero. */

/* Scripted dialogue step for resource_399.  The imports name loader-relocated
 * call words and stay old-style because their arity varies between sites. */

/* Typed wrappers around the two angle calls.  Their parameters are what force
 * the 0x3000 argument to be built from an immediate and a shift at each call. */
static __inline__ void Call_02002630(s32 channel, s32 angle, s32 frames)
{
    Actor_FaceDirection(channel, angle, frames);
}

static __inline__ void Call_02002684(s32 channel, s32 angle, s32 frames)
{
    Actor_FaceDirection(channel, angle, frames);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Byte offset of a flags field on the actor 8 record touched below; bit 1
 * of that byte is toggled at the end of the third branch. */

/* Byte offset of a flags field on the actor records touched below; bit 0
 * of that byte is cleared and later set back for actors 20 (twice). */
static __inline__ void Call0(void (*f)())
{
    void *Func_02003e8c();

    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void *Func_02003e8c();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void *Func_02003e8c();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void *Func_02003e8c();

    f(a0, a1, a2);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void *Func_02003e8c();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void *Func_02003e8c();

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void *Func_02003e8c();

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    void *Func_02003e8c();

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020005dc(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000a3c(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000abc(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value2_02001794(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020019bc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020019e8(void (*f)(), s32 a0)
{
    s32 Func_02003e8c();

    f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 Func_02003e8c();

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000f90(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02000f90(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2_02000f90(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2_02000f90(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_02000f90(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4_02000f90(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Data_0200a430 is this overlay's heading table.  Data_02000240 is a resident
 * table; its word at byte offset 500 selects the subject. */

/* The per-frame callback installed on the subject, named by its address. */

/* Imports named by their relocated call words.  Old-style declarations are
 * mandatory: one of these imports is reached with two arguments here while its
 * other sites pass three or four. */
static __inline__ void AdvanceProbe_02001fa4(s32 heading, s32 *probe)
{
    /* Keep this call behind an inline boundary: the third argument's address is
     * then rematerialized before the split 0x100000 constant is completed. */
    Func_02004278((s32)0x100000, heading, probe);
}

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z -= 0x1000;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = Func_02002210(Random_Next(), 40) + 40;
        } else {
            obj->counter = Func_0200221e(Random_Next(), 20) + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}

s32 OverlayObject_UpdateFacingTowardTarget(void *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    void *target;
    target = FIELD(obj, void *, 0x68);
    if (target != NULL) {
        FIELD(obj, u8, 0x5A) = (u8)(0xFE & FIELD(obj, u8, 0x5A));
        angle = (u16)Func_02002282(FIELD(target, s32, 0x10) - FIELD(obj, s32, 0x10), FIELD(target, s32, 8) - FIELD(obj, s32, 8));
        old = FIELD(obj, u16, 6);
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) delta = 0x1000;
            if (delta < -0x1000) delta = -0x1000;
            FIELD(obj, u16, 6) = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Scene-script selection for resource_399: read the scenario id from the
 * shared table, branch on it and on story flag 0x881, and return the chosen
 * in-image script block.
 */
s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&Value_00000033) {
        return (s32)Data_0200a8a0;
    }
    return (s32)Data_0200a798;
}

/* The whole four-byte owner. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes its one pool word. */
u8 *SceneData_GetTableA990(void)
{
    return (u8 *)0x0200a990;
}

/*
 * The overlay image is writable, so the chosen block is patched in place. The
 * coordinates are written as shifts, which is how a 16.16 whole number is
 * built here. The word at +0x4c is set only on this path and never read back,
 * so its meaning is unverified.
 */
u8 *SceneData_SelectScriptByScene33AndFlag881(void)
{
    u8 *script;

    if (gGameState.scene == ((s32)&Value_00000033)) {
        script = Data_0200aad0;
        Func_020023c6(script);
        if (GameFlag_IsSet(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return script;
    }

    if (GameFlag_IsSet(0x881) != 0) {
        return Data_0200aa58;
    }
    return Data_0200a9e0;
}

void FieldScene_Forward2188(void)
{
    StagedActor_RunHeadingProbeStep();
}

s32 SceneData_SelectTableByWord224B(void)
{
    if (gGameState.scene == (s32)&Value_00000033) {
        return (s32)Data_0200adb8;
    }
    return (s32)Data_0200ac80;
}

void SceneDialogue_RunActorEightFlagGatedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(0x1570);
    } else {
        Event_SetMessage(0x1529);
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Event_ShowMessage(mode, val);
    }
    Event_End();
}

void SceneDialogue_ShowLine1571Or152F(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(0x1571);
    } else {
        Event_SetMessage(0x152f);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActor9Line(void)
{
    Event_Begin();
    Actor_FaceActor(9, 0, 10);
    Event_SetMessage(0x152a);
    Event_AskYesNo(9, 0);
    Event_End();
}

/*
 * One scripted section, bracketed by an open and a close call, in which story
 * flag 0x881 picks between two arms on channel 10.  The arms differ only in the
 * message id and one step call, and stay separate so that every call is written
 * once.  258 is a pose id, 0x3000 three sixteenths of a turn.  Engine_EventAskYesNo's
 * unused s32 return is what fixes that call's argument order.
 */
void SceneDialogue_RunActorTenFlag881Dialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x881) != 0) {
        Event_SetMessage(0x163c);
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Event_Wait(40);
        Actor_SetAnimation(10, 1);
        Event_Wait(20);
        Actor_FaceActor(10, 0, 20);
        Event_AskYesNo(10, 0);
        Call_02002630(10, 0x3000, 10);
        Actor_SetAnimation(10, 9);
    } else {
        Event_SetMessage(0x152d);
        Event_ShowMessage(10, 0);
        Actor_SetAttachedEffect(10, 258);
        Event_Wait(40);
        Actor_SetAnimation(10, 1);
        Event_Wait(20);
        Actor_FaceActor(10, 0, 20);
        Event_ShowMessage(10, 0);
        Call_02002684(10, 0x3000, 10);
        Actor_SetAnimation(10, 9);
    }

    Event_End();
}

/* Picks one of three scripted call sequences depending on two condition
 * checks (codes 2177 and 2091), each acting on actor 9 and/or actor 8. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    void *Func_02003e8c();

    void *actor9_record;
    void *unused_actor9_record;
    void *actor8_record;

    if (GameFlag_IsSet(2177) != 0) {
        Event_Begin();
        unused_actor9_record = ObjectMotion_SetAngleToward_1(9, 0, 0);
        BattleRuntime_WaitIfModeZero_1(10);
        SceneWork_SetStepValue_1(5700);
        Event_AskYesNo(9, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(2091) != 0) {
            Event_Begin();
            Actor_SetAnimation(9, 7);
            Value3(Engine_MapAnimateCells, 33599936, 10, 69);
            SceneWork_SetStepValue_2(5484);
            Event_ShowMessage(9, 0);
            Actor_SetAnimation(9, 8);
            Map_AnimateCells((s32)&Value_0200b1d6, 10, 69);
            Event_End();
        } else {
            Event_Begin();
            actor9_record = Scene_GetRecord_1(9);
            ((struct SceneRecord *)actor9_record)->field_0x64 = 10;
            ObjectMotion_EnableActionAndSetCallback_1(9, 33596660);
            SceneWork_SetStepValue_3(5428);
            BattleEvent_RunActionAndWait_2(9, 0);
            ObjectMotion_EnableActionAndResetMotion_1(8);
            Actor_ShowEmote(8, 256, 40);
            Actor_FaceDirection(8, 53248, 10);
            Actor_StartRepeatedMotion(8, 2);
            Event_ShowMessageAndWait(8, 0, 20);
            ObjectMotion_EnableActionAndSetCallback_2(0, 33596772);
            Actor_SetSpeed(8, 104857, 52428);
            ObjectMotion_MarkActiveAndSetActionCallback_1(8, 33596680);
            BattleRuntime_WaitIfModeZero_2(40);
            Actor_Jump(8, 2, 0);
            Actor_StartRepeatedMotion(8, 2);
            BattleRuntime_WaitIfModeZero_3(8, 258);
            BattleRuntime_WaitIfModeZero_4(60);
            Event_ShowMessageAndWait(8, 0, 10);
            Actor_FaceDirection(8, 12288, 20);
            Actor_StartRepeatedMotion(8, 2);
            BattleEvent_RunActionAndWait_3(8, 0);
            actor8_record = Scene_GetRecord_2(8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set(0x82c);
            Event_End();
        }
    }
}

void SceneDialogue_RunActor12Line(void)
{
    Event_Begin();
    Event_SetMessage(0x153f);
    Event_AskYesNo(12, 0);
    Event_End();
}

void SceneDialogue_RunActor18Line(void)
{
    Event_Begin();
    Event_SetMessage(0x154d);
    Event_AskYesNo(18, 0);
    Event_End();
}

void SceneDialogue_RunActor20BranchScene(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(0x1574);
        Event_ShowMessage(20, 0);
    } else {
        Event_SetMessage(0x1557);
        Event_AskYesNo(20, 0);
        GameFlag_Set(0x82a);
        GameFlag_Set(0x82c);
    }
    Event_End();
}

void SceneDialogue_RunActor20FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(0x1575);
    } else {
        Event_SetMessage(0x155B);
    }
    Event_ShowMessage(20, 0);
    Event_End();
}

void FieldScene_RunScene399_020005dc(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(0x156d);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_End();
}

void SceneDialogue_RunActorEightBranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x82b) != 0) {
        Event_SetMessage(0x156f);
    } else if (GameFlag_IsSet(0x82c) != 0) {
        Event_SetMessage(0x153b);
    } else {
        Event_SetMessage(0x1533);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_RunSingleStep(void)
{
    FieldScene_RunSupplementalSequenceOne();
}

void SceneDialogue_ShowLine156E(void)
{
    Event_Begin();
    Event_SetMessage(0x156e);
    Event_ShowMessage(10, 0);
    Event_End();
}

void SceneDialogue_ShowLine1573Or155A(void)
{
    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(0x1573);
    } else {
        Event_SetMessage(0x155a);
    }
    Event_ShowMessage(19, 0);
    Event_End();
}

void FieldScene_RunScene399_02000a3c(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)Func_02002c8a(0);
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Inn_Open(4, 16);
    } else {
        Event_Begin();
        Actor_FaceActor(16, 0, 10);
        if (GameFlag_IsSet(0x881) != 0) {
            Event_SetMessage(0x1653);
            Event_AskYesNo(16, 0);
        } else {
            Event_SetMessage(0x154b);
            Event_ShowMessage(16, 0);
        }
        Actor_FaceDirection(16, 0x3000, 10);
        Event_End();
    }
}

void FieldScene_RunScene399_02000abc(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)Func_02002d0a_a(0);
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Event_Begin();
        if (GameFlag_IsSet(0x82d) == 0) {
            Event_SetMessage(0x1553);
            Event_ShowMessage(19, 0);
            GameFlag_Set(0x82d);
        }
        Event_End();
        Sanctum_Open(19);
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x881) != 0) {
            Event_SetMessage(0x1671);
            Event_ShowMessage(19, 0);
        } else if (GameFlag_IsSet(3) != 0) {
            Event_SetMessage(0x1572);
            Event_ShowMessage(19, 0);
        } else {
            Event_SetMessage(0x1554);
            (void)Event_AskYesNo(19, 0);
            Actor_FaceDirection(19, 0x3000, 10);
        }
        Event_End();
    }
}

void FieldScene_RunPrimaryScriptChoreography(void)
{
    extern u8 Data_0200a5ec[];

    struct FieldActor *leader;
    s32 tbl;
    struct EventWork *work;

    Event_Begin();
    Actor_SetPosition(3, 0xb60000, 0x960000);
    Camera_MoveTo(0x8d0000, -1, 0xdd0000, 0);
    Task_Wait(1);
    Camera_SetSpeed(0x4ccc, 0x999);
    Camera_MoveTo(0x8c0000, -1, 0xa40000, 1);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 40;
    Event_OpenScreen();
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Actor_SetSpeed(1, 0x6666, 0x3333);
    Actor_SetSpeed(2, 0x6666, 0x3333);
    Actor_WalkToAndWait(0, 142, 221);
    Actor_FaceDirection(0, 0xd000, 0);
    leader = (struct FieldActor *)Value1_02000f90(Func_02003270, 0);
    if (leader != NULL) {
        Actor_SetPosition(1, leader->x.fixed, leader->z.fixed);
    }
    leader = (struct FieldActor *)Value1_02000f90(Func_02003284, 0);
    if (leader != NULL) {
        Actor_SetPosition(2, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkTo(1, 150, 234);
    Actor_WalkToAndWait(2, 134, 234);
    Actor_SetAnimation(1, 1);
    tbl = (s32)Data_0200a74c;
    Call3_02000f90(Func_0200336a, 0, 0x10003, tbl);
    Call3_02000f90(Func_02003374, 1, 0x10003, tbl);
    Call3_02000f90(Func_0200337e, 2, 0x10003, tbl);
    Camera_WaitForMove();
    tbl = (s32)Data_0200a5ec;
    Actor_EnableActionCallback(9, tbl);
    Event_Wait(40);
    Actor_SetAttachedEffect(3, 0x102);
    Event_Wait(40);
    Actor_RunRepeatedMotion(3, 1);
    Event_SetMessage(0x155c);
    Event_ShowMessageAndWait(3, 0, 20);
    Value2_02000f90(Engine_ActorEnableActionCallback, 9, tbl);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(3, 0x8000, 20);
    Actor_FaceDirection(8, 0, 10);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(10);
    Actor_FaceDirection(3, 0x4000, 0);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(3, 0, 10);
    Value2_02000f90(Engine_ActorEnableActionCallback, 9, tbl);
    /* SceneState_StoreTable96adToWork at 0x020016c8. */
    SceneState_StoreTable96adToWork();
    Actor_ShowEmote(3, 0x101, 60);
    Event_ShowMessageAndWait(3, 0, 40);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_SetAnimation(9, 7);
    Map_AnimateCells(0x200b1c0, 10, 69);
    Event_Wait(10);
    Actor_RunRepeatedMotion(3, 2);
    Actor_SetAnimationAndWait(3, 4);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(40);
    Actor_SetAnimation(9, 8);
    Map_AnimateCells(0x200b1d6, 10, 69);
    Event_Wait(40);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_FaceDirection(8, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(3, 0x101, 30);
    Actor_FaceDirection(3, 0x8000, 10);
    Actor_SetAnimation(3, 4);
    Event_ShowMessageAndWait(3, 0, 10);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(40);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    ((struct Work_399 *)Func_02003482(3))->f100 = 0;
    Actor_EnableActionCallback(3, 0x200a670);
    while (*(s16 *)(Func_020034d6(3) + ACTOR_DONE_OFFSET) == 0) {
        Task_Wait(1);
    }
    Camera_MoveTo(0x8c0000, -1, 0xc60000, 1);
    Func_02003510(3);
    Actor_ShowEmote(3, 0x101, 80);
    Event_ShowMessageAndWait(3, 0, 40);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(10);
    Event_ShowMessage(3, 0);
    Audio_PlayCue(131);
    Call2_02000f90(Data_0200a370, 0x10000, 0);
    ColorBuffer_ApplyTarget(0x207e9f, 0);
    Data_0200a378(10);
    Task_Wait(1);
    Audio_PlayCue(220);
    Task_Wait(40);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Data_0200a378(60);
    Task_Wait(60);
    Actor_SetAttachedEffect(3, 0x102);
    Event_Wait(20);
    Actor_FaceDirection(3, 0, 10);
    Actor_SetSpeed(3, 0x20000, 0x10000);
    Actor_WalkToAndWait(3, 202, 198);
    Event_Wait(40);
    Actor_RunRepeatedMotion(3, 2);
    Event_ShowMessage(3, 0);
    Actor_SetAnimationAndWait(3, 4);
    Event_ShowMessageAndWait(3, 0, 20);
    Actor_SetAttachedEffect(3, 0x102);
    Event_Wait(40);
    Event_ShowMessageAndWait(3, 0, 40);
    Actor_ShowEmote(3, 0x100, 40);
    Event_ShowMessage(3, 0);
    Actor_Stop(0);
    Actor_Stop(1);
    Actor_Stop(2);
    Actor_SetSpeed(3, 0x30000, 0x18000);
    ((struct Work_399 *)Func_02003270(3))->f100 = 0;
    Actor_EnableActionCallback(3, 0x200a6e0);
    while (*(s16 *)(Func_02003632(3) + ACTOR_DONE_OFFSET) == 0) {
        Task_Wait(1);
    }
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(2, 0x4000, 10);
    Actor_SetSpeed(0, 0x40000, 0x20000);
    Actor_SetSpeed(1, 0x40000, 0x20000);
    Actor_SetSpeed(2, 0x40000, 0x20000);
    Audio_PlayCue(152);
    ((struct FieldActor *)Func_02003696(0))->unknown_5a &= 254;
    ((struct FieldActor *)Func_020036a8(1))->unknown_5a &= 254;
    ((struct FieldActor *)Func_020036b8(2))->unknown_5a &= 254;
    Actor_SetDestination(0, 132, 206);
    Actor_SetDestination(1, 136, 221);
    Actor_SetDestination(2, 122, 238);
    Func_020036fc(3);
    Event_Wait(80);
    ((struct FieldActor *)Func_020036f0(0))->unknown_5a |= 1;
    ((struct FieldActor *)Func_02003700(1))->unknown_5a |= 1;
    ((struct FieldActor *)Func_02003270(2))->unknown_5a |= 1;
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    tbl = (s32)Data_0200a760;
    Actor_EnableActionCallback(1, tbl);
    Func_0200376e(2, tbl);
    Event_Wait(20);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    work->transition_frames = 24;
    Call1_02000f90(Data_0200a218, 0x82b);
    Event_End();
}

void SceneActor_UpdateCountdownArcPosition(T_0200154c *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = Data_0200b1f0[0];
            b[1] = Data_0200b1f0[1] + 0x80000;
            b[2] = Data_0200b1f0[2];
            Func_02003728_a(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Engine_ObjectDispatchRelease(o);
        }
    }
}

void ActorPresentation_ApplyTableA5ecToActorNine(void)
{
    extern s32 Data_0200a5ec[];

    Actor_EnableActionCallback(9, (s32)Data_0200a5ec);
    Event_ShowMessage(9, 0);
}

void SceneState_StoreTable96adToWork(void)
{
    u8 *work;

    Psynergy_Begin(93, 1);
    work = *(u8 **)0x03001f30;
    Psynergy_SetTarget(3, 9);
    *(s32 *)(work + 36) = (s32)Data_020096ad;
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    Func_02003ab2();
}

void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work)
{
    struct Rec_399 *rec;

    work->f85 = 0;
    work->f100 = 0;
    work->f35 &= ~1;

    rec = work->f80;
    rec->mode = 1;

    Object_SetPalette(work, 9);
    Actor_SetSpriteFlags(work, 0);

    work->f24 = 0x8000;
    work->f28 = 0x8000;
}

void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object)
{
    object->effect.x += (s16)object->effect.spin << 8;
    object->effect.y += 0x8000;
    object->effect.scale_x += 0x7ae;
    object->effect.scale_y += 0x7ae;
    object->effect.spin += 2;
    if (--object->effect.countdown == 0) {
        Engine_ObjectDispatchRelease(object);
    }
}

void FieldScene_RunScene399SequenceA(void)
{
    extern u32 Data_03001e40;
    u32 *frame;
    union FieldObject *object;

    frame = &Data_03001e40;
    if (Value2_02001794(Func_02003906, *frame, 60) == 0) {
        object = (union FieldObject *)Value4(Func_02003968, 222, 0x1cf0000, 0, 0x1240000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Func_0200393c, *frame + 30, 60) == 0) {
        object = (union FieldObject *)Value4(Func_020039a2, 222, 0x1400000, 0x200000, 0x1640000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Func_02003976, *frame + 10, 60) == 0) {
        object = (union FieldObject *)Value4(Func_020039da, 222, 0x760000, 0, 0x460000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Func_020039ae, *frame + 50, 60) == 0) {
        object = (union FieldObject *)Value4(Func_02003a12, 222, 0x1560000, 0, 0x7c0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
    if (Value2_02001794(Func_020039e6, *frame + 80, 60) == 0) {
        object = (union FieldObject *)Value4(Func_02003a48, 222, 0x1af0000, 0, 0xab0000);
        if (object != NULL) {
            SceneActor_ResetActorAndCenterOffsets(object);
            object->effect.countdown = 60;
            object->effect.update = FieldScene_RunSupplementalSequenceTwo;
            Object_SetAnimation(object, 5);
        }
    }
}

void SceneState_UpdateZoneFlagsFromActorZero(void)
{
    T_020018c4 *obj;
    s32 x;
    s32 cx;
    s32 y;
    s32 r;
    s32 g;
    s32 h;
    State *st;

    obj = Func_02003b12(0);
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = GameFlag_IsSet(g);
    if (r != 0)
        return;
    (*(State **)0x03001e70)->unk17 = r;
    Func_02003b42(g);
    GameFlag_Clear(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = *(State **)0x03001e70;
        st->unk17 = 0;
        Func_02003b42(g);
        GameFlag_Clear(h);
        return;
    }
    r = GameFlag_IsSet(h);
    if (r != 0)
        return;
    st = *(State **)0x03001e70;
    st->unk17 = 1;
    Func_02003b62(h);
    GameFlag_Clear(g);
    return;
}

void SceneState_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

    GameFlag_Set(0x203);
    Actor_SetSpritePriority(11, 3);
    a = 15;
    b = 7;
    Map_CopyCellAttributes(15, 6, 1, 1, a, b);
}

void SceneActor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService_02001990 *work;

    Event_Begin();
    work = Func_02003be2(0);
    work->value06 = 0xc000;
    Audio_PlayCue(123);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(8);
}

/* Runs a short fixed sequence of two calls, one 3-argument call passing a
 * fixed-point-looking pair of constants, one 3-argument call passing
 * (0, 232, 204), and a final call, in that order. */
void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    Event_Begin();
    Event_OpenScreen(); /* main:0808a360 */
    Actor_SetSpeed(0, 0x20000, 0x1999);
    Actor_MoveToAndWait(0, 232, 204);
    Event_End();
}

/* Runs a long scripted sequence of position, animation, and timing calls
 * against actor records 0, 3, 19, and 20, with a scene phase word at
 * offset 0x1c0 of the shared scene work record set at the start and near
 * the end. */
void FieldScene_RunThreeActorChoreography(void)
{
    s32 Func_02003e8c();

    struct FieldActor *leader;
    struct EventWork *work;

    Event_Begin();
    Actor_FaceDirection(3, 0xa000, 0);
    Actor_SetSpeed(0, 0x9999, 0x4ccc);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x2b2, 200);
    /* Clear the byte at offset 85 of the returned record. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = 0;
    ObjectMotion_SetSpeedLimitAndAcceleration_1(0xcccc, 0x1999);
    Camera_MoveTo(0x2b20000, 0, 0xa40000, 1);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transition_frames = 48;
    Event_OpenScreen();
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 1);
    Actor_SetSpeed(3, 0x9999, 0x4ccc);
    leader = (struct FieldActor *)Scene_GetRecord_1_020019e8(0);
    if (leader != NULL) {
        Actor_SetPosition(3, leader->x.fixed, leader->z.fixed);
    }
    Actor_WalkToAndWait(3, 0x2a1, 183);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_StartRepeatedMotion(19, 2);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(40);
    Event_SetMessage(0x165b);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(3, 0xe000, 40);
    Actor_SetAnimationAndWait(3, 3);
    Actor_SetAttachedEffect(20, 0x102);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(3, 0xa000, 40);
    Actor_SetAnimationAndWait(3, 4);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    Actor_SetAttachedEffect(19, 0x102);
    Event_Wait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(0, 0xa000, 0);
    Actor_FaceDirection(3, 0xf000, 10);
    Actor_FaceDirection(3, 0x2000, 40);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 40);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_FaceDirection(3, 0xa000, 20);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(60);
    Actor_ShowEmote(3, 0x105, 60);
    Actor_ShowEmote(19, 0x101, 0);
    Actor_ShowEmote(20, 0x101, 60);
    Actor_RunRepeatedMotion(19, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(3, 0xe000, 40);
    Actor_FaceDirection(3, 0xa000, 40);
    Actor_FaceDirection(3, 0xe000, 20);
    Actor_FaceDirection(3, 0x6000, 80);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Actor_FaceDirection(20, 0xf000, 0);
    Actor_FaceDirection(19, 0x7000, 40);
    Actor_FaceDirection(19, 0x5000, 0);
    Actor_FaceDirection(20, 0x3000, 20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    *(u8 *)(Scene_GetRecord_2_020019e8(20) + ACTOR_FLAGS_OFFSET_020019e8) &= 254;
    Actor_WalkToAndWait(20, 0x290, 166);
    Event_Wait(1);
    *(u8 *)(Scene_GetRecord_3(20) + ACTOR_FLAGS_OFFSET_020019e8) |= 1;
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(40);
    Actor_FaceDirection(3, 0xa000, 10);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Actor_FaceDirection(3, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 10);
    Actor_SetAttachedEffect(19, 0x102);
    Actor_SetAttachedEffect(20, 0x102);
    Event_Wait(40);
    Actor_FaceDirection(3, 0xc000, 20);
    Actor_SetAnimation(3, 4);
    Event_ShowMessageAndWait(0x2003, 0, 20);
    Actor_RunRepeatedMotion(19, 1);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(3, 0x2000, 40);
    Actor_FaceDirection(3, 0xc000, 20);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_RunRepeatedMotion(3, 1);
    Event_Wait(20);
    Actor_FaceDirection(3, 0xa000, 20);
    Actor_SetAnimationAndWait(3, 3);
    Event_ShowMessageAndWait(0x2003, 0, 80);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Actor_SetAnimationAndWait(19, 4);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_SetAnimation(20, 4);
    Event_ShowMessageAndWait(0x4014, 0, 20);
    Actor_ShowEmote(3, 0x102, 60);
    Event_ShowMessageAndWait(0x2003, 0, 40);
    Actor_SetAnimationAndWait(19, 3);
    Event_ShowMessageAndWait(19, 0, 10);
    Actor_FaceDirection(3, 0xe000, 20);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(0x4014, 0, 10);
    Actor_FaceDirection(3, 0xa000, 60);
    Actor_FaceDirection(3, 0xe000, 20);
    Actor_FaceDirection(3, 0xa000, 20);
    Actor_FaceDirection(3, 0xc000, 40);
    Actor_SetAnimationAndWait(3, 3);
    Event_ShowMessageAndWait(0x2003, 0, 10);
    Actor_SetAnimation(19, 3);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(40);
    Actor_FaceDirection(3, 0x2000, 20);
    Event_ShowMessageAndWait(0x4003, 0, 20);
    Actor_FaceDirection(0, 0xa000, 20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_WalkToAndWait(3, 0x2b0, 200);
    Actor_SetPosition(3, 0, 0);
    *(u8 *)(Scene_GetRecord_4(20) + ACTOR_FLAGS_OFFSET_020019e8) &= 254;
    Actor_WalkToAndWait(20, 0x284, 166);
    Event_Wait(1);
    *(u8 *)(Scene_GetRecord_5(20) + ACTOR_FLAGS_OFFSET_020019e8) |= 1;
    (*(struct EventWork **)Data_03001ebc)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x82e);
    GameFlag_Clear(0x82d);
    Event_End();
}

void SceneActor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = Data_0200a3f0[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Object_SetAnimation(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Object_SetAnimation(o, 2);
    Func_02004138(o, 0x30);
}

/*
 * The overlay's pathing step for resource_399.  The six pool words after the
 * return belong to this owner.
 *
 * Frame map: the goal marker, then the heading, then the three-word probe
 * position that the stepping imports are handed by address.  The order of the x
 * and z assignments, together with the inline stepping wrapper, is what leaves
 * a single high-register copy before the arithmetic that follows it.
 */
void StagedActor_RunHeadingProbeStep(void)
{
    extern struct SharedData_02000240 Data_02000240;

    struct Subject_02001fa4 *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *p;

    subject = Func_02004358(Data_02000240.selected_subject);

    for (;;) {
        heading = Data_0200a430[(Data_03001ae8 >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, that is on the
         * signed halfword -1, which means "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* Nothing is placed in an argument register before this call. */
        Event_Begin();

        /* The 0x80000 bias is built from an immediate and a shift. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = Func_0200420c((s32)*p, x, z);
        /* 0x100000 is built from an immediate and a shift.  The probe block is
         * passed by address and is advanced by the callee. */
        Func_020041ba((s32)0x100000, heading, probe);

        marker = Func_02004226_a((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Func_02004230((s32)*p, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before the step above. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Func_0200423e(subject, x, subject->y, z);
        /* Same import as the probe above, two arguments here. */
        Object_SetAnimation(subject, 2);
        Func_02004226_b(subject, 48);
        Func_0200425c(subject);
        subject->callback = (void *)Func_02009f24;

        goto advanceProbe;
continueProbe:
        if (Func_0200428e((s32)*p, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Func_02004298(subject, probe[0], probe[1], probe[2]);
        Func_020042a6(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        AdvanceProbe_02001fa4(heading, probe);
        marker = Func_020042e4((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Func_020042da(subject, x, subject->y, z);
        Func_020042e8(subject);
        Task_Wait(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built from an immediate and a shift. */
    subject->state_052 = 0x4000;

tail:
    Task_Wait(10);
    Event_End();
}
