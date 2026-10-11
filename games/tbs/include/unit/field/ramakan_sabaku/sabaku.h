#ifndef ALCHEMY_UNIT_FIELD_RAMAKAN_SABAKU_SABAKU_H
#define ALCHEMY_UNIT_FIELD_RAMAKAN_SABAKU_SABAKU_H

#include "types.h"

struct FieldActor;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

void SetEffectRecordMode(struct FieldActor *work, s32 mode);
void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void FieldScene_ApplyActor13Values3And3(void);
void RamakanSabaku_WaitForActorHeight(u8 *o, s32 limit);
s32 RamakanSabaku_EmitSandEffect(struct FieldActor *actor);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunFlags8B2And8B3Steps(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 RamakanSabaku_ApplyEntryState(void);
s32 RamakanSabaku_ConfigureAreaLayout(void);
void RamakanSabaku_ReturnToArea3(void);
void RamakanSabaku_ShowRandomPartyLine(s32 a0);
void RamakanSabaku_RaiseQuarterTriggers(void);
s32 RamakanSabaku_CalculatePlanarDistance(s32 *position_a, s32 *position_b);
void RamakanSabaku_SweepBack(void);
void RamakanSabaku_UpdateTravelDust(void);
void RamakanSabaku_FaceNearestActor(void);
void RamakanSabaku_UpdateDustAndProbe(void);
void RamakanSabaku_PlayStartledRetreat(void);
s32 RamakanSabaku_EnterArea(s32 a0, s32 a1);
void RamakanSabaku_ResetActorEightPosition(void);
void RamakanSabaku_RunSandstorm(void);
void RamakanSabaku_ClaimSandEffectVram(void);
void RamakanSabaku_SetSandPhase(u16 value);

#endif
