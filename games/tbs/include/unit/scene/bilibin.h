#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Bilibin_GetEntrances(void);
const struct SceneRegion *Bilibin_GetRegions(void);
const u32 *Bilibin_GetExits(void);
const struct ScenePlacement *Bilibin_GetPlacements(void);
const struct SceneEvent *Bilibin_GetEvents(void);
s32 Bilibin_Initialize(void);
extern s32 gBilibin_FaceLeaderScript[];
extern u16 gBilibinEntrancesOther[];
extern struct SceneEntrance gBilibinEntrances1[];
extern struct SceneEntrance gBilibinEntrances3[];
extern struct SceneEntrance gBilibinEntrances2[];
extern s16 gBilibinRegions2[];
extern u32 gBilibinExits[];
extern struct ScenePlacement gBilibinPlacementsOther[];
extern struct ScenePlacement gBilibinPlacements1[];
extern struct ScenePlacement gBilibinPlacements3[];
extern struct ScenePlacement gBilibinPlacements2[];
extern struct SceneEvent gBilibinEventsOther[];
extern struct SceneEvent gBilibinEvents1[];
extern struct SceneEvent gBilibinEvents3[];
extern struct SceneEvent gBilibinEvents2[];
extern u16 gBilibin_DoorSteps6[];
extern u16 gBilibin_DoorSteps4[];
extern u16 gBilibin_DoorSteps5[];
extern u16 gBilibin_DoorSteps1[];
extern u16 gBilibin_DoorSteps0[];
extern u16 gBilibin_DoorSteps2[];
extern u16 gBilibin_DoorSteps3[];
extern u16 *gBilibin_DoorSteps[];
extern u32 gBilibin_DoorCells[];
extern u32 gBilibin_RiverPillarCells[];
extern s32 gBilibin_FacingCellSteps[];
extern s32 gBilibin_TransientEffectScript[];

#endif
