#ifndef ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_UNIT_SCENE_MERCURY_LIGHTHOUSE_ENTRANCE_H

#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *MercuryLighthouseEntrance_GetEntrances(void);
const struct SceneRegion *MercuryLighthouseEntrance_GetRegions(void);
const u32 *MercuryLighthouseEntrance_GetExits(void);
const struct ScenePlacement *MercuryLighthouseEntrance_GetPlacements(void);
const struct SceneEvent *MercuryLighthouseEntrance_GetEvents(void);
s32 MercuryLighthouseEntrance_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedActor_DirectionSteps[];
extern u32 gStagedActor_FootprintKinds[];
extern s32 gStagedActor_FootprintBounds[];
extern s32 gMercuryLighthouseEntrance_RippleScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 gMercuryLighthouseEntrance_LizardManStepCentre[];
extern s32 gMercuryLighthouseEntrance_LizardManStepWest[];
extern s32 gMercuryLighthouseEntrance_LizardManStepEast[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern struct SceneEntrance gMercuryLighthouseEntrance_Entrances[];
extern u32 gMercuryLighthouseEntrance_Exits[];
extern struct ScenePlacement gMercuryLighthouseEntrance_Placements[];
extern struct SceneEvent gMercuryLighthouseEntrance_Events[];
extern s32 gMercuryLighthouseEntrance_SparkBurstScript[];

#endif
