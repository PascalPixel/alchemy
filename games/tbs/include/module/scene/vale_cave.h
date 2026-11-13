#ifndef ALCHEMY_MODULE_SCENE_VALE_CAVE_H
#define ALCHEMY_MODULE_SCENE_VALE_CAVE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gValeCave_WalkTargets[][2];
extern struct SceneEntrance gValeCave_EntrancesOther[];
extern struct SceneEntrance gValeCave_MouthEntrances[];
extern struct SceneEntrance gValeCave_KiteEntrances[];
extern struct SceneEntrance gValeCave_LogsEntrances[];
extern u32 gValeCave_Exits[];
extern struct ActorPlacement gValeCave_MouthActorPlacements[];
extern struct ActorPlacement gValeCave_KiteActorPlacements[];
extern struct ActorPlacement gValeCave_LogsActorPlacements[];
extern struct Trigger gValeCave_TriggersOther[];
extern struct Trigger gValeCave_MouthTriggers[];
extern struct Trigger gValeCave_KiteTriggers[];
extern struct Trigger gValeCave_LogsTriggers[];

#endif
