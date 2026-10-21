#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_KEYS_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_KEYS_H

#include "types.h"
#include "field_scene.h"

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
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gCrossboneIsleKeys_BaneSquashScript[];
extern s32 gCrossboneIsleKeys_BaneWalkTargets[];
extern struct SceneEntrance gCrossboneIsleKeysEntrancesOther[];
extern struct SceneEntrance gCrossboneIsleKeysRoomEntrances[];
extern struct SceneEntrance gCrossboneIsleStatuesEntrances[];
extern struct SceneEntrance gCrossboneIsleBaneEntrances[];
extern u32 gCrossboneIsleKeysExits[];
extern struct ScenePlacement gCrossboneIsleKeysPlacementsOther[];
extern struct ScenePlacement gCrossboneIsleKeysRoomPlacements[];
extern struct ScenePlacement gCrossboneIsleStatuesPlacements[];
extern struct ScenePlacement gCrossboneIsleBanePlacements[];
extern struct SceneEvent gCrossboneIsleKeysEventsOther[];
extern struct SceneEvent gCrossboneIsleKeysRoomEvents[];
extern struct SceneEvent gCrossboneIsleStatuesEvents[];
extern struct SceneEvent gCrossboneIsleBaneEvents[];
extern u32 gCrossboneIsleKeys_SlotColumns[];
extern s32 gCrossboneIsleKeys_IconTimer;

#endif
