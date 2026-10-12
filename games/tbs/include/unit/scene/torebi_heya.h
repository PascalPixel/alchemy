#ifndef ALCHEMY_UNIT_SCENE_TOREBI_HEYA_H
#define ALCHEMY_UNIT_SCENE_TOREBI_HEYA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8 *TorebiHeya_GetEntrances(void);
s32 TorebiHeya_GetDefaultResult(void);
u8 *TorebiHeya_GetExits(void);
s32 TorebiHeya_SelectPlacements(void);
u8 *TorebiHeya_SelectEvents(void);
s32 Scene_InitFacingActors(void);
extern s32 TorebiHeya_ScaleBounceScript[];
extern struct SceneEntrance gTorebiHeyaEntrances[];
extern u32 gTorebiHeyaExits[];
extern struct ScenePlacement gTorebiHeyaPlacements[];
extern struct ScenePlacement gTorebiHeyaPlacementsFlag950[];
extern struct SceneEvent gTorebiHeyaEvents[];
extern struct SceneEvent gTorebiHeyaEventsFlag962[];
extern struct SceneEvent gTorebiHeyaEventsFlag950[];
extern u16 TorebiHeya_ExitCellSteps[];
extern struct MapCellAnim TorebiHeya_ExitCells[];

#endif
