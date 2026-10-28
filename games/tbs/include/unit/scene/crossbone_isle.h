#ifndef ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_H
#define ALCHEMY_UNIT_MAP_CROSSBONE_ISLE_H

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
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern u16 gCrossboneIsle_EntranceCells[];
extern const s32 *gFieldEffect_DurationScripts[];
extern struct SceneEntrance gCrossboneIsle_IsleEntrances[];
extern struct SceneEntrance gCrossboneIsle_CaveEntranceEntrances[];
extern struct SceneEntrance gCrossboneIsle_FloatingLogsEntrances[];
extern struct SceneEntrance gCrossboneIsle_PierEntrances[];
extern struct SceneEntrance gCrossboneIsle_PirateShipEntrances[];
extern u32 gCrossboneIsleExits[];
extern struct ActorPlacement gCrossboneIsle_NoActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_IsleActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_FloatingLogsActorPlacements[];
extern struct ActorPlacement gCrossboneIsle_CorridorActorPlacements[];
extern struct Trigger gCrossboneIsle_CorridorTriggers[];
extern struct Trigger gCrossboneIsle_CaveEntranceTriggers[];
extern struct Trigger gCrossboneIsle_IsleTriggers[];
extern struct Trigger gCrossboneIsle_FloatingLogsTriggers[];
extern struct Trigger gCrossboneIsle_PierTriggers[];
extern struct Trigger gCrossboneIsle_PirateShipTriggers[];

#endif
