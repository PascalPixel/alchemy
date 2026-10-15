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
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gSanctum_ItemIconGrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct SceneEntrance gSanctum_TableA[];
extern u32 gSanctum_TableB[];
extern struct ScenePlacement gSanctum_PlacementA[];
extern struct ScenePlacement gSanctum_PlacementB[];
extern struct ScenePlacement gSanctum_PlacementSequenceB[];
extern struct ScenePlacement gSanctum_PlacementC[];
extern struct ScenePlacement gSanctum_PlacementD[];
extern struct ScenePlacement gSanctum_PlacementE[];
extern struct SceneEvent gSanctum_SceneTableA[];
extern struct SceneEvent gSanctum_SceneTableB[];
extern struct SceneEvent gSanctum_SceneTableC[];
extern struct SceneEvent gSanctum_SceneTableD[];
extern struct SceneEvent gSanctum_SceneTableE[];
extern struct SceneEvent gSanctum_SceneTableF[];
extern struct SceneEvent gSanctum_SceneTableG[];
extern struct SceneEvent gSanctum_SceneTableH[];
extern struct SceneEvent gSanctum_SceneTableI[];
extern s32 gSanctum_CursorBob[];
extern s32 gSanctum_OwnerEffectScript[];
extern s32 gSanctum_SparkEndScript[];

#endif
