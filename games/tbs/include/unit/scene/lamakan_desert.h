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
s32 RamakanSabaku_EnterArea(s32 a0, s32 a1);
void RamakanSabaku_ClaimSandEffectVram(void);
extern const s32 gEffectScript0[];
extern const s32 gEffectScript1[];
extern const s32 gEffectScript2[];
extern const s32 RamakanSabaku_SafePoints1[];
extern const s32 RamakanSabaku_SafePoints2[];
extern const s32 RamakanSabaku_SafePointsOther[];
extern const s32 *gEffectScripts[];
extern struct SceneEntrance gRamakanSabakuEntrancesOther[];
extern struct SceneEntrance gRamakanSabakuEntrances1[];
extern struct SceneEntrance gRamakanSabakuEntrances2[];
extern struct SceneEntrance gRamakanSabakuEntrances3[];
extern struct SceneEntrance gRamakanSabakuEntrances4[];
extern u32 RamakanSabaku_Exits[];
extern struct ScenePlacement gRamakanSabakuPlacementsOther[];
extern struct ScenePlacement gRamakanSabakuPlacements1[];
extern struct ScenePlacement gRamakanSabakuPlacements2[];
extern struct ScenePlacement gRamakanSabakuPlacements3[];
extern struct SceneEvent RamakanSabaku_Events[];
extern s16 RamakanSabaku_SandLevel;
extern s16 RamakanSabaku_SandCounter;
extern s16 RamakanSabaku_SandFlash;

#endif
