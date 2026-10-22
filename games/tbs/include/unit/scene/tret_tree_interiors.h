#ifndef ALCHEMY_UNIT_SCENE_TRET_TREE_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_TRET_TREE_INTERIORS_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 TretTreeInteriors_EnterRoom(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s16 gTretTreeInteriors_MapPatches[];
extern const s32 *gEffectScripts[];
extern s32 gTretTreeInteriors_LeaderGathersActions[];
extern s32 gTretTreeInteriors_GaretGathersActions[];
extern s32 gTretTreeInteriors_IvanGathersActions[];
extern s32 gTretTreeInteriors_MiaGathersActions[];
extern s32 gTretTreeInteriors_FaceSinksActions[];
extern s32 gTretTreeInteriors_RejoinLeaderActions[];
extern u32 gTretTreeInteriors_TintSteps[];
extern struct SceneEntrance gTretTreeInteriorsEntrances[];
extern struct SceneRegion gTretTreeInteriorsRegions[];
extern u32 gTretTreeInteriorsExits[];
extern struct ScenePlacement gTretTreeInteriorsPlacements[];
extern struct SceneEvent gTretTreeInteriorsEvents[];
extern s32 gTretTreeInteriors_TintIndex;

#endif
