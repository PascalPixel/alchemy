#ifndef ALCHEMY_UNIT_SCENE_IMIL_FUCHIN_FALLS_H
#define ALCHEMY_UNIT_SCENE_IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "field_scene.h"

struct EffectOptions;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

extern s32 gImilFuchinFalls_TrackLeader;
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 *ImilFuchinFalls_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ImilFuchinFalls_ApplyEntryHook(void);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                  s32 velocityZ, u32 flags, const struct EffectOptions *extra);
extern s32 *gEffectScripts[];
extern s16 gImilFuchinFallsKeyHeadings[];
extern s16 gImilFuchinFallsHeadings[];
extern s32 gImilFuchinFallsDragonsEye;
extern struct SceneEntrance gImilFuchinFallsEntrancesOther[];
extern struct SceneEntrance gImilFuchinFallsEntrances1[];
extern struct SceneEntrance gImilFuchinFallsEntrances2[];
extern struct SceneEntrance gImilFuchinFallsEntrances3[];
extern struct SceneEntrance gImilFuchinFallsEntrances4[];
extern struct SceneEntrance gImilFuchinFallsEntrances5[];
extern struct SceneEntrance gImilFuchinFallsEntrances6[];
extern struct SceneEntrance gImilFuchinFallsEntrances7[];
extern u32 gImilFuchinFallsExits[];
extern struct ScenePlacement gImilFuchinFallsPlacementsOther[];
extern struct ScenePlacement gImilFuchinFallsPlacements1[];
extern struct ScenePlacement gImilFuchinFallsPlacements2[];
extern struct ScenePlacement gImilFuchinFallsPlacements3[];
extern struct ScenePlacement gImilFuchinFallsPlacements4[];
extern struct ScenePlacement gImilFuchinFallsPlacements5[];
extern struct ScenePlacement gImilFuchinFallsPlacements7[];
extern struct SceneEvent gImilFuchinFallsEventsOther[];
extern struct SceneEvent gImilFuchinFallsEvents1[];
extern struct SceneEvent gImilFuchinFallsEvents2[];
extern struct SceneEvent gImilFuchinFallsEvents3[];
extern struct SceneEvent gImilFuchinFallsEvents4[];
extern struct SceneEvent gImilFuchinFallsEvents5[];
extern struct SceneEvent gImilFuchinFallsEvents6[];
extern struct SceneEvent gImilFuchinFallsEvents7[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];

#endif
