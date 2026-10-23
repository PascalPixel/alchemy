#ifndef ALCHEMY_UNIT_MAP_KALAY_TOLBI_DOCKS_H
#define ALCHEMY_UNIT_MAP_KALAY_TOLBI_DOCKS_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *KalayTolbiDocks_GetWarpDests(void);
const struct MapWarpRegion *KalayTolbiDocks_GetRegions(void);
const u32 *KalayTolbiDocks_GetWarps(void);
const struct ObjectEventTemplate *KalayTolbiDocks_GetObjectEventTemplates(void);
const struct MapEvent *KalayTolbiDocks_GetEvents(void);
s32 KalayTolbiDocks_Initialize(void);
extern u16 gKalayTolbiDocks_KalayDoorCells[];
extern u16 gKalayTolbiDocks_TolbiDoorCells[];
extern s32 gKalayTolbiDocks_BodyguardSpinScript[];
extern struct MapWarpDest gKalayTolbiDocks_NoWarpDests[];
extern struct MapWarpDest gKalayTolbiDocks_KalayWarpDests[];
extern struct MapWarpDest gKalayTolbiDocks_TolbiWarpDests[];
extern struct MapWarpDest gKalayTolbiDocks_InteriorWarpDests[];
extern u32 gKalayTolbiDocks_Warps[];
extern struct ObjectEventTemplate gKalayTolbiDocks_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gKalayTolbiDocks_TolbiObjectEventTemplates[];
extern struct ObjectEventTemplate
    gKalayTolbiDocks_TolbiObjectEventTemplatesStranded[];
extern struct ObjectEventTemplate gKalayTolbiDocks_KalayObjectEventTemplates[];
extern struct ObjectEventTemplate
    gKalayTolbiDocks_KalayObjectEventTemplatesShipGone[];
extern struct ObjectEventTemplate
    gKalayTolbiDocks_InteriorObjectEventTemplates[];
extern struct ObjectEventTemplate
    gKalayTolbiDocks_InteriorObjectEventTemplatesShipGone[];
extern struct ObjectEventTemplate
    gKalayTolbiDocks_InteriorObjectEventTemplatesStranded[];
extern struct MapEvent gKalayTolbiDocks_NoEvents[];
extern struct MapEvent gKalayTolbiDocks_KalayEvents[];
extern struct MapEvent gKalayTolbiDocks_KalayEventsShipGone[];
extern struct MapEvent gKalayTolbiDocks_TolbiEvents[];
extern struct MapEvent gKalayTolbiDocks_TolbiEventsStranded[];
extern struct MapEvent gKalayTolbiDocks_InteriorEvents[];
extern struct MapEvent gKalayTolbiDocks_InteriorEventsShipGone[];
extern struct MapEvent gKalayTolbiDocks_InteriorEventsStranded[];

#endif
