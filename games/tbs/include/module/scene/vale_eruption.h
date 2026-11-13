#ifndef ALCHEMY_MODULE_SCENE_VALE_ERUPTION_H
#define ALCHEMY_MODULE_SCENE_VALE_ERUPTION_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gValeEruption_LeaderWalksDown[];
extern s32 gValeEruption_GaretWalksDown[];
extern s32 gValeEruption_EnterSanctum[];
extern struct SceneEntrance gValeEruption_Entrances[];
extern u32 gValeEruption_Exits[];
extern struct ActorPlacement gValeEruption_ActorPlacements[];
extern struct Trigger gValeEruption_Triggers[];
extern u16 gValeEruption_SanctumDoorOpens[];

#endif
