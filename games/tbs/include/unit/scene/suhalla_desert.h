#ifndef ALCHEMY_UNIT_SCENE_SUHALLA_DESERT_H
#define ALCHEMY_UNIT_SCENE_SUHALLA_DESERT_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance  *Scene_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
u8                          *SuhallaDesert_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent     *Scene_GetEvents(void);
s32                          SuhallaDesert_RunSceneScript(void);
extern s32                   gEffectScript0[];
extern s32                   gEffectScript1[];
extern s32                   gEffectScript2[];
extern const s32            *gEffectScripts[];
extern s32                   gSuhallaDesertPlacements1_Data[];
extern s32                   gSuhallaDesertPlacements2_1[];
extern struct SceneEntrance  gSuhallaDesertEntrancesOther[];
extern struct SceneEntrance  gSuhallaDesertEntrances1[];
extern struct SceneEntrance  gSuhallaDesertEntrances2[];
extern struct SceneEntrance  gSuhallaDesertEntrances3[];
extern u32                   gSuhallaDesertExits[];
extern struct ScenePlacement gSuhallaDesertPlacementsOther[];
extern struct ScenePlacement gSuhallaDesertPlacements1[];
extern struct ScenePlacement gSuhallaDesertPlacements2[];
extern struct ScenePlacement gSuhallaDesertPlacements3[];
extern s32                   gSuhallaDesertActor12Action[];
extern struct SceneEvent     gSuhallaDesertEventsOther[];
extern struct SceneEvent     gSuhallaDesertEvents3[];

#endif
