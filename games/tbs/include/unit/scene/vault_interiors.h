#ifndef ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_VAULT_INTERIORS_H

#include "types.h"
#include "field_scene.h"
#include "static_assert.h"

struct SceneEvent;
struct ScenePlacement;

/// Each stop links to up to three others, with a heading for each connection.
struct VaultInteriorsStopConnection {
  u8 stopIndex;
  u8 facing;
  u8 padding[2];
};

/// A stop on the route network, two cells square, with its links to others.
struct VaultInteriorsStop {
  u8 gridX; ///< 8-pixel cells from x 64
  u8 gridZ; ///< 8-pixel cells from z 624
  u8 padding[2];
  struct VaultInteriorsStopConnection connections[3];
};

STATIC_ASSERT(sizeof(struct VaultInteriorsStopConnection) == 4);
STATIC_ASSERT(sizeof(struct VaultInteriorsStop) == 16);

struct SceneEntrance *Scene_GetEntrances(void);
s16 *Scene_GetRegions(void);
u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct VaultInteriorsStop gVaultInteriors_Stops[];
extern s32 gVaultInteriors_ActionTable[];
extern s32 gVaultInteriors_Villager11ExitScript[];
extern s32 gVaultInteriors_Villager12ExitScript[];
extern s32 gVaultInteriors_VaultScriptA[];
extern s32 gVaultInteriors_VaultScriptB[];
extern s32 gVaultInteriors_VaultScriptC[];
extern s32 gVaultInteriors_VaultScriptD[];
extern s32 gVaultInteriors_VaultScriptE[];
extern s32 gVaultInteriors_PairScriptA[];
extern s32 gVaultInteriors_PairScriptB[];
extern s32 gVaultInteriors_PairScriptC[];
extern s32 gVaultInteriors_PairScriptD[];
extern s32 gVaultInteriors_PairScriptE[];
extern s32 gVaultInteriors_PairScriptF[];
extern s32 gVaultInteriors_PairScriptG[];
extern s32 gVaultInteriors_PairScriptH[];
extern s32 gVaultInteriors_PairScriptI[];
extern s32 gVaultInteriors_PairScriptJ[];
extern s32 gVaultInteriors_PairScriptK[];
extern s32 gVaultInteriors_PairScriptL[];
extern s32 gVaultInteriors_PairScriptM[];
extern s32 gVaultInteriors_PairScriptN[];
extern s32 gVaultInteriors_PairScriptO[];
extern s32 gVaultInteriors_PairScriptP[];
extern s32 gVaultInteriors_PairScriptQ[];
extern s32 gVaultInteriors_PairScriptR[];
extern struct SceneEntrance gVaultInteriors_Entrances[];
extern s16 gVaultInteriors_Regions[];
extern u32 gVaultInteriors_Exits[];
extern struct ScenePlacement gVaultInteriors_Placements[];
extern struct ScenePlacement gVaultInteriors_AtticPlacements[];
extern struct SceneEvent gVaultInteriors_Events[];
extern struct SceneEvent gVaultInteriors_AtticEvents[];
extern struct SceneEvent gVaultInteriors_EventsAfterThieves[];
extern s32 *gVaultInteriors_StepActions[];
extern s32 *gVaultInteriors_IdleActions[];
extern s32 *gVaultInteriors_ResponseScripts[];

#endif
