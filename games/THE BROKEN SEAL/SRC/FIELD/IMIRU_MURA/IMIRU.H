/* Declarations shared by Imil's village sources (resource_399). */
#ifndef IMIRU_H
#define IMIRU_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
extern struct MapRenderWork *gMapWork;
extern u8 gEffectWork[];
extern u8 gKeysHeld[];

enum FlaggedChoreographyMessage {
    MSG_BRRRRR_CHOO_IM_FREEZING_MIA = 0x1529,
    MSG_HI_NEW_IN_IMIL = 0x152a,
    MSG_ONE_TWO_THREE_FOUR = 0x152d,
    MSG_MIA_SHOULD_HERE_BY_NOW = 0x152f,
    MSG_MIA_RUNNING_AROUND_TOWN_CARING = 0x1533,
    MSG_HES_ALWAYS_EXAGGERATING_THINGS_BUT = 0x153b,
    MSG_DO_WANT_WEAPONS = 0x153f,
    MSG_WHY_HAVE_TWO_GROUPS_TRAVELERS = 0x154b,
    MSG_DO_WANT_SEE_RESTAURANT_MENU = 0x154d,
    MSG_MAY_ONLY_STUDENT_BUT_CAN = 0x1553,
    MSG_LOOKING_FOR_MIA = 0x1554,
    MSG_HAVE_VISITED_OLD_COUPLE_WHO = 0x1557,
    MSG_THESE_FOLK_OKAY_THEY_DONT = 0x155a,
    MSG_MIA_WAS_SAYING_SHE_HAS = 0x155b,
    MSG_HOW_FEELING = 0x155c,
    MSG_HAPPENED_IN_LIGHTHOUSE_NORTHEAST = 0x156d,
    MSG_MIA_GOOD_GIRL_WISH_HAD = 0x156e,
    MSG_MIA_CLAN_ONCE_LIVED_HERE = 0x156f,
    MSG_EVEN_IF_MIA_HEALS_US = 0x1570,
    MSG_CANT_UNDERSTAND_WHY_ANY_ONE = 0x1571,
    MSG_AM_HEALER_WHILE_MIA_OUT = 0x1572,
    MSG_FEEL_LIKE_GROWN_UP_WHEN = 0x1573,
    MSG_EVERYONE_COUNTS_ON_MIA_THATS = 0x1574,
    MSG_WE_HAVE_DO_WHATEVER_WE = 0x1575,
    MSG_ONE_TWO_THREE_FOUR_2 = 0x163c,
    MSG_ITS_ALMOST_TIME_FOR_LEAVE = 0x1653,
    MSG_MIA = 0x165b,
    MSG_MIA_GOING_ON_JOURNEY_WITH = 0x1671
};


#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ACTOR_FLAGS_OFFSET 89
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

extern s16 ImiruMura_KeyHeadings[];
extern u8 Data_0200a8a0[];
extern u8 Data_0200a798[];
extern u8 Data_0200a9e0[];
extern u8 Data_0200aa58[];
extern u8 Data_0200aad0[];
extern u8 Data_0200adb8[];
extern u8 Data_0200ac80[];
extern u8 ImiruMura_CellStepsB[];
extern u8 Data_03001ebc[];
extern u8 ImiruMura_PrimaryScript[];
extern u8 ImiruMura_PrimaryScript2[];
extern s32 ImiruMura_ArcOrigin[];
/* Scripts and tables in the overlay's data. */
extern u8 ImiruMura_SceneTable[];
extern u8 ImiruMura_CellStepsA[];
extern u8 ImiruMura_ActorScriptA[];
extern u8 ImiruMura_ActorScriptB[];
extern u8 ImiruMura_ActorScriptC[];
extern u8 ImiruMura_MiaScriptA[];
extern u8 ImiruMura_MiaScriptB[];
extern s16 ImiruMura_SwayHeadings[];
extern u32 Data_03001ae8;
s32 Engine_MathModulo(s32, s32);
s32 Engine_MathModulo(s32, s32);
s32 ArcTan2(s32, s32);
void ObjectDispatch_ApplyValueToChildren(T *, s32);
void Func_020023c6();
void Object_SetActionCallbackAndRefreshById();
s32 Engine_MathModulo();
s32 Engine_MathModulo();
s32 Engine_MathModulo();
s32 Engine_MathModulo();
s32 Engine_MathModulo();
s32 Battle_GetWorkObject1e0();
void Engine_ColorBufferApplySource();
void Engine_ColorBufferInterpolate();
void Engine_ObjectSetTargetAndCallback();
void Engine_ObjectSetTargetAndCallback();
void Engine_ObjectSetTargetAndCallback();
void Object_RefreshSelectorById();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById();
void Vector_AddPolarOffset(s32, s32, s32 *);
void BattleEffect_CleanupSceneObjects(void);
void SceneActor_TurnTowardTableAngle();
struct Subject_02001fa4 *ObjectTable_Get();
s32 GetMapCellCollision();
void Vector_AddPolarOffset();
s32 GetMapCellCollision();
s32 Map_GetTerrainHeight();
void Engine_ObjectSetPosition();
void ObjectDispatch_ApplyValueToChildren();
void Engine_ObjectCommitPosition();
s32 Map_GetTerrainHeight();
void Engine_ObjectSetPosition();
void Engine_ObjectCommitPosition();
void Vector_AddPolarOffset();
s32 GetMapCellCollision();
void Engine_ObjectSetPosition();
void Engine_ObjectCommitPosition();

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
 *   Func_02003616  0x020013ac -> Engine_ActorStop   0x020013cc -> Engine_ActorGet
 *   Func_0200365a  0x020012e8 -> Engine_ColorBufferApplySource   0x020012f0 -> Engine_ColorBufferApplyTarget
 *                  0x02001338 -> Engine_ActorFaceDirection
 *   Func_02003670  0x020012f6 -> Engine_ColorBufferInterpolate   0x02001366 -> Engine_EventShowMessage
 *   Func_02003692  0x02001318 -> Engine_ColorBufferInterpolate   0x02001440 -> Engine_ActorSetSpeed
 *   Func_0200370e  0x02001494 -> Engine_ActorSetDestination   0x020014c4 -> Engine_ActorGet
 *   Func_0200372c  0x0200140a -> Engine_ActorFaceDirection   0x02001502 -> Engine_EventWait
 *   Func_02003738  0x02001416 -> Engine_ActorFaceDirection   0x0200151e -> Engine_GameFlagSet
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
 *   0x0200a330  BattleFx_SpawnLinked    (0x0808a1e8)
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

