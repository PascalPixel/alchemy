#ifndef ALCHEMY_UNIT_MAP_SUHALLA_DESERT_H
#define ALCHEMY_UNIT_MAP_SUHALLA_DESERT_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
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
extern struct ActorTemplate gSuhallaDesertActorTemplatesOther[];
extern struct ActorTemplate gSuhallaDesertNorthActorTemplates[];
extern struct ActorTemplate gSuhallaDesertSouthActorTemplates[];
extern struct ActorTemplate gSuhallaDesertCavesActorTemplates[];
extern s32 gSuhallaDesertChaseScript[];
extern struct Trigger gSuhallaDesertTriggersOther[];
extern struct Trigger gSuhallaDesertCavesTriggers[];

#endif
