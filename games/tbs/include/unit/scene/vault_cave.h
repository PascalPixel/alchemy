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
extern struct SceneEntrance gVaultCave_FallbackEntrances[];
extern struct SceneEntrance gVaultCavePassage_Entrances[];
extern struct SceneEntrance gVaultCaveVambrace_Entrances[];
extern struct SceneEntrance gVaultCaveMouth_Entrances[];
extern struct SceneRegion gVaultCave_Regions[];
extern u32 gVaultCave_FallbackExits[];
extern u32 gVaultCavePassage_Exits[];
extern u32 gVaultCaveVambrace_Exits[];
extern u32 gVaultCaveMouth_Exits[];
extern struct ScenePlacement gVaultCave_FallbackPlacements[];
extern struct ScenePlacement gVaultCavePassage_Placements[];
extern struct ScenePlacement gVaultCaveVambrace_Placements[];
extern struct ScenePlacement gVaultCaveMouth_Placements[];
extern struct SceneEvent gVaultCave_FallbackEvents[];
extern struct SceneEvent gVaultCavePassage_Events[];
extern struct SceneEvent gVaultCaveVambrace_Events[];
extern struct SceneEvent gVaultCaveMouth_Events[];
extern u32 gVaultCave_ScheduleFrames[];
extern s32 gVaultCave_ScheduleTimer;
extern s32 gVaultCave_ScheduleIndex;
extern s32 gVaultCave_TickCounter;
extern s32 gVaultCave_TickValue;

#endif
