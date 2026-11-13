#ifndef ALCHEMY_MODULE_SCENE_VALE_KRADENS_COTTAGE_H
#define ALCHEMY_MODULE_SCENE_VALE_KRADENS_COTTAGE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gValeKradensCottage_RockslideSurvivorEscapeActions[];
extern s32 gValeKradensCottage_RockslideSurvivorSquashes[];
extern s32 gValeKradensCottage_FaceTargetActions[];
extern struct SceneEntrance gValeKradensCottage_Entrances[];
extern u32 gValeKradensCottage_Exits[];
extern struct ActorPlacement gValeKradensCottage_ActorPlacements[];
extern struct Trigger gValeKradensCottage_Triggers[];
extern struct Trigger gValeKradensCottage_StormTriggers[];
extern struct Trigger gValeKradensCottage_TriggersAfterKalay[];
extern u16 gValeKradensCottage_DoorOpens[];

#endif
