#ifndef ALCHEMY_UNIT_FIELD_KORIMA_HASHI_HASHI_H
#define ALCHEMY_UNIT_FIELD_KORIMA_HASHI_HASHI_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;
struct StagedActorEffectRequest;
struct Struct2798;
struct Struct288c;

s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object);
s32 SceneActor_UpdateRandomCounterMode(u8 *object);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunTile10x20Transition(void);
s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request);
void SceneActor_PassSubjectOffsetPosition(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
void FieldScene_RunBranchingFormationPresentation(void);
void Event_SayThenWait(s32 speaker, s32 frames);
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p);
void SceneEffect_SpawnObject26EveryEightFrames(void);
s32 OverlayObject_SelectValueByFrameBit1(s32 obj);
s32 SceneActor_CheckRegionTrigger(struct Struct288c *arg0);
void KorimaHashi_AlternateBridgePostHeights(void);

#endif
