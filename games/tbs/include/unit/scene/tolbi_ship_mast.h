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
extern s32 TolbiShipMast_FollowCameraActions[];
extern s32 TolbiShipMast_LookoutActions[];
extern s32 TolbiShipMast_DriftActions[];
extern s32 TolbiShipMast_WaveActions[];
extern s32 TolbiShipMast_EnsembleWalk9[];
extern s32 TolbiShipMast_EnsembleWalk10[];
extern s32 TolbiShipMast_EnsembleWalk11[];
extern s32 TolbiShipMast_EnsembleWalk12[];
extern s32 TolbiShipMast_EnsembleWalk13[];
extern s32 TolbiShipMast_EnsembleWalk14[];
extern u32 TolbiShipMast_EnsembleWalk15[];
extern struct SceneEntrance TolbiShipMast_SceneTableA[];
extern s16 TolbiShipMast_SceneTableB[];
extern u32 TolbiShipMast_SceneTableC[];
extern struct ScenePlacement TolbiShipMast_SceneTableD[];
extern struct ScenePlacement TolbiShipMast_StagingObjects[];
extern struct ScenePlacement TolbiShipMast_EnsembleObjects[];
extern struct ScenePlacement TolbiShipMast_LandingObjects[];
extern struct SceneEvent TolbiShipMast_SceneTableE[];
extern u32 TolbiShipMast_FlagValues[];

#endif
