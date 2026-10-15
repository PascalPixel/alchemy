#ifndef ALCHEMY_UNIT_SCENE_TOLBI_H
#define ALCHEMY_UNIT_SCENE_TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

u8 *TorebiMachi_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TorebiMachi_GetExits(void);
u8 *TorebiMachi_GetPlacements(void);
u8 *TorebiMachi_SelectEvents(void);
s32 TorebiMachi_ApplyEntryState(s32 a0);
extern s32 gTorebiMachiPlacement2[];
extern s32 gTorebiMachiActor16Action[];
extern s32 gTorebiMachiActor17Action[];
extern struct SceneEntrance gTorebiMachiEntrances[];
extern u32 gTorebiMachiExits[];
extern struct ScenePlacement gTorebiMachiPlacements[];
extern struct SceneEvent gTorebiMachiEvents[];
extern struct SceneEvent gTorebiMachiEvents2[];
extern struct SceneEvent gTorebiMachiEvents3[];
extern u16 gTorebiMachiCellStepData[];
extern struct MapCellAnim gTorebiMachiCellSteps[];

#endif
