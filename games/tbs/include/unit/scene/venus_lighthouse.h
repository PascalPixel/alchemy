#ifndef ALCHEMY_UNIT_MAP_VENUS_LIGHTHOUSE_H
#define ALCHEMY_UNIT_MAP_VENUS_LIGHTHOUSE_H

#include "types.h"
#include "field.h"
#include "map_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;

const struct SceneEntrance *VenusLighthouse_GetEntrances(void);
const struct SceneExitRegion *VenusLighthouse_GetRegions(void);
const u32 *VenusLighthouse_GetExits(void);
const struct ActorTemplate *VenusLighthouse_GetActorTemplates(void);
const struct Trigger *VenusLighthouse_GetTriggers(void);
s32 VenusLighthouse_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern u32 gPushedObject_DirectionSteps[];
extern struct SwitchCell gVenusThirdFloorSockets[];
extern u32 gVenusBalancePlatformHeights[];
extern struct SwitchCell gVenusFourthFloorSockets[];
extern u8 gVenusTracksMovingWest[];
extern u8 gVenusTracksMovingEast[];
extern u8 gVenusTracksMovingNorth[];
extern u8 gVenusTracksMovingSouth[];
extern u8 gVenusThirdFloorTrackLists[];
extern u8 gVenusFourthFloorTrackLists[];
extern const s32 *gFieldEffect_DurationScripts[];
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
extern struct ActorTemplate gVenusLighthouseActorTemplatesOther[];
extern struct ActorTemplate gVenusLighthouseEntryActorTemplates[];
extern struct ActorTemplate gVenusLighthouseGroundFloorActorTemplates[];
extern struct ActorTemplate gVenusLighthouseFirstFloorActorTemplates[];
extern struct ActorTemplate gVenusLighthouseSecondFloorActorTemplates[];
extern struct ActorTemplate gVenusLighthouseThirdFloorActorTemplates[];
extern struct ActorTemplate gVenusLighthouseFourthFloorActorTemplates[];
extern struct Trigger gVenusLighthouseEntryTriggers[];
extern struct Trigger gVenusLighthouseGroundFloorTriggers[];
extern struct Trigger gVenusLighthouseFirstFloorTriggers[];
extern struct Trigger gVenusLighthouseSecondFloorTriggers[];
extern struct Trigger gVenusLighthouseThirdFloorTriggers[];
extern struct Trigger gVenusLighthouseFourthFloorTriggers[];
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
