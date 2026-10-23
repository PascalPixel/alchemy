#ifndef ALCHEMY_UNIT_MAP_LAMAKAN_DESERT_H
#define ALCHEMY_UNIT_MAP_LAMAKAN_DESERT_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *LamakanDesert_GetWarpDests(void);
const struct MapWarpRegion *LamakanDesert_GetRegions(void);
const u32 *LamakanDesert_GetWarps(void);
const struct ObjectEventTemplate *LamakanDesert_GetObjectEventTemplates(void);
const struct MapEvent *LamakanDesert_GetEvents(void);
s32 LamakanDesert_Initialize(s32 a0, s32 a1);
void HeatGauge_Start(void);
extern const s32 gFieldEffect_ShortScript[];
extern const s32 gFieldEffect_MediumScript[];
extern const s32 gFieldEffect_LongScript[];
extern const s32 gLamakanDesertEast_Oases[];
extern const s32 gLamakanDesertSmog_Oases[];
extern const s32 gLamakanDesertOasesOther[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct MapWarpDest gLamakanDesertWarpDestsOther[];
extern struct MapWarpDest gLamakanDesertEastWarpDests[];
extern struct MapWarpDest gLamakanDesertSmogWarpDests[];
extern struct MapWarpDest gLamakanDesertManticoreWarpDests[];
extern struct MapWarpDest gLamakanDesertCaveWarpDests[];
extern u32 gLamakanDesertWarps[];
extern struct ObjectEventTemplate gLamakanDesertObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gLamakanDesertEastObjectEventTemplates[];
extern struct ObjectEventTemplate gLamakanDesertSmogObjectEventTemplates[];
extern struct ObjectEventTemplate gLamakanDesertManticoreObjectEventTemplates[];
extern struct MapEvent gLamakanDesertEvents[];
extern s16 gHeatGaugeLevel;
extern s16 gHeatGaugeSlide;
extern s16 gHeatGaugeFlash;

#endif
