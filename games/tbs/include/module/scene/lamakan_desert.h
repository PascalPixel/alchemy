#ifndef ALCHEMY_MODULE_MAP_LAMAKAN_DESERT_H
#define ALCHEMY_MODULE_MAP_LAMAKAN_DESERT_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *LamakanDesert_GetEntrances(void);
const struct SceneExitRegion *LamakanDesert_GetRegions(void);
const u32 *LamakanDesert_GetExits(void);
const struct ActorPlacement *LamakanDesert_GetActorPlacements(void);
const struct Trigger *LamakanDesert_GetTriggers(void);
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
extern struct ActorPlacement gLamakanDesertActorPlacementsOther[];
extern struct ActorPlacement gLamakanDesertEastActorPlacements[];
extern struct ActorPlacement gLamakanDesertSmogActorPlacements[];
extern struct ActorPlacement gLamakanDesertManticoreActorPlacements[];
extern struct Trigger gLamakanDesertTriggers[];
extern s16 gHeatGaugeLevel;
extern s16 gHeatGaugeSlide;
extern s16 gHeatGaugeFlash;

#endif
