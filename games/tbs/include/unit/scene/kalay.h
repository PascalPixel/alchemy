#ifndef ALCHEMY_UNIT_MAP_KALAY_H
#define ALCHEMY_UNIT_MAP_KALAY_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *Kalay_GetWarpDests(void);
const struct MapWarpRegion *Kalay_GetRegions(void);
const u32 *Kalay_GetWarps(void);
const struct ObjectEventTemplate *Kalay_GetObjectEventTemplates(void);
const struct MapEvent *Kalay_GetEvents(void);
s32 Kalay_Initialize(void);
extern s32 gKalay_TileStepByFacing[];
extern s32 gKalay_TourGroupWalkScript[];
extern s32 gKalay_TourGroupLeaveScript[];
extern s32 gKalay_TouristFidgetScript[];
extern s32 gKalay_GuideCountScript[];
extern s32 gKalay_GuidePaceScript[];
extern s32 gKalay_HeadGuideStopScript[];
extern s32 gKalay_SecondGuideStopScript[];
extern s32 gKalay_HeadGuideReturnScript[];
extern s32 gKalay_SecondGuideReturnScript[];
extern s32 gKalay_PartyFidgetScript[];
extern s32 gKalay_PartyGatherScript[];
extern s32 gKalay_FrostGrowScript[];
extern struct MapWarpDest gKalay_NoWarpDests[];
extern struct MapWarpDest gKalay_TownWarpDests[];
extern struct MapWarpDest gKalay_HammetPalaceWarpDests[];
extern struct MapWarpDest gKalay_TunnelEntranceWarpDests[];
extern struct MapWarpDest gKalay_TunnelCaveWarpDests[];
extern struct MapWarpDest gKalay_TunnelPassageWarpDests[];
extern struct MapWarpDest gKalay_TunnelWarpDests[];
extern u32 gKalay_Warps[];
extern struct ObjectEventTemplate gKalay_NoObjectEventTemplates[];
extern struct ObjectEventTemplate gKalay_TownObjectEventTemplates[];
extern struct ObjectEventTemplate gKalay_TourGroupObjectEventTemplates[];
extern struct ObjectEventTemplate gKalay_HammetPalaceObjectEventTemplates[];
extern struct ObjectEventTemplate gKalay_TunnelEntranceObjectEventTemplates[];
extern struct ObjectEventTemplate gKalay_TunnelObjectEventTemplates[];
extern struct MapEvent gKalay_NoEvents[];
extern struct MapEvent gKalay_TownEvents[];
extern struct MapEvent gKalay_HammetPalaceEvents[];
extern struct MapEvent gKalay_TunnelEntranceEvents[];
extern struct MapEvent gKalay_TunnelCaveEvents[];
extern struct MapEvent gKalay_TunnelPassageEvents[];
extern struct MapEvent gKalay_TunnelEvents[];
extern u16 gKalay_DoorCellSteps[];
extern struct MapGridCellAnimation gKalay_DoorCells[];
extern s32 gKalay_TunnelParticleScript[];

#endif
