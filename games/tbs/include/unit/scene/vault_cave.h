#ifndef ALCHEMY_UNIT_MAP_VAULT_CAVE_H
#define ALCHEMY_UNIT_MAP_VAULT_CAVE_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
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
extern struct ActorTemplate gVaultCave_FallbackActorTemplates[];
extern struct ActorTemplate gVaultCavePassage_ActorTemplates[];
extern struct ActorTemplate gVaultCaveVambrace_ActorTemplates[];
extern struct ActorTemplate gVaultCaveMouth_ActorTemplates[];
extern struct Trigger gVaultCave_FallbackTriggers[];
extern struct Trigger gVaultCavePassage_Triggers[];
extern struct Trigger gVaultCaveVambrace_Triggers[];
extern struct Trigger gVaultCaveMouth_Triggers[];
extern u32 gVaultCave_ScheduleFrames[];
extern s32 gVaultCave_ScheduleTimer;
extern s32 gVaultCave_ScheduleIndex;
extern s32 gVaultCave_TickCounter;
extern s32 gVaultCave_TickValue;

#endif
