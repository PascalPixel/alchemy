#ifndef ALCHEMY_UNIT_SCENE_TOLBI_H
#define ALCHEMY_UNIT_SCENE_TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8 *Tolbi_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *Tolbi_GetExits(void);
u8 *Tolbi_GetPlacements(void);
u8 *Tolbi_SelectEvents(void);
s32 Tolbi_ApplyEntryState(s32 a0);
extern s32 gTolbiPlacement2[];
extern s32 gTolbiActor16Action[];
extern s32 gTolbiActor17Action[];
extern struct SceneEntrance gTolbiEntrances[];
extern u32 gTolbiExits[];
extern struct ScenePlacement gTolbiPlacements[];
extern struct SceneEvent gTolbiEvents[];
extern struct SceneEvent gTolbiEvents2[];
extern struct SceneEvent gTolbiEvents3[];
extern u16 gTolbiCellStepData[];
extern struct MapCellAnim gTolbiCellSteps[];

#endif
