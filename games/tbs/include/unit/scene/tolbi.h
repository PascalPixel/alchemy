#ifndef ALCHEMY_UNIT_SCENE_TOLBI_H
#define ALCHEMY_UNIT_SCENE_TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

const struct SceneEntrance *Tolbi_GetEntrances(void);
const struct SceneRegion *Tolbi_GetRegions(void);
const u32 *Tolbi_GetExits(void);
const struct ScenePlacement *Tolbi_GetPlacements(void);
const struct SceneEvent *Tolbi_SelectEvents(void);
s32 Tolbi_ApplyEntryState(s32 unused);
extern s32 gTolbiMirroredActor[];
extern s32 gTolbiOlderBrotherRun[];
extern s32 gTolbiYoungerSisterRun[];
extern struct SceneEntrance gTolbiEntrances[];
extern u32 gTolbiExits[];
extern struct ScenePlacement gTolbiPlacements[];
extern struct SceneEvent gTolbiEventsFestival[];
extern struct SceneEvent gTolbiEventsFinalsDay[];
extern struct SceneEvent gTolbiEventsAfterColosso[];
extern u16 gTolbiDoorOpening[];
extern struct MapCellAnim gTolbiDoors[];

#endif
