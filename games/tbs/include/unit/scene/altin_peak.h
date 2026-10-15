#ifndef ALCHEMY_UNIT_SCENE_ALTIN_PEAK_H
#define ALCHEMY_UNIT_SCENE_ALTIN_PEAK_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
u8 *ArutinYama_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 ArutinYama_RunAreaScene(void);
extern s8 ArutinYama_PaletteScript[];
extern s32 ArutinYama_ActorScript[];
extern s32 ArutinYama_EarlyActorScript[];
extern s32 ArutinYama_LogRideScript[];
extern s32 ArutinYama_LeaderRideScript[];
extern s32 ArutinYama_SparkScript[];
extern s32 ArutinYama_GeraldScript[];
extern s32 ArutinYama_IvanScript[];
extern s32 ArutinYama_MiaScript[];
extern s32 ArutinYama_CelebrateScript[];
extern s32 ArutinYama_PartyScript[];
extern struct SceneEntrance gArutinYamaEntrancesOther[];
extern struct SceneEntrance gArutinYamaEntrances1[];
extern struct SceneEntrance gArutinYamaEntrances2[];
extern struct SceneEntrance gArutinYamaEntrances3[];
extern struct SceneEntrance gArutinYamaEntrances4[];
extern struct SceneEntrance gArutinYamaEntrances5[];
extern struct SceneEntrance gArutinYamaEntrances6[];
extern struct SceneEntrance gArutinYamaEntrances7[];
extern struct SceneEntrance gArutinYamaEntrances8[];
extern struct SceneEntrance gArutinYamaEntrances9[];
extern struct SceneEntrance gArutinYamaEntrances10[];
extern struct SceneEntrance gArutinYamaEntrances11[];
extern s16 gArutinYamaRegions9[];
extern s16 gArutinYamaRegions10[];
extern u32 ArutinYama_StatueTable[];
extern struct ScenePlacement gArutinYamaPlacementsOther[];
extern struct ScenePlacement gArutinYamaPlacements1[];
extern struct ScenePlacement gArutinYamaPlacements3[];
extern struct ScenePlacement gArutinYamaPlacements5[];
extern struct ScenePlacement gArutinYamaPlacements6[];
extern struct ScenePlacement gArutinYamaPlacements7[];
extern struct ScenePlacement gArutinYamaPlacements8[];
extern struct ScenePlacement gArutinYamaPlacements9[];
extern struct ScenePlacement gArutinYamaPlacements10[];
extern struct ScenePlacement gArutinYamaPlacements11[];
extern struct SceneEvent gArutinYamaEventsOther[];
extern struct SceneEvent gArutinYamaEvents1[];
extern struct SceneEvent ArutinYama_OpenedAreaScript[];
extern struct SceneEvent gArutinYamaEvents2[];
extern struct SceneEvent gArutinYamaEvents3[];
extern struct SceneEvent gArutinYamaEvents4[];
extern struct SceneEvent gArutinYamaEvents5[];
extern struct SceneEvent gArutinYamaEvents6[];
extern struct SceneEvent gArutinYamaEvents7[];
extern struct SceneEvent gArutinYamaEvents8[];
extern struct SceneEvent gArutinYamaEvents9[];
extern struct SceneEvent gArutinYamaEvents10[];
extern struct SceneEvent gArutinYamaEvents11[];
extern s32 ArutinYama_CueTicks;
extern u32 ArutinYama_RollRadii[];

#endif
