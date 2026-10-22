#ifndef ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_H
#define ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_H

#include "types.h"
#include "field.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *VenusLighthouse_GetEntrances(void);
const struct SceneRegion *VenusLighthouse_GetRegions(void);
const u32 *VenusLighthouse_GetExits(void);
const struct ScenePlacement *VenusLighthouse_GetPlacements(void);
const struct SceneEvent *VenusLighthouse_GetEvents(void);
s32 VenusLighthouse_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern u32 gStagedActor_DirectionSteps[];
extern struct SwitchCell gVenusThirdFloorSockets[];
extern u32 gVenusBalancePlatformHeights[];
extern struct SwitchCell gVenusFourthFloorSockets[];
extern u8 gVenusTracksMovingWest[];
extern u8 gVenusTracksMovingEast[];
extern u8 gVenusTracksMovingNorth[];
extern u8 gVenusTracksMovingSouth[];
extern u8 gVenusThirdFloorTrackLists[];
extern u8 gVenusFourthFloorTrackLists[];
extern const s32 *gEffectScripts[];
extern s32 gVenusGreyStatueScript[];
extern s32 gVenusGreyPillarScript[];
extern s32 gVenusLeaderDazedScript[];
extern s32 gVenusMiniatureLighthouseScript[];
extern s32 gVenusSprayScript[];
extern s32 gVenusTrackNorthSouthScript[];
extern s32 gVenusTrackNorthWestScript[];
extern s32 gVenusTrackNorthEastScript[];
extern s32 gVenusTrackWestEastScript[];
extern s32 gVenusTrackSouthEastScript[];
extern s32 gVenusTrackSouthWestScript[];
extern s32 gVenusSecondFloorLightToGate[];
extern s32 gVenusThirdFloorLightToNortheast[];
extern s32 gVenusThirdFloorLightToSoutheast[];
extern s32 gVenusLightGoesOut[];
extern s32 gVenusThirdFloorLightNorthwestToGate[];
extern s32 gVenusThirdFloorLightNorthwestToSouthwest[];
extern s32 gVenusThirdFloorLightNortheastToNorthwest[];
extern s32 gVenusThirdFloorLightNortheastToSoutheast[];
extern s32 gVenusThirdFloorLightSouthwestToNorthwest[];
extern s32 gVenusThirdFloorLightSouthwestToGate[];
extern s32 gVenusThirdFloorLightSoutheastToNortheast[];
extern s32 gVenusThirdFloorLightSoutheastToSouthwest[];
extern s32 gVenusFourthFloorLightFromStatue[];
extern s32 gVenusFourthFloorLightToSoutheast[];
extern s32 gVenusFourthFloorLightSoutheastToMiddle[];
extern s32 gVenusFourthFloorLightMiddleToWest[];
extern s32 gVenusFourthFloorLightMiddleToEast[];
extern s32 gVenusFourthFloorLightWestToNorthwest[];
extern s32 gVenusFourthFloorLightEastToNortheast[];
extern s32 gVenusFourthFloorLightToSouthwest[];
extern s32 gVenusFourthFloorLightSouthwestToFarWest[];
extern s32 gVenusFourthFloorLightFarWestToWest[];
extern s32 gVenusFourthFloorLightNorthwestToGate[];
extern u16 gVenusSecondFloorGateCells[];
extern u16 gVenusThirdFloorGateCells[];
extern u16 gVenusFourthFloorGateCells[];
extern struct SceneEntrance gVenusLighthouseEntryEntrances[];
extern struct SceneEntrance gVenusLighthouseEntrancesOther[];
extern struct SceneEntrance gVenusLighthouseFirstFloorEntrances[];
extern struct SceneEntrance gVenusLighthouseSecondFloorEntrances[];
extern struct SceneEntrance gVenusLighthouseThirdFloorEntrances[];
extern struct SceneEntrance gVenusLighthouseFourthFloorEntrances[];
extern u32 gVenusLighthouseExits[];
extern struct ScenePlacement gVenusLighthousePlacementsOther[];
extern struct ScenePlacement gVenusLighthouseEntryPlacements[];
extern struct ScenePlacement gVenusLighthouseGroundFloorPlacements[];
extern struct ScenePlacement gVenusLighthouseFirstFloorPlacements[];
extern struct ScenePlacement gVenusLighthouseSecondFloorPlacements[];
extern struct ScenePlacement gVenusLighthouseThirdFloorPlacements[];
extern struct ScenePlacement gVenusLighthouseFourthFloorPlacements[];
extern struct SceneEvent gVenusLighthouseEntryEvents[];
extern struct SceneEvent gVenusLighthouseGroundFloorEvents[];
extern struct SceneEvent gVenusLighthouseFirstFloorEvents[];
extern struct SceneEvent gVenusLighthouseSecondFloorEvents[];
extern struct SceneEvent gVenusLighthouseThirdFloorEvents[];
extern struct SceneEvent gVenusLighthouseFourthFloorEvents[];
extern const u8 *gVenusThirdFloorSocketTracks[];
extern s32 *gVenusThirdFloorNorthwestPaths[];
extern s32 *gVenusThirdFloorNortheastPaths[];
extern s32 *gVenusThirdFloorSouthwestPaths[];
extern s32 *gVenusThirdFloorSoutheastPaths[];
extern s32 **gVenusThirdFloorSocketPaths[];
extern const u8 *gVenusFourthFloorSocketTracks[];
extern s32 *gVenusFourthFloorNorthwestPaths[];
extern s32 *gVenusFourthFloorNortheastPaths[];
extern s32 *gVenusFourthFloorWestPaths[];
extern s32 *gVenusFourthFloorEastPaths[];
extern s32 *gVenusFourthFloorFarWestPaths[];
extern s32 *gVenusFourthFloorMiddlePaths[];
extern s32 *gVenusFourthFloorSouthwestPaths[];
extern s32 *gVenusFourthFloorSoutheastPaths[];
extern s32 **gVenusFourthFloorSocketPaths[];

#endif
