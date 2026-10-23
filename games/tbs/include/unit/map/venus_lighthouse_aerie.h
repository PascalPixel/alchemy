#ifndef ALCHEMY_UNIT_MAP_VENUS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_UNIT_MAP_VENUS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;
struct MapWarpRegion;

const struct MapWarpDest *VenusLighthouseAerie_GetWarpDests(void);
const struct MapWarpRegion *VenusLighthouseAerie_GetRegions(void);
const u32 *VenusLighthouseAerie_GetWarps(void);
const struct ObjectEventTemplate *
VenusLighthouseAerie_GetObjectEventTemplates(void);
const struct MapEvent *VenusLighthouseAerie_GetEvents(void);
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
extern struct MapWarpDest gVenusLighthouseAerieWarpDests[];
extern u32 gVenusLighthouseAerieWarps[];
extern struct ObjectEventTemplate gVenusLighthouseAerieObjectEventTemplates[];
extern s32 gVenusLighthouseAerie_LandedPlatform;
extern struct MapEvent gVenusLighthouseAerieEvents[];
extern s32 gVenusLighthouseAerie_BesideParticleScript[];

#endif
