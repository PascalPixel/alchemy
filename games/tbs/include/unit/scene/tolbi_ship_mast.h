#ifndef ALCHEMY_UNIT_SCENE_TOLBI_SHIP_MAST_H
#define ALCHEMY_UNIT_SCENE_TOLBI_SHIP_MAST_H

#include "types.h"
#include "field_scene.h"
#include "field_runtime.h"

const struct SceneEntrance *TolbiShipMast_GetEntrances(void);
const struct SceneRegion *TolbiShipMast_GetRegions(void);
const u32 *TolbiShipMast_GetExits(void);
const struct ScenePlacement *TolbiShipMast_GetPlacements(void);
const struct SceneEvent *TolbiShipMast_GetEvents(void);
s32 TolbiShipMast_Initialize(void);
extern s32 gTolbiShipMast_DriftY;
extern s32 gTolbiShipMast_DriftX;
extern s32 gTolbiShipMast_SwayY;
extern s32 gTolbiShipMast_SwayUnused;
extern s32 gTolbiShipMast_CameraCenter[2];
extern s32 gTolbiShipMast_CameraOrigin[2];
extern s32 gTolbiShipMast_SwayX;
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
extern struct SceneEntrance gTolbiShipMast_Entrances[];
extern struct SceneRegion gTolbiShipMast_Regions[];
extern u32 gTolbiShipMast_Exits[];
extern struct ScenePlacement gTolbiShipMast_Placements[];
extern struct ScenePlacement gTolbiShipMast_StagingObjects[];
extern struct ScenePlacement gTolbiShipMast_EnsembleObjects[];
extern struct ScenePlacement gTolbiShipMast_LandingObjects[];
extern struct SceneEvent gTolbiShipMast_Events[];
extern u32 gTolbiShipMast_PassengerRowValues[];

#endif
