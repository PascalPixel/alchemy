#ifndef ALCHEMY_UNIT_MAP_TOLBI_H
#define ALCHEMY_UNIT_MAP_TOLBI_H

#include "types.h"
#include "event_layer.h"
#include "scene_tables.h"

const struct SceneEntrance *Tolbi_GetEntrances(void);
const struct SceneExitRegion *Tolbi_GetRegions(void);
const u32 *Tolbi_GetExits(void);
const struct ActorPlacement *Tolbi_GetActorPlacements(void);
const struct Trigger *Tolbi_SelectTriggers(void);
s32 Tolbi_ApplyEntryState(s32 unused);
extern s32 gTolbiMirroredActor[];
extern s32 gTolbiOlderBrotherRun[];
extern s32 gTolbiYoungerSisterRun[];
extern struct SceneEntrance gTolbiEntrances[];
extern u32 gTolbiExits[];
extern struct ActorPlacement gTolbiActorPlacements[];
extern struct Trigger gTolbiTriggersFestival[];
extern struct Trigger gTolbiTriggersFinalsDay[];
extern struct Trigger gTolbiTriggersAfterColosso[];
extern u16 gTolbiDoorOpening[];
extern struct MapGridCellAnimation gTolbiDoors[];

#endif
