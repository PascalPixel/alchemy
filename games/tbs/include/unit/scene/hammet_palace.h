#ifndef ALCHEMY_UNIT_MAP_HAMMET_PALACE_H
#define ALCHEMY_UNIT_MAP_HAMMET_PALACE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct MapEvent;
struct ObjectEventTemplate;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gHammetPalace_PartyActions[];
extern struct SceneEntrance gHammetPalaceEntrancesOther[];
extern struct SceneEntrance gHammetPalaceEntrances[];
extern u32 gHammetPalaceExits[];
extern struct ObjectEventTemplate gHammetPalaceObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gHammetPalaceObjectEventTemplates[];
extern struct MapEvent gHammetPalaceEventsOther[];
extern struct MapEvent gHammetPalaceEvents[];

#endif
