#ifndef ALCHEMY_MODULE_MAP_SUHALLA_H
#define ALCHEMY_MODULE_MAP_SUHALLA_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSuhallaEntrances[];
extern u32 gSuhallaExits[];
extern struct ActorPlacement gSuhallaActorPlacements[];
extern struct ActorPlacement gSuhallaActorPlacementsLater[];
extern s16 gSuhallaDoorCells[][2];
extern u16 gSuhallaDoorOpenSteps[];
extern u16 gSuhallaWideDoorOpenSteps[];
extern struct Trigger gSuhallaTriggers[];
extern struct Trigger gSuhallaTriggersLater[];

#endif
