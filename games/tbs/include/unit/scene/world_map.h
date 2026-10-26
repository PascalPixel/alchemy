#ifndef ALCHEMY_UNIT_MAP_WORLD_MAP_H
#define ALCHEMY_UNIT_MAP_WORLD_MAP_H

#include "types.h"
#include "scene_tables.h"

struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *WorldMap_GetEntrances(void);
const struct SceneExitRegion *WorldMap_GetRegions(void);
const u32 *WorldMap_GetExits(void);
const struct ActorTemplate *WorldMap_GetActorTemplates(void);
const struct Trigger *WorldMap_GetTriggers(void);
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
extern struct SceneEntrance gWorldSceneEntrances[];
extern u32 gWorldMapExits[];
extern s32 gWorldMapKolimaBridgeScript[];
extern s32 gWorldMapAlignToMapScript[];
extern s32 gWorldMapIodemScript[];
extern s32 gWorldMapStormScript[];
extern struct ActorTemplate gWorldMapActorTemplates[];
extern struct ActorTemplate gWorldMapActorTemplatesHammetsCaravan[];
extern struct ActorTemplate gWorldMapActorTemplatesFromWestLunpaCave[];
extern struct ActorTemplate gWorldMapActorTemplatesCrossingEnds[];
extern struct ActorTemplate gWorldMapActorTemplatesCrossing[];
extern struct ActorTemplate
    gWorldMapActorTemplatesFromKalayTunnelEntranceSouth[];
extern struct ActorTemplate gWorldMapActorTemplatesJennaAndKradenAdrift[];
extern struct ActorTemplate gWorldMapActorTemplatesFromClear[];
extern struct ActorTemplate gWorldMapActorTemplatesBeforeShebasFall[];
extern struct Trigger gWorldTriggers[];
extern u32 gWorldMapPassengerRowValues[];
extern s32 gIslandWakeScript[];

#endif
