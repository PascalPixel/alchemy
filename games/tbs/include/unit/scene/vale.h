#ifndef ALCHEMY_UNIT_SCENE_VALE_H
#define ALCHEMY_UNIT_SCENE_VALE_H

#include "types.h"
#include "field_scene.h"

s32 MapStagedScene_SelectPrimaryData(void);
s32 MapStagedScene_GetEmptyData(void);
s32 MapStagedScene_SelectSecondaryData(void);
s32 MapStagedScene_SelectTertiaryData(void);
s32 MapStagedScene_SelectQuaternaryData(void);
s32 Vale_ApplyEntryState(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gValeActor22Actions[];
extern s32 gLeaderHammerAction[];
extern s32 gGaretAction[];
extern s32 gVillagerAction[];
extern s32 gDustBurstScript[];
extern struct SceneEntrance gValeEntrances[];
extern u32 gValeExits[];
extern struct ScenePlacement gValePlacements[];
extern struct ScenePlacement gValePlacements2[];
extern struct ScenePlacement gValePlacements3[];
extern struct ScenePlacement gValePlacements4[];
extern struct SceneEvent gValeEvents[];
extern struct SceneEvent gValeEvents2[];
extern u16 gValeCellAnimC[];
extern u16 gValeCellAnimA[];
extern u16 gValeCellAnimB[];
extern u16 gValeCellAnimD[];
extern s32 gValeLeaderWalkActions[];
extern s32 gValeActor22WalkActions[];
extern s32 gValePairTableA[];
extern s32 gValePairTableB[];
extern s32 gValePairTableC[];
extern s32 gValePairTableD[];

#endif