/* Byte offset of a flags field on the actor 8 record touched below; bit 1
 * of that byte is toggled at the end of the third branch. */

/* Byte offset of a flags field on the actor records touched below; bit 0
 * of that byte is cleared and later set back for actors 20 (twice). */
static __inline__ void Call0(void (*f)())
{

    f();
}

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

static __inline__ s32 Value0(s32 (*f)())
{

    return f();
}

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020005dc(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02000a3c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_02000abc(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2_02001794(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call3_020019bc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call1_020019e8(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{

    f(a0, a1, a2, a3);
}

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

/* ImiruMura_SwayHeadings is this overlay's heading table.  Data_02000240 is a resident
 * table; its word at byte offset 500 selects the subject. */

/* The per-frame callback installed on the subject, named by its address. */

/* Imports named by their relocated call words.  Old-style declarations are
 * mandatory: one of these imports is reached with two arguments here while its
 * other sites pass three or four. */
static __inline__ void AdvanceProbe_02001fa4(s32 heading, s32 *probe)
{
    /* Keep this call behind an inline boundary: the third argument's address is
     * then rematerialized before the split 0x100000 constant is completed. */
    Vector_AddPolarOffset((s32)0x100000, heading, probe);
}

/* The sources call one another across files. */
s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj);
s32 OverlayObject_UpdateFacingTowardTarget(void *obj);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetTableA990(void);
void FieldScene_Forward2188(void);
void SceneDialogue_RunActorEightFlagGatedDialogue(void);
void SceneDialogue_ShowLine1571Or152F(void);
void SceneDialogue_RunActor9Line(void);
void SceneDialogue_RunActorTenFlag881Dialogue(void);
void FieldScene_RunSupplementalSequenceOne(void);
void SceneDialogue_RunActor12Line(void);
void SceneDialogue_RunActor18Line(void);
void SceneDialogue_RunActor20BranchScene(void);
void SceneDialogue_RunActor20FlaggedLine(void);
void FieldScene_RunScene399_020005dc(void);
void SceneDialogue_RunActorEightBranchedDialogue(void);
void FieldScene_RunSingleStep(void);
void SceneDialogue_ShowLine156E(void);
void SceneDialogue_ShowLine1573Or155A(void);
void FieldScene_RunScene399_02000a3c(void);
void FieldScene_RunScene399_02000abc(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void SceneActor_UpdateCountdownArcPosition(T_0200154c *o);
void ActorPresentation_ApplyTableA5ecToActorNine(void);
void SceneState_StoreTable96adToWork(void);
void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work);
void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object);
void FieldScene_RunScene399SequenceA(void);
void SceneState_UpdateZoneFlagsFromActorZero(void);
void SceneState_UpdateActor11WithFlag203(void);
void SceneActor_SetActorZeroFacingC000AndRun(void);
void RunEventScript02(void);
void FieldScene_RunThreeActorChoreography(void);
void SceneActor_TurnTowardTableAngle(s32 z);
void StagedActor_RunHeadingProbeStep(void);

#endif
