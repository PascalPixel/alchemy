#ifndef ALCHEMY_UNIT_MAP_XIAN_H
#define ALCHEMY_UNIT_MAP_XIAN_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *Xian_GetEntrances(void);
const struct SceneExitRegion *Xian_GetRegions(void);
const u32 *Xian_GetExits(void);
const struct ActorTemplate *Xian_GetActorTemplates(void);
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
extern struct SceneEntrance gXian_Entrances[];
extern u32 gXian_Exits[];
extern struct ActorTemplate gXian_ActorTemplates[];
extern struct MapEvent gXian_Events[];

#endif
