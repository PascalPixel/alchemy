#ifndef ALCHEMY_UNIT_FIELD_SUHARA_SABAKU_SABAKU_H
#define ALCHEMY_UNIT_FIELD_SUHARA_SABAKU_SABAKU_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SuharaSabaku_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 SuharaSabaku_RunSceneScript(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gSuharaSabakuPlacements1_Data[];
extern s32 gSuharaSabakuPlacements2_1[];
extern struct SceneEntrance gSuharaSabakuEntrancesOther[];
extern struct SceneEntrance gSuharaSabakuEntrances1[];
extern struct SceneEntrance gSuharaSabakuEntrances2[];
extern struct SceneEntrance gSuharaSabakuEntrances3[];
extern u32 gSuharaSabakuExits[];
extern struct ScenePlacement gSuharaSabakuPlacementsOther[];
extern struct ScenePlacement gSuharaSabakuPlacements1[];
extern struct ScenePlacement gSuharaSabakuPlacements2[];
extern struct ScenePlacement gSuharaSabakuPlacements3[];
extern s32 gSuharaSabakuActor12Action[];
extern struct SceneEvent gSuharaSabakuEventsOther[];
extern struct SceneEvent gSuharaSabakuEvents3[];

#endif
