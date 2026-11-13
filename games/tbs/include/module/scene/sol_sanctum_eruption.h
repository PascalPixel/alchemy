#ifndef ALCHEMY_MODULE_SCENE_SOL_SANCTUM_ERUPTION_H
#define ALCHEMY_MODULE_SCENE_SOL_SANCTUM_ERUPTION_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct SceneEntrance gSolSanctumEruption_Entrances[];
extern u32 gSolSanctumEruption_Exits[];
extern struct ActorPlacement gSolSanctumEruption_ActorPlacements[];
extern struct Trigger gSolSanctumEruption_Triggers[];
extern u16 gSolSanctumEruption_UnusedStarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctum_RingMaxScales[];
extern s32 gSolSanctum_RingScaleSteps[];

#endif
