#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_KEYS_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_KEYS_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneExitRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern s32 gCrossboneIsleKeys_BaneSquashScript[];
extern s32 gCrossboneIsleKeys_BaneWalkTargets[];
extern struct SceneEntrance gCrossboneIsleKeysEntrancesOther[];
extern struct SceneEntrance gCrossboneIsleKeysRoomEntrances[];
extern struct SceneEntrance gCrossboneIsleStatuesEntrances[];
extern struct SceneEntrance gCrossboneIsleBaneEntrances[];
extern u32 gCrossboneIsleKeysExits[];
extern struct ActorPlacement gCrossboneIsleKeysActorPlacementsOther[];
extern struct ActorPlacement gCrossboneIsleKeysRoomActorPlacements[];
extern struct ActorPlacement gCrossboneIsleStatuesActorPlacements[];
extern struct ActorPlacement gCrossboneIsleBaneActorPlacements[];
extern struct Trigger gCrossboneIsleKeysTriggersOther[];
extern struct Trigger gCrossboneIsleKeysRoomTriggers[];
extern struct Trigger gCrossboneIsleStatuesTriggers[];
extern struct Trigger gCrossboneIsleBaneTriggers[];
extern u32 gCrossboneIsleKeys_SlotColumns[];
extern s32 gCrossboneIsleKeys_IconTimer;

#endif
