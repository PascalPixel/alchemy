#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_STEPPING_STONES_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_STEPPING_STONES_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Scene_GetWarpDests(void);
const struct MapWarpRegion *Scene_GetWarpRegions(void);
const u32 *Scene_GetWarps(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gCrossboneIsleSteppingStones_ActionTable[];
extern u16 gCrossboneIsleSteppingStones_SwitchCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_SwitchOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[];
extern struct MapWarpDest gCrossboneIsleSteppingStonesWarpDestsOther[];
extern struct MapWarpDest gCrossboneIsleSteppingStonesWarpDests[];
extern struct MapWarpDest gCrossboneIsleIcePillarsWarpDests[];
extern struct MapWarpDest gCrossboneIsleFloodgateWarpDests[];
extern struct MapWarpDest gCrossboneIsleStackedPillarsWarpDests[];
extern struct MapWarpDest gCrossboneIsleDragonStatueWarpDests[];
extern u32 gCrossboneIsleSteppingStonesWarps[];
extern struct ObjectEventTemplate
    gCrossboneIsleSteppingStonesNoObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCrossboneIsleSteppingStonesObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCrossboneIsleIcePillarsObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsleFloodgateObjectEventTemplates[];
extern struct ObjectEventTemplate
    gCrossboneIsleSteppingStonesObjectEventTemplatesOther[];
extern struct ObjectEventTemplate
    gCrossboneIsleDragonStatueObjectEventTemplates[];
extern struct MapEvent gCrossboneIsleSteppingStonesEventsOther[];
extern struct MapEvent gCrossboneIsleSteppingStonesEvents[];
extern struct MapEvent gCrossboneIsleIcePillarsEvents[];
extern struct MapEvent gCrossboneIsleFloodgateEvents[];
extern struct MapEvent gCrossboneIsleStackedPillarsEvents[];
extern struct MapEvent gCrossboneIsleDragonStatueEvents[];

#endif
