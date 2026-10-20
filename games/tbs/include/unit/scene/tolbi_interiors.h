#ifndef ALCHEMY_UNIT_SCENE_TOLBI_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

const struct SceneEntrance *TolbiInteriors_GetEntrances(void);
const struct SceneRegion *TolbiInteriors_GetRegions(void);
const u32 *TolbiInteriors_GetExits(void);
const struct ScenePlacement *TolbiInteriors_SelectPlacements(void);
const struct SceneEvent *TolbiInteriors_SelectEvents(void);
s32 TolbiInteriors_ApplyEntryState(void);
extern s32 gTolbiInteriorsSquashStretch[];
extern struct SceneEntrance gTolbiInteriorsEntrances[];
extern u32 gTolbiInteriorsExits[];
extern struct ScenePlacement gTolbiInteriorsPlacements[];
extern struct ScenePlacement gTolbiInteriorsPlacementsAfterColosso[];
extern struct SceneEvent gTolbiInteriorsEventsFestival[];
extern struct SceneEvent gTolbiInteriorsEventsFinalsDay[];
extern struct SceneEvent gTolbiInteriorsEventsAfterColosso[];
extern u16 gTolbiInteriorsDoorOpening[];
extern struct MapCellAnim gTolbiInteriorsDoors[];

#endif
