#ifndef ALCHEMY_UNIT_MAP_TREE_ROOMS_H
#define ALCHEMY_UNIT_MAP_TREE_ROOMS_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 TreeRooms_EnterRoom(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s16 gTreeRooms_MapPatches[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gTreeRooms_LeaderGathersActions[];
extern s32 gTreeRooms_GaretGathersActions[];
extern s32 gTreeRooms_IvanGathersActions[];
extern s32 gTreeRooms_MiaGathersActions[];
extern s32 gTreeRooms_FaceSinksActions[];
extern s32 gTreeRooms_RejoinLeaderActions[];
extern u32 gTreeRooms_TintSteps[];
extern struct MapWarpDest gTreeRoomsWarpDests[];
extern struct MapWarpRegion gTreeRoomsRegions[];
extern u32 gTreeRoomsWarps[];
extern struct ObjectEventTemplate gTreeRoomsObjectEventTemplates[];
extern struct MapEvent gTreeRoomsEvents[];
extern s32 gTreeRooms_TintIndex;

#endif
