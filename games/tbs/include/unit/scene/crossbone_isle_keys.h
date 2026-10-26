#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_KEYS_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_KEYS_H

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
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gCrossboneIsleKeys_BaneSquashScript[];
extern s32 gCrossboneIsleKeys_BaneWalkTargets[];
extern struct SceneEntrance gCrossboneIsleKeysEntrancesOther[];
extern struct SceneEntrance gCrossboneIsleKeysRoomEntrances[];
extern struct SceneEntrance gCrossboneIsleStatuesEntrances[];
extern struct SceneEntrance gCrossboneIsleBaneEntrances[];
extern u32 gCrossboneIsleKeysExits[];
extern struct ObjectEventTemplate gCrossboneIsleKeysObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gCrossboneIsleKeysRoomObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsleStatuesObjectEventTemplates[];
extern struct ObjectEventTemplate gCrossboneIsleBaneObjectEventTemplates[];
extern struct MapEvent gCrossboneIsleKeysEventsOther[];
extern struct MapEvent gCrossboneIsleKeysRoomEvents[];
extern struct MapEvent gCrossboneIsleStatuesEvents[];
extern struct MapEvent gCrossboneIsleBaneEvents[];
extern u32 gCrossboneIsleKeys_SlotColumns[];
extern s32 gCrossboneIsleKeys_IconTimer;

#endif
