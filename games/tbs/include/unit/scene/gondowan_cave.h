#ifndef ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H
#define ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gGondowanCaveEntrancesOther[];
extern struct SceneEntrance gGondowanPassageEntrances[];
extern struct SceneEntrance gGondowanCaveRiverEntrances[];
extern struct SceneEntrance gGondowanCaveEntrances[];
extern u32 gGondowanCaveExits[];
extern struct ActorPlacement gGondowanCaveActorPlacementsOther[];
extern struct ActorPlacement gGondowanPassageActorPlacements[];
extern struct ActorPlacement gGondowanPassageActorPlacementsWithIodem[];
extern struct ActorPlacement gGondowanCaveRiverActorPlacements[];
extern struct ActorPlacement gGondowanCaveActorPlacements[];
extern struct Trigger gGondowanCaveTriggersOther[];
extern struct Trigger gGondowanPassageTriggers[];
extern struct Trigger gGondowanPassageTriggersWithIodem[];
extern struct Trigger gGondowanCaveRiverTriggers[];
extern struct Trigger gGondowanCaveTriggers[];

#endif
