#ifndef ALCHEMY_UNIT_SCENE_TOLBI_H
#define ALCHEMY_UNIT_SCENE_TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8 *Tolbi_GetEntrances(void);
s32 Tolbi_GetRegions(void);
u8 *Tolbi_GetExits(void);
u8 *Tolbi_GetPlacements(void);
u8 *Tolbi_SelectEvents(void);
s32 Tolbi_ApplyEntryState(s32 unused);
extern s32 gTolbiMirroredActor[];
extern s32 gTolbiOlderBrotherRun[];
extern s32 gTolbiYoungerSiblingRun[];
extern struct SceneEntrance gTolbiEntrances[];
extern u32 gTolbiExits[];
extern struct ScenePlacement gTolbiPlacements[];
extern struct SceneEvent gTolbiEventsFestival[];
extern struct SceneEvent gTolbiEventsFinalsDay[];
extern struct SceneEvent gTolbiEventsAfterColosso[];
extern u16 gTolbiDoorOpening[];
extern struct MapCellAnim gTolbiDoors[];

#endif
