#ifndef ALCHEMY_UNIT_SCENE_HAIDIA_BABI_H
#define ALCHEMY_UNIT_SCENE_HAIDIA_BABI_H

#include "types.h"
#include "field_scene.h"

s32 *HaidiaBabi_GetEntrances(void);
s32 SceneData_ReturnZero(void);
s32 HaidiaBabi_SelectExits(void);
s32 HaidiaBabi_SelectPlacements(void);
s32 HaidiaBabi_SelectEvents(void);
s32 HaidiaBabi_RestoreEntryState(void);
extern s32 gHaidiaBabiSharedAction[];
extern s32 gHaidiaBabiActorExitAction[];
extern s32 gHaidiaBabiLeaderExitAction[];
extern s32 gHaidiaBabiRampActor8Action[];
extern s32 gHaidiaBabiRampLeaderAction[];
extern s32 gHaidiaBabiRampActor8ActionB[];
extern s32 gHaidiaBabiRampLeaderActionB[];
extern s32 gHaidiaBabiRampFinalAction[];
extern s32 gHaidiaBabiRampActor10Action[];
extern s32 gHaidiaBabiRampActor10ActionB[];
extern s32 gHaidiaBabiActor8Departure[];
extern s32 gHaidiaBabiBagLiftScript[];
extern s32 gHaidiaBabiBagShowScript[];
extern struct SceneEntrance gHaidiaBabiEntrances[];
extern u32 gHaidiaBabiExits[];
extern u32 gHaidiaBabiExits2[];
extern struct ScenePlacement gHaidiaBabiPlacements[];
extern struct ScenePlacement gHaidiaBabiPlacements2[];
extern struct ScenePlacement gHaidiaBabiPlacements3[];
extern struct ScenePlacement gHaidiaBabiPlacements4[];
extern struct SceneEvent gHaidiaBabiEvents4[];
extern struct SceneEvent gHaidiaBabiEvents[];
extern struct SceneEvent gHaidiaBabiEvents3[];
extern struct SceneEvent gHaidiaBabiEvents5[];
extern struct SceneEvent gHaidiaBabiEvents6[];

#endif
