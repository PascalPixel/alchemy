#include "mem.h"
#ifndef ALCHEMY_PROBE_H
#define ALCHEMY_PROBE_H

#include "makyuri.h"
#include "makyuri_heya.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000

enum {
    ITEM_HERMES_WATER = 184
};

extern u8 gEffectWork[];

struct WorkPointers {
    u8 *event;
    u8 *unknown_04[7];
    u8 *scene;
};


typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 t0;
    s32 t1;
} SceneEvent;

struct EffectAnchor {
    u8 pad00[8];
    s32 f8;
    u8 pad0c[4];
    s32 f16;
};

struct AnchoredEffect {
    u8 pad00[8];
    s32 f8;
    s32 f12;
    s32 f16;
    u8 pad14[4];
    s32 f24;
    s32 f28;
    u8 pad20[68];
    s16 f100;
    u8 pad66[2];
    struct EffectAnchor *f104;
};

struct SceneLookupRecord {
    u8 pad00[216];
    u16 tbl[1];
};

extern const u16 MakyuriHeya_GateCells[];
extern const u16 MakyuriHeya_GateCloseCells[];
extern const u16 MakyuriHeya_FloorSwitchCells[];

void Vector_AddPolarOffset(s32, s32, s32 *);
void AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);
void BattleFx_SetPhaseRequest(s32, s32);
void UiWork_PushValueSlot(s32, s32);
void BattleFx_PlayQueuedSound();
s32 PartyInventory_FindOwner();
s32 Inventory_Find();
struct SceneLookupRecord *UnitGet();

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
void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SceneState_ApplyPair12And21(void);
s32 SceneData_ReturnZero(void);
u8 *MakyuriHeya_GetExits(void);
void MakyuriHeya_TickSpawnTimer(void);
void MakyuriHeya_ClearSceneRecordAndTarget(void);
void FieldScene_RunSingleStep(void);
void MakyuriHeya_TriggerPillarRise(void);
void SceneDialogue_RunLine1637(void);
void SceneActor_RunActorZeroHandledMotion(s32 a);
void MakyuriHeya_DropLeaderToColumn(s32 a0);
void MakyuriHeya_RunExitThreeScene(void);
void MakyuriHeya_RunExitThirteenScene(void);
void MakyuriHeya_RunExitFiveScene(void);
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
s32 MakyuriHeya_StartActorNineParticles(void);
s32 MakyuriHeya_StopActorNinePositionCallback(void);
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
void OverlayObject_SpawnKind24AtObject(struct FieldActor *src);
s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work);
s32 SceneEffect_AdvanceScaleOverSixteenFrames(struct FieldActor *o);
s32 OverlayObject_StepScaleUpSixteenFrames(struct FieldActor *o);
void SceneState_ClearCurrentRecordAndReleaseTarget();
void OverlayObject_ReleasePublishedAttachmentB(void);

#endif
