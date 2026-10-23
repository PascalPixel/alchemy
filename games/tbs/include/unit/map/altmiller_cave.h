#ifndef ALCHEMY_UNIT_MAP_ALTMILLER_CAVE_H
#define ALCHEMY_UNIT_MAP_ALTMILLER_CAVE_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ObjectEventTemplate;
struct LightWheel;

const struct MapWarpDest *AltmillerCave_GetWarpDests(void);
const struct MapWarpRegion *AltmillerCave_GetRegions(void);
const u32 *AltmillerCave_GetWarps(void);
const struct ObjectEventTemplate *AltmillerCave_GetObjectEventTemplates(void);
const struct MapEvent *AltmillerCave_GetEvents(void);
s32 AltmillerCave_Initialize(void);
extern s32 gStagedObjectEvent_DirectionSteps[];
extern u32 gStagedObjectEvent_FootprintKinds[];
extern s32 gStagedObjectEvent_FootprintBounds[];
extern struct LightWheel *gAltmillerCave_LightWheel;
extern struct MapWarpDest gAltmillerCave_WarpDests[];
extern u32 gAltmillerCave_Warps[];
extern struct ObjectEventTemplate gAltmillerCave_ObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gAltmillerCave_BabiObjectEventTemplates[];
extern struct ObjectEventTemplate gAltmillerCave_SquallObjectEventTemplates[];
extern struct ObjectEventTemplate gAltmillerCave_DraughtObjectEventTemplates[];
extern u32 gAltmillerCave_PulseScales[];
extern s32 gAltmillerCave_GuardsArrivePath[];
extern s32 gAltmillerCave_SecondGuardLeavePath[];
extern s32 gAltmillerCave_FirstGuardLeavePath[];
extern s32 gAltmillerCave_BabiLeavePath[];
extern struct MapEvent gAltmillerCave_EventsOther[];
extern struct MapEvent gAltmillerCave_BabiEvents[];
extern struct MapEvent gAltmillerCave_VialChestEvents[];
extern struct MapEvent gAltmillerCave_SquallEvents[];
extern struct MapEvent gAltmillerCave_CookieChestEvents[];
extern struct MapEvent gAltmillerCave_DraughtEvents[];

#endif
