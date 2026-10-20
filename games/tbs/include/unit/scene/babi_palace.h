#ifndef ALCHEMY_UNIT_SCENE_BABI_PALACE_H
#define ALCHEMY_UNIT_SCENE_BABI_PALACE_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *BabiPalace_GetRegions(void);
const u32 *BabiPalace_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 BabiPalace_ApplyEntryState(void);
extern u16 gBabiPalace_DoorCells[];
extern s32 gBabiPalace_IndigestionScript[];
extern struct SceneEntrance gBabiPalace_EntrancesOther[];
extern struct SceneEntrance gBabiPalace_Entrances2[];
extern struct SceneRegion gBabiPalace_Regions[];
extern u32 gBabiPalace_Exits[];
extern struct ScenePlacement gBabiPalace_Placements2[];
extern struct ScenePlacement gBabiPalace_PlacementsOther[];
extern struct ScenePlacement gBabiPalace_PlacementsColosso[];
extern struct ScenePlacement gBabiPalace_PlacementsAfterColosso[];
extern struct SceneEvent gBabiPalace_EventsOther[];
extern struct SceneEvent gBabiPalace_EventsColosso[];
extern struct SceneEvent gBabiPalace_EventsAfterColosso[];
extern struct SceneEvent gBabiPalace_Events2[];
extern struct SceneEvent gBabiPalace_Events2Colosso[];

#endif
