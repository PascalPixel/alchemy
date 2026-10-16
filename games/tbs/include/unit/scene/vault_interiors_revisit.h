#ifndef ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_REVISIT_H
#define ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_REVISIT_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *VaultInteriorsRevisit_GetEntrances(void);
const struct SceneRegion *VaultInteriorsRevisit_GetRegions(void);
const u32 *VaultInteriorsRevisit_GetExits(void);
const struct ScenePlacement *VaultInteriorsRevisit_GetPlacements(void);
const struct SceneEvent *VaultInteriorsRevisit_GetEvents(void);
s32 VaultInteriorsRevisit_Initialize(void);
extern struct SceneEntrance gVaultInteriorsRevisit_Entrances[];
extern u32 gVaultInteriorsRevisit_Exits[];
extern struct ScenePlacement gVaultInteriorsRevisit_Placements[];
extern struct SceneEvent gVaultInteriorsRevisit_Events[];

#endif
