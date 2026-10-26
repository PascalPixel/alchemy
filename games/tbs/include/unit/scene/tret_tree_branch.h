#ifndef ALCHEMY_UNIT_MAP_TRET_TREE_BRANCH_H
#define ALCHEMY_UNIT_MAP_TRET_TREE_BRANCH_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *TretTreeBranch_GetWarpDests(void);
const struct MapWarpRegion *TretTreeBranch_GetRegions(void);
const u32 *TretTreeBranch_GetWarps(void);
const struct ObjectEventTemplate *TretTreeBranch_GetObjectEventTemplates(void);
const struct MapEvent *TretTreeBranch_GetEvents(void);
s32 TretTreeBranch_Initialize(void);
extern struct MapWarpDest gTretTreeBranch_WarpDests[];
extern u32 gTretTreeBranch_Warps[];
extern struct ObjectEventTemplate gTretTreeBranch_ObjectEventTemplates[];
extern struct MapEvent gTretTreeBranch_Events[];
extern u16 gTretTreeBranch_UnusedCellSteps[];

#endif
