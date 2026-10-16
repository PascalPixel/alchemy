#ifndef ALCHEMY_UNIT_SCENE_SOL_STAR_H
#define ALCHEMY_UNIT_SCENE_SOL_STAR_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *SolStar_GetEntrances(void);
struct SceneRegion *SolStar_GetRegions(void);
u32 *SolStar_GetExits(void);
struct ScenePlacement *SolStar_GetPlacements(void);
struct SceneEvent *SolStar_GetEvents(void);
s32 SolStar_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gSolStar_AngleScript[];
extern s32 gSolStar_PresentItemScript[];
extern struct SceneEntrance gSolStar_Entrances[];
extern u32 gSolStar_Exits[];
extern struct ScenePlacement gSolStar_Placements[];
extern struct SceneEvent gSolStar_Events[];
extern u16 gSolStar_StarCells[];
extern u8 gSol_RingDrift[][3];
extern u8 gSol_RingSwing[][3];
extern s8 gSol_RingDirection[][3];
extern s32 gSolRing_MaxScales[];
extern s32 gSolRing_ScaleSteps[];

#endif
