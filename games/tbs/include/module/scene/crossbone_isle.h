#ifndef ALCHEMY_MODULE_SCENE_CROSSBONE_ISLE_H
#define ALCHEMY_MODULE_SCENE_CROSSBONE_ISLE_H

#include "types.h"
#include "scene_tables.h"

extern u16 gCrossboneIsle_EntranceCells[];
extern struct SceneEntrance gCrossboneIsle_IsleEntrances[];
extern struct SceneEntrance gCrossboneIsle_CaveEntranceEntrances[];
extern struct SceneEntrance gCrossboneIsle_FloatingLogsEntrances[];
extern struct SceneEntrance gCrossboneIsle_PierEntrances[];
extern struct SceneEntrance gCrossboneIsle_PirateShipEntrances[];
extern u32 gCrossboneIsleExits[];
extern struct ActorPlacement gCrossboneIsle_NoActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_IsleActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_FloatingLogsActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_CorridorActorPlacements[];
extern struct Trigger gCrossboneIsle_CorridorTriggers[];
extern struct Trigger gCrossboneIsle_CaveEntranceTriggers[];
extern struct Trigger gCrossboneIsle_IsleTriggers[];
extern struct Trigger gCrossboneIsle_FloatingLogsTriggers[];
extern struct Trigger gCrossboneIsle_PierTriggers[];
extern struct Trigger gCrossboneIsle_PirateShipTriggers[];

#endif
