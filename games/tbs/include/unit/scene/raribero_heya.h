#ifndef ALCHEMY_UNIT_SCENE_RARIBERO_HEYA_H
#define ALCHEMY_UNIT_SCENE_RARIBERO_HEYA_H

#include "types.h"
#include "field_scene.h"

u8 *RariberoHeya_GetEntrances(void);
u8 *RariberoHeya_GetRegions(void);
u8 *RariberoHeya_GetExits(void);
u8 *RariberoHeya_GetPlacements(void);
u8 *RariberoHeya_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gRariberoPoseAction[];
extern struct SceneEntrance gRariberoEntrances[];
extern s16 gRariberoSanctumRegions[];
extern s16 gRariberoRegions[];
extern u32 gRariberoExits[];
extern struct ScenePlacement gRariberoPlacements[];
extern struct ScenePlacement gRariberoHeyaPlacements[];
extern struct ScenePlacement gRariberoHeyaPlacements9a7[];
extern struct SceneEvent gRariberoEvents[];
extern struct SceneEvent gRariberoEvents9a7[];
extern struct SceneEvent gRariberoHeyaEvents[];

#endif
