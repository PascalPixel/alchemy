#ifndef ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_MODULE_SCENE_MERCURY_LIGHTHOUSE_ENTRANCE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern s32 gMercuryLighthouseEntrance_RippleScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern s32 gMercuryLighthouseEntrance_LizardManStepCentre[];
extern s32 gMercuryLighthouseEntrance_LizardManStepWest[];
extern s32 gMercuryLighthouseEntrance_LizardManStepEast[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern struct SceneEntrance gMercuryLighthouseEntrance_Entrances[];
extern u32 gMercuryLighthouseEntrance_Exits[];
extern struct ActorPlacement gMercuryLighthouseEntrance_ActorPlacements[];
extern struct Trigger gMercuryLighthouseEntrance_Triggers[];
extern s32 gGlint_TwinkleScript[];

#endif
