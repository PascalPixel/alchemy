#ifndef ALCHEMY_MODULE_MAP_MCCOY_PALACE_H
#define ALCHEMY_MODULE_MAP_MCCOY_PALACE_H

#include "types.h"
#include "scene_tables.h"

struct Trigger;
struct ActorPlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gPalace_FacingActions[];
extern s32 gPalace_PacingActions[];
extern struct SceneEntrance gMcCoyPalaceEntrances[];
extern u32 gMcCoyPalaceExits[];
extern struct ActorPlacement gMcCoyPalaceNoActorPlacements[];
extern struct ActorPlacement gMcCoyPalaceActorPlacements[];
extern struct Trigger gMcCoyPalaceNoTriggers[];
extern struct Trigger gMcCoyPalaceTriggers[];

#endif
