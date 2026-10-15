#ifndef ALCHEMY_UNIT_SCENE_TRET_BRANCH_H
#define ALCHEMY_UNIT_SCENE_TRET_BRANCH_H

#include "types.h"
#include "field_scene.h"

u8 *TretBranch_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TretBranch_GetExits(void);
u8 *TretBranch_GetPlacements(void);
u8 *TretBranch_GetEvents(void);
s32 TretBranch_InitializeBackdropTransition(void);
extern struct SceneEntrance gTretBranch_SceneTable0[];
extern u32 gTretBranch_SceneTable1[];
extern struct ScenePlacement gTretBranch_SceneTable2[];
extern struct SceneEvent gTretBranch_SceneTable3[];
extern u16 gTretBranch_CellSteps[];

#endif
