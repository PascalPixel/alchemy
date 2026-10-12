#ifndef ALCHEMY_UNIT_FIELD_RUNPA_JO_RUNPA_JO_H
#define ALCHEMY_UNIT_FIELD_RUNPA_JO_RUNPA_JO_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 GetEmptySceneData(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 FieldScene_DispatchActorUpdate(void);
extern s32 gRunpaJoRandomPick;
extern s32 gRunpaJoPlacements2_1[];
extern s32 gRunpaJoPlacements2_0[];
extern s32 gRunpaJoPlacements2_3[];
extern s32 gRunpaJoPlacements1_Data[];
extern s32 gRunpaJoPlacements2_2[];
extern struct SceneEntrance gRunpaJoEntrances1[];
extern struct SceneEntrance gRunpaJoEntrances2[];
extern struct SceneEntrance gRunpaJoEntrances3[];
extern struct SceneEntrance gRunpaJoEntrancesOther[];
extern u32 gRunpaJoExitsOther[];
extern u32 gRunpaJoExits2[];
extern u32 gRunpaJoExits3And4[];
extern struct ScenePlacement gRunpaJoPlacementsOther[];
extern struct ScenePlacement gRunpaJoPlacementsRunpaDou[];
extern struct ScenePlacement gRunpaJoPlacements1[];
extern struct ScenePlacement gRunpaJoPlacements2[];
extern struct ScenePlacement gRunpaJoPlacements3[];
extern struct ScenePlacement gRunpaJoPlacements4[];
extern struct SceneEvent gRunpaJoNoEvents[];
extern struct SceneEvent gRunpaJoEvents1[];
extern struct SceneEvent gRunpaJoEvents2[];
extern struct SceneEvent gRunpaJoEvents3[];
extern struct SceneEvent gRunpaJoEventsOther[];
extern u32 gRunpaJoPairTableA[];
extern u32 gRunpaJoSupportPairs[];
extern u32 gRunpaJoPairTableB[];
extern u32 gRunpaJoPairTableC[];

#endif
