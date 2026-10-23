#ifndef ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_UNIT_SCENE_VENUS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "map_tables.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *VenusLighthouseAerie_GetEntrances(void);
const struct SceneRegion *VenusLighthouseAerie_GetRegions(void);
const u32 *VenusLighthouseAerie_GetExits(void);
const struct ScenePlacement *VenusLighthouseAerie_GetPlacements(void);
const struct SceneEvent *VenusLighthouseAerie_GetEvents(void);
s32 VenusLighthouseAerie_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 gVenusLighthouseAerie_JoinLeaderAction[];
extern s32 gVenusLighthouseAerie_StepBackWestAction[];
extern s32 gVenusLighthouseAerie_StepBackEastAction[];
extern s32 gVenusLighthouseAerie_ScaleStepAction[];
extern s32 gVenusLighthouseAerie_BeamOpenAction[];
extern s32 gVenusLighthouseAerie_BeamNarrowAction[];
extern s32 gVenusLighthouseAerie_BeamRiseAction[];
extern s32 gVenusLighthouseAerie_BeamCloseAction[];
extern s32 gVenusLighthouseAerie_BeamEndAction[];
extern s32 gVenusLighthouseAerie_OrbitParticleScript[];
extern s32 gVenusLighthouseAerie_RiseParticleScript[];
extern s32 gVenusLighthouseAerie_WalkOutAction[];
extern s32 gVenusLighthouseAerie_RiseActionScript[];
extern s32 gVenusLighthouseAerie_StepBackAction[];
extern s32 gVenusLighthouseAerie_LeapAction[];
extern s32 gVenusLighthouseAerie_PartyExitAction[];
extern s32 gVenusLighthouseAerie_PartyHeadingAction[];
extern struct SceneEntrance gVenusLighthouseAerieEntrances[];
extern u32 gVenusLighthouseAerieExits[];
extern struct ScenePlacement gVenusLighthouseAeriePlacements[];
extern s32 gVenusLighthouseAerie_LandedPlatform;
extern struct SceneEvent gVenusLighthouseAerieEvents[];
extern s32 gVenusLighthouseAerie_BesideParticleScript[];

#endif
