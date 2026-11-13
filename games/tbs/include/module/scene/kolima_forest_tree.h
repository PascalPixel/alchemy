#ifndef ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_TREE_H
#define ALCHEMY_MODULE_SCENE_KOLIMA_FOREST_TREE_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gKolimaForestTree_GaretApproachActions[];
extern s32 gKolimaForestTree_IvanApproachActions[];
extern s32 gKolimaForestTree_MiaApproachActions[];
extern s32 gKolimaForestTree_RejoinPartyActions[];
extern struct SceneEntrance gKolimaForestTreeEntrances[];
extern struct SceneExitRegion gKolimaForestTreeRegions[];
extern u32 gKolimaForestTreeExits[];
extern struct ActorPlacement gKolimaForestTreeActorPlacements[];
extern struct Trigger gKolimaForestTreeTriggers[];
extern s32 gKolimaForestTreeEffectScript[];

#endif
