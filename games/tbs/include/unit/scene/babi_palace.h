#ifndef ALCHEMY_UNIT_SCENE_BABI_PALACE_H
#define ALCHEMY_UNIT_SCENE_BABI_PALACE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 BabiPalace_GetRegions(void);
s32 BabiPalace_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BabiPalace_ApplyEntryState(void);
extern u16 gBabiPalace_DoorCells[];
extern s32 gBabiPalace_IndigestionScript[];
extern struct SceneEntrance gBabiPalaceEntrancesOther[];
extern struct SceneEntrance gBabiPalaceEntrances2[];
extern s16 gBabiPalace_Regions[];
extern u32 gBabiPalace_Exits[];
extern struct ScenePlacement gBabiPalacePlacements2[];
extern struct ScenePlacement gBabiPalacePlacementsOther[];
extern struct ScenePlacement gBabiPalacePlacementsColosso[];
extern struct ScenePlacement gBabiPalacePlacementsAfterColosso[];
extern struct SceneEvent gBabiPalaceEventsOther[];
extern struct SceneEvent gBabiPalaceEventsColosso[];
extern struct SceneEvent gBabiPalaceEventsAfterColosso[];
extern struct SceneEvent gBabiPalaceEvents2[];
extern struct SceneEvent gBabiPalaceEvents2Colosso[];

#endif
