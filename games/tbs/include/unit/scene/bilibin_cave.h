#ifndef ALCHEMY_UNIT_MAP_BILIBIN_CAVE_H
#define ALCHEMY_UNIT_MAP_BILIBIN_CAVE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;

const struct SceneEntrance *BilibinCave_GetEntrances(void);
const struct SceneExitRegion *BilibinCave_GetRegions(void);
const u32 *BilibinCave_GetExits(void);
const struct ActorPlacement *BilibinCave_GetActorPlacements(void);
const struct Trigger *BilibinCave_GetTriggers(void);
s32 BilibinCave_Initialize(void);
extern s32 gBilibinCave_ClearSpriteScript[];
extern struct SceneEntrance gBilibinCaveEntrancesOther[];
extern struct SceneEntrance gBilibinCaveEntranceEntrances[];
extern struct SceneEntrance gBilibinCaveRapierEntrances[];
extern struct SceneEntrance gBilibinCaveIceEntrances[];
extern u32 gBilibinCaveExits[];
extern struct ActorPlacement gBilibinCaveActorPlacementsOther[];
extern struct ActorPlacement gBilibinCaveEntranceActorPlacements[];
extern struct ActorPlacement gBilibinCaveRapierActorPlacements[];
extern struct ActorPlacement gBilibinCaveIceActorPlacements[];
extern struct Trigger gBilibinCaveTriggersOther[];
extern struct Trigger gBilibinCaveEntranceTriggers[];
extern struct Trigger gBilibinCaveRapierTriggers[];
extern struct Trigger gBilibinCaveIceTriggers[];
extern s32 gBilibinCave_PushSteps[];

#endif
