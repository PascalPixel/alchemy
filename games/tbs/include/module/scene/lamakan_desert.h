#ifndef ALCHEMY_MODULE_SCENE_LAMAKAN_DESERT_H
#define ALCHEMY_MODULE_SCENE_LAMAKAN_DESERT_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
void HeatGauge_Start(void);
/// The middles of the oases, where a leader stepping into one bathes: x and z
/// of each, in 16.16 fixed-point pixels.
extern const s32 gLamakanDesertEast_Oases[];
extern const s32 gLamakanDesertSmog_Oases[];
extern const s32 gLamakanDesertOasesOther[];
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
