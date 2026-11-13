#ifndef ALCHEMY_MODULE_SCENE_TRET_TREE_BRANCH_H
#define ALCHEMY_MODULE_SCENE_TRET_TREE_BRANCH_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gTretTreeBranch_Entrances[];
extern u32 gTretTreeBranch_Exits[];
extern struct ActorPlacement gTretTreeBranch_ActorPlacements[];
extern struct Trigger gTretTreeBranch_Triggers[];
extern u16 gTretTreeBranch_UnusedCellSteps[];

#endif
