#ifndef ALCHEMY_UNIT_MAP_MCCOY_PALACE_GROUNDS_H
#define ALCHEMY_UNIT_MAP_MCCOY_PALACE_GROUNDS_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern u16 gPalaceGateOpenSteps[];
extern s32 gGuardWatchesParty[];
extern struct SceneEntrance gMcCoyPalaceGroundsEntrances[];
extern u32 gMcCoyPalaceGroundsExits[];
extern struct ActorTemplate gMcCoyPalaceGroundsNoActorTemplates[];
extern struct ActorTemplate gMcCoyPalaceGroundsActorTemplates[];
extern struct MapEvent gMcCoyPalaceGroundsNoEvents[];
extern struct MapEvent gMcCoyPalaceGroundsEvents[];

#endif
