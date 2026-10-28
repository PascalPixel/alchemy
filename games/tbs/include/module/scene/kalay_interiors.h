#ifndef ALCHEMY_MODULE_MAP_KALAY_INTERIORS_H
#define ALCHEMY_MODULE_MAP_KALAY_INTERIORS_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *KalayInteriors_GetEntrances(void);
const struct SceneExitRegion *KalayInteriors_GetRegions(void);
const u32 *KalayInteriors_GetExits(void);
const struct ActorPlacement *KalayInteriors_GetActorPlacements(void);
const struct Trigger *KalayInteriors_GetTriggers(void);
s32 KalayInteriors_Initialize(void);
extern struct SceneEntrance gKalayInteriors_NoEntrances[];
extern struct SceneEntrance gKalayInteriors_HouseEntrances[];
extern struct SceneEntrance gKalayInteriors_SanctumEntrances[];
extern u32 gKalayInteriors_Exits[];
extern struct ActorPlacement gKalayInteriors_NoActorPlacements[];
extern struct ActorPlacement gKalayInteriors_HouseActorPlacements[];
extern struct ActorPlacement gKalayInteriors_InnActorPlacements[];
extern struct ActorPlacement gKalayInteriors_SanctumActorPlacements[];
extern struct Trigger gKalayInteriors_NoTriggers[];
extern struct Trigger gKalayInteriors_HouseTriggers[];
extern struct Trigger gKalayInteriors_InnTriggers[];
extern struct Trigger gKalayInteriors_SanctumTriggers[];
extern u16 gKalayInteriors_DoorCellSteps[];
extern struct MapGridCellAnimation gKalayInteriors_DoorCells[];

#endif
