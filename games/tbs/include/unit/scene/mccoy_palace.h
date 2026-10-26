#ifndef ALCHEMY_UNIT_MAP_MCCOY_PALACE_H
#define ALCHEMY_UNIT_MAP_MCCOY_PALACE_H

#include "types.h"
#include "scene_tables.h"

struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gPalace_FacingActions[];
extern s32 gPalace_PacingActions[];
extern struct SceneEntrance gMcCoyPalaceEntrances[];
extern u32 gMcCoyPalaceExits[];
extern struct ActorTemplate gMcCoyPalaceNoActorTemplates[];
extern struct ActorTemplate gMcCoyPalaceActorTemplates[];
extern struct Trigger gMcCoyPalaceNoTriggers[];
extern struct Trigger gMcCoyPalaceTriggers[];

#endif
