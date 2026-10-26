#ifndef ALCHEMY_UNIT_MAP_TOLBI_SHIP_MAST_H
#define ALCHEMY_UNIT_MAP_TOLBI_SHIP_MAST_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"

const struct SceneEntrance *TolbiShipMast_GetEntrances(void);
const struct SceneExitRegion *TolbiShipMast_GetRegions(void);
const u32 *TolbiShipMast_GetExits(void);
const struct ActorTemplate *TolbiShipMast_GetActorTemplates(void);
const struct MapEvent *TolbiShipMast_GetEvents(void);
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
extern struct ActorTemplate gTolbiShipMast_ActorTemplates[];
extern struct ActorTemplate gTolbiShipMast_FirstAttackActorTemplates[];
extern struct ActorTemplate gTolbiShipMast_BirdActorTemplates[];
extern struct ActorTemplate gTolbiShipMast_LandingActorTemplates[];
extern struct MapEvent gTolbiShipMast_Events[];
extern u32 gTolbiShipMast_PassengerRowValues[];

#endif
