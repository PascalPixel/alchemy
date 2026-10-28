#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_ENTRANCE_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *MercuryLighthouseEntrance_GetEntrances(void);
const struct SceneExitRegion *MercuryLighthouseEntrance_GetRegions(void);
const u32 *MercuryLighthouseEntrance_GetExits(void);
const struct ActorPlacement *MercuryLighthouseEntrance_GetActorPlacements(void);
const struct Trigger *MercuryLighthouseEntrance_GetTriggers(void);
s32 MercuryLighthouseEntrance_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern s32 gMercuryLighthouseEntrance_RippleScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern const s32 *gFieldEffect_DurationScripts[];
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
extern s32 gMercuryLighthouseEntrance_SparkBurstScript[];

#endif
