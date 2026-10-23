#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_INTERIORS_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *KolimaInteriors_GetEntrances(void);
const struct SceneRegion *KolimaInteriors_GetRegions(void);
const u32 *KolimaInteriors_GetExits(void);
const struct ScenePlacement *KolimaInteriors_GetPlacements(void);
const struct SceneEvent *KolimaInteriors_GetEvents(void);
s32 KolimaInteriors_Initialize(void);
extern struct SceneEntrance gKolimaInteriors_Entrances[];
extern u32 gKolimaInteriors_Exits[];
extern struct ScenePlacement gKolimaInteriors_Placements[];
extern struct SceneEvent gKolimaInteriors_Events[];

#endif
