#ifndef ALCHEMY_UNIT_MAP_WORLD_MAP_H
#define ALCHEMY_UNIT_MAP_WORLD_MAP_H

#include "types.h"
#include "map_tables.h"

struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *WorldMap_GetWarpDests(void);
const struct MapWarpRegion *WorldMap_GetRegions(void);
const u32 *WorldMap_GetWarps(void);
const struct ObjectEventTemplate *WorldMap_GetObjectEventTemplates(void);
const struct MapEvent *WorldMap_GetEvents(void);
s32 WorldMap_Initialize(void);
extern s32 gWorldMapIodem;
extern u16 gWorldMapBlend;
extern s32 gCaravanFirstWagonArrives[];
extern s32 gCaravanFirstWagonDeparts[];
extern s32 gCaravanFirstWagonReturns[];
extern s32 gCaravanWagonHeadsNorth[];
extern s32 gCaravanSecondWagonArrives[];
extern s32 gCaravanSecondWagonDeparts[];
extern s32 gCaravanSecondWagonReturns[];
extern s32 gCaravanThirdWagonArrives[];
extern s32 gCaravanThirdWagonDeparts[];
extern s32 gCaravanThirdWagonReturns[];
extern s32 gCaravanFourthWagonArrives[];
extern s32 gCaravanFourthWagonDeparts[];
extern s32 gCaravanFourthWagonReturns[];
extern s32 gCaravanRockGuideScript[];
extern s32 gBlackOrbLeaderScript[];
extern s32 gEndingAlexWalk[];
extern s32 gEndingKradenWalk[];
extern s32 gEndingJennaWalk[];
extern s32 gQuakeDustScript[];
extern s32 gCrossingShipSailsWest[];
extern s32 gCrossingShipVeersNorth[];
extern s32 gIslandDriftsEast[];
extern s32 gWorldMapFlintScript[];
extern struct MapWarpDest gWorldMapWarpDests[];
extern u32 gWorldMapWarps[];
extern s32 gWorldMapKolimaBridgeScript[];
extern s32 gWorldMapAlignToMapScript[];
extern s32 gWorldMapIodemScript[];
extern s32 gWorldMapStormScript[];
extern struct ObjectEventTemplate gWorldMapObjectEventTemplates[];
extern struct ObjectEventTemplate gWorldMapObjectEventTemplatesHammetsCaravan[];
extern struct ObjectEventTemplate
    gWorldMapObjectEventTemplatesFromWestLunpaCave[];
extern struct ObjectEventTemplate gWorldMapObjectEventTemplatesCrossingEnds[];
extern struct ObjectEventTemplate gWorldMapObjectEventTemplatesCrossing[];
extern struct ObjectEventTemplate
    gWorldMapObjectEventTemplatesFromKalayTunnelEntranceSouth[];
extern struct ObjectEventTemplate
    gWorldMapObjectEventTemplatesJennaAndKradenAdrift[];
extern struct ObjectEventTemplate gWorldMapObjectEventTemplatesFromClear[];
extern struct ObjectEventTemplate
    gWorldMapObjectEventTemplatesBeforeShebasFall[];
extern struct MapEvent gWorldMapEvents[];
extern u32 gWorldMapPassengerRowValues[];
extern s32 gIslandWakeScript[];

#endif
