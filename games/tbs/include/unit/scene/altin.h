#ifndef ALCHEMY_UNIT_MAP_ALTIN_H
#define ALCHEMY_UNIT_MAP_ALTIN_H

#include "types.h"
#include "event.h"
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
extern s32 gCottageRoomsD_CookScript[];
extern struct SceneEntrance gAltin_NoEntrances[];
extern struct SceneEntrance gAltin_Entrances[];
extern struct SceneEntrance gCottageRoomsD_Entrances[];
extern u32 gAltin_Exits[];
extern struct ActorPlacement gAltin_NoActorPlacements[];
extern struct ActorPlacement gAltin_ActorPlacements[];
extern struct ActorPlacement gCottageRoomsD_ActorPlacements[];
extern struct Trigger gAltin_NoTriggers[];
extern struct Trigger gAltin_Triggers[];
extern struct Trigger gCottageRoomsD_Triggers[];
extern u16 gAltin_SanctumDoorCells[];
extern u16 gAltin_SouthwestDoorCells[];
extern u16 gAltin_SoutheastDoorCells[];
extern u16 gAltin_EastDoorCells[];
extern u16 gAltin_NorthwestDoorCells[];
extern u16 gAltin_NorthAndWestDoorCells[];
extern u16 gAltin_NortheastDoorCells[];
extern struct MapGridCellAnimation gAltin_Doors[];

#endif
