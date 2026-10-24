#ifndef ALCHEMY_UNIT_MAP_XIAN_H
#define ALCHEMY_UNIT_MAP_XIAN_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *Xian_GetWarpDests(void);
const struct MapWarpRegion *Xian_GetRegions(void);
const u32 *Xian_GetWarps(void);
const struct ObjectEventTemplate *Xian_GetObjectEventTemplates(void);
const struct MapEvent *Xian_GetEvents(void);
s32 Xian_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gXian_WaterCarrierRoute[];
extern s32 gXian_TrainingTreeJump[];
extern s32 gXian_TreeImpactScript[];
extern s32 gXian_TrainingParticleScript[];
extern u16 gXian_HouseDoorOpens[];
extern u16 gXian_NorthHouseDoorOpens[];
extern u16 gXian_InnDoorOpens[];
extern u16 gXian_SanctumDoorOpens[];
extern u16 gXian_SchoolDoorOpens[];
extern struct MapWarpDest gXian_WarpDests[];
extern u32 gXian_Warps[];
extern struct ObjectEventTemplate gXian_ObjectEventTemplates[];
extern struct MapEvent gXian_Events[];

#endif
