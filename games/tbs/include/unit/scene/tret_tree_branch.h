#ifndef ALCHEMY_UNIT_SCENE_TRET_TREE_BRANCH_H
#define ALCHEMY_UNIT_SCENE_TRET_TREE_BRANCH_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *TretTreeBranch_GetEntrances(void);
const struct SceneRegion *TretTreeBranch_GetRegions(void);
const u32 *TretTreeBranch_GetExits(void);
const struct ScenePlacement *TretTreeBranch_GetPlacements(void);
const struct SceneEvent *TretTreeBranch_GetEvents(void);
s32 TretTreeBranch_Initialize(void);
extern struct SceneEntrance gTretTreeBranch_Entrances[];
extern u32 gTretTreeBranch_Exits[];
extern struct ScenePlacement gTretTreeBranch_Placements[];
extern struct SceneEvent gTretTreeBranch_Events[];
extern u16 gTretTreeBranch_CellSteps[];

#endif
