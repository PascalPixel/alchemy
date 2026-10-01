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
s32 ArcTan2(s32, s32);
void ObjectDispatch_ApplyValueToChildren(T *, s32);
void Func_020023c6();
void Object_SetActionCallbackAndRefreshById();
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
