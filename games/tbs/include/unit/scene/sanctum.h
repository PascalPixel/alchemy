#ifndef ALCHEMY_UNIT_SCENE_SANCTUM_H
#define ALCHEMY_UNIT_SCENE_SANCTUM_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *Sanctum_GetEntrances(void);
struct SceneRegion *Sanctum_GetRegions(void);
u32 *Sanctum_GetExits(void);
struct ScenePlacement *Sanctum_GetPlacements(void);
struct SceneEvent *Sanctum_GetEvents(void);
s32 Sanctum_Initialize(void);
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gSanctum_ItemIconGrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct SceneEntrance gSanctum_Entrances[];
extern u32 gSanctum_Exits[];
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
