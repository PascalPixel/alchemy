#ifndef ALCHEMY_UNIT_MAP_TRET_TREE_BRANCH_H
#define ALCHEMY_UNIT_MAP_TRET_TREE_BRANCH_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *TretTreeBranch_GetEntrances(void);
const struct SceneExitRegion *TretTreeBranch_GetRegions(void);
const u32 *TretTreeBranch_GetExits(void);
const struct ObjectEventTemplate *TretTreeBranch_GetObjectEventTemplates(void);
const struct MapEvent *TretTreeBranch_GetEvents(void);
s32 TretTreeBranch_Initialize(void);
extern struct SceneEntrance gTretTreeBranch_Entrances[];
extern u32 gTretTreeBranch_Exits[];
extern struct ObjectEventTemplate gTretTreeBranch_ObjectEventTemplates[];
extern struct MapEvent gTretTreeBranch_Events[];
extern u16 gTretTreeBranch_UnusedCellSteps[];

#endif
