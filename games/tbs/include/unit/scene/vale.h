#ifndef ALCHEMY_UNIT_SCENE_VALE_H
#define ALCHEMY_UNIT_SCENE_VALE_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Vale_GetEntrances(void);
const struct SceneRegion *Vale_GetRegions(void);
const u32 *Vale_GetExits(void);
const struct ScenePlacement *Vale_GetPlacements(void);
const struct SceneEvent *Vale_GetEvents(void);
s32 Vale_Initialize(void);
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gValeAlephGuardActions[];
extern s32 gLeaderHammerAction[];
extern s32 gGaretAction[];
extern s32 gValeGrowingWaterAction[];
extern s32 gDustBurstScript[];
extern struct SceneEntrance gVale_Entrances[];
extern u32 gVale_Exits[];
extern struct ScenePlacement gVale_Placements[];
extern struct ScenePlacement gVale_PlacementsAfterLeaving[];
extern struct ScenePlacement gVale_PlacementsAfterKalay[];
extern struct ScenePlacement gVale_RepairPlacements[];
extern struct SceneEvent gVale_Events[];
extern struct SceneEvent gVale_EventsAfterLeaving[];
extern u16 gValeCellAnimC[];
extern u16 gValeCellAnimA[];
extern u16 gValeCellAnimB[];
extern u16 gValeCellAnimD[];
extern s32 gValeLeaderWalkActions[];
extern s32 gValeAlephGuardWalkActions[];
extern s32 gValePairTableA[];
extern s32 gValePairTableB[];
extern s32 gValePairTableC[];
extern s32 gValePairTableD[];

#endif
