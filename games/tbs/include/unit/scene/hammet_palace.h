#ifndef ALCHEMY_UNIT_SCENE_HAMMET_PALACE_H
#define ALCHEMY_UNIT_SCENE_HAMMET_PALACE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
u8                          *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern s32                   gHammetPalace_PartyActions[];
extern struct SceneEntrance  gHammetPalaceEntrancesOther[];
extern struct SceneEntrance  gHammetPalaceEntrances[];
extern u32                   gPlacement_Messages[];
extern struct ScenePlacement gHammetPalacePlacementsOther[];
extern struct ScenePlacement gHammetPalacePlacements[];
extern struct SceneEvent     gHammetPalaceEventsOther[];
extern struct SceneEvent     gHammetPalaceEvents[];

#endif
