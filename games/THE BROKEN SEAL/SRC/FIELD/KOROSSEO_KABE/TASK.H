#ifndef KOROSSEO_KABE_TASK_H
#define KOROSSEO_KABE_TASK_H

/* Declarations shared by the Colosso wall stage's scene code. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum TaskMessage {
    MSG_WOULD_LIKE_FRIEND_CHEER_FOR = 0x207d,
    MSG_IF_KNOW_WHO_WANT_CHEER = 0x207e,
    MSG_ROBIN_WILL_CHEER_FOR_WAY = 0x207f,
    MSG_DO_YOUR_BEST = 0x2083,
    MSG_UNFORTUNATELY_WE_HAVE_FULL_HOUSE = 0x2084,
    MSG_MATCH_ABOUT_BEGIN_PLEASE_TAKE = 0x2085,
    MSG_OPERATOR_LIFTS_WILL_CHEER_FOR = 0x20a1,
    MSG_SHIFTING_FLOOR_STAGE = 0x20a2,
    MSG_HERE_YOUR_OBJECTIVE_RIDE_LOGS = 0x20a5,
    MSG_LOG_ROLLING_STAGE = 0x20a6,
    MSG_ROBIN_DID_GET_GOOD_LOOK = 0x20e5,
    MSG_WAIT_SHOULDNT_DECIDE_WHERE_BEST = 0x20e8
};

#if defined(TBS_EDITION_JA)
#define SCENE_TEXT_VALUE 0x98f
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define SCENE_TEXT_VALUE 0xa5a
#else
#define SCENE_TEXT_VALUE 0x96a
#endif

/* The engine's work blocks, one pointer per slot; slot 59 holds the stage's
 * descriptor. */
extern u8 gWorkSlot[];
#define WORK_SLOT_STAGE 59
extern u8 gEffectWork[];

typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

typedef struct MovedObject {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} MovedObject;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

/* The mode task's records; the halfword at +26 holds the per-mode span in
 * sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct SparkSource {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} SparkSource;

typedef struct RaisedEffect {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} RaisedEffect;

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

/* The stage's data, in the overlay's read-only data and work. */
extern u8 HexDigits[];
extern u8 KorosseoKabe_MarkerGraphics[];
extern s32 KorosseoKabe_SparkScript[];
extern s32 KorosseoKabe_RaiseScript[];
extern u32 KorosseoKabe_DirectionSteps[];
extern s32 KorosseoKabe_ApproachScript[];
extern s32 KorosseoKabe_SpectatorPhase;
extern u32 KorosseoKabe_SpectatorTimer;
extern s16 KorosseoKabe_CursorSlot;
extern struct ModeRecord KorosseoKabe_ModeRecordDefault;
extern struct ModeRecord KorosseoKabe_ModeRecordTwo;
extern struct ModeRecord KorosseoKabe_ModeRecordThree;
extern struct ModeRecord KorosseoKabe_ModeRecordThreeAlt;
extern struct ModeRecord KorosseoKabe_ModeRecordFour;
extern u16 Korosseo_ModeTaskMode;
extern u16 Korosseo_ModeTaskParam;
extern u16 Korosseo_ModeTaskTimer;
extern s32 Korosseo_ModeTaskScript;
extern u16 Korosseo_ModeMoveTarget;
extern u16 Korosseo_ModeMoveDuration;
extern s32 Korosseo_ModeTaskPosition;
extern u16 KorosseoKabe_MarkerX;
extern u16 KorosseoKabe_MarkerY;
extern u16 KorosseoKabe_MarkerStyle;
extern u16 KorosseoKabe_MarkerFrame;
extern u16 KorosseoKabe_MarkerDuration;
extern u16 KorosseoKabe_MarkerTargetX;
extern u16 KorosseoKabe_MarkerTargetY;
extern u16 KorosseoKabe_MarkerStartX;
extern u16 KorosseoKabe_MarkerStartY;
extern u16 KorosseoKabe_MarkerElapsed;

