#ifndef ALCHEMY_UNIT_MAP_SUHALLA_H
#define ALCHEMY_UNIT_MAP_SUHALLA_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSuhallaEntrances[];
extern u32 gSuhallaExits[];
extern struct ActorTemplate gSuhallaActorTemplates[];
extern struct ActorTemplate gSuhallaActorTemplatesLater[];
extern s16 gSuhallaDoorCells[][2];
extern u16 gSuhallaDoorOpenSteps[];
extern u16 gSuhallaWideDoorOpenSteps[];
extern struct MapEvent gSuhallaEvents[];
extern struct MapEvent gSuhallaEventsLater[];

#endif
