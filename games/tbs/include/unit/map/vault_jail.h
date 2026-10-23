#ifndef ALCHEMY_UNIT_SCENE_VAULT_JAIL_H
#define ALCHEMY_UNIT_SCENE_VAULT_JAIL_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern const struct SceneEntrance gVaultJail_Entrances[];
extern const u32 gVaultJail_Exits[];
extern const struct ScenePlacement gVaultJail_ThiefPlacements[];
extern const struct ScenePlacement gVaultJail_PrisonerPlacements[];
extern const struct SceneEvent gVaultJail_ThiefEvents[];
extern const struct SceneEvent gVaultJail_PrisonerEvents[];

#endif
