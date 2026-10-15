#ifndef ALCHEMY_UNIT_SCENE_BILIBIN_H
#define ALCHEMY_UNIT_SCENE_BILIBIN_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance  *Scene_GetEntrances(void);
const struct SceneRegion    *Scene_GetRegions(void);
u8                          *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          Scene_Initialize(void);
extern s32                   gVillage_VillagerActions[];
extern u16                   gBilibinEntrancesOther[];
extern struct SceneEntrance  gBilibinEntrances1[];
extern struct SceneEntrance  gBilibinEntrances3[];
extern struct SceneEntrance  gBilibinEntrances2[];
extern s16                   gBilibinRegions2[];
extern u32                   gPlacement_Messages[];
extern struct ScenePlacement gBilibinPlacementsOther[];
extern struct ScenePlacement gBilibinPlacements1[];
extern struct ScenePlacement gBilibinPlacements3[];
extern struct ScenePlacement gBilibinPlacements2[];
extern struct SceneEvent     gBilibinEventsOther[];
extern struct SceneEvent     gBilibinEvents1[];
extern struct SceneEvent     gBilibinEvents3[];
extern struct SceneEvent     gBilibinEvents2[];
extern u16                   gVillage_DoorCellStep6[];
extern u16                   gVillage_DoorCellStep4[];
extern u16                   gVillage_DoorCellStep5[];
extern u16                   gVillage_DoorCellStep1[];
extern u16                   gVillage_DoorCellStep0[];
extern u16                   gVillage_DoorCellStep2[];
extern u16                   gVillage_DoorCellStep3[];
extern u16                  *gVillage_DoorCellSteps[];
extern u32                   gVillage_DoorCellOrigins[];
extern u32                   gVillage_RepaintCells[];
extern s32                   gBilibin_FacingCellSteps[];
extern s32                   gVillage_SpawnScript[];

#endif
