#ifndef ALCHEMY_MODULE_MAP_CROSSBONE_ISLE_STEPPING_STONES_H
#define ALCHEMY_MODULE_MAP_CROSSBONE_ISLE_STEPPING_STONES_H

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
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gCrossboneIsleSteppingStones_ActionTable[];
extern u16 gCrossboneIsleSteppingStones_SwitchCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_SwitchOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrancesOther[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances[];
extern struct SceneEntrance gCrossboneIsleIcePillarsEntrances[];
extern struct SceneEntrance gCrossboneIsleFloodgateEntrances[];
extern struct SceneEntrance gCrossboneIsleStackedPillarsEntrances[];
extern struct SceneEntrance gCrossboneIsleDragonStatueEntrances[];
extern u32 gCrossboneIsleSteppingStonesExits[];
extern struct ActorPlacement gCrossboneIsleSteppingStonesNoActorPlacements[];
extern struct ActorPlacement gCrossboneIsleSteppingStonesActorPlacements[];
extern struct ActorPlacement gCrossboneIsleIcePillarsActorPlacements[];
extern struct ActorPlacement gCrossboneIsleFloodgateActorPlacements[];
extern struct ActorPlacement gCrossboneIsleSteppingStonesActorPlacementsOther[];
extern struct ActorPlacement gCrossboneIsleDragonStatueActorPlacements[];
extern struct Trigger gCrossboneIsleSteppingStonesTriggersOther[];
extern struct Trigger gCrossboneIsleSteppingStonesTriggers[];
extern struct Trigger gCrossboneIsleIcePillarsTriggers[];
extern struct Trigger gCrossboneIsleFloodgateTriggers[];
extern struct Trigger gCrossboneIsleStackedPillarsTriggers[];
extern struct Trigger gCrossboneIsleDragonStatueTriggers[];

#endif
