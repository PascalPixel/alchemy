#ifndef ALCHEMY_UNIT_MAP_LAMAKAN_DESERT_H
#define ALCHEMY_UNIT_MAP_LAMAKAN_DESERT_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *LamakanDesert_GetEntrances(void);
const struct SceneExitRegion *LamakanDesert_GetRegions(void);
const u32 *LamakanDesert_GetExits(void);
const struct ObjectEventTemplate *LamakanDesert_GetObjectEventTemplates(void);
const struct MapEvent *LamakanDesert_GetEvents(void);
s32 LamakanDesert_Initialize(void);
void HeatGauge_Start(void);
extern const s32 gFieldEffect_ShortScript[];
extern const s32 gFieldEffect_MediumScript[];
extern const s32 gFieldEffect_LongScript[];
extern const s32 gLamakanDesertEast_Oases[];
extern const s32 gLamakanDesertSmog_Oases[];
extern const s32 gLamakanDesertOasesOther[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gLamakanDesertEntrancesOther[];
extern struct SceneEntrance gLamakanDesertEastEntrances[];
extern struct SceneEntrance gLamakanDesertSmogEntrances[];
extern struct SceneEntrance gLamakanDesertManticoreEntrances[];
extern struct SceneEntrance gLamakanDesertCaveEntrances[];
extern u32 gLamakanDesertExits[];
extern struct ObjectEventTemplate gLamakanDesertObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gLamakanDesertEastObjectEventTemplates[];
extern struct ObjectEventTemplate gLamakanDesertSmogObjectEventTemplates[];
extern struct ObjectEventTemplate gLamakanDesertManticoreObjectEventTemplates[];
extern struct MapEvent gLamakanDesertEvents[];
extern s16 gHeatGaugeLevel;
extern s16 gHeatGaugeSlide;
extern s16 gHeatGaugeFlash;

#endif
