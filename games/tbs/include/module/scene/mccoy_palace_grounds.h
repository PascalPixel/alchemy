#ifndef ALCHEMY_MODULE_MAP_MCCOY_PALACE_GROUNDS_H
#define ALCHEMY_MODULE_MAP_MCCOY_PALACE_GROUNDS_H

#include "types.h"
#include "scene_tables.h"

struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern u16 gPalaceGateOpenSteps[];
extern s32 gGuardWatchesParty[];
extern struct SceneEntrance gMcCoyPalaceGroundsEntrances[];
extern u32 gMcCoyPalaceGroundsExits[];
extern struct ActorPlacement gMcCoyPalaceGroundsNoActorPlacements[];
extern struct ActorPlacement gMcCoyPalaceGroundsActorPlacements[];
extern struct Trigger gMcCoyPalaceGroundsNoTriggers[];
extern struct Trigger gMcCoyPalaceGroundsTriggers[];

#endif
