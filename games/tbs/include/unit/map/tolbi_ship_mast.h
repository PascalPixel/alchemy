#ifndef ALCHEMY_UNIT_MAP_TOLBI_SHIP_MAST_H
#define ALCHEMY_UNIT_MAP_TOLBI_SHIP_MAST_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"

const struct MapWarpDest *TolbiShipMast_GetWarpDests(void);
const struct MapWarpRegion *TolbiShipMast_GetRegions(void);
const u32 *TolbiShipMast_GetWarps(void);
const struct ObjectEventTemplate *TolbiShipMast_GetObjectEventTemplates(void);
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
extern struct MapWarpDest gTolbiShipMast_WarpDests[];
extern struct MapWarpRegion gTolbiShipMast_Regions[];
extern u32 gTolbiShipMast_Warps[];
extern struct ObjectEventTemplate gTolbiShipMast_ObjectEventTemplates[];
extern struct ObjectEventTemplate
    gTolbiShipMast_FirstAttackObjectEventTemplates[];
extern struct ObjectEventTemplate gTolbiShipMast_BirdObjectEventTemplates[];
extern struct ObjectEventTemplate gTolbiShipMast_LandingObjectEventTemplates[];
extern struct MapEvent gTolbiShipMast_Events[];
extern u32 gTolbiShipMast_PassengerRowValues[];

#endif
