#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_INTERIORS_H

#include "types.h"
#include "map_tables.h"

struct SceneRegion;

const struct SceneEntrance *BilibinInteriors_GetEntrances(void);
const struct SceneRegion *BilibinInteriors_GetRegions(void);
const u32 *BilibinInteriors_GetExits(void);
const struct ScenePlacement *BilibinInteriors_GetPlacements(void);
const struct SceneEvent *BilibinInteriors_GetEvents(void);
s32 BilibinInteriors_Initialize(void);
extern struct SceneEntrance gBilibinInteriorsEntrances[];
extern u32 gBilibinInteriorsExits[];
extern struct ScenePlacement gBilibinInteriorsPlacements[];
extern struct SceneEvent gBilibinInteriorsEvents[];

#endif
