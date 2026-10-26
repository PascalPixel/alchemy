#ifndef ALCHEMY_UNIT_MAP_STONE_ROOMS_H
#define ALCHEMY_UNIT_MAP_STONE_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gStoneRooms_Entrances[];
extern const u32 gStoneRooms_Exits[];
extern const struct ActorTemplate gStoneRooms_ThiefActorTemplates[];
extern const struct ActorTemplate gStoneRooms_PrisonerActorTemplates[];
extern const struct Trigger gStoneRooms_ThiefTriggers[];
extern const struct Trigger gStoneRooms_PrisonerTriggers[];

#endif
