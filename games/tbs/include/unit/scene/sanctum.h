#ifndef ALCHEMY_UNIT_MAP_SANCTUM_H
#define ALCHEMY_UNIT_MAP_SANCTUM_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Sanctum_GetEntrances(void);
const struct SceneExitRegion *Sanctum_GetRegions(void);
const u32 *Sanctum_GetExits(void);
const struct ActorTemplate *Sanctum_GetActorTemplates(void);
const struct Trigger *Sanctum_GetTriggers(void);
s32 Sanctum_Initialize(void);
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gSanctum_ItemIconGrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct SceneEntrance gSanctum_Entrances[];
extern u32 gSanctum_Exits[];
extern struct ActorTemplate gSanctum_NoActorTemplates[];
extern struct ActorTemplate gSanctum_ValeActorTemplates[];
extern struct ActorTemplate gSanctum_IvanActorTemplate[];
extern struct ActorTemplate gSanctum_VaultBilibinActorTemplates[];
extern struct ActorTemplate gSanctum_QuestActorTemplates[];
extern struct ActorTemplate gSanctum_AltinXianActorTemplates[];
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
