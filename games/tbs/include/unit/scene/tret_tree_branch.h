#ifndef ALCHEMY_UNIT_SCENE_TRET_TREE_BRANCH_H
#define ALCHEMY_UNIT_SCENE_TRET_TREE_BRANCH_H

#include "types.h"
#include "field_scene.h"

u8 *TretTreeBranch_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *TretTreeBranch_GetExits(void);
u8 *TretTreeBranch_GetPlacements(void);
u8 *TretTreeBranch_GetEvents(void);
s32 TretTreeBranch_InitializeBackdropTransition(void);
extern struct SceneEntrance gTretTreeBranch_SceneTable0[];
extern u32 gTretTreeBranch_SceneTable1[];
extern struct ScenePlacement gTretTreeBranch_SceneTable2[];
extern struct SceneEvent gTretTreeBranch_SceneTable3[];
extern u16 gTretTreeBranch_CellSteps[];

#endif
