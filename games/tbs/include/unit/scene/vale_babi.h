#ifndef ALCHEMY_UNIT_SCENE_VALE_BABI_H
#define ALCHEMY_UNIT_SCENE_VALE_BABI_H

#include "types.h"
#include "field_scene.h"

s32                         *ValeBabi_GetEntrances(void);
s32                          SceneData_ReturnZero(void);
s32                          ValeBabi_SelectExits(void);
s32                          ValeBabi_SelectPlacements(void);
s32                          ValeBabi_SelectEvents(void);
s32                          ValeBabi_RestoreEntryState(void);
extern s32                   gValeBabiSharedAction[];
extern s32                   gValeBabiActorExitAction[];
extern s32                   gValeBabiLeaderExitAction[];
extern s32                   gValeBabiRampActor8Action[];
extern s32                   gValeBabiRampLeaderAction[];
extern s32                   gValeBabiRampActor8ActionB[];
extern s32                   gValeBabiRampLeaderActionB[];
extern s32                   gValeBabiRampFinalAction[];
extern s32                   gValeBabiRampActor10Action[];
extern s32                   gValeBabiRampActor10ActionB[];
extern s32                   gValeBabiActor8Departure[];
extern s32                   gValeBabiBagLiftScript[];
extern s32                   gValeBabiBagShowScript[];
extern struct SceneEntrance  gValeBabiEntrances[];
extern u32                   gValeBabiExits[];
extern u32                   gValeBabiExits2[];
extern struct ScenePlacement gValeBabiPlacements[];
extern struct ScenePlacement gValeBabiPlacements2[];
extern struct ScenePlacement gValeBabiPlacements3[];
extern struct ScenePlacement gValeBabiPlacements4[];
extern struct SceneEvent     gValeBabiEvents4[];
extern struct SceneEvent     gValeBabiEvents[];
extern struct SceneEvent     gValeBabiEvents3[];
extern struct SceneEvent     gValeBabiEvents5[];
extern struct SceneEvent     gValeBabiEvents6[];

#endif
