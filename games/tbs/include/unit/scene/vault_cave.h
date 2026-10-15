#ifndef ALCHEMY_UNIT_SCENE_VAULT_CAVE_H
#define ALCHEMY_UNIT_SCENE_VAULT_CAVE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
u8 *VaultCave_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 VaultCave_ApplyEntryState(void);
extern const s16 VaultCave_DirectionSteps[];
extern const s32 VaultCave_PuffScript[];
extern struct SceneEntrance gVaultCaveEntrancesOther[];
extern struct SceneEntrance gVaultCaveEntrances1[];
extern struct SceneEntrance gVaultCaveEntrances2[];
extern struct SceneEntrance gVaultCaveEntrances3[];
extern s16 gVaultCaveRegions[];
extern u32 gVaultCaveExitsOther[];
extern u32 gVaultCaveExits1[];
extern u32 gVaultCaveExits2[];
extern u32 gVaultCaveExits3[];
extern struct ScenePlacement gVaultCavePlacementsOther[];
extern struct ScenePlacement gVaultCavePlacements1[];
extern struct ScenePlacement gVaultCavePlacements2[];
extern struct ScenePlacement gVaultCavePlacements3[];
extern struct SceneEvent gVaultCaveEventsOther[];
extern struct SceneEvent gVaultCaveEvents1[];
extern struct SceneEvent gVaultCaveEvents2[];
extern struct SceneEvent gVaultCaveEvents3[];
extern u32 VaultCave_ScheduleFrames[];
extern s32 VaultCave_ScheduleTimer;
extern s32 VaultCave_ScheduleIndex;
extern s32 VaultCave_TickCounter;
extern s32 VaultCave_TickValue;

#endif
