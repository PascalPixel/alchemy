#ifndef ALCHEMY_MODULE_MAP_SUHALLA_DESERT_H
#define ALCHEMY_MODULE_MAP_SUHALLA_DESERT_H

#include "types.h"
#include "scene_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gSuhallaDesertStormScript[];
extern s32 gSuhallaDesertFourthStormScript[];
extern struct SceneEntrance gSuhallaDesertEntrancesOther[];
extern struct SceneEntrance gSuhallaDesertNorthEntrances[];
extern struct SceneEntrance gSuhallaDesertSouthEntrances[];
extern struct SceneEntrance gSuhallaDesertCavesEntrances[];
extern u32 gSuhallaDesertExits[];
extern struct ActorPlacement gSuhallaDesertActorPlacementsOther[];
extern struct ActorPlacement gSuhallaDesertNorthActorPlacements[];
extern struct ActorPlacement gSuhallaDesertSouthActorPlacements[];
extern struct ActorPlacement gSuhallaDesertCavesActorPlacements[];
extern s32 gSuhallaDesertChaseScript[];
extern struct Trigger gSuhallaDesertTriggersOther[];
extern struct Trigger gSuhallaDesertCavesTriggers[];

#endif
