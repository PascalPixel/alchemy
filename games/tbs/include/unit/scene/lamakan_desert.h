#ifndef ALCHEMY_UNIT_SCENE_LAMAKAN_DESERT_H
#define ALCHEMY_UNIT_SCENE_LAMAKAN_DESERT_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *LamakanDesert_GetEntrances(void);
const struct SceneRegion *LamakanDesert_GetRegions(void);
const u32 *LamakanDesert_GetExits(void);
const struct ScenePlacement *LamakanDesert_GetPlacements(void);
const struct SceneEvent *LamakanDesert_GetEvents(void);
s32 LamakanDesert_Initialize(s32 a0, s32 a1);
void HeatGauge_Start(void);
extern const s32 gEffectScript0[];
extern const s32 gEffectScript1[];
extern const s32 gEffectScript2[];
extern const s32 gLamakanDesertRecoveryPoints1[];
extern const s32 gLamakanDesertRecoveryPoints2[];
extern const s32 gLamakanDesertRecoveryPointsOther[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gLamakanDesertEntrancesOther[];
extern struct SceneEntrance gLamakanDesertEastEntrances[];
extern struct SceneEntrance gLamakanDesertSmogEntrances[];
extern struct SceneEntrance gLamakanDesertManticoreEntrances[];
extern struct SceneEntrance gLamakanDesertCaveEntrances[];
extern u32 gLamakanDesertExits[];
extern struct ScenePlacement gLamakanDesertPlacementsOther[];
extern struct ScenePlacement gLamakanDesertEastPlacements[];
extern struct ScenePlacement gLamakanDesertSmogPlacements[];
extern struct ScenePlacement gLamakanDesertManticorePlacements[];
extern struct SceneEvent gLamakanDesertEvents[];
extern s16 gHeatGaugeLevel;
extern s16 gHeatGaugeSlide;
extern s16 gHeatGaugeFlash;

#endif
