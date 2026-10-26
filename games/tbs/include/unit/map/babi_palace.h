#ifndef ALCHEMY_UNIT_MAP_BABI_PALACE_H
#define ALCHEMY_UNIT_MAP_BABI_PALACE_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *BabiPalace_GetRegions(void);
const u32 *BabiPalace_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 BabiPalace_ApplyEntryState(void);
extern u16 gBabiPalace_DoorCells[];
extern s32 gBabiPalace_IndigestionScript[];
extern struct MapWarpDest gBabiPalace_WarpDests[];
extern struct MapWarpDest gBabiPalace_OutsideWarpDests[];
extern struct MapWarpRegion gBabiPalace_Regions[];
extern u32 gBabiPalace_Warps[];
extern struct ObjectEventTemplate gBabiPalace_OutsideObjectEventTemplates[];
extern struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesBabiMissing[];
extern struct ObjectEventTemplate gBabiPalace_ObjectEventTemplatesFinals[];
extern struct ObjectEventTemplate
    gBabiPalace_ObjectEventTemplatesAfterColosso[];
extern struct MapEvent gBabiPalace_EventsBabiMissing[];
extern struct MapEvent gBabiPalace_EventsFinals[];
extern struct MapEvent gBabiPalace_EventsAfterColosso[];
extern struct MapEvent gBabiPalace_OutsideEventsBabiMissing[];
extern struct MapEvent gBabiPalace_OutsideEventsFinals[];
extern struct MapEvent gBabiPalace_OutsideEventsAfterColosso[];

#endif
