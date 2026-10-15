#ifndef ALCHEMY_UNIT_SCENE_TOLBI_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8 *TolbiInteriors_GetEntrances(void);
s32 TolbiInteriors_GetRegions(void);
u8 *TolbiInteriors_GetExits(void);
s32 TolbiInteriors_SelectPlacements(void);
u8 *TolbiInteriors_SelectEvents(void);
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
