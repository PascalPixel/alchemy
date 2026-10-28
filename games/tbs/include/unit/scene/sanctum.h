#ifndef ALCHEMY_UNIT_MAP_SANCTUM_H
#define ALCHEMY_UNIT_MAP_SANCTUM_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Sanctum_GetEntrances(void);
const struct SceneExitRegion *Sanctum_GetRegions(void);
const u32 *Sanctum_GetExits(void);
const struct ActorPlacement *Sanctum_GetActorPlacements(void);
const struct Trigger *Sanctum_GetTriggers(void);
s32 Sanctum_Initialize(void);
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gSanctum_ItemIconGrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct SceneEntrance gSanctum_Entrances[];
extern u32 gSanctum_Exits[];
extern struct ActorPlacement gSanctum_NoActorPlacements[];
extern struct ActorPlacement gSanctum_ValeActorPlacements[];
extern struct ActorPlacement gSanctum_IvanActorPlacement[];
extern struct ActorPlacement gSanctum_VaultBilibinActorPlacements[];
extern struct ActorPlacement gSanctum_QuestActorPlacements[];
extern struct ActorPlacement gSanctum_AltinXianActorPlacements[];
extern struct Trigger gSanctum_ValeTriggers[];
extern struct Trigger gSanctum_VaultTriggers[];
extern struct Trigger gSanctum_VaultRevisitTriggers[];
extern struct Trigger gSanctum_QuestTriggers[];
extern struct Trigger gSanctum_ValeTriggersAfterLeaving[];
extern struct Trigger gSanctum_ValeTriggersAfterKalay[];
extern struct Trigger gSanctum_AltinTriggers[];
extern struct Trigger gSanctum_BilibinTriggers[];
extern struct Trigger gSanctum_XianTriggers[];
extern s32 gSanctum_CursorBob[];
extern s32 gSanctum_OwnerEffectScript[];
extern s32 gSanctum_SparkEndScript[];

#endif
