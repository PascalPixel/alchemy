#ifndef ALCHEMY_UNIT_MAP_KALAY_INTERIORS_H
#define ALCHEMY_UNIT_MAP_KALAY_INTERIORS_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *KalayInteriors_GetEntrances(void);
const struct SceneExitRegion *KalayInteriors_GetRegions(void);
const u32 *KalayInteriors_GetExits(void);
const struct ActorTemplate *KalayInteriors_GetActorTemplates(void);
const struct Trigger *KalayInteriors_GetTriggers(void);
s32 KalayInteriors_Initialize(void);
extern struct SceneEntrance gKalayInteriors_NoEntrances[];
extern struct SceneEntrance gKalayInteriors_HouseEntrances[];
extern struct SceneEntrance gKalayInteriors_SanctumEntrances[];
extern u32 gKalayInteriors_Exits[];
extern struct ActorTemplate gKalayInteriors_NoActorTemplates[];
extern struct ActorTemplate gKalayInteriors_HouseActorTemplates[];
extern struct ActorTemplate gKalayInteriors_InnActorTemplates[];
extern struct ActorTemplate gKalayInteriors_SanctumActorTemplates[];
extern struct Trigger gKalayInteriors_NoTriggers[];
extern struct Trigger gKalayInteriors_HouseTriggers[];
extern struct Trigger gKalayInteriors_InnTriggers[];
extern struct Trigger gKalayInteriors_SanctumTriggers[];
extern u16 gKalayInteriors_DoorCellSteps[];
extern struct MapGridCellAnimation gKalayInteriors_DoorCells[];

#endif
