#ifndef ALCHEMY_UNIT_SCENE_KAREI_KYUDEN_H
#define ALCHEMY_UNIT_SCENE_KAREI_KYUDEN_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 KareiKyuden_PartyActions[];
extern struct SceneEntrance gKareiKyudenEntrancesOther[];
extern struct SceneEntrance gKareiKyudenEntrances[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gKareiKyudenPlacementsOther[];
extern struct ScenePlacement gKareiKyudenPlacements[];
extern struct SceneEvent gKareiKyudenEventsOther[];
extern struct SceneEvent gKareiKyudenEvents[];

#endif
