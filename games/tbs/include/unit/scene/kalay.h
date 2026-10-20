#ifndef ALCHEMY_UNIT_SCENE_KALAY_H
#define ALCHEMY_UNIT_SCENE_KALAY_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Kalay_GetEntrances(void);
const struct SceneRegion *Kalay_GetRegions(void);
const u32 *Kalay_GetExits(void);
const struct ScenePlacement *Kalay_GetPlacements(void);
const struct SceneEvent *Kalay_GetEvents(void);
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
extern struct SceneEntrance gKalay_NoEntrances[];
extern struct SceneEntrance gKalay_TownEntrances[];
extern struct SceneEntrance gKalay_PalaceGateEntrances[];
extern struct SceneEntrance gKalay_CaveEntrances[];
extern struct SceneEntrance gKalay_Entrances4[];
extern struct SceneEntrance gKalay_Entrances5[];
extern struct SceneEntrance gKalay_TunnelEntrances[];
extern u32 gKalay_Exits[];
extern struct ScenePlacement gKalay_NoPlacements[];
extern struct ScenePlacement gKalay_TownPlacements[];
extern struct ScenePlacement gKalay_TourGroupPlacements[];
extern struct ScenePlacement gKalay_PalaceGatePlacements[];
extern struct ScenePlacement gKalay_CavePlacements[];
extern struct ScenePlacement gKalay_TunnelPlacements[];
extern struct SceneEvent gKalay_NoEvents[];
extern struct SceneEvent gKalay_TownEvents[];
extern struct SceneEvent gKalay_PalaceGateEvents[];
extern struct SceneEvent gKalay_CaveEvents[];
extern struct SceneEvent gKalay_Events4[];
extern struct SceneEvent gKalay_Events5[];
extern struct SceneEvent gKalay_TunnelEvents[];
extern u16 gKalay_DoorCellSteps[];
extern struct MapCellAnim gKalay_DoorCells[];
extern s32 gKalay_TunnelParticleScript[];

#endif
