#ifndef ALCHEMY_UNIT_SCENE_TOLBI_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TOLBI_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8                          *TolbiInteriors_GetEntrances(void);
s32                          TolbiInteriors_GetDefaultResult(void);
u8                          *TolbiInteriors_GetExits(void);
s32                          TolbiInteriors_SelectPlacements(void);
u8                          *TolbiInteriors_SelectEvents(void);
s32                          Scene_InitFacingActors(void);
extern s32                   gTolbiInteriors_ScaleBounceScript[];
extern struct SceneEntrance  gTolbiInteriorsEntrances[];
extern u32                   gTolbiInteriorsExits[];
extern struct ScenePlacement gTolbiInteriorsPlacements[];
extern struct ScenePlacement gTolbiInteriorsPlacementsFlag950[];
extern struct SceneEvent     gTolbiInteriorsEvents[];
extern struct SceneEvent     gTolbiInteriorsEventsFlag962[];
extern struct SceneEvent     gTolbiInteriorsEventsFlag950[];
extern u16                   gTolbiInteriors_ExitCellSteps[];
extern struct MapCellAnim    gTolbiInteriors_ExitCells[];

#endif
