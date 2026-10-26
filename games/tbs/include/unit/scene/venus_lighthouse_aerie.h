#ifndef ALCHEMY_UNIT_MAP_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_UNIT_MAP_VENUS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "scene_tables.h"

struct SceneEntrance;
struct Trigger;
struct ActorTemplate;
struct SceneExitRegion;

const struct SceneEntrance *VenusLighthouseAerie_GetEntrances(void);
const struct SceneExitRegion *VenusLighthouseAerie_GetRegions(void);
const u32 *VenusLighthouseAerie_GetExits(void);
const struct ActorTemplate *VenusLighthouseAerie_GetActorTemplates(void);
const struct Trigger *VenusLighthouseAerie_GetTriggers(void);
s32 VenusLighthouseAerie_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern const s32 *gFieldEffect_DurationScripts[];
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
extern struct ActorTemplate gVenusLighthouseAerieActorTemplates[];
extern s32 gVenusLighthouseAerie_LandedPlatform;
extern struct Trigger gVenusLighthouseAerieTriggers[];
extern s32 gVenusLighthouseAerie_BesideParticleScript[];

#endif
