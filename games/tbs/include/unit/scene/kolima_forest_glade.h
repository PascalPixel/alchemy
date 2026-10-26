#ifndef ALCHEMY_UNIT_MAP_KOLIMA_FOREST_GLADE_H
#define ALCHEMY_UNIT_MAP_KOLIMA_FOREST_GLADE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *KolimaForestGlade_GetEntrances(void);
const struct SceneExitRegion *KolimaForestGlade_GetRegions(void);
const u32 *KolimaForestGlade_GetExits(void);
const struct ObjectEventTemplate *
KolimaForestGlade_GetObjectEventTemplates(void);
const struct MapEvent *KolimaForestGlade_GetEvents(void);
s32 KolimaForestGlade_Initialize(void);
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern struct SceneEntrance gKolimaForestGlade_Entrances[];
extern u32 gKolimaForestGlade_Exits[];
extern struct ObjectEventTemplate gKolimaForestGlade_ObjectEventTemplates[];
extern struct MapEvent gKolimaForestGlade_Events[];

#endif
