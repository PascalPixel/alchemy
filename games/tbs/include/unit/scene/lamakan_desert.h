#ifndef ALCHEMY_UNIT_SCENE_LAMAKAN_DESERT_H
#define ALCHEMY_UNIT_SCENE_LAMAKAN_DESERT_H

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
s32 LamakanDesert_EnterArea(s32 a0, s32 a1);
void LamakanDesert_ClaimSandEffectVram(void);
extern const s32 gEffectScript0[];
extern const s32 gEffectScript1[];
extern const s32 gEffectScript2[];
extern const s32 LamakanDesert_SafePoints1[];
extern const s32 LamakanDesert_SafePoints2[];
extern const s32 LamakanDesert_SafePointsOther[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gLamakanDesertEntrancesOther[];
extern struct SceneEntrance gLamakanDesertEntrances1[];
extern struct SceneEntrance gLamakanDesertEntrances2[];
extern struct SceneEntrance gLamakanDesertEntrances3[];
extern struct SceneEntrance gLamakanDesertEntrances4[];
extern u32 LamakanDesert_Exits[];
extern struct ScenePlacement gLamakanDesertPlacementsOther[];
extern struct ScenePlacement gLamakanDesertPlacements1[];
extern struct ScenePlacement gLamakanDesertPlacements2[];
extern struct ScenePlacement gLamakanDesertPlacements3[];
extern struct SceneEvent LamakanDesert_Events[];
extern s16 LamakanDesert_SandLevel;
extern s16 LamakanDesert_SandCounter;
extern s16 LamakanDesert_SandFlash;

#endif
