#ifndef ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
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
extern struct ActorPlacement gVenusLighthouseAerieActorPlacements[];
extern s32 gVenusLighthouseAerie_LandedPlatform;
extern struct Trigger gVenusLighthouseAerieTriggers[];
extern s32 gVenusLighthouseAerie_BesideParticleScript[];

#endif
