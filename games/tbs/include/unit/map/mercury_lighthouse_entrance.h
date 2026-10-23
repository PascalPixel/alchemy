#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_ENTRANCE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_ENTRANCE_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *MercuryLighthouseEntrance_GetWarpDests(void);
const struct MapWarpRegion *MercuryLighthouseEntrance_GetRegions(void);
const u32 *MercuryLighthouseEntrance_GetWarps(void);
const struct ObjectEventTemplate *
MercuryLighthouseEntrance_GetObjectEventTemplates(void);
const struct MapEvent *MercuryLighthouseEntrance_GetEvents(void);
s32 MercuryLighthouseEntrance_Initialize(void);
extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
extern s32 gStagedObjectEvent_DirectionSteps[];
extern u32 gStagedObjectEvent_FootprintKinds[];
extern s32 gStagedObjectEvent_FootprintBounds[];
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
extern struct MapWarpDest gMercuryLighthouseEntrance_WarpDests[];
extern u32 gMercuryLighthouseEntrance_Warps[];
extern struct ObjectEventTemplate
    gMercuryLighthouseEntrance_ObjectEventTemplates[];
extern struct MapEvent gMercuryLighthouseEntrance_Events[];
extern s32 gMercuryLighthouseEntrance_SparkBurstScript[];

#endif
