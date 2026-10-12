#ifndef ALCHEMY_UNIT_SCENE_VINASU_CHOJO_H
#define ALCHEMY_UNIT_SCENE_VINASU_CHOJO_H

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
extern s32 VinasuChojo_PairActionScript[];
extern s32 SceneAction_EntryGroup[];
extern s32 SceneAction_EntryPair[];
extern s32 VinasuChojo_ScaleStepAction[];
extern s32 VinasuChojo_BeamOpenAction[];
extern s32 VinasuChojo_BeamNarrowAction[];
extern s32 VinasuChojo_BeamRiseAction[];
extern s32 VinasuChojo_BeamCloseAction[];
extern s32 VinasuChojo_BeamEndAction[];
extern s32 VinasuChojo_OrbitParticleScript[];
extern s32 VinasuChojo_RiseParticleScript[];
extern s32 VinasuChojo_WalkOutAction[];
extern s32 VinasuChojo_RiseActionScript[];
extern s32 VinasuChojo_StepBackAction[];
extern s32 VinasuChojo_LeapAction[];
extern s32 VinasuChojo_PartyExitAction[];
extern s32 VinasuChojo_PartyHeadingAction[];
extern struct SceneEntrance gVinasuChojoEntrances[];
extern u32 gVinasuChojoExits[];
extern struct ScenePlacement gVinasuChojoPlacements[];
extern s32 VinasuChojo_NearestActor;
extern struct SceneEvent gVinasuChojoEvents[];
extern s32 VinasuChojo_BesideParticleScript[];

#endif
