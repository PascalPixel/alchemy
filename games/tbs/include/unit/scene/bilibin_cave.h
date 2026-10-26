#ifndef ALCHEMY_UNIT_MAP_BILIBIN_CAVE_H
#define ALCHEMY_UNIT_MAP_BILIBIN_CAVE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ActorTemplate;

const struct SceneEntrance *BilibinCave_GetEntrances(void);
const struct SceneExitRegion *BilibinCave_GetRegions(void);
const u32 *BilibinCave_GetExits(void);
const struct ActorTemplate *BilibinCave_GetActorTemplates(void);
const struct MapEvent *BilibinCave_GetEvents(void);
s32 BilibinCave_Initialize(void);
extern s32 gBilibinCave_ClearSpriteScript[];
extern struct SceneEntrance gBilibinCaveEntrancesOther[];
extern struct SceneEntrance gBilibinCaveEntranceEntrances[];
extern struct SceneEntrance gBilibinCaveRapierEntrances[];
extern struct SceneEntrance gBilibinCaveIceEntrances[];
extern u32 gBilibinCaveExits[];
extern struct ActorTemplate gBilibinCaveActorTemplatesOther[];
extern struct ActorTemplate gBilibinCaveEntranceActorTemplates[];
extern struct ActorTemplate gBilibinCaveRapierActorTemplates[];
extern struct ActorTemplate gBilibinCaveIceActorTemplates[];
extern struct MapEvent gBilibinCaveEventsOther[];
extern struct MapEvent gBilibinCaveEntranceEvents[];
extern struct MapEvent gBilibinCaveRapierEvents[];
extern struct MapEvent gBilibinCaveIceEvents[];
extern s32 gBilibinCave_PushSteps[];

#endif
