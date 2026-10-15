#ifndef ALCHEMY_UNIT_SCENE_SANCTUM_H
#define ALCHEMY_UNIT_SCENE_SANCTUM_H

#include "types.h"
#include "field_scene.h"

void *Sanctum_GetEntrances(void);
s32 SceneData_ReturnZero(void);
void *Sanctum_GetExits(void);
void *SceneData_SelectPlacementTableBySubstate(void);
u8 *SceneData_SelectTableBySceneIdAndFlags(void);
s32 Sanctum_ApplyEntryState(void);
extern u32 Sanctum_LeaderCircleScript[];
extern u32 Sanctum_GaretCircleScript[];
extern s32 Sanctum_ItemIconGrowScript[];
extern s32 Sanctum_ItemIconEndScript[];
extern struct SceneEntrance Sanctum_TableA[];
extern u32 Sanctum_TableB[];
extern struct ScenePlacement Sanctum_PlacementA[];
extern struct ScenePlacement Sanctum_PlacementB[];
extern struct ScenePlacement Sanctum_PlacementSequenceB[];
extern struct ScenePlacement Sanctum_PlacementC[];
extern struct ScenePlacement Sanctum_PlacementD[];
extern struct ScenePlacement Sanctum_PlacementE[];
extern struct SceneEvent Sanctum_SceneTableA[];
extern struct SceneEvent Sanctum_SceneTableB[];
extern struct SceneEvent Sanctum_SceneTableC[];
extern struct SceneEvent Sanctum_SceneTableD[];
extern struct SceneEvent Sanctum_SceneTableE[];
extern struct SceneEvent Sanctum_SceneTableF[];
extern struct SceneEvent Sanctum_SceneTableG[];
extern struct SceneEvent Sanctum_SceneTableH[];
extern struct SceneEvent Sanctum_SceneTableI[];
extern s32 Sanctum_CursorBob[];
extern s32 Sanctum_OwnerEffectScript[];
extern s32 Sanctum_SparkEndScript[];

#endif
