#include "RUNTIME_MEM.H"
#ifndef ALCHEMY_PROBE_H
#define ALCHEMY_PROBE_H

/* Declarations shared by the Mercury Lighthouse room's scene sources. */
#include "MAKYURI.H"
#include "MAKYURI_HEYA.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/* Values that look like 16.16 fixed-point fractions, named by that
 * fraction's decimal value. */
#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000

enum {
    /* Message 0x182 + 184. */
    ITEM_HERMES_WATER = 184
};

extern u8 gEffectWork[];

/* The IWRAM work pointers from gWork: the event work, and 0x20 bytes on, the
 * scene's. */
struct WorkPointers {
    u8 *event;
    u8 *unknown_04[7];
    u8 *scene;
};

extern struct WorkPointers gWork;

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 t0;
    s32 t1;
} SceneEvent;

/* The effect record an overlay object carries at +0x50. */
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
    struct EffectAnchor *f104;  /* +104 */
};

/* A party member's state holds a table of halfwords at +216. */
struct SceneLookupRecord {
    u8 pad00[216];
    u16 tbl[1];                 /* +216 */
};

/* Map cell animations and object scripts in the overlay's data. */
extern const u16 MakyuriHeya_GateCells[];
extern const u16 MakyuriHeya_GateCloseCells[];
extern const u16 MakyuriHeya_FloorSwitchCells[];

/* Engine imports called with the arities these scenes pass. */
void Vector_AddPolarOffset(s32, s32, s32 *);
void AnimationObjects_SelectAnimation(u8 *rec, s32 arg1);
void BattleFx_SetPhaseRequest(s32, s32);
void UiWork_PushValueSlot(s32, s32);
void BattleFx_PlayQueuedSound();
s32 PartyInventory_FindOwner();
s32 Inventory_Find();
struct SceneLookupRecord *Owner_GetState();

/* The staged-actor code and the overlay's other sources. */
s32 StagedActor_FindClearPosition();
void SceneActor_MoveAndRedraw();
void StagedActor_AdvancePair(void);
void Makyuri_TickSpawnTimer();
void Makyuri_RunActorMove();
void MakyuriIriguchi_RaisePillar();
void MakyuriHeya_SyncBlockFlags(void);
void MakyuriHeya_OpenThreeStepStair(void);
void MakyuriHeya_UpdateLeaderEffectTarget(void);
s32 MakyuriHeya_TrailSparks(struct FieldActor *actor);
void SetEffectRecordMode(struct EffectWork *work, s32 mode);
void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SceneState_ApplyPair12And21(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetTableE614(void);
void FieldScene_CallHelper67e8(void);
void FieldScene_Forward646c(void);
void FieldScene_RunSingleStep(void);
void FieldScene_CallHelper6364(void);
void SceneDialogue_RunLine1637(void);
void SceneActor_RunActorZeroHandledMotion(s32 a);
void MakyuriHeya_DropLeaderToColumn(s32 a0);
void SceneState_ApplyWork16cMinus50A(void);
void SceneState_ApplyWork16cMinus50(void);
void SceneState_ApplyWork16cMinus50B(void);
void MakyuriHeya_ExitWhenChannelsOpen(void);
void MakyuriHeya_WalkLeaderIn(void);
void FieldScene_RunFourCallSequence(void);
void FieldScene_RunActorElevenAtTile5And13(void);
void MakyuriHeya_TriggerSmallFloorSwitch(void);
void MakyuriHeya_CloseFloorSwitch(void);
void FieldScene_RunOpeningAuxiliarySequence();
void MakyuriHeya_RunSequenceG();
void MakyuriHeya_SprayAtFountain(void);
void FieldScene_RunSupplementalSequenceOne();
void MakyuriHeya_RunSequenceF(void);
void MakyuriHeya_RunSequenceE(void);
void FieldScene_RunFourSteps(void);
void SceneState_RunWhenActor8AtTile10x23(void);
void FieldScene_RunScene39cSequenceA(void);
void SceneEffect_SpawnParticleRowsByMode(s32 mode);
void SceneActor_UseActorNinePositionWithYOffset(void);
s32 SceneData_LoadBlockA2c5(void);
s32 SceneData_ApplyTableA2c5AndReturnZero(void);
void FieldScene_RunFourStepSequence(void);
void SceneState_ApplyRectWhenActor20AtColumn28(void);
void SceneEffect_RotatePaletteEntries40To47(void);
void FieldScene_RunActorThreeBranchSequence(void);
void SceneDialogue_RunActor3TimedLine(void);
void SceneEffect_RotatePaletteEntries97To103(void);
void FieldScene_RunScriptedSteps0And1576(void);
void FieldScene_RunScriptedSteps0And953(void);
void FieldScene_RunFlag881Dialogue(void);
void FieldScene_RunActor184Sequence(void);
void MakyuriHeya_ArriveWithSparks(void);
void SceneEffect_SpawnParticleRowsAndDrawTiles();
void SceneEffect_SpawnParticleEveryFourthFrame(void);
void SceneEffect_SpawnRandomEveryFourFrames(s32, s32, s32);
void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z);
void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w);
void MakyuriHeya_CastPsynergyAtActor11(void);
void SceneState_SetRecordTableValue(s32, s32);
void OverlayObject_SpawnKind24AtObject(u8 *src);
s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work);
s32 SceneEffect_AdvanceScaleOverSixteenFrames(u8 *o);
s32 OverlayObject_StepScaleUpSixteenFrames(u8 *o);
void SceneState_ClearCurrentRecordAndReleaseTarget();
void OverlayObject_ReleasePublishedAttachmentB(void);

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

#endif
