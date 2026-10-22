#ifndef ALCHEMY_UNIT_SCENE_XIAN_H
#define ALCHEMY_UNIT_SCENE_XIAN_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Xian_GetEntrances(void);
const struct SceneRegion *Xian_GetRegions(void);
const u32 *Xian_GetExits(void);
const struct ScenePlacement *Xian_GetPlacements(void);
const struct SceneEvent *Xian_GetEvents(void);
s32 Xian_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
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
extern struct ScenePlacement gXian_Placements[];
extern struct SceneEvent gXian_Events[];

#endif
