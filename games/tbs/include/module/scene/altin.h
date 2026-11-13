#ifndef ALCHEMY_MODULE_SCENE_ALTIN_H
#define ALCHEMY_MODULE_SCENE_ALTIN_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"

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
