#ifndef ALCHEMY_UNIT_FIELD_COMMON_IMIRU_FUCHIN_FUCHIN_H
#define ALCHEMY_UNIT_FIELD_COMMON_IMIRU_FUCHIN_FUCHIN_H

#include "types.h"
#include "field_scene.h"

struct EffectOptions;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

extern s32 ImiruFuchin_TrackLeader;
const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 *ImiruFuchin_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ImiruFuchin_ApplyEntryHook(void);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags, const struct EffectOptions *extra);
extern s32 *gEffectScripts[];
extern s16 gImiruFuchinKeyHeadings[];
extern s16 gImiruFuchinHeadings[];
extern s32 gImiruFuchinDragonsEye;
extern struct SceneEntrance gImiruFuchinEntrancesOther[];
extern struct SceneEntrance gImiruFuchinEntrances1[];
extern struct SceneEntrance gImiruFuchinEntrances2[];
extern struct SceneEntrance gImiruFuchinEntrances3[];
extern struct SceneEntrance gImiruFuchinEntrances4[];
extern struct SceneEntrance gImiruFuchinEntrances5[];
extern struct SceneEntrance gImiruFuchinEntrances6[];
extern struct SceneEntrance gImiruFuchinEntrances7[];
extern u32 gImiruFuchinExits[];
extern struct ScenePlacement gImiruFuchinPlacementsOther[];
extern struct ScenePlacement gImiruFuchinPlacements1[];
extern struct ScenePlacement gImiruFuchinPlacements2[];
extern struct ScenePlacement gImiruFuchinPlacements3[];
extern struct ScenePlacement gImiruFuchinPlacements4[];
extern struct ScenePlacement gImiruFuchinPlacements5[];
extern struct ScenePlacement gImiruFuchinPlacements7[];
extern struct SceneEvent gImiruFuchinEventsOther[];
extern struct SceneEvent gImiruFuchinEvents1[];
extern struct SceneEvent gImiruFuchinEvents2[];
extern struct SceneEvent gImiruFuchinEvents3[];
extern struct SceneEvent gImiruFuchinEvents4[];
extern struct SceneEvent gImiruFuchinEvents5[];
extern struct SceneEvent gImiruFuchinEvents6[];
extern struct SceneEvent gImiruFuchinEvents7[];
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];

#endif
