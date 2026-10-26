#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_CLIFF_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_CLIFF_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *KolimaForestCliff_GetEntrances(void);
const struct SceneExitRegion *KolimaForestCliff_GetRegions(void);
const u32 *KolimaForestCliff_GetExits(void);
const struct ActorTemplate *KolimaForestCliff_GetActorTemplates(void);
const struct Trigger *KolimaForestCliff_GetTriggers(void);
s32 KolimaForestCliff_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern struct SceneEntrance gKolimaForestCliffEntrances[];
extern u32 gKolimaForestCliffExits[];
extern struct ActorTemplate gKolimaForestCliffActorTemplates[];
extern struct Trigger gKolimaForestCliffTriggers[];

#endif
