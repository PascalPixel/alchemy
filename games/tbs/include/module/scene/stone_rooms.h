#ifndef ALCHEMY_MODULE_MAP_STONE_ROOMS_H
#define ALCHEMY_MODULE_MAP_STONE_ROOMS_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gStoneRooms_Entrances[];
extern const u32 gStoneRooms_Exits[];
extern const struct ActorPlacement gStoneRooms_ThiefActorPlacements[];
extern const struct ActorPlacement gStoneRooms_PrisonerActorPlacements[];
extern const struct Trigger gStoneRooms_ThiefTriggers[];
extern const struct Trigger gStoneRooms_PrisonerTriggers[];

#endif
