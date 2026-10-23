#ifndef ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_H
#define ALCHEMY_UNIT_MAP_MERCURY_LIGHTHOUSE_H

#include "types.h"
#include "map_tables.h"

struct MapWarpDest;
struct MapEvent;
struct ObjectEventTemplate;

const struct MapWarpDest *MercuryLighthouse_GetWarpDests(void);
const struct MapWarpRegion *MercuryLighthouse_GetRegions(void);
const u32 *MercuryLighthouse_GetWarps(void);
const struct ObjectEventTemplate *
MercuryLighthouse_GetObjectEventTemplates(void);
const struct MapEvent *MercuryLighthouse_GetEvents(void);
s32 MercuryLighthouse_Initialize(void);
extern s32 gShortEffectScript[];
extern s32 gMediumEffectScript[];
extern s32 gLongEffectScript[];
extern s32 gPushedObject_DirectionSteps[];
extern u32 gPushedObject_FootprintKinds[];
extern s32 gPushedObject_FootprintBounds[];
extern s32 gMercuryLighthouse_RippleScript[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_RampScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern const s32 *gEffectScripts[];
extern s32 gMercuryLighthouse_SparkBurstScript[];
extern s32 gMercuryLighthouse_PushScriptA[];
extern s32 gMercuryLighthouse_PushScriptB[];
extern s32 gMercuryLighthouse_PushScriptD[];
extern s32 gMercuryLighthouse_PushScriptC[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
extern u16 gMercuryLighthouse_GateCells[];
extern u16 gMercuryLighthouse_GateCloseCells[];
extern u16 gMercuryLighthouse_FloorSwitchCells[];
extern u16 gMercuryLighthouse_FloorSwitchCloseCells[];
extern u16 gMercuryLighthouse_ColumnCells[];
extern struct MapWarpDest gMercuryLighthouseWarpDestsOther[];
extern struct MapWarpDest gMercuryLighthouseMiddleWarpDests[];
extern struct MapWarpDest gMercuryLighthouseUpperWarpDests[];
extern struct MapWarpDest gMercuryLighthouseExteriorWarpDests[];
extern u32 gMercuryLighthouse_Warps[];
extern struct ObjectEventTemplate gMercuryLighthouseObjectEventTemplatesOther[];
extern struct ObjectEventTemplate gMercuryLighthouseLowerObjectEventTemplates[];
extern struct ObjectEventTemplate
    gMercuryLighthouseMiddleObjectEventTemplates[];
extern struct ObjectEventTemplate gMercuryLighthouseUpperObjectEventTemplates[];
extern struct ObjectEventTemplate
    gMercuryLighthouseExteriorObjectEventTemplates[];
extern struct MapEvent gMercuryLighthouseLowerEvents[];
extern struct MapEvent gMercuryLighthouseMiddleEvents[];
extern struct MapEvent gMercuryLighthouseUpperEvents[];
extern struct MapEvent gMercuryLighthouseEventsOther[];

#endif
