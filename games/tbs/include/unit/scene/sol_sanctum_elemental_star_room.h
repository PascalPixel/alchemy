#ifndef ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_H
#define ALCHEMY_UNIT_SCENE_SOL_SANCTUM_ELEMENTAL_STAR_ROOM_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *SolSanctumElementalStarRoom_GetEntrances(void);
struct SceneRegion *SolSanctumElementalStarRoom_GetRegions(void);
u32 *SolSanctumElementalStarRoom_GetExits(void);
struct ScenePlacement *SolSanctumElementalStarRoom_GetPlacements(void);
struct SceneEvent *SolSanctumElementalStarRoom_GetEvents(void);
s32 SolSanctumElementalStarRoom_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gSolSanctumElementalStarRoom_AngleScript[];
extern s32 gSolSanctumElementalStarRoom_PresentItemScript[];
extern struct SceneEntrance gSolSanctumElementalStarRoom_Entrances[];
extern u32 gSolSanctumElementalStarRoom_Exits[];
extern struct ScenePlacement gSolSanctumElementalStarRoom_Placements[];
extern struct SceneEvent gSolSanctumElementalStarRoom_Events[];
extern u16 gSolSanctumElementalStarRoom_StarCells[];
extern u8 gSolSanctum_RingDrift[][3];
extern u8 gSolSanctum_RingSwing[][3];
extern s8 gSolSanctum_RingDirection[][3];
extern s32 gSolSanctumRing_MaxScales[];
extern s32 gSolSanctumRing_ScaleSteps[];

#endif
