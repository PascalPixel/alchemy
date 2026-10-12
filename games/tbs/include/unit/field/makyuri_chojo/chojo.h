#ifndef ALCHEMY_UNIT_FIELD_MAKYURI_CHOJO_CHOJO_H
#define ALCHEMY_UNIT_FIELD_MAKYURI_CHOJO_CHOJO_H

#include "types.h"
#include "field_scene.h"

struct OverlayObject;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
void FieldScene_RunFourActorPresentation(void);
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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gMakyuriChojoEntrances[];
extern u32 gMakyuriChojoExits[];
extern struct ScenePlacement gMakyuriChojoPlacements[];
extern struct SceneEvent gMakyuriChojoEvents[];
extern s32 MakyuriChojo_NearestActor;
extern s32 Data_02003c54[];

#endif
