#ifndef ALCHEMY_UNIT_MAP_SANCTUM_H
#define ALCHEMY_UNIT_MAP_SANCTUM_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Sanctum_GetWarpDests(void);
const struct MapWarpRegion *Sanctum_GetRegions(void);
const u32 *Sanctum_GetWarps(void);
const struct ObjectEventTemplate *Sanctum_GetObjectEventTemplates(void);
const struct MapEvent *Sanctum_GetEvents(void);
s32 Sanctum_Initialize(void);
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gSanctum_ItemIconGrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct MapWarpDest gSanctum_WarpDests[];
extern u32 gSanctum_Warps[];
extern struct ObjectEventTemplate gSanctum_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gSanctum_ValeObjectEventTemplates[];
extern struct ObjectEventTemplate gSanctum_IvanObjectEventTemplate[];
extern struct ObjectEventTemplate gSanctum_VaultBilibinObjectEventTemplates[];
extern struct ObjectEventTemplate gSanctum_QuestObjectEventTemplates[];
extern struct ObjectEventTemplate gSanctum_AltinXianObjectEventTemplates[];
extern struct MapEvent gSanctum_ValeEvents[];
extern struct MapEvent gSanctum_VaultEvents[];
extern struct MapEvent gSanctum_VaultRevisitEvents[];
extern struct MapEvent gSanctum_QuestEvents[];
extern struct MapEvent gSanctum_ValeEventsAfterLeaving[];
extern struct MapEvent gSanctum_ValeEventsAfterKalay[];
extern struct MapEvent gSanctum_AltinEvents[];
extern struct MapEvent gSanctum_BilibinEvents[];
extern struct MapEvent gSanctum_XianEvents[];
extern s32 gSanctum_CursorBob[];
extern s32 gSanctum_OwnerEffectScript[];
extern s32 gSanctum_SparkEndScript[];

#endif
