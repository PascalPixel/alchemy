#ifndef ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_TREE_H
#define ALCHEMY_UNIT_SCENE_KOLIMA_FOREST_TREE_H

#include "types.h"
#include "field_scene.h"

u8 *KolimaForestTree_GetEntrances(void);
u8 *KolimaForestTree_GetRegions(void);
u8 *KolimaForestTree_GetExits(void);
u8 *KolimaForestTree_GetPlacements(void);
u8 *KolimaForestTree_GetEvents(void);
s32 KolimaForestTree_PrepareActors(void);
extern s32 gSceneAction_ActorOneEntry[];
extern s32 gSceneAction_ActorTwoEntry[];
extern s32 gSceneAction_ActorThreeEntry[];
extern s32 gSceneAction_GroupFinish[];
extern struct SceneEntrance gKolimaForestTreeEntrances[];
extern s16 gKolimaForestTreeRegions[];
extern u32 gKolimaForestTreeExits[];
extern struct ScenePlacement gKolimaForestTreePlacements[];
extern struct SceneEvent gKolimaForestTreeEvents[];
extern s32 gKolimaForestTreeEffectScript[];

#endif
