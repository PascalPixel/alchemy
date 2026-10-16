#ifndef ALCHEMY_UNIT_SCENE_SOL_ERUPTION_H
#define ALCHEMY_UNIT_SCENE_SOL_ERUPTION_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance *SolEruption_GetEntrances(void);
struct SceneRegion *SolEruption_GetRegions(void);
u32 *SolEruption_GetExits(void);
struct ScenePlacement *SolEruption_GetPlacements(void);
struct SceneEvent *SolEruption_GetEvents(void);
s32 SolEruption_Initialize(void);
extern s32 gEruption_ArcOriginsHead[];
extern s32 gEruption_ArcOrigins[];
extern struct SceneEntrance gSolEruption_Entrances[];
extern u32 gSolEruption_Exits[];
extern struct ScenePlacement gSolEruption_Placements[];
extern struct SceneEvent gSolEruption_Events[];
extern u16 gSolEruption_StarCells[];
extern s32 gEruption_EmberScript[];
extern u8 gSol_RingDrift[][3];
extern u8 gSol_RingSwing[][3];
extern s8 gSol_RingDirection[][3];
extern s32 gSolRing_MaxScales[];
extern s32 gSolRing_ScaleSteps[];

#endif
