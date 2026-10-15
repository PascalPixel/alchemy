#ifndef ALCHEMY_UNIT_SCENE_VALE_H
#define ALCHEMY_UNIT_SCENE_VALE_H

#include "types.h"
#include "field_scene.h"

s32 MapStagedScene_SelectPrimaryData(void);
s32 MapStagedScene_GetEmptyData(void);
s32 MapStagedScene_SelectSecondaryData(void);
s32 MapStagedScene_SelectTertiaryData(void);
s32 MapStagedScene_SelectQuaternaryData(void);
s32 HaidiaMura_ApplyEntryState(void);
extern s32 StagedActor_DirectionSteps[];
extern u32 StagedActor_FootprintKinds[];
extern s32 StagedActor_FootprintBounds[];
extern s32 gHaidiaMuraActor22Actions[];
extern s32 gLeaderHammerAction[];
extern s32 gGeraldAction[];
extern s32 gVillagerAction[];
extern s32 gDustBurstScript[];
extern struct SceneEntrance gHaidiaMuraEntrances[];
extern u32 gHaidiaMuraExits[];
extern struct ScenePlacement gHaidiaMuraPlacements[];
extern struct ScenePlacement gHaidiaMuraPlacements2[];
extern struct ScenePlacement gHaidiaMuraPlacements3[];
extern struct ScenePlacement gHaidiaMuraPlacements4[];
extern struct SceneEvent gHaidiaMuraEvents[];
extern struct SceneEvent gHaidiaMuraEvents2[];
extern u16 gHaidiaMuraCellAnimC[];
extern u16 gHaidiaMuraCellAnimA[];
extern u16 gHaidiaMuraCellAnimB[];
extern u16 gHaidiaMuraCellAnimD[];
extern s32 gHaidiaMuraLeaderWalkActions[];
extern s32 gHaidiaMuraActor22WalkActions[];
extern s32 gHaidiaMuraPairTableA[];
extern s32 gHaidiaMuraPairTableB[];
extern s32 gHaidiaMuraPairTableC[];
extern s32 gHaidiaMuraPairTableD[];

#endif
