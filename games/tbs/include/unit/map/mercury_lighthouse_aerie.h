#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_AERIE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *MercuryLighthouseAerie_GetWarpDests(void);
const struct MapWarpRegion *MercuryLighthouseAerie_GetRegions(void);
const u32 *MercuryLighthouseAerie_GetWarps(void);
const struct ObjectEventTemplate *
MercuryLighthouseAerie_GetObjectEventTemplates(void);
const struct MapEvent *MercuryLighthouseAerie_GetEvents(void);
s32 MercuryLighthouseAerie_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern struct MapWarpDest gMercuryLighthouseAerieWarpDests[];
extern u32 gMercuryLighthouseAerieWarps[];
extern struct ObjectEventTemplate gMercuryLighthouseAerieObjectEventTemplates[];
extern struct MapEvent gMercuryLighthouseAerieEvents[];
extern s32 gMercuryLighthouseAerie_PlatformUnderfoot;
extern s32 gMercuryLighthouseAerie_SparkScript[];

#endif
