#ifndef ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_REVISIT_H
#define ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_REVISIT_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gVaultInteriorsRevisit_Entrances[];
extern u32 gVaultInteriorsRevisit_Exits[];
extern struct ScenePlacement gVaultInteriorsRevisit_Placements[];
extern struct SceneEvent gVaultInteriorsRevisit_Events[];

#endif
