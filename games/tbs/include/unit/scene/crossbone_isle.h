#ifndef ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_H
#define ALCHEMY_UNIT_SCENE_CROSSBONE_ISLE_H

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
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u16 gCrossboneIsle_EntranceCells[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gCrossboneIsle_IsleEntrances[];
extern struct SceneEntrance gCrossboneIsle_CaveEntranceEntrances[];
extern struct SceneEntrance gCrossboneIsle_FloatingLogsEntrances[];
extern struct SceneEntrance gCrossboneIsle_PierEntrances[];
extern struct SceneEntrance gCrossboneIsle_PirateShipEntrances[];
extern u32 gCrossboneIsleExits[];
extern struct ScenePlacement gCrossboneIsle_NoPlacements[];
extern struct ScenePlacement gCrossboneIsle_IslePlacements[];
extern struct ScenePlacement gCrossboneIsle_FloatingLogsPlacements[];
extern struct ScenePlacement gCrossboneIsle_CorridorPlacements[];
extern struct SceneEvent gCrossboneIsle_CorridorEvents[];
extern struct SceneEvent gCrossboneIsle_CaveEntranceEvents[];
extern struct SceneEvent gCrossboneIsle_IsleEvents[];
extern struct SceneEvent gCrossboneIsle_FloatingLogsEvents[];
extern struct SceneEvent gCrossboneIsle_PierEvents[];
extern struct SceneEvent gCrossboneIsle_PirateShipEvents[];

#endif