/* Import veneers, named by the main-image code each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void *Engine_AllocateBlock();
s32 GameFlag_GetByte();
void GameFlag_SetByte();
void Vector_AddPolarOffset();
s32 Runtime_BumpAllocateAlternatePool();
void Sys_Free();
void Resource_ResetEntry();
s32 Resource_FindFreeEntry();
s32 Resource_GetTableEntry();
void ObjectDispatch_InitFromTable6();
void ObjectDispatch_WaitForValue16();
s32 Object_CheckMovementCollision();
void Object_CommitPosition();
void UiWork_PushValueSlot();
s32 Resource_LoadFixedBlockBIntoFreeSlot();
u16 *Owner_GetState();
void Inventory_AddItem();
void Inventory_Equip();
s32 Party_CountActiveOwners();
void Party_AddActiveOwner();
void Party_RemoveActiveOwner();
void Owner_RefreshActiveRatios();
void Audio_PlayCueFromEventWork();
void Object_LinkObjectAndSetCallback();
void BattleEffect_PauseObject();
s32 Menu_OpenCharacterSelector();
s32 AudioCommand_GetStateByte();
void BattleFx_RunRisingObjectSequence();

/* The stage's code still in the listing, and the shared Colosso code. */
void KorosseoKabe_RollLogToCell();
void Korosseo_SelectSoloCompetitor();
void Korosseo_FinishSoloRound();
s32 KorosseoKabe_RunStateInteraction();
void KorosseoKabe_ShowFollowUpPrompt();
void Korosseo_LoadPortrait();
s32 Korosseo_FadeInCompetitor();
void Korosseo_RestoreCompetitor();
void Korosseo_UpdatePathRival(void);
void Korosseo_UpdateModeTask(void);
void Korosseo_UpdateMarker(void);
void FieldScene_RunExtendedActorSequence(void);

/* The area's functions, in cartridge order. */
void SceneData_NoOpHandler(void);
void FieldScene_RunSingleStep(void);
void SceneState_ConfigureRegionByActorElevenColumn(void);
void FieldScene_RunTwoStepSequence(void);
void SceneState_StoreSlotTileXToWork832To848(void);
void SceneState_SetWorkByte35(void);
void SceneState_SetFlag331AndConfigureRegion46_17(void);
void FieldScene_SetFlag332AndDrawTiles(void);
void FieldScene_SetFlag333AndDrawTiles(void);
void SceneState_SendWord250With6(void);
void FieldScene_Forward4358(void);
void SceneActor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2);
void SceneState_ApplyRectsForActors15To17(void);
void FieldScene_RunStep15At29By26(void);
void FieldScene_RunStep15At33By26(void);
void FieldScene_RunStep16At45By26(void);
void FieldScene_RunStep16At49By26(void);
void FieldScene_RunStep17At40By23(void);
void FieldScene_RunStep17At40By25(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_PlaceSpectatorRow(void);
void SceneState_ApplyTable8715AndValue104(void);
void SceneState_WaitForStatusWords(void);
void SceneState_InstallTask8714AndApplyTwoRects(void);
void FieldScene_RunPairedEntranceWalk(s32 a0);
void FieldScene_RunSecondActorInteraction(s32 a0);
void FieldScene_RunSceneThreeCoordinator(s32 a0);
void SceneActor_PlacePartyAtSavedTiles(void);
void KorosseoKabe_MarkSceneProgress(void);
void KorosseoKabe_SelectNearestActor(void);
void RunPartyCountInteractionCopyA(s32 actorId);
void FieldScene_RunSixSteps896To936(void);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void OverlayObject_NotifyMatchingEntries(s32 no, s32 val);
void KorosseoKabe_InitializeModeTask(u32 mode, u32 param);
void KorosseoKabe_RunScriptedTransition(s32 mode);
void FieldScene_RunLateSequence(s32 a0);
void StagedActor_ResetMotionAfterRefresh(s32 slot);
void SceneState_InitCursorWhenUnset();
void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style);
void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration);
void SceneEffect_RemoveMarkerTask(void);
void SceneActor_PlaceWithScale20000(s32 selector, s32 x, s32 z);
void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z);
void Text_WriteU32AsHex(u8 *buf, u32 value);
void SceneState_EmptyHook(void);
void SceneState_SetHalfword1000To9(void);
void SceneState_WaitUntilStatusNine(void);
void KorosseoKabe_SpawnRandomSceneEffect(SparkSource *a);
s32 KorosseoKabe_RaiseLinkedSceneEffect(RaisedEffect *a);
s32 FieldScene_RunFlag211ApproachScene(s32 handleA, s32 handleB);
void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth);
void SceneState_InitControlRecordAndStartTask(void);
void SceneState_SetWorkHalfwordDc(s16 v);
s32 *SceneData_FindSlotAtPosition(s32 *pos, void *self);
void StagedActor_PushActorAhead(void);
s32 *SceneActor_FindOccupantAheadOfSubject(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static inline void InitializeActorZero(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
}

#endif
