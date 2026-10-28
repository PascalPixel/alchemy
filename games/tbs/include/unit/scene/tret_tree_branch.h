#ifndef ALCHEMY_UNIT_MAP_TRET_TREE_BRANCH_H
#define ALCHEMY_UNIT_MAP_TRET_TREE_BRANCH_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *TretTreeBranch_GetEntrances(void);
const struct SceneExitRegion *TretTreeBranch_GetRegions(void);
const u32 *TretTreeBranch_GetExits(void);
const struct ActorPlacement *TretTreeBranch_GetActorPlacements(void);
const struct Trigger *TretTreeBranch_GetTriggers(void);
s32 TretTreeBranch_Initialize(void);
extern struct SceneEntrance gTretTreeBranch_Entrances[];
extern u32 gTretTreeBranch_Exits[];
extern struct ActorPlacement gTretTreeBranch_ActorPlacements[];
extern struct Trigger gTretTreeBranch_Triggers[];
extern u16 gTretTreeBranch_UnusedCellSteps[];

#endif
