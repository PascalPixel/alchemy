#ifndef ALCHEMY_UNIT_SCENE_KORIMA_KI_H
#define ALCHEMY_UNIT_SCENE_KORIMA_KI_H

#include "types.h"
#include "field_scene.h"

u8 *KorimaKi_GetEntrances(void);
u8 *KorimaKi_GetRegions(void);
u8 *KorimaKi_GetExits(void);
u8 *KorimaKi_GetPlacements(void);
u8 *KorimaKi_GetEvents(void);
s32 KorimaKi_PrepareActors(void);
extern s32 SceneAction_ActorOneEntry[];
extern s32 SceneAction_ActorTwoEntry[];
extern s32 SceneAction_ActorThreeEntry[];
extern s32 SceneAction_GroupFinish[];
extern struct SceneEntrance gKorimaKiEntrances[];
extern s16 gKorimaKiRegions[];
extern u32 gKorimaKiExits[];
extern struct ScenePlacement gKorimaKiPlacements[];
extern struct SceneEvent gKorimaKiEvents[];
extern s32 gKorimaKiEffectScript[];

#endif
