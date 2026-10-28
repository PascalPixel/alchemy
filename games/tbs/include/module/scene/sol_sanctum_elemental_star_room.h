#ifndef ALCHEMY_MODULE_MAP_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_H
#define ALCHEMY_MODULE_MAP_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *SolSanctumElementalStarRoom_GetEntrances(void);
const struct SceneExitRegion *SolSanctumElementalStarRoom_GetRegions(void);
const u32 *SolSanctumElementalStarRoom_GetExits(void);
const struct ActorPlacement *
SolSanctumElementalStarRoom_GetActorPlacements(void);
const struct Trigger *SolSanctumElementalStarRoom_GetTriggers(void);
s32 SolSanctumElementalStarRoom_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
extern s32 gSolSanctumElementalStarRoom_AngleScript[];
extern s32 gSolSanctumElementalStarRoom_PresentItemScript[];
extern struct SceneEntrance gSolSanctumElementalStarRoom_Entrances[];
extern u32 gSolSanctumElementalStarRoom_Exits[];
extern struct ActorPlacement gSolSanctumElementalStarRoom_ActorPlacements[];
extern struct Trigger gSolSanctumElementalStarRoom_Triggers[];
extern u16 gSolSanctumElementalStarRoom_StarCells[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctumRing_MaxScales[];
extern s32 gSolSanctumRing_ScaleSteps[];

#endif
