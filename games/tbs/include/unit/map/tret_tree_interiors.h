#ifndef ALCHEMY_UNIT_MAP_TRET_TREE_INTERIORS_H
#define ALCHEMY_UNIT_MAP_TRET_TREE_INTERIORS_H

#include "types.h"
#include "map_tables.h"
#include "field_runtime.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 TretTreeInteriors_EnterRoom(void);
extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern s16 gTretTreeInteriors_MapPatches[];
extern const s32 *gEffectScripts[];
extern s32 gTretTreeInteriors_LeaderGathersActions[];
extern s32 gTretTreeInteriors_GaretGathersActions[];
extern s32 gTretTreeInteriors_IvanGathersActions[];
extern s32 gTretTreeInteriors_MiaGathersActions[];
extern s32 gTretTreeInteriors_FaceSinksActions[];
extern s32 gTretTreeInteriors_RejoinLeaderActions[];
extern u32 gTretTreeInteriors_TintSteps[];
extern struct MapWarpDest gTretTreeInteriorsWarpDests[];
extern struct MapWarpRegion gTretTreeInteriorsRegions[];
extern u32 gTretTreeInteriorsWarps[];
extern struct ObjectEventTemplate gTretTreeInteriorsObjectEventTemplates[];
extern struct MapEvent gTretTreeInteriorsEvents[];
extern s32 gTretTreeInteriors_TintIndex;

#endif
