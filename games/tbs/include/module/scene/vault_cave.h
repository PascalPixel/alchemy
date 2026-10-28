#ifndef ALCHEMY_MODULE_MAP_VAULT_CAVE_H
#define ALCHEMY_MODULE_MAP_VAULT_CAVE_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern const s16 gVaultCave_DirectionSteps[];
extern const s32 gVaultCave_PuffScript[];
extern struct SceneEntrance gVaultCave_FallbackEntrances[];
extern struct SceneEntrance gVaultCavePassage_Entrances[];
extern struct SceneEntrance gVaultCaveVambrace_Entrances[];
extern struct SceneEntrance gVaultCaveMouth_Entrances[];
extern struct SceneExitRegion gVaultCave_Regions[];
extern u32 gVaultCave_FallbackExits[];
extern u32 gVaultCavePassage_Exits[];
extern u32 gVaultCaveVambrace_Exits[];
extern u32 gVaultCaveMouth_Exits[];
extern struct ActorPlacement gVaultCave_FallbackActorPlacements[];
extern struct ActorPlacement gVaultCavePassage_ActorPlacements[];
extern struct ActorPlacement gVaultCaveVambrace_ActorPlacements[];
extern struct ActorPlacement gVaultCaveMouth_ActorPlacements[];
extern struct Trigger gVaultCave_FallbackTriggers[];
extern struct Trigger gVaultCavePassage_Triggers[];
extern struct Trigger gVaultCaveVambrace_Triggers[];
extern struct Trigger gVaultCaveMouth_Triggers[];
extern u32 gVaultCave_ScheduleFrames[];
extern s32 gVaultCave_ScheduleTimer;
extern s32 gVaultCave_ScheduleIndex;
extern s32 gVaultCave_DropFrames;
extern s32 gVaultCave_SelectedDrop;

#endif
