#ifndef ALCHEMY_UNIT_MAP_TOLBI_SHIP_MAST_H
#define ALCHEMY_UNIT_MAP_TOLBI_SHIP_MAST_H

#include "types.h"
#include "scene_tables.h"
#include "scene.h"

const struct SceneEntrance *TolbiShipMast_GetEntrances(void);
const struct SceneExitRegion *TolbiShipMast_GetRegions(void);
const u32 *TolbiShipMast_GetExits(void);
const struct ActorPlacement *TolbiShipMast_GetActorPlacements(void);
const struct Trigger *TolbiShipMast_GetTriggers(void);
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
extern s32 gTolbiShipMast_BirdDeparture0[];
extern s32 gTolbiShipMast_BirdDeparture1[];
extern s32 gTolbiShipMast_BirdDeparture2[];
extern s32 gTolbiShipMast_BirdDeparture3[];
extern s32 gTolbiShipMast_BirdDeparture4[];
extern s32 gTolbiShipMast_BirdDeparture5[];
extern u32 gTolbiShipMast_BirdDeparture6[];
extern struct SceneEntrance gTolbiShipMast_Entrances[];
extern struct SceneExitRegion gTolbiShipMast_Regions[];
extern u32 gTolbiShipMast_Exits[];
extern struct ActorPlacement gTolbiShipMast_ActorPlacements[];
extern struct ActorPlacement gTolbiShipMast_FirstAttackActorPlacements[];
extern struct ActorPlacement gTolbiShipMast_BirdActorPlacements[];
extern struct ActorPlacement gTolbiShipMast_LandingActorPlacements[];
extern struct Trigger gTolbiShipMast_Triggers[];
extern u32 gTolbiShipMast_PassengerRowValues[];

#endif
