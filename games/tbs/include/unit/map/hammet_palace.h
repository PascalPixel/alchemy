#ifndef ALCHEMY_UNIT_SCENE_HAMMET_PALACE_H
#define ALCHEMY_UNIT_SCENE_HAMMET_PALACE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gHammetPalace_PartyActions[];
extern struct SceneEntrance gHammetPalaceEntrancesOther[];
extern struct SceneEntrance gHammetPalaceEntrances[];
extern u32 gHammetPalaceExits[];
extern struct ScenePlacement gHammetPalacePlacementsOther[];
extern struct ScenePlacement gHammetPalacePlacements[];
extern struct SceneEvent gHammetPalaceEventsOther[];
extern struct SceneEvent gHammetPalaceEvents[];

#endif
