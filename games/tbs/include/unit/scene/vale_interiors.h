#ifndef ALCHEMY_UNIT_SCENE_VALE_INTERIORS_H
#define ALCHEMY_UNIT_SCENE_VALE_INTERIORS_H

#include "types.h"
#include "field_scene.h"

void *HaidiaHeya_GetEntrances(void);
int SceneData_ReturnZero(void);
void *HaidiaHeya_SelectExits(void);
void *SceneData_SelectFlaggedTable(void);
void *HaidiaHeya_SelectEvents(void);
s32 Scene_RunSupplementalSequenceOne(void);
extern s32 gValeFaceTargetScript[];
extern s32 gValeActor10Script[];
extern s32 gValePartyScript[];
extern s32 gValeTimedActorScript[];
extern struct SceneEntrance gValeHouseEntrances[];
extern u32 gValeHouseExits[];
extern u32 gValeHouseLateExits[];
extern struct ScenePlacement gValeHousePlacements[];
extern struct ScenePlacement gValeHousePlacementsAfterLeaving[];
extern struct ScenePlacement gValeHouseReturnPlacements[];
extern struct SceneEvent gValeHouseLateEvents[];

#endif
