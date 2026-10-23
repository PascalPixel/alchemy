#ifndef ALCHEMY_UNIT_MAP_ALPINE_CROSSING_LAMA_TEMPLE_H
#define ALCHEMY_UNIT_MAP_ALPINE_CROSSING_LAMA_TEMPLE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern s32 gAlpineCrossing_WorkerObjectEventTemplateScript[];
extern s32 gAlpineCrossing_FeizhiPath[];
extern s32 gAlpineCrossing_LeaderPath[];
extern s32 gAlpineCrossing_HsuStruggle[];
extern u16 gLamaTemple_DoorOpenCells[];
extern u16 gLamaTemple_DoorCloseCells[];
extern struct MapWarpDest gLamaTemple_WarpDests[];
extern struct MapWarpDest gAlpineCrossing_WarpDests[];
extern u32 gAlpineCrossingLamaTemple_Warps[];
extern struct ObjectEventTemplate gLamaTemple_ObjectEventTemplates[];
extern struct ObjectEventTemplate gAlpineCrossing_ObjectEventTemplates[];
extern struct MapEvent gLamaTemple_Events[];
extern struct MapEvent gAlpineCrossing_Events[];

#endif
