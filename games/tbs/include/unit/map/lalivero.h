#ifndef ALCHEMY_UNIT_SCENE_LALIVERO_H
#define ALCHEMY_UNIT_SCENE_LALIVERO_H

#include "types.h"
#include "map_tables.h"

struct SceneRegion;

const struct SceneEntrance *Lalivero_GetEntrances(void);
const struct SceneRegion *Lalivero_GetRegions(void);
const u32 *Lalivero_GetExits(void);
const struct ScenePlacement *Lalivero_GetPlacements(void);
const struct SceneEvent *Lalivero_GetEvents(void);
s32 Lalivero_Initialize(void);
extern s32 gLalivero_CryingScript[];
extern struct SceneEntrance gLaliveroEntrances[];
extern u32 gLaliveroExits[];
extern struct ScenePlacement gLaliveroPlacements[];
extern struct ScenePlacement gLaliveroPlacementsAfterLighthouse[];
extern s16 gLaliveroDoorCells[][2];
extern u16 gLaliveroDoorOpenSteps[];
extern u16 gLaliveroGateOpenSteps[];
extern struct SceneEvent gLaliveroEvents[];
extern struct SceneEvent gLaliveroEventsAfterLighthouse[];

#endif
