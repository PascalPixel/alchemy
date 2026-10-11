#ifndef ALCHEMY_UNIT_FIELD_MAKYURI_CHOJO_CHOJO_H
#define ALCHEMY_UNIT_FIELD_MAKYURI_CHOJO_CHOJO_H

#include "types.h"

struct OverlayObject;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void MakyuriChojo_SetSpritePriority(struct OverlayObject *object, s32 priority);
void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 type);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
void FieldScene_RunFourActorPresentation(void);
s32 FixedPoint_Distance(s32 *first_position, s32 *second_position);
s32 FindNearestF2Actor(void);
void SceneActor_InitializeMotion(void);
void MakyuriChojo_StepOntoNearestActor(void);
void SceneActor_SetMode55OnSevenRecords(void);
void RunScene58Sequence(void);
void RunScene59Sequence(void);
void MakyuriChojo_BringLiftUp(void);
void MakyuriChojo_TakeLiftDown(void);
void MakyuriChojo_SinkActorPair(void);
void MakyuriChojo_RiseActorPair(void);
void MakyuriChojo_CollectBandSlots(s32 *out, s32 y);
void MakyuriChojo_LowerCollectedActors(void);
void MakyuriChojo_RaiseCollectedActors(void);
void FieldScene_ConfigureValue93Scene(void);
void SceneEffect_UpdateArcPosition(struct OverlayObject *object);
void MakyuriChojo_FlickerActorEight(void);

#endif
