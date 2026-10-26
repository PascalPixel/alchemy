#ifndef ALCHEMY_UNIT_MAP_SOL_SANCTUM_PSYNERGY_STONE_H
#define ALCHEMY_UNIT_MAP_SOL_SANCTUM_PSYNERGY_STONE_H

#include "types.h"
#include "map_tables.h"

struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumPsynergyStoneEntrances[];
extern u32 gSolSanctumPsynergyStoneExits[];
extern struct ActorTemplate gSolSanctumPsynergyStoneActorTemplates[];
extern struct Trigger gSolSanctumPsynergyStoneTriggers[];

#endif
