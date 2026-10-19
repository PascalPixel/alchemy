#ifndef ALCHEMY_UNIT_SCENE_VAULT_CAVE_H
#define ALCHEMY_UNIT_SCENE_VAULT_CAVE_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern const s16 gVaultCave_DirectionSteps[];
extern const s32 gVaultCave_PuffScript[];
extern struct SceneEntrance gVaultCaveEntrancesOther[];
extern struct SceneEntrance gVaultCaveEntrances1[];
extern struct SceneEntrance gVaultCaveEntrances2[];
extern struct SceneEntrance gVaultCaveEntrances3[];
extern struct SceneRegion gVaultCaveRegions[];
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
extern u32 gVaultCave_ScheduleFrames[];
extern s32 gVaultCave_ScheduleTimer;
extern s32 gVaultCave_ScheduleIndex;
extern s32 gVaultCave_TickCounter;
extern s32 gVaultCave_TickValue;

#endif
