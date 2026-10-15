#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_MAST_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_MAST_H

#include "types.h"
#include "field_scene.h"

u8 *TolbiShipMast_GetEntrances(void);
u8 *TolbiShipMast_GetRegions(void);
u8 *TolbiShipMast_GetExits(void);
u8 *TolbiShipMast_GetPlacements(void);
u8 *TolbiShipMast_GetEvents(void);
s32 TolbiShipMast_ApplyEntryState(void);
extern s32 gTolbiShipMast_FollowCameraActions[];
extern s32 gTolbiShipMast_LookoutActions[];
extern s32 gTolbiShipMast_DriftActions[];
extern s32 gTolbiShipMast_WaveActions[];
extern s32 gTolbiShipMast_EnsembleWalk9[];
extern s32 gTolbiShipMast_EnsembleWalk10[];
extern s32 gTolbiShipMast_EnsembleWalk11[];
extern s32 gTolbiShipMast_EnsembleWalk12[];
extern s32 gTolbiShipMast_EnsembleWalk13[];
extern s32 gTolbiShipMast_EnsembleWalk14[];
extern u32 gTolbiShipMast_EnsembleWalk15[];
extern struct SceneEntrance gTolbiShipMast_SceneTableA[];
extern s16 gTolbiShipMast_SceneTableB[];
extern u32 gTolbiShipMast_SceneTableC[];
extern struct ScenePlacement gTolbiShipMast_SceneTableD[];
extern struct ScenePlacement gTolbiShipMast_StagingObjects[];
extern struct ScenePlacement gTolbiShipMast_EnsembleObjects[];
extern struct ScenePlacement gTolbiShipMast_LandingObjects[];
extern struct SceneEvent gTolbiShipMast_SceneTableE[];
extern u32 gTolbiShipMast_FlagValues[];

#endif
