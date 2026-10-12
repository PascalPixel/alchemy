#ifndef ALCHEMY_UNIT_SCENE_BIRIBINO_MURA_H
#define ALCHEMY_UNIT_SCENE_BIRIBINO_MURA_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 Mura_VillagerActions[];
extern u16 gBiribinoMuraEntrancesOther[];
extern struct SceneEntrance gBiribinoMuraEntrances1[];
extern struct SceneEntrance gBiribinoMuraEntrances3[];
extern struct SceneEntrance gBiribinoMuraEntrances2[];
extern s16 gBiribinoMuraRegions2[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gBiribinoMuraPlacementsOther[];
extern struct ScenePlacement gBiribinoMuraPlacements1[];
extern struct ScenePlacement gBiribinoMuraPlacements3[];
extern struct ScenePlacement gBiribinoMuraPlacements2[];
extern struct SceneEvent gBiribinoMuraEventsOther[];
extern struct SceneEvent gBiribinoMuraEvents1[];
extern struct SceneEvent gBiribinoMuraEvents3[];
extern struct SceneEvent gBiribinoMuraEvents2[];
extern u16 Mura_DoorCellStep6[];
extern u16 Mura_DoorCellStep4[];
extern u16 Mura_DoorCellStep5[];
extern u16 Mura_DoorCellStep1[];
extern u16 Mura_DoorCellStep0[];
extern u16 Mura_DoorCellStep2[];
extern u16 Mura_DoorCellStep3[];
extern u16 *Mura_DoorCellSteps[];
extern u32 Mura_DoorCellOrigins[];
extern u32 Mura_RepaintCells[];
extern s32 BiribinoMura_FacingCellSteps[];
extern s32 Mura_SpawnScript[];

#endif
