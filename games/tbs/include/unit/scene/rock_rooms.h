#ifndef ALCHEMY_UNIT_MAP_ROCK_ROOMS_H
#define ALCHEMY_UNIT_MAP_ROCK_ROOMS_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

extern const struct SceneEntrance gInteriorEntrances[];
extern const u32 gInteriorExits[];
extern const struct ActorTemplate gInteriorActorTemplates[];
extern const struct Trigger gInteriorSealedTriggers[];
extern const struct Trigger gInteriorReopenedTriggers[];
extern const struct Trigger gGateToSuhallaTriggers[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);

#endif
