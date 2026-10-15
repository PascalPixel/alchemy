#ifndef ALCHEMY_UNIT_SCENE_VENUS_AERIE_H
#define ALCHEMY_UNIT_SCENE_VENUS_AERIE_H

#include "types.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern const s32 *gEffectScripts[];
extern s32 VenusAerie_PairActionScript[];
extern s32 SceneAction_EntryGroup[];
extern s32 SceneAction_EntryPair[];
extern s32 VenusAerie_ScaleStepAction[];
extern s32 VenusAerie_BeamOpenAction[];
extern s32 VenusAerie_BeamNarrowAction[];
extern s32 VenusAerie_BeamRiseAction[];
extern s32 VenusAerie_BeamCloseAction[];
extern s32 VenusAerie_BeamEndAction[];
extern s32 VenusAerie_OrbitParticleScript[];
extern s32 VenusAerie_RiseParticleScript[];
extern s32 VenusAerie_WalkOutAction[];
extern s32 VenusAerie_RiseActionScript[];
extern s32 VenusAerie_StepBackAction[];
extern s32 VenusAerie_LeapAction[];
extern s32 VenusAerie_PartyExitAction[];
extern s32 VenusAerie_PartyHeadingAction[];
extern struct SceneEntrance gVenusAerieEntrances[];
extern u32 gVenusAerieExits[];
extern struct ScenePlacement gVenusAeriePlacements[];
extern s32 VenusAerie_NearestActor;
extern struct SceneEvent gVenusAerieEvents[];
extern s32 VenusAerie_BesideParticleScript[];

#endif
