#ifndef ALCHEMY_UNIT_FIELD_RAMAKAN_SABAKU_SABAKU_H
#define ALCHEMY_UNIT_FIELD_RAMAKAN_SABAKU_SABAKU_H

#include "types.h"
#include "field_scene.h"

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
extern const s32 gEffectScript0[];
extern const s32 gEffectScript1[];
extern const s32 gEffectScript2[];
extern const s32 RamakanSabaku_SafePoints1[];
extern const s32 RamakanSabaku_SafePoints2[];
extern const s32 RamakanSabaku_SafePointsOther[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gRamakanSabakuEntrancesOther[];
extern struct SceneEntrance gRamakanSabakuEntrances1[];
extern struct SceneEntrance gRamakanSabakuEntrances2[];
extern struct SceneEntrance gRamakanSabakuEntrances3[];
extern struct SceneEntrance gRamakanSabakuEntrances4[];
extern u32 RamakanSabaku_Exits[];
extern struct ScenePlacement gRamakanSabakuPlacementsOther[];
extern struct ScenePlacement gRamakanSabakuPlacements1[];
extern struct ScenePlacement gRamakanSabakuPlacements2[];
extern struct ScenePlacement gRamakanSabakuPlacements3[];
extern struct SceneEvent RamakanSabaku_Events[];
extern s16 RamakanSabaku_SandLevel;
extern s16 RamakanSabaku_SandCounter;
extern s16 RamakanSabaku_SandFlash;

#endif
