#ifndef ALCHEMY_UNIT_MAP_LUNPA_H
#define ALCHEMY_UNIT_MAP_LUNPA_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gLunpaEntrances[];
extern struct SceneEntrance gGateEntrances[];
extern u32 gLunpaExits[];
extern u32 gGateExits[];
extern struct ActorTemplate gLunpaActorTemplates[];
extern struct ActorTemplate gGateActorTemplates[];
extern struct Trigger gLunpaSealedTriggers[];
extern struct Trigger gLunpaReopenedTriggers[];
extern struct Trigger gGateTriggers[];
extern u32 gLunpaUnusedWord;

#endif
