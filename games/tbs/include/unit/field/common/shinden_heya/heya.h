#ifndef ALCHEMY_UNIT_FIELD_COMMON_SHINDEN_HEYA_HEYA_H
#define ALCHEMY_UNIT_FIELD_COMMON_SHINDEN_HEYA_HEYA_H

#include "types.h"
#include "field_scene.h"

void *ShindenHeya_GetEntrances(void);
s32 SceneData_ReturnZero(void);
void *ShindenHeya_GetExits(void);
void *SceneData_SelectPlacementTableBySubstate(void);
u8 *SceneData_SelectTableBySceneIdAndFlags(void);
s32 ShindenHeya_ApplyEntryState(void);
extern u32 ShindenHeya_LeaderCircleScript[];
extern u32 ShindenHeya_GeraldCircleScript[];
extern s32 ShindenHeya_ItemIconGrowScript[];
extern s32 ShindenHeya_ItemIconEndScript[];
extern struct SceneEntrance ShindenHeya_TableA[];
extern u32 ShindenHeya_TableB[];
extern struct ScenePlacement ShindenHeya_PlacementA[];
extern struct ScenePlacement ShindenHeya_PlacementB[];
extern struct ScenePlacement ShindenHeya_PlacementSequenceB[];
extern struct ScenePlacement ShindenHeya_PlacementC[];
extern struct ScenePlacement ShindenHeya_PlacementD[];
extern struct ScenePlacement ShindenHeya_PlacementE[];
extern struct SceneEvent ShindenHeya_SceneTableA[];
extern struct SceneEvent ShindenHeya_SceneTableB[];
extern struct SceneEvent ShindenHeya_SceneTableC[];
extern struct SceneEvent ShindenHeya_SceneTableD[];
extern struct SceneEvent ShindenHeya_SceneTableE[];
extern struct SceneEvent ShindenHeya_SceneTableF[];
extern struct SceneEvent ShindenHeya_SceneTableG[];
extern struct SceneEvent ShindenHeya_SceneTableH[];
extern struct SceneEvent ShindenHeya_SceneTableI[];
extern s32 ShindenHeya_CursorBob[];
extern s32 ShindenHeya_OwnerEffectScript[];
extern s32 ShindenHeya_SparkEndScript[];

#endif
