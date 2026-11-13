#ifndef ALCHEMY_MODULE_SCENE_SUHALLA_H
#define ALCHEMY_MODULE_SCENE_SUHALLA_H

#include "types.h"
#include "scene_tables.h"

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
