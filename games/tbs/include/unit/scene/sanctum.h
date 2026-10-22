#ifndef ALCHEMY_UNIT_SCENE_SANCTUM_H
#define ALCHEMY_UNIT_SCENE_SANCTUM_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Sanctum_GetEntrances(void);
const struct SceneRegion *Sanctum_GetRegions(void);
const u32 *Sanctum_GetExits(void);
const struct ScenePlacement *Sanctum_GetPlacements(void);
const struct SceneEvent *Sanctum_GetEvents(void);
s32 Sanctum_Initialize(void);
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gSanctum_ItemIconGrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct SceneEntrance gSanctum_Entrances[];
extern u32 gSanctum_Exits[];
extern struct ScenePlacement gSanctum_NoPlacements[];
extern struct ScenePlacement gSanctum_ValePlacements[];
extern struct ScenePlacement gSanctum_IvanPlacement[];
extern struct ScenePlacement gSanctum_VaultBilibinPlacements[];
extern struct ScenePlacement gSanctum_QuestPlacements[];
extern struct ScenePlacement gSanctum_AltinXianPlacements[];
extern struct SceneEvent gSanctum_ValeEvents[];
extern struct SceneEvent gSanctum_VaultEvents[];
extern struct SceneEvent gSanctum_VaultRevisitEvents[];
extern struct SceneEvent gSanctum_QuestEvents[];
extern struct SceneEvent gSanctum_ValeEventsAfterLeaving[];
extern struct SceneEvent gSanctum_ValeEventsAfterKalay[];
extern struct SceneEvent gSanctum_AltinEvents[];
extern struct SceneEvent gSanctum_BilibinEvents[];
extern struct SceneEvent gSanctum_XianEvents[];
extern s32 gSanctum_CursorBob[];
extern s32 gSanctum_OwnerEffectScript[];
extern s32 gSanctum_SparkEndScript[];

#endif
