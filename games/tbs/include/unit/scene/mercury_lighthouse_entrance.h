#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

const struct SceneEntrance *MercuryLighthouseEntrance_GetEntrances(void);
const struct SceneExitRegion *MercuryLighthouseEntrance_GetRegions(void);
const u32 *MercuryLighthouseEntrance_GetExits(void);
const struct ActorTemplate *MercuryLighthouseEntrance_GetActorTemplates(void);
const struct Trigger *MercuryLighthouseEntrance_GetTriggers(void);
s32 MercuryLighthouseEntrance_Initialize(void);
extern s32 gFieldEffect_ShortScript[];
extern s32 gFieldEffect_MediumScript[];
extern s32 gFieldEffect_LongScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
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
extern struct ActorTemplate gMercuryLighthouseEntrance_ActorTemplates[];
extern struct Trigger gMercuryLighthouseEntrance_Triggers[];
extern s32 gMercuryLighthouseEntrance_SparkBurstScript[];

#endif
