#ifndef ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H
#define ALCHEMY_UNIT_MAP_GONDOWAN_CAVE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
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
extern struct ActorTemplate gGondowanCaveActorTemplatesOther[];
extern struct ActorTemplate gGondowanPassageActorTemplates[];
extern struct ActorTemplate gGondowanPassageActorTemplatesWithIodem[];
extern struct ActorTemplate gGondowanCaveRiverActorTemplates[];
extern struct ActorTemplate gGondowanCaveActorTemplates[];
extern struct Trigger gGondowanCaveTriggersOther[];
extern struct Trigger gGondowanPassageTriggers[];
extern struct Trigger gGondowanPassageTriggersWithIodem[];
extern struct Trigger gGondowanCaveRiverTriggers[];
extern struct Trigger gGondowanCaveTriggers[];

#endif
